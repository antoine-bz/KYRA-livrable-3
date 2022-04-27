/********** Moteur de tournoi : duel ************/
/* 
Meme mécanisme que le moteur de tournoi, pour des parties à deux 
Avec ou sans chronomètre
*/

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

T_TabParties duel; 

T_TabJoueurs joueurs;
int delaiChoixCoup = DELAICHOIXCOUP; 

T_Score gainJaune = {1,0,0,0}; 
T_Score gainRouge = {0,0,1,0}; 
int nbJoueurs; 
int tempsJ1; int tempsJ2; 
int delaiChoixCoupJaune; int delaiChoixCoupRouge; 

sem_t semAttente; //tom 2/06/2019

T_Partie partie;

int main(int argc, char ** argv)
{
	struct sigaction newact;	// déclaration de la structure 
	char buffer[MAXLEN];
	char bNom[MAXLEN];

	FILE * fp; 
	DIR * dp;
	struct dirent * pDir; 
	int r,i;
	int ronde =1;
	char c = 'y'; 
	char saisie[MAXLEN];

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

	printf("\n------------------ Appuyez sur ENTREE pour démarrer ------------------\n"); 
	getchar();

	// Il faudrait pouvoir jouer autant de parties que l'on veut 
	// entre ces deux joueurs 
	
	if (joueurs.nb %2 ==1) {
		fprintf(stderr, "Nombre de joueurs impair : le duel ne peut avoir lieu\n");
		exit(1);
	}

	printf("----------------------------- Demarrage ------------------------------\n");

	printf("Joueur 1 : %s\n", joueurs.tab[0].nom); 
	printf("Temps de réflexion par coup [%d] : ", delaiChoixCoup);
	fgets(saisie, MAXLEN, stdin);
	if (strcmp(saisie,"\n") == 0) tempsJ1 = delaiChoixCoup; 
	else tempsJ1 = atoi(saisie);

	// tom 2/06/2019
	if (tempsJ1 == 0) {
		printf0("Initialiation sem. attente\n");
		sem_init(&(semAttente),0,0);		
		printf("Attention : un temps nul requiert un joueur humain particulier\n");
	}

	printf("Joueur 2 : %s\n", joueurs.tab[1].nom);
	printf("Temps de réflexion par coup [%d] : ", delaiChoixCoup); // tom 2/06/2019
	fgets(saisie, MAXLEN, stdin);
	if (strcmp(saisie,"\n") == 0) tempsJ2 = delaiChoixCoup;
	else tempsJ2 = atoi(saisie);

	// tom 2/06/2019
	if (tempsJ2 == 0) {
		printf0("Initialiation sem. attente\n");
		sem_init(&(semAttente),0,0);
		printf("Attention : un temps nul requiert un joueur humain particulier\n");
	}


	printf("Joueurs : %s (%d sec/coup) -  %s (%d sec/coup)\n", joueurs.tab[0].nom, tempsJ1, joueurs.tab[1].nom, tempsJ2);
	printf("\n------------------ Appuyez sur ENTREE pour démarrer ------------------\n"); 
	getchar(); 


	sprintf(bNom,"%s/tournoi.js",PATH_WEB);


	while( (c != 'n') && (c != 'N')) {

		initPartie(ronde, 1-(ronde%2), (ronde%2)); // partie ronde 1 entre joueur 0 et 1
		// Lors d'une ronde impaire (1) J0 joue avec jaunes 
		if (ronde%2) delaiChoixCoupJaune=tempsJ1; else delaiChoixCoupJaune=tempsJ2; 
		if (ronde%2) delaiChoixCoupRouge=tempsJ2; else delaiChoixCoupRouge=tempsJ1; 

		writeDuel(bNom, duel, joueurs);

		printf1("Création sem. partie %d\n",ronde);
		sem_init(&(partie.sem),0,0);
		partie.status = ENCOURS; 
		CHECK_DIF((pthread_create(&(partie.thread), NULL,thread_partie, (void*)ronde)), 0, "pthread_create");

		sem_wait(&(partie.sem));
		CHECK_DIF((pthread_join(partie.thread, NULL)),0,"pthread_join"); 

		printf("Continuer le duel [Y-n] ? "); 
		c = getchar();

		listerDuel();
		writeDuel(bNom, duel, joueurs); 

		ronde++;
	}

	printf("---------------------------- Fin du duel -----------------------------\n");
	killJoueurs(); 
	
	exit(0);
}

