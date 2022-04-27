/********** Moteur de tournoi : moteur ************/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <semaphore.h>
#include <pthread.h>
#include "avalam.h"
#include "topologie.h"
#include "moteur.h"

#include <fcntl.h>           /* Pour les constantes O_* */
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <semaphore.h>
#include <sys/wait.h>
#include <errno.h>



T_TabJoueurs joueurs; 
T_TabParties tournoi;
int delaiChoixCoup = DELAICHOIXCOUP; 

T_Score gainJaune = {1,0,0,0}; 
T_Score gainRouge = {0,0,1,0}; 

int main(int argc, char ** argv)
{
	struct sigaction newact;	// déclaration de la structure 
	char buffer[MAXLEN];
	char bNom[MAXLEN];

	FILE * fp; 
	DIR * dp;
	struct dirent * pDir; 
	int r,i;

	sprintf(bNom,"%s/tournoi.js",PATH_WEB);

	if (argc == 2) {
		delaiChoixCoup = atoi(argv[1]); 
		printf("Délai choix coup : %d secondes\n", delaiChoixCoup);
	}

	printf("PID process moteur : %d\n",getpid());
	// On enregistre le PID du moteur dans le fichier "moteur"

	// On demande à tous les processus du répertoire run de se terminer 	
	dp = opendir(PATH_RUN); 
	if (dp != NULL) {
		while (	(pDir = readdir(dp)) != NULL ) {
			if (pDir->d_type == DT_REG)
			if (strcmp(pDir->d_name,PID_FILE) != 0) {
				printf1("Suppression processus joueur : %s \n", pDir->d_name);
				// NB : on pourrait le faire avec un appel direct à kill()
				sprintf(buffer,"kill %s",pDir->d_name);
				CHECK_IF(system(buffer),-1,"suppression processus");
	
				sprintf(buffer,"rm -rf \"%s/%s\"",PATH_RUN,pDir->d_name);
				printf1("Suppression fichier joueur : %s\n",buffer);
				CHECK_IF(system(buffer),-1,"supression contenu de .../run");
			}
		}
		closedir(dp);
	}

	// On crée le répertoire run si nécessaire 
	sprintf(buffer,"mkdir -p \"%s\"",PATH_RUN);
	printf1("Création répertoire run :  %s\n",buffer);
	CHECK_IF(system(buffer),-1,"creation de .../run");

	sprintf(buffer,"%s/%s",PATH_RUN,PID_FILE);
	CHECK_IF(fp = fopen(buffer,"w"),NULL, "fopen");
	fprintf(fp,"%d",getpid()); 
	fclose(fp);

	newact.sa_sigaction = deroute;	
	CHECK_IF(sigemptyset(&newact.sa_mask),-1,"problème sigemptyset");
	newact.sa_flags = SA_SIGINFO | SA_RESTART | SA_NODEFER;

	// on installe le handler deroute
	// A partir de maintenant, toute occurence d'un signal SIGUSR1 déclenche l'appel de la fonction deroute  
	CHECK_IF(sigaction(SIGUSR1, &newact, NULL),-1,"problème sigaction");
	CHECK_IF(sigaction(SIGUSR2, &newact, NULL),-1,"problème sigaction");
	CHECK_IF(sigaction(SIGINT, &newact, NULL),-1,"problème sigaction");

	printf("Attente de connexion des joueurs\n");
	printf("------------------ Appuyez sur ENTREE pour démarrer ------------------\n"); 
	getchar();

	printf("----------------------------- Demarrage ------------------------------\n");

	printf("Nombre de joueurs : %d\n", joueurs.nb);
	if (joueurs.nb %2 ==1) {
		printf0("Nombre impair : on ajoute un joueur baptisé 'exempt'\n");
		addJoueur(0,"exempt", ATTEND); // souci lié au pid : si on kill 0, souci ! 
	}

	initTournoi();

	printf("------------------------------ Tournoi -------------------------------\n");
	listerTournoi();
	writeTournoi(bNom, tournoi, joueurs); 


	for(r=0;r< tournoi.nbRondes; r++)
	{
		printf("------------------------------ Ronde %d ------------------------------\n", r+1);	

		for(i=(joueurs.nb/2) * r ;i<(joueurs.nb/2) * (r+1);i++){
 
			// On écrit à chaque joueur quelle couleur il joue dans son fichier pid, 
			printf3("Lancement partie %d (%s - %s)\n", i, tournoi.parties[i].nomJ , tournoi.parties[i].nomR);

			// On va créer autant de semaphores qu'il y a de parties 
			// Chaque semaphore de partie sera libéré par le thread qui vient de terminer 
			// On pourra donc bloquer et poursuivre le programme principal
			printf1("Création sem. partie %d\n",i);
			sem_init(&(tournoi.parties[i].sem),0,0);

			tournoi.parties[i].status = ENCOURS; 

			if ( (tournoi.parties[i].pidJ != 0) && (tournoi.parties[i].pidR != 0) )
			{
				// partie entre deux moteurs
				// Il faudrait créer un thread de traitement pour chaque partie, permettant de gérer le temps de jeu de chaque joueur 
				CHECK_DIF((pthread_create(&(tournoi.parties[i].thread), NULL,thread_partie, (void*)i)), 0, "pthread_create");
 			} else {
				// un exempt : l'autre gagne sur le plus petit des scores
				// On pourrait aussi ne pas compter la partie...
				sem_post(&(tournoi.parties[i].sem));
				if (tournoi.parties[i].pidJ == 0)	terminerPartie(i,gainRouge);
				else terminerPartie(i,gainJaune);
			}
		}

		writeTournoi(bNom, tournoi, joueurs);

		// On attend la fin des parties
		for(i=(joueurs.nb/2) * r ;i<(joueurs.nb/2) * (r+1);i++){
			sem_wait(&(tournoi.parties[i].sem)); 
			// attention aux parties avec un exempt
			printf("Partie %d terminee\n",i); 
		
			if ( (tournoi.parties[i].pidJ != 0) && (tournoi.parties[i].pidR != 0) ) {
				printf0("Attente fin du thread\n");
				listerPartie(tournoi.parties[i].feuille);
				CHECK_DIF((pthread_join(tournoi.parties[i].thread, NULL)),0,"pthread_join");
				printf1("Thread %d termine\n",i);
			}
		}

		printf("------------------------------ Tournoi -------------------------------\n");
		listerTournoi();
		writeTournoi(bNom, tournoi, joueurs); 
	}

	printf("--------------------------- Fin du Tournoi ---------------------------\n");
	killJoueurs(); 
	
	exit(0);
}

