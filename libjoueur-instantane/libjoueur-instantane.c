/********* Moteur de tournoi : joueur ***************/

/*
Architecture : 
Le programme principal reçoit les signaux, et gère la partie
A chaque coup, il déclenche un processus fils, dont il conserve le PID
Il tue ce processus avant de passer au coup suivant
NB : on passe maintenant par une architecture multiprocess, 
car il y a avait des soucis d'annulation de threads

Il faudrait que la fonction ecrireIndexCoup refuse de saisir le coup d'un joueur s'il n'est pas envoyé par le processus s'occupant du coup actif. 
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h>           /* Pour les constantes O_* */
#include <sys/stat.h>        /* Pour les constantes des modes */
#include <semaphore.h>

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#include "avalam.h"
#include "moteur.h"
#include "topologie.h"
#include "duel-instantane.h"



T_TabJoueurs joueurs; 
char * nom ="";

int pidMoteur; 
int myPid; // pid du process de gestion d'un joueur 
int pidCurrentProcessCoup=-1; // pid du process de gestion du coup en cours 

sem_t * pSem; // sem. de synchro avec le moteur de tournoi 

T_Etat etat; // etat de l'automate de gestion d'un joueur 

// meta-donnees sur la partie en cours 
octet couleur; 
int numCoup ;
T_Position p; 
T_Score s ;
T_ListeCoups l; 

// Fonction externe devant être définie par les étudiants pour chaque joueur 
extern void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups);

void whoamj() {
	if (couleur == ROU) 
		printf("\t\t\t\t\t\t\t%s (%d pour %d - coup %d) : " , nom, getpid(),myPid, numCoup); 
	else  
		printf("%s (%d pour %d - coup %d ) : ", nom, getpid(), myPid, numCoup);
}

void whoj(char * s) {
	if (couleur == ROU) 
		printf("\t\t\t\t\t\t\t%s (%d pour %d - coup %d) : %s\n" , nom, getpid(),myPid, numCoup, s); 
	else  
		printf("%s (%d pour %d - coup %d) : %s\n", nom, getpid(),myPid, numCoup, s);
}

// Process 'process_coup'

void process_coup(void) {	
	// Ne doit etre sensible à aucun signal SIGUSR1 et SIGUSR2, contrairement à son père 
	// Par contre, sensible au SIGINT 
	// Au moment où ce process se déclenche, on connait la couleur et le numéro du coup  

	struct sigaction newact;	
	newact.sa_handler = SIG_IGN;	
	// On interdit à ce process d'être sensible à un signal
	CHECK_IF(sigaction(SIGUSR1, &newact, NULL),-1,"problème sigaction");
	CHECK_IF(sigaction(SIGUSR2, &newact, NULL),-1,"problème sigaction");
	
	whoamj(); printf("Lancement process %d de recherche d'un coup(%d) (coul=%d)\n", getpid(), numCoup, couleur); 

	// On écrit dans le fichier pid le coup choisi, par défaut le premier
	if (l.nb>0) {	
		whoamjd(); printf2("Choix coup par défaut %d -> %d\n",l.coups[0].origine,l.coups[0].destination); 
		ecrireIndexCoup(0); 
	}
	else {
		whoj("Plus aucun coup jouable\n");
		fprintf(stderr,"NE DEVRAIT PAS ARRIVER\n");
	}

	// On confirme le lancement au moteur
	// Le process bénéficie des descripteurs ouverts par son père...
	// Il peut incrémenter le sémaphore
	sem_post(pSem);

	// Appel de la fonction des étudiants
	choisirCoup(p, l);

	// TODO: il faudrait utiliser des sémaphores...

	whojd("Fin de la fonction utilisateur\n");
	whojd("Mise en pause en attente de terminaison\n");

	pause();
	
	// On quitte le programme
	whojd("Fin du process de choix du coup\n");
	exit(0);	  
}