void initPartie(octet ronde, int iJ,  int iR) {
	T_Partie * nextP; 
	partie.iJ= iJ; 
	partie.pidJ= joueurs.tab[iJ].pid; 
	strcpy(partie.nomJ, joueurs.tab[iJ].nom);

	partie.iR= iR; 
	partie.pidR= joueurs.tab[iR].pid; 
	strcpy(partie.nomR, joueurs.tab[iR].nom);

	partie.status = ENCOURS; ; 
	partie.ronde = ronde; 

	partie.feuille.nb = 0; 
	partie.nbCoups = 0;

	printf3("ajout partie %d entre %s et %s\n",ronde,joueurs.tab[iJ].nom, joueurs.tab[iR].nom);

	// il faudrait ajouter cette partie aux parties du duel	
	nextP = (T_Partie *) malloc(ronde * sizeof(T_Partie));
	
	// On copie les parties déjà réalisées
	memcpy(nextP,duel.parties, (ronde-1) * sizeof(T_Partie)); 

	// On met à jour les méta-données 
	duel.nbRondes = ronde;
	duel.nbParties = ronde;
	free(duel.parties);
	duel.parties = nextP; 

	// On ajoute la nouvelle partie 
	duel.parties[ronde-1] = partie;
}


void terminerDuel(T_Partie * p, T_Score score) {

	char bNom[MAXLEN];
	sprintf(bNom,"%s/p%d.js",PATH_WEB, (p->ronde)-1);

	p->status = TERMINE; 
	p->score = score;

	if ( score.nbJ > score.nbR) p->vainqueur = JAU;
	else if ( score.nbR > score.nbJ) p->vainqueur = ROU;
	else if ( score.nbJ5 > score.nbR5) p->vainqueur = JAU;
	else if ( score.nbR5 > score.nbJ5) p->vainqueur = ROU;
	else p->vainqueur = EGALITE;

	if (p->vainqueur == JAU) joueurs.tab[p->iJ].nbPoints++;
	else if (p->vainqueur == ROU) joueurs.tab[p->iR].nbPoints++; 
	else {
		joueurs.tab[p->iJ].nbPoints += 0.5; 
		joueurs.tab[p->iR].nbPoints += 0.5; 
	}

	writeFeuille(bNom,*p); 

	// On vient de finaliser cette partie, on la stocke dans le tournoi
	duel.parties[p->ronde-1] = *p;
}