void whoami(octet numP) {
	printf("Partie %d : %s(%d) - %s(%d) : " , numP,tournoi.parties[numP].nomJ,tournoi.parties[numP].pidJ, tournoi.parties[numP].nomR, tournoi.parties[numP].pidR); 
}

void whop(octet numP) {
	printf("P%d ", numP); 
}

void * thread_partie(void * i) {
	int numP = (int) i; 
	char bNom[MAXLEN]; 

	whop(numP);printf("Lancement thread de gestion de la partie %d\n", numP); 
	sprintf(bNom,"%s/%d.js",PATH_WEB, numP);

	whopd(numP);printf1("Indication couleur au joueur %d\n",tournoi.parties[numP].pidJ); 
	ecrireFichierJoueur(tournoi.parties[numP].pidJ, "j");
	kill(tournoi.parties[numP].pidJ, SIGUSR1);

	whopd(numP);printf1("attente confirmation couleur par joueur %d ...\n", tournoi.parties[numP].pidJ);
	sem_wait(joueurs.tab[tournoi.parties[numP].iJ].pSem);  
	whopd(numP);printf0("... ok\n");

	whopd(numP);printf1("Indication couleur au joueur %d\n",tournoi.parties[numP].pidR); 
	ecrireFichierJoueur(tournoi.parties[numP].pidR, "r");
	kill(tournoi.parties[numP].pidR, SIGUSR1);

	whopd(numP);printf1("attente confirmation couleur par joueur %d ...\n", tournoi.parties[numP].pidR);
	sem_wait(joueurs.tab[tournoi.parties[numP].iR].pSem);  
	whopd(numP);printf0("... ok\n");

	whop(numP);printf("Debut partie %d\n",numP);

	// Chaque thread va gérer une partie, comme avalam_standalone
	T_Position p = getPositionInitiale();
	writeDirect(bNom, p, tournoi.parties[numP]);

	T_ListeCoups l = getCoupsLegaux(p);
	T_Score score ;
	char * s;
	int indexCoupJoue; 

	while(l.nb > 0) {
		// On indique à qui c'est le tour 
		if (p.trait == JAU) {
			whop(numP);printf("Indication de son tour au joueur %d\n", tournoi.parties[numP].pidJ);
			kill(tournoi.parties[numP].pidJ, SIGUSR1);
			whopd(numP);printf1("attente confirmation par joueur %d ...\n", tournoi.parties[numP].pidJ);
			sem_wait(joueurs.tab[tournoi.parties[numP].iJ].pSem);  
			whopd(numP);printf0("... ok\n");
		}
		else {
			whop(numP);printf("Indication de son tour au joueur %d\n", tournoi.parties[numP].pidR);
			kill(tournoi.parties[numP].pidR, SIGUSR1);
			whopd(numP);printf1("attente confirmation par joueur %d ...\n", tournoi.parties[numP].pidR);
			sem_wait(joueurs.tab[tournoi.parties[numP].iR].pSem);  
			whopd(numP);printf0("... ok\n");		
		}
		
		// On attend 30s
		sleep(delaiChoixCoup);

		// On lui dit d'arrêter en lui réenvoyant un signal
		// On récupère son coup et on le joue 
		if (p.trait == JAU) {
			whopd(numP);printf1("attente confirmation coup par joueur %d ...\n", tournoi.parties[numP].pidJ);
			kill(tournoi.parties[numP].pidJ, SIGUSR1);
			sem_wait(joueurs.tab[tournoi.parties[numP].iJ].pSem);  
			whopd(numP);printf0("... ok\n");

			s = lireFichierJoueur(tournoi.parties[numP].pidJ); 
		}
		else {
			whopd(numP);printf1("attente confirmation coup par joueur %d ...\n", tournoi.parties[numP].pidR);
			kill(tournoi.parties[numP].pidR, SIGUSR1);
			sem_wait(joueurs.tab[tournoi.parties[numP].iR].pSem);  
			whopd(numP);printf0("... ok\n");

			s = lireFichierJoueur(tournoi.parties[numP].pidR);
		}

		
		whopd(numP);printf1("Nb coups possibles = %d ",l.nb);
		whopd(numP);printf1("Contenu fichier = %s ",s);
		indexCoupJoue = atoi(s);
		whopd(numP);printf1("Coup converti = %d\n", indexCoupJoue);
 
		if ((indexCoupJoue>= 0) && (indexCoupJoue<l.nb)) {
			whoami(numP); printf("Coup %d\n", tournoi.parties[numP].nbCoups);
			whoami(numP); printf("%s a choisi le coup %d\n",COLNAME(p.trait), indexCoupJoue );
			whoami(numP); printf("Soit : %d -> %d\n", l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination);
			

			tournoi.parties[numP].feuille.coups[tournoi.parties[numP].nbCoups].origine = l.coups[indexCoupJoue].origine; 
			tournoi.parties[numP].feuille.coups[tournoi.parties[numP].nbCoups].destination = l.coups[indexCoupJoue].destination; 
			tournoi.parties[numP].feuille.nb++; 
			tournoi.parties[numP].nbCoups++; 

			p = jouerCoup(p, l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination) ;
			l = getCoupsLegaux(p);
		} else {
			whoami(numP); fprintf(stderr,"SAISIE INCORRECTE ! \n"); 
			whoami(numP); fprintf(stderr,"FIN DE LA PARTIE %d\n",numP);
			return NULL; // On quitte le thread 
		}

		// On informe l'adversaire du coup en préparant un fichier pour lui 
		if (p.trait == JAU) {
			ecrireFichierJoueur(tournoi.parties[numP].pidJ,s) ;
		}
		else {
			ecrireFichierJoueur(tournoi.parties[numP].pidR,s) ;
		}

		free(s);

		// on enregistre la nouvelle position 
			writeDirect(bNom, p, tournoi.parties[numP]);

		// et on recommence 
	}

	whoami(numP); printf("Partie terminée !\n");

	// On envoie SIGUSR2 aux deux processus
	// Il faut attendre que les joueurs accusent réception de ces signaux 
	// Pour que l'on soit sûr que la partie est bien terminée 
	kill(tournoi.parties[numP].pidJ, SIGUSR2);
	// On doit se synchroniser avec un processus, correspondant à un joueur 
	// On crée autant de sem. que de joueurs 
	whopd(numP);printf1("attente terminaison partie par joueur %d ...\n", tournoi.parties[numP].pidJ);
	sem_wait(joueurs.tab[tournoi.parties[numP].iJ].pSem);  
	whopd(numP);printf0("... ok\n");


	kill(tournoi.parties[numP].pidR, SIGUSR2);
	whopd(numP);printf1("attente terminaison partie par joueur %d ...\n", tournoi.parties[numP].pidR);
	sem_wait(joueurs.tab[tournoi.parties[numP].iR].pSem);  
	whopd(numP);printf0("... ok\n");

	score = evaluerScore(p);
	terminerPartie(numP,score);

	// On libère le sémaphore associé 
	sem_post(&(tournoi.parties[numP].sem)); 

	//listerTournoi();

	whop(numP);printf("Fin Thread %d\n",numP);
	pthread_exit(NULL);
}


