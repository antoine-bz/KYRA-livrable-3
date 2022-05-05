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

// STRAT 8 boucler tour mycolor -> ennemi MAL BON OK
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if((currentPosition.cols[o].couleur==myColor) 
		&& (currentPosition.cols[d].couleur!=myColor) 
		&& (currentPosition.cols[o].nb+currentPosition.cols[d].nb==5)){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}
	}	
// STRAT 8 boucler tour v2 MAL BON OK
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur==myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==5){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}
	}	

// STRAT
/*	for(i=0;i<listeCoups.nb; i++) {
		int nbvois1=0,nbvois2=0;

		T_Voisins voisins1;
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		if(currentPosition.cols[o].couleur!=myColor && currentPosition.cols[d].couleur!=myColor){
			voisins = getVoisins(o);
			nbvois=nbVoisins(o);
			for (j = 0; j < nbvois; j++){

				if(currentPosition.cols[voisins.cases[j]].couleur==myColor){

					voisins1 = getVoisins(voisins.cases[j]);
					nbvois1=nbVoisins(voisins.cases[j]);
					for (int k = 0; k < nbvois1; k++)
					{
						if(currentPosition.cols[voisins1.cases[j]].couleur==myColor)
						nbvois2++;
					}
					if (nbvois2 == 0){
						if(!danger(i, listeCoups,currentPosition)){
							ecrireIndexCoup(i);
							return;
						}
					}
				}
			}
		}
		
	}*/
// STRAT 3 isolement 1 voisin ennemies BON MAL OK
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
//nbv =nombre de vois de couleur adverse
		if((currentPosition.evolution.bonusJ==o || currentPosition.evolution.bonusJ==d )
		|| (currentPosition.evolution.bonusR==o || currentPosition.evolution.bonusR==d ))
			if(currentPosition.cols[d].couleur!=myColor)
				if(nbv==0 && currentPosition.cols[o].couleur==myColor){
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
		if((currentPosition.evolution.bonusJ==o || currentPosition.evolution.bonusJ==d )
		|| (currentPosition.evolution.bonusR==o || currentPosition.evolution.bonusR==d ))
		if(currentPosition.cols[o].couleur==myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==3){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}

/////strat 3 4 version focus adverse
// STRAT 3 isolement 1 voisin ennemies
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
//nbv =nombre de vois de couleur adverse
		if(currentPosition.cols[d].couleur!=myColor)
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
		if(currentPosition.cols[d].couleur!=myColor)
		if(nbvois<=4 && currentPosition.cols[o].couleur==myColor && nbv>=3){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}

///////
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
// STRAT contre-isolement BON MAL
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;

		voisins = getVoisins(d);
		nbvois=nbVoisins(d);
		if((currentPosition.evolution.bonusJ==o || currentPosition.evolution.bonusJ==d )
		|| (currentPosition.evolution.bonusR==o || currentPosition.evolution.bonusR==d ))
		if(nbvois==1 && currentPosition.cols[o].couleur==myColor&& currentPosition.cols[d].couleur!=myColor){
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
		}	
	}
// STRAT 5 prendre pions adverses
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		if((currentPosition.evolution.bonusJ==o || currentPosition.evolution.bonusJ==d )
		|| (currentPosition.evolution.bonusR==o || currentPosition.evolution.bonusR==d ))
			if(currentPosition.cols[o].couleur!=myColor && currentPosition.cols[d].couleur!=myColor)
				if (currentPosition.cols[o].nb==1 && currentPosition.cols[d].nb==1){
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
			if(!danger(i, listeCoups,currentPosition)){
				ecrireIndexCoup(i);
				return;
			}
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
	if((currentPosition.evolution.malusJ==o || currentPosition.evolution.malusJ==d )
	|| (currentPosition.evolution.malusR==o || currentPosition.evolution.malusR==d ))
		return 1;
	return 0;
}