void * thread_partie(void * i) {
	int ronde = (int) i; 
	char bNom[MAXLEN]; 

	printf("Lancement thread de gestion de la partie %d\n", ronde); 
	sprintf(bNom,"%s/%d.js",PATH_WEB, ronde-1);

	printf1("Indication couleur au joueur %d\n",partie.pidJ); 
	ecrireFichierJoueur(partie.pidJ, "j");
	kill(partie.pidJ, SIGUSR1);

	printf1("attente confirmation couleur par joueur %d ...\n", partie.pidJ);
	sem_wait(joueurs.tab[partie.iJ].pSem);  
	printf0("... ok\n");

	printf1("Indication couleur au joueur %d\n",partie.pidR); 
	ecrireFichierJoueur(partie.pidR, "r");
	kill(partie.pidR, SIGUSR1);

	printf1("attente confirmation couleur par joueur %d ...\n", partie.pidR);
	sem_wait(joueurs.tab[partie.iR].pSem);  
	printf0("... ok\n");

	printf("Debut partie %d\n",ronde);

	// Chaque thread va gérer une partie, comme avalam_standalone
	T_Position p = getPositionInitiale();
	writeDirect(bNom, p, partie);

	T_ListeCoups l = getCoupsLegaux(p);
	T_Score score ;
	char * s;
	int indexCoupJoue; 

	while(l.nb > 0) {
		// On indique à qui c'est le tour 
		if (p.trait == JAU) {
			printf("Indication de son tour au joueur %d\n", partie.pidJ);
			kill(partie.pidJ, SIGUSR1);
			printf1("attente confirmation par joueur %d ...\n", partie.pidJ);
			sem_wait(joueurs.tab[partie.iJ].pSem);  
			printf0("... ok\n");

			// On attend une durée fonction du joueur au trait 
			// sleep(delaiChoixCoupJaune); // tom 2/06/2019
			if (delaiChoixCoupJaune) sleep(delaiChoixCoupJaune);
			else {
				printf("Attente joueur Jaune\n");
				printf0("Attente sem. attente\n");
				sem_wait(&(semAttente));
			}
			// Fin tom 2/06/2019
		}
		else {
			printf("Indication de son tour au joueur %d\n", partie.pidR);
			kill(partie.pidR, SIGUSR1);
			printf1("attente confirmation par joueur %d ...\n", partie.pidR);
			sem_wait(joueurs.tab[partie.iR].pSem);  
			printf0("... ok\n");	

			// On attend une durée fonction du joueur au trait 
			//sleep(delaiChoixCoupRouge); // tom 2/06/2019
			if (delaiChoixCoupRouge) sleep(delaiChoixCoupRouge);
			else {
				printf("Attente joueur Rouge\n");
				printf0("Attente sem. attente\n");
				sem_wait(&(semAttente));
			}		
			// Fin tom 2/06/2019
		}
				
		// On lui dit d'arrêter en lui réenvoyant un signal
		// On récupère son coup et on le joue 
		if (p.trait == JAU) {
			printf1("attente confirmation coup par joueur %d ...\n", partie.pidJ);
			kill(partie.pidJ, SIGUSR1);
			sem_wait(joueurs.tab[partie.iJ].pSem);  
			printf0("... ok\n");

			s = lireFichierJoueur(partie.pidJ); 
		}
		else {
			printf1("attente confirmation coup par joueur %d ...\n", partie.pidR);
			kill(partie.pidR, SIGUSR1);
			sem_wait(joueurs.tab[partie.iR].pSem);  
			printf0("... ok\n");

			s = lireFichierJoueur(partie.pidR);
		}

		
		printf1("Nb coups possibles = %d ",l.nb);
		printf1("Contenu fichier = %s ",s);
		indexCoupJoue = atoi(s);
		printf1("Coup converti = %d\n", indexCoupJoue);
 
		if ((indexCoupJoue>= 0) && (indexCoupJoue<l.nb)) {
			printf("Coup %d\n", partie.nbCoups);
			printf("%s a choisi le coup %d\n",COLNAME(p.trait), indexCoupJoue );
			printf("Soit : %d -> %d\n", l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination);
			

			partie.feuille.coups[partie.nbCoups].origine = l.coups[indexCoupJoue].origine; 
			partie.feuille.coups[partie.nbCoups].destination = l.coups[indexCoupJoue].destination; 
			partie.feuille.nb++; 
			partie.nbCoups++; 

			p = jouerCoup(p, l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination) ;
			l = getCoupsLegaux(p);
		} else {
			fprintf(stderr,"SAISIE INCORRECTE ! \n"); 
			fprintf(stderr,"FIN DE LA PARTIE %d\n",ronde);
			return NULL; // On quitte le thread 
		}

		// On informe l'adversaire du coup en préparant un fichier pour lui 
		if (p.trait == JAU) {
			ecrireFichierJoueur(partie.pidJ,s) ;
		}
		else {
			ecrireFichierJoueur(partie.pidR,s) ;
		}

		free(s);

		// on enregistre la nouvelle position 
			writeDirect(bNom, p, partie);

		// et on recommence 
	}

	printf("Partie terminée !\n");

	// On envoie SIGUSR2 aux deux processus
	// Il faut attendre que les joueurs accusent réception de ces signaux 
	// Pour que l'on soit sûr que la partie est bien terminée 
	kill(partie.pidJ, SIGUSR2);
	// On doit se synchroniser avec un processus, correspondant à un joueur 
	// On crée autant de sem. que de joueurs 
	printf1("attente terminaison partie par joueur %d ...\n", partie.pidJ);
	sem_wait(joueurs.tab[partie.iJ].pSem);  
	printf0("... ok\n");


	kill(partie.pidR, SIGUSR2);
	printf1("attente terminaison partie par joueur %d ...\n", partie.pidR);
	sem_wait(joueurs.tab[partie.iR].pSem);  
	printf0("... ok\n");

	score = evaluerScore(p);
	terminerDuel(&partie,score);

	// On libère le sémaphore associé 
	sem_post(&(partie.sem)); 

	printf("Fin Thread %d\n",ronde);
	pthread_exit(NULL);
}

