/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i;int n;int j;
	//octet i; octet n; octet numéro;
	octet o, d; 
	octet myColor = currentPosition.trait; 
	
	T_Voisins v; octet nbVois=0;

	// afficherListeCoups(listeCoups);

	//printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	
	for(i=0;i<listeCoups.nb; i++) {														//Tour Isolé
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

	// Si je peux gagner une colonne, je la prends 

	T_Voisins v = getVoisins(d);
		if(v.nb==1 && currentPosition.cols[o].couleur== myColor) //si le pion d'origine n'a qu'1 voisin et que ce voisin est de notre couleur : on prends
		{															//on vérifie si ils peuvent s'empiler biensur : mais index coups : deja fait
			ecrireIndexCoup(i);
					return;
		}	
	}
	
	
	for(i=0;i<listeCoups.nb; i++) {														//regle des n
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

	// Si je peux gagner une colonne, je la prends 
		/*for(n=1;n<5;n++)
		{
			if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == n)
				&& (currentPosition.cols[d].nb == 5-n) ) 
				{
					printf("On choisit ce coup ! \n"); 
					ecrireIndexCoup(i);
					return; // on quitte la fonction 
				}
		}*/
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5))
			{
				printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				return; // on quitte la fonction 
			}
	} 
 	
 	/*for(i=0;i<listeCoups.nb; i++) 												//SACRIFICE
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

		v = getVoisins(o);
		if(v.nb==2)
		{
            if((currentPosition.cols[v.cases[1]].couleur != myColor)&&(currentPosition.cols[v.cases[0]].couleur != myColor))
            {
            	if((currentPosition.cols[v.cases[0]].nb + currentPosition.cols[v.cases[1]].nb + currentPosition.cols[o].nb )>5)
            	{
            		jouerCoup(currentPosition,v.cases[0],v.cases[1]);
            		return;
            	}
            }
        }
	}*/
 
 	for(i=0;i<listeCoups.nb; i++) {														//coups de base
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

	// Si je peux gagner une colonne, je la prends 

			if ((currentPosition.cols[d].couleur != myColor)) 
				{
				
					printf("On choisit ce coup ! \n"); 
					ecrireIndexCoup(i);
					return; // on quitte la fonction 
				}
	}
	
	
	//

}
