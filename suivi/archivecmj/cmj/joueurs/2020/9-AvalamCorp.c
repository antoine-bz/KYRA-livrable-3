/********* Moteur de tournoi : joueur ***************/
//Groupe 9: AvalamCorp

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups, T_Voisins voisins) 
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

	// aléatoire 
	// ecrireIndexCoup(rand()%listeCoups.nb);
{
	int i,j;
	int vois, vois_other, vois_me=0; 
	octet o, d, v; 
	int a; 
	int cpt;
	octet myColor = currentPosition.trait; 

	// afficherListeCoups(listeCoups);

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
//-------------------------Strategie 1 de notre livrable 2 

	//jouer avec des pions adverses
	for(i=0;i<listeCoups.nb; i++) 
	{
		printf("\t---------------------------- Strat 1 en cours :\n");
		printf("Jouer avec les pions adverses : \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		vois = 0;

	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		) 
	{
		
		
		voisins = getVoisins(d);
		
		for (j = 0; j < voisins.nb; j++)
		{
			v = voisins.cases[j];

			if ( (currentPosition.cols[v].couleur !=myColor)
				&& (currentPosition.cols[v].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb != 5) 
				)
			{
				vois = vois + 1; // on incrémente vois
				
				
			}
		}
		if (vois == voisins.nb) // si vois = nombre de voisins, on peut jouer le coup
			{					
		
				printf("On choisit ce coup ! -Strategie 1----------------------------\n"); 
				ecrireIndexCoup(i);
				return; // on quitte la fonction 
			}	
	}
	}

//-----------------------fin Strategie 1



//-------------------------Strategie 2 (c'est la stratégie 3 de notre livrable 2)  

	//verification de l'encerclement d'un adversaire
	//remporter le point dans ce cas là
	for(i=0;i<listeCoups.nb; i++) 
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		vois_me = 1;
		vois_other=0;


	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		) 
	{
		printf("\t---------------------------- Strat 2 en cours :\n");
		printf("Encercler l'adversaire !!\n");
		voisins = getVoisins(d);
		printf("On récupère les voisins\n");
		for (j = 0; j < voisins.nb; j++)
		{
			v = voisins.cases[j];

			if ( (currentPosition.cols[v].couleur !=myColor)
				&& (currentPosition.cols[v].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb != 5) 
				)
			{
				vois_other = vois_other + 1; // on incrémente vois
				printf("Incrémentation de vois adversaire \n");
			}
		}
		if ((vois_me == 2)
			&&(vois_other ==1)
			) // si vois = nombre de voisins, on peut jouer le coup
			{					
				printf("On choisit ce coup ! -Strategie 2----------------------------\n"); 
				ecrireIndexCoup(i);
				return; // on quitte la fonction 
			}	
	}
}

//-----------------------fin Strategie 2