int main(int argc, char ** argv)
{
	struct sigaction newact;	// déclaration de la structure 
	char buffer[MAXLEN];
	FILE * fp; 

	myPid = getpid();

	srand(time(NULL));

	// Recup NOM du joueur 
	if (argc != 2) {
		fprintf(stderr,"Il faut saisir votre nom !\n"); 
		return 1;
	}

	// Déclaration d'un sémaphore pour gérer les acks de ce joueur
	printf0("Creation semaphore\n");
	sprintf(buffer, "/j%d.avalam", myPid);

	printf1("nom Sem : %s\n", buffer);
	CHECK_IF((pSem = sem_open(buffer, O_CREAT | O_EXCL, S_IRWXU, 0)), SEM_FAILED, "sem_open"); 

	printf0("Creation deroutements\n");
	newact.sa_sigaction = deroute;	
	CHECK_IF(sigemptyset(&newact.sa_mask),-1,"problème sigemptyset");
	newact.sa_flags = SA_SIGINFO;

	// Si SA_SIGINFO est indiqué dans sa_flags, alors sa_sigaction (plutôt que sa_handler) 
	// pointe vers le gestionnaire  de  signal pour signum.

	// on installe le handler deroute
	// A partir de maintenant, toute occurence d'un signal SIGUSR1 déclenche l'appel de la fonction deroute  
	CHECK_IF(sigaction(SIGUSR1, &newact, NULL),-1,"problème sigaction");
	CHECK_IF(sigaction(SIGUSR2, &newact, NULL),-1,"problème sigaction");
	CHECK_IF(sigaction(SIGINT, &newact, NULL),-1,"problème sigaction");

	printf0("Recup nom joueur\n");
	nom = argv[1];
	printf("Process joueur : pid=%d, nom=%s\n", myPid, nom);

	// On enregistre le PID du joueur dans le fichier run/"pid"
	printf0("Enregistrement PID joueur\n");
	sprintf(buffer,"%s/%d",PATH_RUN, myPid);
	CHECK_IF(fp = fopen(buffer,"w"),NULL, "fopen");
	fprintf(fp,"%s",nom); 
	fclose(fp);

	// recup PID moteur
	sprintf(buffer,"%s/moteur.pid",PATH_RUN);
	CHECK_IF(fp = fopen(buffer,"r"),NULL, "fopen");
	fgets(buffer,MAXLEN,fp); 
	pidMoteur = atoi(buffer);
	printf("PID moteur = %d\n", pidMoteur);
	fclose(fp);

	etat = ATTENTE_APPARIEMENT; 

	// envoi signal au moteur pour se présenter 
	kill(pidMoteur, SIGUSR2);

	while(1) {
		pause();
		printf0("On traverse pause depuis main\n");
	}

	// TODO : supprimer le fichier lié au process... 
	// C'est fait automatiquement lorsqu'on relance 
	exit(0);
}