void listerPartie(T_FeuillePartie f) {
	octet i; 
	for(i=0;i<f.nb;i++) {
		printf("%2d. %2d->%2d\n", i, f.coups[i].origine,  f.coups[i].destination); 
	}
}

void listerTournoi(){
	int i,j, r; 
	printf("------------------ TOURNOI -----------------\n");
	printf("Nb rondes : %d\n", tournoi.nbRondes);
	printf("%d parties (%d joueurs)\n", tournoi.nbParties, joueurs.nb);
	for(r=0;r< tournoi.nbRondes; r++)
	{
	printf("------------------ Ronde %d ----------------\n", r+1);
		for(i=(joueurs.nb/2) * r ;i<(joueurs.nb/2) * (r+1);i++){
			printf("%d : ", i);			
			if (tournoi.parties[i].status != TERMINE) {
				printf("[%s] - [%s]\n", tournoi.parties[i].nomJ,tournoi.parties[i].nomR);			
			} else {
				printf("[%d(%d) %s] ", tournoi.parties[i].score.nbJ, tournoi.parties[i].score.nbJ5, tournoi.parties[i].nomJ);
				if (tournoi.parties[i].vainqueur == EGALITE) printf("1/2 - 1/2");
				else if (tournoi.parties[i].vainqueur == JAU) printf("1 - 0");
				else printf("0 - 1");
				printf(" [%s %d(%d)]\n", tournoi.parties[i].nomR,tournoi.parties[i].score.nbR, tournoi.parties[i].score.nbR5);
			}
		}
	}

	for(j=0;j<joueurs.nb;j++) {
		printf("Joueur %d : %s : %.1f points\n", j, joueurs.tab[j].nom, joueurs.tab[j].nbPoints);
	}
} 