void listerPartie(T_FeuillePartie f) {
	octet i; 
	for(i=0;i<f.nb;i++) {
		printf("%2d. %2d->%2d\n", i, f.coups[i].origine,  f.coups[i].destination); 
	}
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
		// tom 2/06/2019
		if ((delaiChoixCoupJaune==0) || (delaiChoixCoupRouge==0)) {
			printf0("Attente terminée\n");
			sem_post(&(semAttente));
		} 
		else {
			fprintf(stderr,"\tSignal SIGUSR1 reçu de %d\n", info->si_pid); 
			fprintf(stderr,"\tNE DEVRAIT PAS ARRIVER\n"); 
		}

		break;

		case SIGUSR2 : 
			printf("\tArrivee d'un nouveau joueur : %d\n", info->si_pid); 
			// Peut-être faudrait-il protéger cette zone contre des accès concurrents ?

			if (joueurs.nb == 2) {
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

void writeDuel(char * filename, T_TabParties duel, T_TabJoueurs joueurs) {
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
	fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(duel.parties[0].status));
	fprintf(fp, "%s:\"%s\",",STR_J,duel.parties[0].nomJ); 
	fprintf(fp, "%s:\"%s\",",STR_R,duel.parties[0].nomR);
	fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(duel.parties[0].vainqueur));
	fprintf(fp, "%s:%d,",STR_SCORE_J,duel.parties[0].score.nbJ); 
	fprintf(fp, "%s:%d,",STR_SCORE_J5,duel.parties[0].score.nbJ5); 
	fprintf(fp, "%s:%d,",STR_SCORE_R,duel.parties[0].score.nbR); 
	fprintf(fp, "%s:%d",STR_SCORE_R5,duel.parties[0].score.nbR5);
	fprintf(fp, "}");

	//autres
	for(i=1 ;i<(joueurs.nb/2);i++){
		fprintf(fp, ",\n\t\t{");
		fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(duel.parties[i].status));
		fprintf(fp, "%s:\"%s\",",STR_J,duel.parties[i].nomJ); 
		fprintf(fp, "%s:\"%s\",",STR_R,duel.parties[i].nomR);
		fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(duel.parties[i].vainqueur));
		fprintf(fp, "%s:%d,",STR_SCORE_J,duel.parties[i].score.nbJ); 
		fprintf(fp, "%s:%d,",STR_SCORE_J5,duel.parties[i].score.nbJ5); 
		fprintf(fp, "%s:%d,",STR_SCORE_R,duel.parties[i].score.nbR); 
		fprintf(fp, "%s:%d",STR_SCORE_R5,duel.parties[i].score.nbR5);
		fprintf(fp, "}");
	}

	fprintf(fp, "\n\t]}");

	// autres rondes
	for(r=1;r< duel.nbRondes; r++)
	{
		fprintf(fp, ",\n\t{%s:%d,\n",STR_RONDE,r+1);
		fprintf(fp, "\t%s:[\n", STR_PARTIES);

		//premiere
		fprintf(fp, "\t\t{");
		fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(duel.parties[(joueurs.nb/2) * r].status)); 
		fprintf(fp, "%s:\"%s\",",STR_J,duel.parties[(joueurs.nb/2) * r].nomJ); 
		fprintf(fp, "%s:\"%s\",",STR_R,duel.parties[(joueurs.nb/2) * r].nomR);
		fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(duel.parties[(joueurs.nb/2) * r].vainqueur));
		fprintf(fp, "%s:%d,",STR_SCORE_J,duel.parties[(joueurs.nb/2) * r].score.nbJ); 
		fprintf(fp, "%s:%d,",STR_SCORE_J5,duel.parties[(joueurs.nb/2) * r].score.nbJ5); 
		fprintf(fp, "%s:%d,",STR_SCORE_R,duel.parties[(joueurs.nb/2) * r].score.nbR); 
		fprintf(fp, "%s:%d",STR_SCORE_R5,duel.parties[(joueurs.nb/2) * r].score.nbR5);
		fprintf(fp, "}");


		//autres
		for(i=(joueurs.nb/2) * r +1;i<(joueurs.nb/2) * (r+1);i++){
			fprintf(fp, ",\n\t\t{");
			fprintf(fp, "%s:\"%s\",",STR_STATUT,STATUTNAME(duel.parties[i].status)); 
			fprintf(fp, "%s:\"%s\",",STR_J,duel.parties[i].nomJ); 
			fprintf(fp, "%s:\"%s\",",STR_R,duel.parties[i].nomR);
			fprintf(fp, "%s:\"%s\",",STR_RESULTAT,SRESULTAT(duel.parties[i].vainqueur));
			fprintf(fp, "%s:%d,",STR_SCORE_J,duel.parties[i].score.nbJ); 
			fprintf(fp, "%s:%d,",STR_SCORE_J5,duel.parties[i].score.nbJ5); 
			fprintf(fp, "%s:%d,",STR_SCORE_R,duel.parties[i].score.nbR); 
			fprintf(fp, "%s:%d",STR_SCORE_R5,duel.parties[i].score.nbR5);
			fprintf(fp, "}");
		}

		fprintf(fp, "\n\t]}");
	}

	fprintf(fp,"\n]});");
	fclose(fp);
}




