/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

//Prototypes des fonctions
int NombreVoisins(octet pion, T_ListeCoups listeCoups);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	int i; 
	octet o, d; 
	octet myColor = currentPosition.trait; 
	int IndiceCoup=0;
	int PuissanceCoupActuel=0;
	int MeilleurCoup=0;
	int NombreOrigine=0;
	int NombreDestination=0;	

	printf("La liste de coup est: \n");
	afficherListeCoups(listeCoups);

	//printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb;i++) //On parcours la liste des coups
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		//printf("Coup %d : ", i); //Indice du coup
		printf("On test %3d sur %3d\n",o,d);

		//Je peux faire une Pile Finie pour empêcher l'adversaire d'en faire une ! (deux pions isolés)
		if((NombreVoisins(o,listeCoups)==1 && NombreVoisins(d,listeCoups)==1) //Deux pions isolés
		&&(currentPosition.cols[o].couleur == myColor) //pion origine de notre couleur
		&&(currentPosition.cols[d].couleur != myColor) //pion destination de la couleur adverse
		&&(currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5)) //qui peuvent faire une Pile Finie*
		{
			PuissanceCoupActuel=8;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//Je peux faire une Pile pour empêcher l'adversaire d'en faire une ! (deux pions isolés)
		if((NombreVoisins(o,listeCoups)==1 && NombreVoisins(d,listeCoups)==1) //Deux pions isolés
		&&(currentPosition.cols[o].couleur == myColor) //pion origine de notre couleur
		&&(currentPosition.cols[d].couleur != myColor) //pion destination de la couleur adverse
		&&(currentPosition.cols[o].nb+currentPosition.cols[d].nb < 5)) //qui ne fait pas une Pile Finie*
		{
			PuissanceCoupActuel=7;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//Je peux isoler un de mes pions (sur un pion adverse)
		if((currentPosition.cols[o].couleur == myColor) 
		&& (currentPosition.cols[d].couleur != myColor) 
		&& (NombreVoisins(d,listeCoups) == 1)) 
		{	
			PuissanceCoupActuel=6;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//Je peux isoler un de mes pions (sur un de mes pions)
		if((currentPosition.cols[o].couleur == myColor) //Origine de notre couleur
		&& (currentPosition.cols[d].couleur == myColor) //Destination de couleur différente
		&& (NombreVoisins(d,listeCoups) == 1)) //L'origine est le seul voisin de la destination->la destination isolée/de ma couleur
		{	
			PuissanceCoupActuel=5;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//Je peux faire une Pile Finie sur une tour adverse !
		if((currentPosition.cols[o].couleur == myColor) 
		&& (currentPosition.cols[d].couleur != myColor) 
		&& (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5)) 
		{	
			PuissanceCoupActuel=4;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//Je peux faire une Pile Finie !
		if((currentPosition.cols[o].couleur == myColor) 
		&& (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5)) 
		{ 
			PuissanceCoupActuel=3;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//J'empêche l'adversaire de faire une pile isolée
		if((currentPosition.cols[o].couleur != myColor)
		&& (currentPosition.cols[d].couleur != myColor)
		&& (NombreVoisins(o,listeCoups) == 1)
		&& (NombreVoisins(d,listeCoups) > 1)) 
		{
			PuissanceCoupActuel=2;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//J'empile deux pions adverses pour faire des piles de deux
		if((currentPosition.cols[o].couleur != myColor)
		&&(currentPosition.cols[d].couleur != myColor)
		&&(currentPosition.cols[o].nb+currentPosition.cols[d].nb == 2))
		{
			PuissanceCoupActuel=1;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
		//J'empile deux pions adverses !
		if((currentPosition.cols[o].couleur != myColor)
		&&(currentPosition.cols[d].couleur != myColor))
		{
			PuissanceCoupActuel=0;
			if(PuissanceCoupActuel>MeilleurCoup)
			{
				IndiceCoup=i;
				MeilleurCoup=PuissanceCoupActuel;
			}
		}
	}
	switch(PuissanceCoupActuel)
	{
		case 8:
			printf("Je peux faire une Pile Finie pour empêcher l'adversaire d'en faire une ! (deux pions isolés)\n");
			break;
		case 7:
			printf("Je peux faire une Pile pour empêcher l'adversaire d'en faire une ! (deux pions isolés)\n");
			break;
		case 6:
			printf("Je peux isoler un de mes pions (sur un pion adverse)\n");
			break;
		case 5:
			printf("Je peux isoler un de mes pions\n");
			break;
		case 4:
			printf("Je peux faire une Pile Finie sur une tour adverse !\n"); 
			break;
		case 3:
			printf("Je peux faire une Pile Finie !\n");
			break;
		case 2:
			printf("J'empêche l'adversaire de faire une pile isolée !\n");
			break;
		case 1:
			printf("J'empile deux pions adverses pour faire des piles de deux !\n");
			break;
		case 0:
			printf("J'empile deux pions adverses !\n");
			break;
	}
	ecrireIndexCoup(IndiceCoup);
	return; // on quitte la fonction 
} 

		
int NombreVoisins(octet pion, T_ListeCoups listeCoups)
{
	int i=0;
	int nombreVoisins=0;
	for(i=0;i<listeCoups.nb;i++)
	{
		if(listeCoups.coups[i].origine==pion)
			nombreVoisins++; 
	}
	return nombreVoisins;
}