void addPartie(int idP, octet ronde, int iJ,  int iR) {
	// On fournit l'id de partie pour pouvoir les organiser chronologiquement
	tournoi.parties[idP].iJ= iJ; 
	tournoi.parties[idP].pidJ= joueurs.tab[iJ].pid; 
	strcpy(tournoi.parties[idP].nomJ, joueurs.tab[iJ].nom);

	tournoi.parties[idP].iR= iR; 
	tournoi.parties[idP].pidR= joueurs.tab[iR].pid; 
	strcpy(tournoi.parties[idP].nomR, joueurs.tab[iR].nom);

	tournoi.parties[idP].status = ATTENTE; 
	tournoi.parties[idP].ronde = ronde; 

	tournoi.parties[idP].feuille.nb = 0; 
	tournoi.parties[idP].nbCoups = 0;

	printf3("ajout partie %d entre %s et %s\n",idP,joueurs.tab[iJ].nom, joueurs.tab[iR].nom);
}

void initTournoi() {

	// On classe les joueurs, on ajoute un "exempt" pour assurer la parité
	// On utilise le principe des tournois de blitz : 
	// Une table fixe, les autres tournent autour
	// On joue deux parties à chaque fois 

	// X et 1 2 3 4 5 6 7
	// X joue 1, puis les parties se répartissent à chaque fois avec des joueurs calculés à partir de 1 : 
	// -> de manière croissante pour les premiers : 1+1, 1+2, ...
	// -> de manière décroissante pour les suivants, modulo le nombre : (n-1 + 1-1)%(n-1), (n-1 + 1-2)%(n-1)

	// X 2 3 4
	// 1 7 6 5

	// X 3 4 5
	// 2 1 7 6

	// X 4 5 6
	// 3 2 1 7

	// X 5 6 7
	// 4 3 2 1

	// X 6 7 1
	// 5 4 3 2

	// X 7 1 2
	// 6 5 4 3

	// X 1 2 3
	// 7 6 5 4

	// n joueurs =2 * (n-1) parties 

	// On crée chaque partie entre les deux mêmes joueurs même s'ils ne jouent pas leurs deux parties de suite. 
 
	tournoi.nbParties = joueurs.nb *  (joueurs.nb-1); 
	tournoi.parties = (T_Partie *) calloc( tournoi.nbParties, sizeof(T_Partie));
	
	// premier joueur : pivot = 0
	int ronde; //ad du pivot = num ronde
	int n=joueurs.nb; //nb joueurs du tournoi (pair)
	int nbP; // nb de parties dans une ronde 
	int nextH; 
	int nextB; 
	int numP=0;

	tournoi.nbRondes = 2*(joueurs.nb-1); 

	for(ronde=1;ronde<n;ronde++) {

		//printf("\n\nronde %d\n", ronde);
		//printf("partie 0 entre 0 et %d\n",ronde);

		addPartie(numP, ronde, 0,  ronde);
		// il faut ajouter une autre partie couleurs inversées, (n-1) rondes plus tard
		// Or, il y a n/2 parties dans chaque ronde 

		addPartie((n/2)*(ronde+n-2),ronde+n-1, ronde, 0);

		numP++;

		nextH = (ronde+1 <n) ? ronde+1: 1;
		nextB = (ronde-1==0) ? n-1 : ronde-1; 

		for(nbP=1;nbP<n/2; nbP++){			
			//printf("partie %d entre %d et %d\n",nbP, nextH, nextB);
			addPartie(numP, ronde, nextH, nextB);
			addPartie((n/2)*(ronde+n-2)+nbP, ronde+n-1, nextB, nextH);

			nextH = (nextH+1 <n) ? nextH+1 : 1; 
			nextB = (nextB-1>0) ? nextB-1:n-1;

			numP++;
			 
		}
	}


}