void listerDuel(){
	int i,j, r; 
	printf("------------------ TOURNOI -----------------\n");
	printf("Nb parties : %d\n", duel.nbRondes);
	for(r=0;r< duel.nbRondes; r++)
	{
	printf("------------------ Partie %d ----------------\n", r+1);
		for(i=(joueurs.nb/2) * r ;i<(joueurs.nb/2) * (r+1);i++){
			printf("%d : ", i);			
			if (duel.parties[i].status != TERMINE) {
				printf("[%s] - [%s]\n", duel.parties[i].nomJ,duel.parties[i].nomR);			
			} else {
				printf("[%d(%d) %s] ", duel.parties[i].score.nbJ, duel.parties[i].score.nbJ5, duel.parties[i].nomJ);
				if (duel.parties[i].vainqueur == EGALITE) printf("1/2 - 1/2");
				else if (duel.parties[i].vainqueur == JAU) printf("1 - 0");
				else printf("0 - 1");
				printf(" [%s %d(%d)]\n", duel.parties[i].nomR,duel.parties[i].score.nbR, duel.parties[i].score.nbR5);
			}
		}
	}

	for(j=0;j<joueurs.nb;j++) {
		printf("Joueur %d : %s : %.1f points\n", j, joueurs.tab[j].nom, joueurs.tab[j].nbPoints);
	}
} 














