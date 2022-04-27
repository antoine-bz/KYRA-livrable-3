/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "avalam.h"
#include "moteur.h"

#define LISTECOUPS_FIN_DE_PARTIE 10  // 20% Des pions réstants 

octet rand_a_b(int a, int b);

octet estValide(T_Position p, octet origine, octet destination);

int memeCouleur(T_Position currentPosition, octet pos1,octet pos2);

int garderCoups(T_Position currentPosition, octet pos1,octet pos2);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i,cpt,testVoisin,j,k; 
	T_Voisins Vo, Vd;
	octet dangerCase=-1;
	octet o, d; 
	int ordreCoup[5] = {-1,-1,-1,-1,-1};
	octet myColor = currentPosition.trait; 
	// afficherListeCoups(listeCoups);

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		Vo=getVoisins(o); 
		Vd=getVoisins(d); 
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
		
		if ((currentPosition.cols[d].nb + currentPosition.cols[d].nb) != 4) {
		
			// Si je peux gagner une colonne, je la prends 
			if  ((currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 5){
				if (garderCoups==0){
					if (currentPosition.cols[o].couleur == myColor){
						ordreCoup[0]=i;
					}
					if (currentPosition.cols[o].nb == 2){ 
						dangerCase=o;
					}
					if (currentPosition.cols[d].nb == 2){	
						dangerCase=d;
					}
				}
			}
			
			if(dangerCase != -1){
				if((currentPosition.cols[d].nb == 1) && (o == dangerCase) && (estValide(currentPosition, d, o) == VRAI)){
					ordreCoup[2]=i;
					
				}	
			} 

			//Isolement 
			if((Vo.nb == 1)){
				if(currentPosition.cols[d].couleur == myColor){
					ordreCoup[1]=i;
				}  	
			}

			if (listeCoups.nb < LISTECOUPS_FIN_DE_PARTIE) {  // Si on est en fin de partie
				// Si on est entrain de gagner (on a plus de piles de 5 ou autant que l'adversaire)
				if (myColor == 1) {  // Le joueur est jaune
					if (evaluerScore(currentPosition).nbJ5 > evaluerScore(currentPosition).nbR5) {
						// Alors on cherche à faire une pile de 5
					}
					// Si on est entrain de perdre (on a moins de piles de 5 que l'adversaire)
					else {
						// On cherche à isoler des pions (si la destination n'a pas de voisins)
						if(nbVoisins(d) == 1) {
							ordreCoup[3]=i;
						
						}
					}
				} else {  // Le joueur est rouge
					if (evaluerScore(currentPosition).nbR5 > evaluerScore(currentPosition).nbJ5) {
					}	
					// Si on est entrain de perdre (on a moins de piles de 5 que l'adversaire)
					else {
						// On cherche à isoler des pions (si la destination n'a pas de voisins)
						if(nbVoisins(d) == 1) {
							ordreCoup[3]=i;
						
						}
					}
				}
			}


			if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor)  && ( (currentPosition.cols[o].nb < 5) || (currentPosition.cols[d].nb < 5) ) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb ==  5) && (Vd.nb == 1) && (Vo.nb == 1)) {
				ordreCoup[1]=i;
			}
		
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor)){
			ordreCoup[4]=i;
		}
		}
	}

	for(j=0;j<5;j++){
		if (ordreCoup[j] != -1){
			ecrireIndexCoup(ordreCoup[j]);
			return;
		}
	}

	

}

int garderCoups(T_Position currentPosition, octet pos1,octet pos2)
{
    T_Voisins leVoisin1, leVoisin2;

    leVoisin1=getVoisins(pos1);
	leVoisin2=getVoisins(pos2);
    if((leVoisin2.nb)==1 && (leVoisin1.nb)==1 &&( memeCouleur(currentPosition,pos1,pos2) == VRAI))
    {
        return 1;
    }
    else{
        return 0;
    }


}


int memeCouleur(T_Position currentPosition, octet pos1 ,octet pos2)
{
    if(currentPosition.cols[pos1].couleur == currentPosition.cols[pos2].couleur)
    {
        return VRAI;
    }
    else
    {
        return FAUX;
    }
}


