/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
#include <unistd.h>


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

	ecrireIndexCoup(listeCoups.nb-1); // dernier

}