void terminerPartie(int numP, T_Score score) {

	char bNom[MAXLEN];
	sprintf(bNom,"%s/p%d.js",PATH_WEB, numP);


	tournoi.parties[numP].status = TERMINE; 
	tournoi.parties[numP].score = score;

	if ( score.nbJ > score.nbR) tournoi.parties[numP].vainqueur = JAU;
	else if ( score.nbR > score.nbJ) tournoi.parties[numP].vainqueur = ROU;
	else if ( score.nbJ5 > score.nbR5) tournoi.parties[numP].vainqueur = JAU;
	else if ( score.nbR5 > score.nbJ5) tournoi.parties[numP].vainqueur = ROU;
	else tournoi.parties[numP].vainqueur = EGALITE;

	if (tournoi.parties[numP].vainqueur == JAU) joueurs.tab[tournoi.parties[numP].iJ].nbPoints++;
	else if (tournoi.parties[numP].vainqueur == ROU) joueurs.tab[tournoi.parties[numP].iR].nbPoints++; 
	else {
		joueurs.tab[tournoi.parties[numP].iJ].nbPoints += 0.5; 
		joueurs.tab[tournoi.parties[numP].iR].nbPoints += 0.5; 
	}

	writeFeuille(bNom,tournoi.parties[numP]); 

	
}

void addJoueur(int pid, char * nom, T_StatutJoueur status){
	char buffer[MAXLEN];
	joueurs.tab[joueurs.nb].pid = pid; 
	strcpy(joueurs.tab[joueurs.nb].nom, nom);
	joueurs.tab[joueurs.nb].status = status;
	joueurs.tab[joueurs.nb].nbPoints = 0; 

	if (pid != 0) {
		printf1("Creation semaphore pour joueur %d\n",pid);
		sprintf(buffer, "/j%d.avalam", pid);
		CHECK_IF(joueurs.tab[joueurs.nb].pSem = sem_open(buffer, 0), SEM_FAILED, "sem_open"); 
	}

	printf("\tAjout du joueur : %s (PID %d)\n", nom, pid);
	joueurs.nb++;

	printf("\t%d joueur(s) actuellement\n", joueurs.nb);
}


