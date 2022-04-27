#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups){
	int i, j; 
	octet o, d, v;
	T_Voisins voisins; 
	octet myColor = currentPosition.trait; 
	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	/*Le programme vas commencer par chercher à créer ue pile de 5 en privilègeant de couvrir les pons adverses*/
	// le cas où on trouve une pile proie de la couleur de l'advesaire et qu'on empile une pile de notre couleur dessus
	for ( i=0;i<listeCoups.nb; i++)
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination; 
		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5 && 
			(currentPosition.cols[o].couleur == myColor &&currentPosition.cols[d].couleur != myColor))
		{

			printf("On joue ce coup :");
			printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
			printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			ecrireIndexCoup(i);
			return;
		}
	}
	// le cas où on trouve une pile proie de la couleur de l'advesaire et qu'on empile une pile de la même couleur dessus
	for (int i = 0; i < listeCoups.nb; i++)
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination; 
		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5 && 
			(currentPosition.cols[o].couleur != myColor &&currentPosition.cols[d].couleur != myColor))
		{

			printf("On joue ce coup :");
			printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
			printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			ecrireIndexCoup(i);
			return;
		}
	}

//appliquant maitenat la stratégie qui réagit face à un pile de 3 au voisignage


	for (int i = 0; i < listeCoups.nb; i++)
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		//pratique pour isoller mes pions8
		if (currentPosition.cols[o].couleur == myColor && currentPosition.cols[o].couleur != myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb >3 )
		{				
			voisins=getVoisins(d);
			for (j = 0; j < voisins.nb; j++)
			{
				v=voisins.cases[j];
				if (currentPosition.cols[v].nb==3)
				{
					printf("On joue ce coup :");
					printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
					printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
					ecrireIndexCoup(i);
				}
			}
		}
	}

// On passe maintenant à la stratégie qui nous permet de crée des piles de 3 puisqu'o priveligie cette méthode
	for ( i=0;i<listeCoups.nb; i++){
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination; 
		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 3 && 
			(currentPosition.cols[o].couleur == myColor &&currentPosition.cols[d].couleur != myColor))
		{
			printf("On joue ce coup :");
			printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
			printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			ecrireIndexCoup(i);
			return;
		}
		}
  //creer des piles de 2 avec les pions adverses
		for ( i=0;i<listeCoups.nb; i++){
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination; 
		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 2 && 
			(currentPosition.cols[o].couleur != myColor &&currentPosition.cols[d].couleur != myColor))
		{
			printf("On joue ce coup :");
			printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
			printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			ecrireIndexCoup(i);
			return;
		}
		}

	//creer une pile de 2 en couvrant un pion adverse
		for ( i=0;i<listeCoups.nb; i++){
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination; 
		if (currentPosition.cols[o].couleur == myColor &&currentPosition.cols[d].couleur != myColor)
		{
			printf("On joue ce coup :");
			printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
			printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			ecrireIndexCoup(i);
			return;
		}
		}

// on effectue le premier coup qui ne donne pas une pile de 4

		for ( i=0;i<listeCoups.nb; i++)
		{
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination; 
			if(currentPosition.cols[o].nb+currentPosition.cols[d].nb != 4){
				printf("On joue ce coup :");
				printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
				printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
				ecrireIndexCoup(i);
				return;
			}

		}		

		// joueur un coup aléatoire
		ecrireIndexCoup(rand()%(listeCoups.nb-1));


	}










