/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	int i,j;
	int voiscase,nbvois; 
	octet o, d, v; 
	int a; 
	octet myColor = currentPosition.trait;
	T_Voisins voisins;

/////////////EXEMPLES DE STRAT////////////////////////////


// STRAT 3 isolement 1 voisin
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		if(nbvois==1 && currentPosition.cols[o].couleur==myColor){
			ecrireIndexCoup(i);
			return;
		}	
	}

// STRAT isolement adversaire
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		if(nbvois<=4 && currentPosition.cols[o].couleur==myColor && ){
			ecrireIndexCoup(i);
			return;
		}	
	}

// STRAT 8 boucler tour mycolor -> ennemi
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur==myColor && currentPosition.cols[d].couleur!=myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==5){
			ecrireIndexCoup(i);
			return;
		}
	}	

// STRAT 8 boucler tour v2 
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur==myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==5){
			ecrireIndexCoup(i);
			return;
		}
	}	
	
// STRAT 2 prendre tour de 2 
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[is].destination;

		if(currentPosition.cols[o].couleur==myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==3){
			ecrireIndexCoup(i);
			return;
		}	
	}

// STRAT contre-isolement
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		if(nbvois==1 && currentPosition.cols[o].couleur==myColor&& currentPosition.cols[d].couleur!=myColor){
			ecrireIndexCoup(i);
			return;
		}	
	}

// STRAT 
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		if(nbvois<2 && currentPosition.cols[o].couleur==myColor){
			ecrireIndexCoup(i);
			return;
		}	
	}

// STRAT 5 prendre pions adverses
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur!=myColor && currentPosition.cols[d].couleur!=myColor)
		if (currentPosition.cols[o].nb==1 && currentPosition.cols[d].nb==1){
			ecrireIndexCoup(i);
			return;
		}
	}

// STRAT 1 empiler pions adverses
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[d].couleur!=myColor)
		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb<4){
			ecrireIndexCoup(i);
			return;
		}
	}


	
} //fin de la fonction