void deroute (int signal_number, siginfo_t * info, void * context)
{
	int i;
	char * col;
	char * s; 
	int indexCoupJoue; 
	void * res; 
	char buffer[MAXLEN];

	whoamjd();printf1("Signal reçu, PID emetteur : %d !!\n",info->si_pid);

	switch (signal_number) {
		case SIGUSR1 : 
			whojd("Signal SIGUSR1 reçu.\n"); 

			switch(etat) {
				case ATTENTE_APPARIEMENT : 
					// Une nouvelle partie va commencer
					col = lireFichierJoueur(myPid);
					whoamjd(); printf1("Col lue : .%s.\n",col);
					if (strcmp(col,"j") == 0) couleur = JAU; 
					else couleur = ROU; 

					whoj("Nouvelle partie");
					whoamj();printf("Je suis %s\n", COLNAME(couleur));

					// On initialise la partie 
					p = getPositionInitiale();
					l = getCoupsLegaux(p);
					numCoup = 0; 

					// On confirme notre couleur au moteur	
					sem_post(pSem);

					// On ne commence pas tout de suite, on attend que le moteur nous renvoie un signal de départ					
					if (couleur == JAU) {
						etat = ATTENTE_DEBUT;
						whoj("C'est mon tour !");
					}
					else {
						etat = ATTENTE_TOUR_ADVERSAIRE;
						whoj("C'est le tour de mon adversaire !"); 
					}
				break;

				case ATTENTE_DEBUT :
					// On commence depuis la position initiale 
					// On lance un processus de recherche du prochain coup 
					// Ce processus devra écrire régulièrement le coup choisi dans un fichier portant le nom du pid du process 
					// On devra le terminer lorsqu'on recevra un signal SIGUSR1
					etat = MON_TOUR;
					whoj("La partie commence !");
					
					CHECK_IF((pidCurrentProcessCoup = fork()),-1, "fork");
					if (pidCurrentProcessCoup == 0) {
						// Je suis ton fils !
						process_coup();
						whojd("Terminaison du fils choix coup depuis traitement ATTENTE_DEBUT\n"); 
						exit(0); 
					} 

					// Le process confirmera lui-même au moteur
				break;

				case ATTENTE_TOUR_ADVERSAIRE : 
					// On reçoit un signal : il faut jouer le coup sélectionné par l'adversaire sur la position

					// Celui-ci est contenu dans le fichier pid.
					s = lireFichierJoueur(myPid); 
					whoamj();printf("Saisie adversaire : %s\n", s); 
					indexCoupJoue = atoi(s);
					free(s); 

					if ((indexCoupJoue>= 0) && (indexCoupJoue<l.nb)) {
						whoamj(); printf("L'adversaire a choisi le coup %d\n",indexCoupJoue );
						whoamj(); printf("Soit : %d -> %d\n", l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination);
						p = jouerCoup(p, l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination) ;
						l = getCoupsLegaux(p);
						numCoup++;
					} else {
						whoamj(); fprintf(stderr,"SAISIE INCORRECTE ! \n"); 
						exit(1); 
					}
					
					etat = MON_TOUR;
					whoj("C'est mon tour !");

					CHECK_IF((pidCurrentProcessCoup = fork()),-1, "fork");
					if (pidCurrentProcessCoup == 0) {
						// Je suis ton fils !
						process_coup();
						whoamj();printf("Terminaison du fils depuis traitement ATTENTE_TOUR_ADVERSAIRE\n"); 
						exit(0); 
					} else {
						// Je suis ton père : rien à faire de plus... 
					}

					// Le process confirmera lui-même au moteur
				break;

				case MON_TOUR  : 
					// On reçoit un signal : il faut jouer le coup sélectionné par le moteur sur la position
					whoj("Temps de calcul atteint\n");

					// Il faut d'abord tuer le process de calcul du coup
					if (pidCurrentProcessCoup!= -1) {
						whoamjd();printf1("Demande de terminaison du processus fils %d\n",pidCurrentProcessCoup); 
						CHECK_IF(kill(pidCurrentProcessCoup, SIGINT),-1, "kill");
						// Attendre terminaison ? 
						whoamjd();printf1("Attente de terminaison du processus fils %d\n",pidCurrentProcessCoup); 
						waitpid(pidCurrentProcessCoup, NULL, 0);
						whoamjd();printf1("Terminaison du processus fils %d OK\n",pidCurrentProcessCoup);
						pidCurrentProcessCoup = -1;
					}


					s = lireFichierJoueur(myPid); 
					whoamjd();printf1("Saisie fonction choisirCoup : %s\n", s); 
					indexCoupJoue = atoi(s);
					free(s);

					if ((indexCoupJoue>= 0) && (indexCoupJoue<l.nb)) {
						whoamj(); printf("Je joue le coup %d\n",indexCoupJoue );
						whoamj(); printf("Soit : %d -> %d\n", l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination);
						p = jouerCoup(p, l.coups[indexCoupJoue].origine, l.coups[indexCoupJoue].destination) ;
						l = getCoupsLegaux(p);
						numCoup++;
					} else {
						whoamj(); fprintf(stderr,"SAISIE INCORRECTE ! \n"); 
						exit(0); 
					}
					
					// puis on attend l'adversaire 
					etat = ATTENTE_TOUR_ADVERSAIRE;
					whoj("C'est le tour de mon adversaire !");

					// On confirme au moteur qu'on est prêt pour la suite
					whojd("Libération sem\n");
					sem_post(pSem);

				break;
			}

		break;
		case SIGUSR2 : 

			whojd("Signal SIGUSR2 reçu.\n"); 
			whoj("Fin de la partie \n");

			// Il faut tuer le process de calcul du coup 
			// Normalement inutile - sauf cas d'abandons avant la fin de la partie...
			if (pidCurrentProcessCoup!= -1) {
				whoamjd();printf1("Demande de terminaison du processus fils %d\n",pidCurrentProcessCoup); 
				CHECK_IF(kill(pidCurrentProcessCoup, SIGINT),-1, "kill");
				// Attendre terminaison ? 
				whoamjd();printf1("Attente de terminaison du processus fils %d\n",pidCurrentProcessCoup); 
				waitpid(pidCurrentProcessCoup, NULL, 0);
				whoamjd();printf1("Terminaison du processus fils %d OK\n",pidCurrentProcessCoup);
				pidCurrentProcessCoup = -1;
			}

			etat = ATTENTE_APPARIEMENT; 

			sem_post(pSem);
			whojd("Libération sem\n");
	
		break;
		case SIGINT : 
			whoamjd();printf0("Signal SIGINT reçu.\n"); 
			whoamj();printf("Terminaison du programme %d\n", getpid());

			// Si c'est le process de gestion d'une partie, il faut supprimer le semaphore
			if (myPid == getpid()) {
				printf1("sem_close pour process %d\n",myPid);		
				CHECK_IF(sem_close(pSem), -1, "sem_close");

				printf1("sem_unlink pour process %d\n",myPid);		
				sprintf(buffer, "/j%d.avalam", myPid); 
				if (sem_unlink(buffer) == (-1)) {
					if (errno == ENOENT) printf0("Semaphore supprime\n");
					else perror("PB SUPPRESSION SEMAPHORE\n");
				}
			}
			exit(0); 
		break;
	}
}



// jamais aucun joueur ne doit ecrire dans un autre fichier que le sien 

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

void ecrireCoup(octet origine, octet destination) {
	char buffer[6]; 
	sprintf(buffer, "%02d:%02d", origine, destination);
	ecrireFichierJoueur(myPid, buffer); 
}

void ecrireIndexCoup(int index) {
	char buffer[6]; 

	if (index>=0 && index<l.nb) {
		sprintf(buffer, "%d", index);
		ecrireFichierJoueur(myPid, buffer); 
	} else {
		fprintf(stderr,"Index coup %d invalide : non pris en compte\n", index); 
	}
}


void finaliserCoup() {
	// On confirme notre coup au moteur	
	sem_post(pSem);
}


