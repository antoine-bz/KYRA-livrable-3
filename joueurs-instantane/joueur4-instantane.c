/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
#include "duel-instantane.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i; 
	octet o, d; 
	octet myColor = currentPosition.trait; 

	// afficherListeCoups(listeCoups);

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

	// Si je peux gagner une colonne, je la prends 
	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].nb == 4) ) {
			printf("On choisit ce coup ! \n"); 
			ecrireIndexCoup(i);
			finaliserCoup();
			return; // on quitte la fonction 
		}
	} 

	// Sinon, je joue le premier coup possible 
	ecrireIndexCoup(0);
	finaliserCoup();
 


}
