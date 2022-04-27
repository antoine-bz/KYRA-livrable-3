#include <semaphore.h>
#include <signal.h>

/* TODO: doit être ajouté sur serveur...
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <semaphore.h>
#include <pthread.h>
*/

#define PATH_RUN "./run"
#define PATH_WEB "./web/data"
#define PID_FILE "moteur.pid"
#define MAXLEN 1000
#define MAXPROCESS 256
#define DELAICHOIXCOUP 30

// TODO: a supprimer 
// etat des threads des parties 
// #define INACTIF 0
// #define ACTIF 1


#define SRESULTAT(r) ((r==EGALITE) ? "1/2-1/2" : ((r==JAU) ? "1-0" : "0-1"))
#define STATUTNAME(r) ((r==ATTENTE) ? "attente" : ((r==ENCOURS) ? "encours" : "termine"))


typedef enum statusJ {ATTEND,JOUE} T_StatutJoueur; 
typedef enum statusP {ATTENTE,ENCOURS,TERMINE} T_StatutPartie; 
typedef enum etat {ATTENTE_APPARIEMENT,ATTENTE_DEBUT, ATTENTE_TOUR_ADVERSAIRE, MON_TOUR} T_Etat;

typedef unsigned char octet; 

typedef struct {
	int pid; 
	char nom[MAXLEN];
	T_StatutJoueur status;
	float nbPoints;
	//sem_t sem; // new 10/04
	sem_t * pSem; 
} T_ProcessJoueur;

typedef struct {
	octet nb;
	T_ProcessJoueur tab[MAXPROCESS];
} T_TabJoueurs; 

typedef struct {
	int nb; 
	T_Coup coups[48];// normalement 48-48/5
} T_FeuillePartie; 

typedef struct {
	int pidJ; char nomJ[MAXLEN]; int iJ; 
	int pidR; char nomR[MAXLEN]; int iR; 
	T_StatutPartie status;
	T_Score score;
	octet vainqueur; // JAU(1) ou ROU(2) ou EGALITE(0)
	octet ronde;
	sem_t sem; 
	pthread_t thread;
	octet nbCoups; 
	T_FeuillePartie feuille; 
	// Lui associer la position courante ? 
	// Lui associer un numéro  ? 
}	T_Partie;

typedef struct {
	octet nbRondes;
	int nbParties; // Modif 1/06/2019
	T_Partie * parties;
} T_TabParties; 


void deroute(int, siginfo_t *, void *);

void addJoueur(int pid, char * nom, T_StatutJoueur status); 
void initTournoi(); 
void listerTournoi(); 

void initPartie(octet ronde, int iJ,  int iR); // duel
void addPartie(int idP, octet ronde, int iJ,  int iR); 
void listerPartie(T_FeuillePartie f); 
void listerParties();
void listerDuel(); //duel
void terminerPartie(int numP, T_Score s); 
void terminerDuel(T_Partie * p, T_Score score); // duel
void * thread_partie(void * i);

char * lireFichierJoueur(int pid); 
void ecrireFichierJoueur(int pid, char * s); 
void ecrireCoup(octet origine, octet destination); 
void ecrireIndexCoup(int index); 
void writeDirect(char * filename, T_Position p, T_Partie g);
void writeFeuille(char * filename, T_Partie p) ; 
void writeTournoi(char * filename, T_TabParties tournoi, T_TabJoueurs joueurs); 
void writeDuel(char * filename, T_TabParties duel, T_TabJoueurs joueurs); //duel
void killJoueurs(void);

void whoami(octet numP); 
void whop(octet numP); 
void whoamj(); 
void whoj(); 




