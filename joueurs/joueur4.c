/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	int i=0; 

	while(1) {
		ecrireIndexCoup(i%listeCoups.nb);
		printf("ecrireIndexCoup %d\n", i%listeCoups.nb);
		i++; 
		sleep(1);
	}

}
