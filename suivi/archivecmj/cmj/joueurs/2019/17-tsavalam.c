#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avalam.h"
#include "moteur.h"
#include "topologie.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
		
	int i; 
	octet o, d; 
	octet myColor = currentPosition.trait;
	
for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].nb == 4) ) {
			printf("On choisit ce coup ! \n"); 
			ecrireIndexCoup(i);
			return; // on quitte la fonction 
		}
 
}







/*
- Si il y a une tour de 3 adversaires et que j’ai un de mes pions à côté mais qu’il y a des pions adverse à coté également, je n’empile pas mon pion dessus.
- Si je commence la partie et que j’ai la priorité  je réalise l’ouverture.
 - J’analyse les 2 crans autour des pions joué par l’autre.
- Si je ne rentre dans aucune condition importante, j’empile les pions de couleurs adverses en tour de 2 ( pour mettre en place notre stratégie global).
- Si tour de 4 : elle est à nous ? : oui-> la poser sur un pion adverse : non -> on a des pions adjacents ? : oui -> on la prend: non -> on gâche un pion adverse adjacent sur elle */


/*
-Si il y a une tour de 3, et que c’est une tour adverse, et si le nombre de voisins ennemis est =< 2 alors il faut éloigner les pions de l’ennemie de la tour. En revanche si le nombre de pions voisins ennemis est >2 alors essayer de gâcher le plus de pions adverses : la tour est considéré comme non récupérable
*/
/*
if ( ( (currentPosition.cols[o].couleur != myColor)&&(currentPosition.cols[o].nb==3) ) || ((currentPosition.cols[o].couleur != myColor)&&(currentPosition.cols[o].nb==4))	)
{
	int j=0;

	do{
		j ++;
	
	}while( (currentPosition.cols[topologie[o].cases[j]].couleur == myColor) || (j<strlen(topologie[d].cases)) ); //on fait tout ça tant que l'on n'a pas encore parcouru tout les voisins ou qu'on n'a pas encore trouvé d'ennemi voisin



//---------------------------------------------------------------------------VERIF------------------------------------------------------------------------------------


	if ((currentPosition.cols[topologie[o].cases[j]].couleur == myColor)) //alors c'est qu'on s'est arrété car on a analysé tout les voisins : il n'y a pas de voisins ennemis
	{
		// si il n'y a pas de voisin ennemi : on regarde si on a un voisin a nous pour prendre la tour
		j=0;
		do
		{
			j ++;
	
		}while( (currentPosition.cols[topologie[o].cases[j]].couleur != myColor) || (j<strlen(topologie[d].cases)) );
		
		
			if ((currentPosition.cols[topologie[o].cases[i]].couleur == myColor))//si le dernier j est de notre couleur : on a un pion a nous a proximité
			{
				ecrireIndexCoup(j);
			
			}
		
		
	}
	else // c'est qu'il y au moins 1 voisins ennemi : currentPosition.cols[T_Voisins topologie[o].cases[j]].couleur
	{
		//on essaye d'éloigner le voisin ennemi de la tour
		
		ecrireIndexCoup(j);
		
	}


}*/	


/*
-Si il y a une tour de 2 pions du joueur adverse et à proximité de celle-ci un tour  de 2 ou 3 pions qui à le sommet de notre couleur. On les empiles pour avoir notre pion au sommet sauf si cela aboutit à une tour de 4 avec pions adverse à ses côtés.*/
for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			if ( (currentPosition.cols[o].couleur == myColor) && ((currentPosition.cols[o].nb == 2) && (currentPosition.cols[d].nb == 3) )  && ((currentPosition.cols[o].nb == 3) || (currentPosition.cols[d].nb == 2)) ) 
			{ 	//si il y une tour de 2ou3 pions de macouleur a proximité d'une tour de 2ou3 alors je la saisie
				ecrireIndexCoup(i);
				return;
			}
}

for(i=0;i<listeCoups.nb; i++) {
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
		
		if ( (currentPosition.cols[o].couleur != myColor)
		&& (currentPosition.cols[d].couleur != myColor) ) {
			ecrireIndexCoup(i);
			return; // on quitte la fonction 
		}
		}	

} 
		