void deroute (int signal_number, siginfo_t * info, void * context)
{
	char * nom;
	FILE * fp; 
	int i;
	int j;

	printf1("Signal reçu, PID emetteur : %d !!\n",info->si_pid);

	switch (signal_number) {
		case SIGUSR1 : 
		fprintf(stderr,"\tSignal SIGUSR1 reçu de %d\n", info->si_pid); 
		fprintf(stderr,"\tNE DEVRAIT PAS ARRIVER\n"); 
		break;

		case SIGUSR2 : 
			printf("\tArrivee d'un nouveau joueur : %d\n", info->si_pid); 
			// Peut-être faudrait-il protéger cette zone contre des accès concurrents ?

			if (joueurs.nb == MAXPROCESS) {
					printf("\t\tTrop de joueurs !\n"); 
					return;
			}

			// Il faut vérifier qu'il n'existe pas déjà
			for(i=0;i<joueurs.nb;i++) {
				if (joueurs.tab[i].pid == info->si_pid) {
					fprintf(stderr,"\t\tCe joueur est déjà référencé !\n"); 
					return;
				}
			}

			nom = lireFichierJoueur(info->si_pid); 
			addJoueur(info->si_pid, nom, ATTEND);
			free(nom);

			// il doit avoir inscrit son nom dans un fichier portant son PID
			// joueurs.tab[joueurs.nb].status = ATTEND; 
			// joueurs.nb++;

		break;

		case SIGINT : 
			printf0("\tSignal SIGINT reçu.\n"); 		
			killJoueurs();
			exit(0); 
		break;

	}
}


void killJoueurs(void) {
	char buffer[MAXLEN];
	int i;
	int status; 

	for(i=0;i<joueurs.nb; i++) {

		// Ne pas s'occuper de l'exempt !
		if (joueurs.tab[i].pid == 0) continue;

		printf("Envoi d'un signal kill au process %d\n",joueurs.tab[i].pid);
		kill(joueurs.tab[i].pid, SIGINT);

		// NB : ce n'est pas un process fils... 
		// printf("Attente terminaison process %d\n",joueurs.tab[i].pid);
		// CHECK_IF(waitpid(joueurs.tab[i].pid, &status, 0), -1, "waitpid");
		// printf("Proces %d terminé\n",joueurs.tab[i].pid);

		printf1("sem_close pour process %d\n",joueurs.tab[i].pid);		
		CHECK_IF(sem_close(joueurs.tab[i].pSem), -1, "sem_close");

		printf1("sem_unlink pour process %d\n",joueurs.tab[i].pid);		
		sprintf(buffer, "/j%d.avalam", joueurs.tab[i].pid); 
		if (sem_unlink(buffer) == (-1)) {
			if (errno == ENOENT) printf("Semaphore supprime\n");
			else perror("Pb suppression semaphore\n");
		}

		printf("Suppression fichier process %d\n",joueurs.tab[i].pid);		
		sprintf(buffer,"rm -rf \"%s/%d\"",PATH_RUN,joueurs.tab[i].pid);
		printf1("%s\n",buffer);
		CHECK_IF(system(buffer),-1,"supression contenu de .../run");
	}
}

char * lireFichierJoueur (int pid) {
	FILE * fp; 
	char buffer[MAXLEN];
	sprintf(buffer,"%s/%d",PATH_RUN, pid);
	CHECK_IF(fp = fopen(buffer,"r"),NULL, "fopen");
	fgets(buffer,MAXLEN,fp);  			
	fclose(fp);
	return strdup(buffer);
}


void ecrireFichierJoueur(int pid, char * s) {
	// on n'ajoute pas de saut de ligne 
	FILE * fp; 
	char buffer[MAXLEN];
	sprintf(buffer,"%s/%d",PATH_RUN, pid);
	CHECK_IF(fp = fopen(buffer,"w"),NULL, "fopen");
	fprintf(fp,"%s",s);  			
	fclose(fp);
}

