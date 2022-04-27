/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"



void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	int choix; 
	int i, o, d; 

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
	}

	printf("Votre choix ? ");
	scanf("%d", &choix); 
	printf("Vous avez choisi %d\n", choix);
	ecrireIndexCoup(choix);
}


