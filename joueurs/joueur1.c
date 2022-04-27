/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	int i,j;
	int vois, vois_other, vois_me=0; 
	octet o, d, v; 
	int a; 
	octet myColor = currentPosition.trait;
	T_Voisins voisins;

// STRAT 1
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		vois = 0;
		voisins = getVoisins(d);

		//STRAT 1



	}	
// STRAT 2
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		vois = 0;
		voisins = getVoisins(d);

		//STRAT 2



	}	

}