void writeDirect(char * filename, T_Position p, T_Partie g) {
	FILE * fp; 
	int i;

	T_Score s = evaluerScore(p); 


	// Le fichier ne devrait-il pas être préfixé par le PID du process ? 
	// On devrait lui faire porter le nom du groupe, passé en ligne de commandes
	fp = fopen(filename,"w"); 

	// On écrit le trait 
	fprintf(fp, "traiterJson({\n%s:%d,\n",STR_TURN,p.trait);

	fprintf(fp, "%s:\"%s\",\n",STR_J,g.nomJ); 
	fprintf(fp, "%s:\"%s\",\n",STR_R,g.nomR); 
	fprintf(fp, "%s:%d,\n",STR_RONDE,g.ronde); 

	// On écrit les scores
	fprintf(fp, "%s:%d,\n",STR_SCORE_J,s.nbJ); 
	fprintf(fp, "%s:%d,\n",STR_SCORE_J5,s.nbJ5); 
	fprintf(fp, "%s:%d,\n",STR_SCORE_R,s.nbR); 
	fprintf(fp, "%s:%d,\n",STR_SCORE_R5,s.nbR5);

	// Les colonnes // attention aux "," ?
	fprintf(fp, "%s:[\n",STR_COLS);

	// première 
	fprintf(fp, "\t{%s:%d, %s:%d}",STR_NB,p.cols[0].nb, STR_COULEUR,p.cols[0].couleur); 	

	// autres
	for(i=1;i<NBCASES;i++) {
		fprintf(fp, ",\n\t{%s:%d, %s:%d}",STR_NB,p.cols[i].nb, STR_COULEUR,p.cols[i].couleur); 	
	}
	fprintf(fp,"\n]\n"); // avec ou sans "," ? 

	fprintf(fp,"});");

	fclose(fp);
}


void writeFeuille(char * filename, T_Partie p) {
	FILE * fp; 
	int i;

	// Le fichier ne devrait-il pas être préfixé par le PID du process ? 
	// On devrait lui faire porter le nom du groupe, passé en ligne de commandes
	fp = fopen(filename,"w"); 

	fprintf(fp, "traiterJson({\n"); 

	// Les joueurs 
	fprintf(fp, "%s:\"%s\",\n",STR_J,p.nomJ); 
	fprintf(fp, "%s:\"%s\",\n",STR_R,p.nomR); 
	fprintf(fp, "%s:%d,\n",STR_RONDE,p.ronde);
	fprintf(fp, "%s:\"%s\",\n",STR_RESULTAT,SRESULTAT(p.vainqueur));
		

	// On écrit les scores
	fprintf(fp, "%s:%d,\n",STR_SCORE_J,p.score.nbJ); 
	fprintf(fp, "%s:%d,\n",STR_SCORE_J5,p.score.nbJ5); 
	fprintf(fp, "%s:%d,\n",STR_SCORE_R,p.score.nbR); 
	fprintf(fp, "%s:%d,\n",STR_SCORE_R5,p.score.nbR5);

	// Les coups // attention aux "," ?
	fprintf(fp, "%s:[\n",STR_COUPS);

	
	// premier (il n'existe peut-être pas en cas d'exempt...
	if (p.feuille.nb>0) 
		fprintf(fp, "\t{%s:%d,%s:%d}",STR_ORIGINE,p.feuille.coups[0].origine,STR_DESTINATION,p.feuille.coups[0].destination); 	

	// autres
	for(i=1;i<p.feuille.nb;i++) {
		fprintf(fp, ",\n\t{%s:%d, %s:%d}",STR_ORIGINE,p.feuille.coups[i].origine,STR_DESTINATION,p.feuille.coups[i].destination); 	
	}

	fprintf(fp,"\n]});");

	fclose(fp);
}