//-------------------------Strategie 3
	// On teste si on peut faire une pile de 5 en empilant sur une pile 
	// différente de notre couleur
	for(i=0;i<listeCoups.nb; i++) 
	{
		printf("\t---------------------------- Strat 3 en cours :\n");
		printf("Vérifier la complétion en pile de 5 :  \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  


	if ( 			(currentPosition.cols[o].couleur == myColor)
			    &&  (currentPosition.cols[d].couleur != myColor)
				&& 	((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 5) 
		) 
		{
			printf("On choisit ce coup ! -Strategie 3----------------------------\n"); 
			//printf("On choisit ce coup ! - On empile sur une couleur différente --> pile de 5 \n"); 
			ecrireIndexCoup(i);

			return; // on quitte la fonction 
		}
	}
//-----------------------fin Strategie 3


//-------------------------Strategie 4
	// Si je peux gagner une colonne, je la prends (1+4, 2+3 ...)
	for(i=0;i<listeCoups.nb; i++) 
	{
		printf("\t---------------------------- Strat 4 en cours :\n");
		printf("Vérifier la complétion quelconque en pile de 5 : \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  

		
	if ( 			(currentPosition.cols[o].couleur == myColor)
				&& 	((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 5) 
		) 
		{
			printf("On choisit ce coup ! -Strategie 4----------------------------\n"); 
			//printf("On choisit ce coup ! - Pile de 5\n"); 
			ecrireIndexCoup(i);
			return; // on quitte la fonction 
		}
	}
//-----------------------fin Strategie 4


//-------------------------Strategie 5
	// J'empile mes pions sur ceux de l'adversaire pour faire une pile de 4, sauf s'il y a 
	// un pion de l'autre couleur à côté 

for(i=0;i<listeCoups.nb; i++) 
{
	printf("\t---------------------------- Strat 5 en cours :\n");
	printf("Vérifier la complétion en pile de 4 : \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		vois = 0;

	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		&& ((currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 4)
		) 
	{
		
		voisins = getVoisins(d);
		
		for (j = 0; j < voisins.nb; j++)
		{
			v = voisins.cases[j];

			if (currentPosition.cols[v].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb != 5) // si la pile du voisin est différente de 2
			{
				vois = vois + 1; // on incrémente vois
		
		
			}
		}
		if (vois == voisins.nb) // si vois = nombre de voisins, on peut jouer le coup
			{					// c-a-d il n'y aucun voisin qui est une pile de 2
				
				printf("On choisit ce coup ! -Strategie 5----------------------------\n"); 
				ecrireIndexCoup(i);
				return; // on quitte la fonction 
			}	
	}
}
//-----------------------fin Strategie 5


//-------------------------Strategie 6
	// J'empile mes pions sur ceux de l'adversaire pour faire une pile de 3, sauf s'il y a
	// une pile de 2 à côté
for(i=0;i<listeCoups.nb; i++) 
{
	printf("\t---------------------------- Strat 6 en cours :\n");
	printf("Vérifier la complétion en pile de 3 : \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		vois = 0;

	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 3)
		) 
	{
		
		voisins = getVoisins(d);
		
		for (j = 0; j < voisins.nb; j++)
		{
			v = voisins.cases[j];

			if (currentPosition.cols[v].nb + currentPosition.cols[o].nb + currentPosition.cols[d].nb != 5) // si la pile du voisin est différente de 2
			{
				vois = vois + 1; // on incrémente vois
				
				
			}
		}
		if (vois == voisins.nb) // si vois = nombre de voisins, on peut jouer le coup
			{					// c-a-d il n'y aucun voisin qui est une pile de 2
				
				printf("On choisit ce coup ! -Strategie 6----------------------------\n"); 
				ecrireIndexCoup(i);
				return; // on quitte la fonction 
			}	
	}
}
//-----------------------fin Strategie 6


//-------------------------Strategie 7
	// Sinon j'empile mes pions sur ceux de l'adversaire sauf s'ils conduisent à une pile 4
	// Et je n'empile pas mes pions si ça donne une pile de 3 et une pile de 2 de différentes couleurs
	// Sinon on donne une pile de 5 gratuite pour l'adversaire	
for(i=0;i<listeCoups.nb; i++) 
{
	printf("\t---------------------------- Strat 7 en cours :\n");
	printf(" Empiler une partie de mes pions sur l’adversaire sauf pile de 4 ou piles avec couleurs différentes : \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		vois = 0;  

	if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 4)
		) 
	{
		
		voisins = getVoisins(d);
		
		for (j = 0; j < voisins.nb; j++)
		{
			v = voisins.cases[j];

			if (currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[v].nb != 5)
			{
				// si la pile d'origine + la pile de dest + la pile du voisin est différent de 5
				vois = vois + 1; // on incrémente vois
				// en incrémentant vois, on test tous les voisins de la position de destination
		
		
			}
		}
		if (vois == voisins.nb) // si vois = nombre de voisins, on peut jouer le coup
		{						// c-a-d l'adversaire ne peut pas avoir une pile de 5 gratuite
		
			printf("On choisit ce coup ! -Strategie 7----------------------------\n"); 
			ecrireIndexCoup(i);
			return; // on quitte la fonction 
		}
	}
}
//-----------------------fin Strategie 7


//-------------------------Strategie 8
	// Sinon, j'empile des pions adverses sauf s'ils conduisent à des tas de 5 ou 4
 for(i=0;i<listeCoups.nb; i++) 
 {
 	printf("\t---------------------------- Strat 8 en cours :\n");
 	printf("Empiler des pions adverses pour réduire les points de l’adversaire : \n");
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  

	if ( (currentPosition.cols[o].couleur != myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 5 || 
			(currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 4 )
		) 
	{
			printf("On choisit ce coup ! -Strategie 8----------------------------\n"); 
			ecrireIndexCoup(i);
			return; // on quitte la fonction 
	}
 }
 //-----------------------fin Strategie 8


//-------------------------Strategie 9
for ( i=0;i<listeCoups.nb; i++){
		printf("\t---------------------------- Strat 9 en cours :\n");
		
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination; 
		if ((currentPosition.cols[o].nb+currentPosition.cols[d].nb == 2) && 
			(currentPosition.cols[o].couleur != myColor &&currentPosition.cols[d].couleur != myColor))
		{
			printf("Pile de 2 de l'adversaire :");
			printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
			printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
			printf("On choisit ce coup ! -Strategie 9----------------------------\n"); 
			ecrireIndexCoup(i);
			return;
		}
}
//-----------------------fin Strategie 9

//-------------------------Strategie 10
	// Sinon, je tire au sort 
	while (1) 
	{
		printf("\t---------------------------- Strat 10 en cours :\n");
		printf(" Tirer au sort  :  \n");
		a = rand()%listeCoups.nb;
		o = listeCoups.coups[a].origine; 
		d = listeCoups.coups[a].destination;  
 
		if ( (currentPosition.cols[o].couleur != myColor)
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 5) ) 
		{
			printf("On choisit ce coup ! -Strategie 10----------------------------\n"); 
			ecrireIndexCoup(a);
			return; // on quitte la fonction 
		}
	}
//-----------------------fin Strategie 10

}
