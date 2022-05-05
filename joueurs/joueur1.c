/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int danger(int indice_coup, T_ListeCoups listeCoups, T_Position currentPosition);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	int i,j,voiscase,nbvois,nbv; 
	octet o,d,vois; 
	octet myColor = currentPosition.trait;
	T_Voisins voisins;

/////////////EXEMPLES DE STRAT////////////////////////////

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

// STRAT 3 isolement 1 voisin
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		if(nbvois==1 && currentPosition.cols[o].couleur==myColor){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}

// STRAT 3 isolement 1 voisin
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		nbv=0;
		
		for(j=0; j<nbvois;j++){//compte le nb de vois ennemies par rapport a la destination
			vois = voisins.cases[j];
			if (currentPosition.cols[vois].couleur!=myColor){
				nbv++;
			}
		}

		if(nbv==0 && currentPosition.cols[o].couleur==myColor){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}

// STRAT 4 isolement adversaire
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);
		nbv=0;

		for(j=0; j<nbvois;j++){
			vois = voisins.cases[j];
			if (currentPosition.cols[vois].couleur==myColor){
				nbv++;
			}
		}
		if(nbvois<=4 && currentPosition.cols[o].couleur==myColor && nbv>=3){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}


	
// STRAT 2 prendre tour de 2 
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur==myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==3){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}

// STRAT contre-isolement
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);

		if(nbvois==1 && currentPosition.cols[o].couleur==myColor&& currentPosition.cols[d].couleur!=myColor){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}

// STRAT isolement v4060
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

// STRAT 5 prendre pions adverses
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur!=myColor && currentPosition.cols[d].couleur!=myColor)
		if (currentPosition.cols[o].nb==1 && currentPosition.cols[d].nb==1){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}
	}

// STRAT 1 empiler pions adverses
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[d].couleur!=myColor)
		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb<4){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}
	}


	
} //fin de la fonction

int danger(int indice_coup, T_ListeCoups listeCoups, T_Position currentPosition){
	octet o,d,vois,a;
	int nbvois;
	T_Voisins voisins;

	o = listeCoups.coups[indice_coup].origine; 
	d = listeCoups.coups[indice_coup].destination;
		
	voisins = getVoisins(d);
	nbvois=nbVoisins(d);
	
	for (int i = 0; i < nbvois; i++)
	{
		a=voisins.cases[i];
		if(currentPosition.cols[o].nb+currentPosition.cols[d].nb+currentPosition.cols[a].nb==5)
			return 1;
	}
	return 0;
}