void writeTournoi(char * filename, T_TabParties tournoi, T_TabJoueurs joueurs) {
	FILE * fp; 
	int i,j, r;

	// Le fichier ne devrait-il pas être préfixé par le PID du process ? 
	// On devrait lui faire porter le nom du groupe, passé en ligne de commandes
	fp = fopen(filename,"w"); 

	fprintf(fp, "traiterJson({%s:[\n", STR_JOUEURS); 

	//premier
	fprintf(fp, "\t{%s:\"%s\",%s:%.2f}",STR_NOM,joueurs.tab[0].nom, STR_SCORE,joueurs.tab[0].nbPoints); 	

	//autres 
	for(j=1;j<joueurs.nb;j++) {
		fprintf(fp, ",\n\t{%s:\"%s\",%s:%.2f}",STR_NOM, joueurs.tab[j].nom, STR_SCORE, joueurs.tab[j].nbPoints); 	
	}

	fprintf(fp,"\n],\n");
	fprintf(fp, "%s:[\n", STR_RONDES); 

	//premiere ronde
	fprintf(fp, "\t{%s:%d,\n",STR_RONDE, 1);
	fprintf(fp, "\t%s:[\n", STR_PARTIES);

	//premiere
	fprintf(fp, "\t\t{");
	fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(tournoi.parties[0].status));
	fprintf(fp, "%s:\"%s\",",STR_J,tournoi.parties[0].nomJ); 
	fprintf(fp, "%s:\"%s\",",STR_R,tournoi.parties[0].nomR);
	fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(tournoi.parties[0].vainqueur));
	fprintf(fp, "%s:%d,",STR_SCORE_J,tournoi.parties[0].score.nbJ); 
	fprintf(fp, "%s:%d,",STR_SCORE_J5,tournoi.parties[0].score.nbJ5); 
	fprintf(fp, "%s:%d,",STR_SCORE_R,tournoi.parties[0].score.nbR); 
	fprintf(fp, "%s:%d",STR_SCORE_R5,tournoi.parties[0].score.nbR5);
	fprintf(fp, "}");

	//autres
	for(i=1 ;i<(joueurs.nb/2);i++){
		fprintf(fp, ",\n\t\t{");
		fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(tournoi.parties[i].status));
		fprintf(fp, "%s:\"%s\",",STR_J,tournoi.parties[i].nomJ); 
		fprintf(fp, "%s:\"%s\",",STR_R,tournoi.parties[i].nomR);
		fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(tournoi.parties[i].vainqueur));
		fprintf(fp, "%s:%d,",STR_SCORE_J,tournoi.parties[i].score.nbJ); 
		fprintf(fp, "%s:%d,",STR_SCORE_J5,tournoi.parties[i].score.nbJ5); 
		fprintf(fp, "%s:%d,",STR_SCORE_R,tournoi.parties[i].score.nbR); 
		fprintf(fp, "%s:%d",STR_SCORE_R5,tournoi.parties[i].score.nbR5);
		fprintf(fp, "}");
	}

	fprintf(fp, "\n\t]}");

	// autres rondes
	for(r=1;r< tournoi.nbRondes; r++)
	{
		fprintf(fp, ",\n\t{%s:%d,\n",STR_RONDE,r+1);
		fprintf(fp, "\t%s:[\n", STR_PARTIES);

		//premiere
		fprintf(fp, "\t\t{");
		fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(tournoi.parties[(joueurs.nb/2) * r].status)); 
		fprintf(fp, "%s:\"%s\",",STR_J,tournoi.parties[(joueurs.nb/2) * r].nomJ); 
		fprintf(fp, "%s:\"%s\",",STR_R,tournoi.parties[(joueurs.nb/2) * r].nomR);
		fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(tournoi.parties[(joueurs.nb/2) * r].vainqueur));
		fprintf(fp, "%s:%d,",STR_SCORE_J,tournoi.parties[(joueurs.nb/2) * r].score.nbJ); 
		fprintf(fp, "%s:%d,",STR_SCORE_J5,tournoi.parties[(joueurs.nb/2) * r].score.nbJ5); 
		fprintf(fp, "%s:%d,",STR_SCORE_R,tournoi.parties[(joueurs.nb/2) * r].score.nbR); 
		fprintf(fp, "%s:%d",STR_SCORE_R5,tournoi.parties[(joueurs.nb/2) * r].score.nbR5);
		fprintf(fp, "}");


		//autres
		for(i=(joueurs.nb/2) * r +1;i<(joueurs.nb/2) * (r+1);i++){
			fprintf(fp, ",\n\t\t{");
			fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(tournoi.parties[i].status)); 
			fprintf(fp, "%s:\"%s\",",STR_J,tournoi.parties[i].nomJ); 
			fprintf(fp, "%s:\"%s\",",STR_R,tournoi.parties[i].nomR);
			fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(tournoi.parties[i].vainqueur));
			fprintf(fp, "%s:%d,",STR_SCORE_J,tournoi.parties[i].score.nbJ); 
			fprintf(fp, "%s:%d,",STR_SCORE_J5,tournoi.parties[i].score.nbJ5); 
			fprintf(fp, "%s:%d,",STR_SCORE_R,tournoi.parties[i].score.nbR); 
			fprintf(fp, "%s:%d",STR_SCORE_R5,tournoi.parties[i].score.nbR5);
			fprintf(fp, "}");
		}

		fprintf(fp, "\n\t]}");
	}

	fprintf(fp,"\n]});");
	fclose(fp);
}

















