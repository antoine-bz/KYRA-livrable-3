/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	int reps[300];
	int nbrRep = 0;
	
	//code de M Bourdeaud'huy 
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
			return; // on quitte la fonction 
		}
	} 	
	
	//petit truc en plus de baptiste 
	for (i = 0; i < listeCoups.nb; ++i){
		if(
		currentPosition.cols[listeCoups.coups[i].origine].couleur == currentPosition.trait &&
		currentPosition.cols[listeCoups.coups[i].destination].couleur != currentPosition.trait
		){
			reps[nbrRep] = i;
			nbrRep++;
		}
	}
		
	
	if(nbrRep > 0){
		ecrireIndexCoup(reps[0]); 
	}else{
		nbrRep = 0;
		for (i = 0; i < listeCoups.nb; ++i){
			if(
			/*currentPosition.cols[listeCoups.coups[i].origine].couleur == currentPosition.trait &&*/
			currentPosition.cols[listeCoups.coups[i].destination].couleur != currentPosition.trait
			){
			reps[nbrRep] = i;
			nbrRep++;
			}
		}
		
		
		if(nbrRep > 0){
			ecrireIndexCoup(reps[0]); 
		}else{
			ecrireIndexCoup(0);
		}
	}
	
	
	
}


