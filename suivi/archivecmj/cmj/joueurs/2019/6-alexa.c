
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

// CODE DEVELOPPE PAR : Cherni Koutheir
// AIDE EXTERIEURE : Zoubiri Riad ; Dorian Keurnick ; Julien Domont 

int VerifCoup (T_Position currentPosition, T_ListeCoups listeCoups,int coupChoisi);
void Boucle (int,int,T_ListeCoups);



void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{


	int x,y=0,isoler,pion_a_bouger=0,pion_a_sousmetre=0;
	octet o,d; 
	octet myColor = currentPosition.trait; 
	
	T_Voisins voisins;
	T_Voisins voisin1;

	int nbVoisinsD, nbVoisinsO;
	int nbvoisinennemis;
	int AddVoisinEnnemi;


	int l=0;
	int i,n; 
	int a; 
	int k;
 	int j;


	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));

	//STRATEGIE DE L'ISOLATION ET CONTRE ISOLATION

	for(i=0;i<listeCoups.nb; i++) 
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  

		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur));

		nbVoisinsD=0;
		nbVoisinsO=0;

		// JE PARCOURS LES VOISINS DE DEST :

		for(j=0;j<getVoisins(d).nb;j++)
		{
			if((currentPosition.cols[getVoisins(d).cases[j]].nb == 1) 
			|| (currentPosition.cols[getVoisins(d).cases[j]].nb == 2) 
			|| (currentPosition.cols[getVoisins(d).cases[j]].nb == 3) 
			|| (currentPosition.cols[getVoisins(d).cases[j]].nb == 4))
				nbVoisinsD++;
		}

		for(j=0;j<getVoisins(o).nb;j++)
		{
			if((currentPosition.cols[getVoisins(o).cases[j]].nb == 1) 
			|| (currentPosition.cols[getVoisins(o).cases[j]].nb == 2) 
			|| (currentPosition.cols[getVoisins(o).cases[j]].nb == 3) 
			|| (currentPosition.cols[getVoisins(o).cases[j]].nb == 4))
				nbVoisinsO++;
		}

		printf("\n\n nbVoisinsD : %d\n nbVoisinsO : %d\n", nbVoisinsD, nbVoisinsO);

		//LES DIFFERENTS CAS D'ISOLATION : 

		if(currentPosition.cols[o].couleur != myColor)	
		{
			voisins=getVoisins(o);
			isoler=0;
			for(y=0;y<voisins.nb;y++)
			{

				if(currentPosition.cols[voisins.cases[y]].nb==0 
				|| currentPosition.cols[voisins.cases[y]].nb==5)
					isoler++;


				if(currentPosition.cols[voisins.cases[y]].nb==1 
				|| currentPosition.cols[voisins.cases[y]].nb==2 
				|| currentPosition.cols[voisins.cases[y]].nb==3 
				|| currentPosition.cols[voisins.cases[y]].nb==4)
					x=voisins.cases[y];

			}


			if(isoler==(voisins.nb)-1)
			{

				if(currentPosition.cols[x].couleur == myColor)
				{
					pion_a_bouger=d;
					pion_a_sousmetre=o;										
				}
			}
		}


						
		if(currentPosition.cols[o].couleur == myColor)
		{
			voisins=getVoisins(o);
			isoler=0;

			for(y=0;y<voisins.nb;y++)

			{

				if(currentPosition.cols[voisins.cases[y]].nb==0 
				|| currentPosition.cols[voisins.cases[y]].nb==5)
					isoler++;


				if(currentPosition.cols[voisins.cases[y]].nb==1 
				|| currentPosition.cols[voisins.cases[y]].nb==2 
				|| currentPosition.cols[voisins.cases[y]].nb==3 
				|| currentPosition.cols[voisins.cases[y]].nb==4)
					x=voisins.cases[y];

			}
			
			if(isoler==(voisins.nb)-1)

			{
				pion_a_bouger=d;
				voisin1=getVoisins(d);
				for(j=0;j<voisin1.nb;j++)

				{
					if(currentPosition.cols[j].couleur != myColor)

					{
						pion_a_sousmetre = voisin1.cases[j];
						Boucle (pion_a_sousmetre,pion_a_bouger,listeCoups);
					}

				}
			}
		}


		if(o==pion_a_bouger && d==pion_a_sousmetre)
		{

			printf("COUP CHOISI : ISOLER ! \n");
			ecrireIndexCoup(i);

		return; // JE SORS DE MA FONCTION ICI

		}
		
		

		// CONTRER L'ISOLATION 

		if((currentPosition.cols[o].couleur != myColor)
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) <= 5)
		&& (nbVoisinsO==1)
		&& (VerifCoup(currentPosition,listeCoups,i) == 0))

		{

			printf("\n\n\n CONTRE ISOLATION \n\n\n");
			printf("COUP CHOISI ! \n");
			ecrireIndexCoup(i);
			return; // JE SORS DE MA FONCTION ICI

		}
	}


	//LES AUTRES COUPS POSSIBLE QUE LA STRATEGIE DE L'ISOLATION

	for(i=0;i<listeCoups.nb; i++) 

	{

		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  


		nbvoisinennemis=0;
		AddVoisinEnnemi=0;

		//ON PARCOURS LES VOISINS DE d ( DESTINATION )

		for(j=0;j<getVoisins(d).nb;j++)

		{

			if(currentPosition.cols[getVoisins(d).cases[j]].couleur != myColor)
			{
				nbvoisinennemis++;
				
				if((currentPosition.cols[getVoisins(d).cases[j]].nb+currentPosition.cols[o].nb+currentPosition.cols[d].nb) < 5) //Si l'addition d'un voisin adverse plus le coup joué(origine + dest) est inférieur à 5
				AddVoisinEnnemi=1;
			}

			
		}

		
		// SI ON PEUT FAIRE UNE TOUR AVEC UNE TALLE < 5  SANS LA PERDRE 

		if ((currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor)
		&& ((nbvoisinennemis == 0) || AddVoisinEnnemi==0)
		&&(VerifCoup(currentPosition,listeCoups,i) == 0))

		{
			printf("COUP CHOISI ! \n");
			ecrireIndexCoup(i);
			return; // JE SORS DE MA FONCTION ICI
		}

		// SI ON PEUT GAGNER UNE TOUR (1+4, 2+3 ...)

		if ((currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].couleur != myColor)
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 5)
		&& (currentPosition.cols[d].couleur != currentPosition.cols[o].couleur))

		{

			printf("COUP CHOISI ! \n");
			ecrireIndexCoup(i);
			return; // JE SORS DE MA FONCTION ICI

		}

		
	} 


	// STANDARD : J'EMPILE LES PIONS ENNEMIS SAUF S'ILS FONT DES TOURS DE 5 OU 4 ( FAIRE GAGNER L'ENNEMI)

	for(i=0;i<listeCoups.nb; i++) 

	{

		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  

		if ((currentPosition.cols[o].couleur != myColor)
		&& (currentPosition.cols[d].couleur != myColor) 
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 5) 
		&& (currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 4 
		&& (VerifCoup(currentPosition,listeCoups,i) == 0) )  
		{
			printf("COUP CHOISI ! \n");
			printf("%d\n",currentPosition.cols[o].couleur );
			ecrireIndexCoup(i);
			return; // JE SORS DE MA FONCTION ICI

		}
	}


	 // CAS STANDARD 2 : ON TIRE AU SORT
	

	while (1) 

	{

		a = rand()%listeCoups.nb;
		o = listeCoups.coups[a].origine; 
		d = listeCoups.coups[a].destination;  
 
		if ( (currentPosition.cols[o].couleur != myColor)
		&& ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) != 5) 
		&& VerifCoup(currentPosition,listeCoups,i) == 0) 
		{

			printf("COUP CHOISI ! \n");
			ecrireIndexCoup(a);
			return; // JE SORS DE MA FONCTION ICI

		}
	}
	
}


// JE TESTE SI LE COUP CHOISI PAR UNE STRATEGIE NE DONNE PAS UNE OPPORTUNITE DE MARQUER UN POINT A L'ENNEMI

int VerifCoup(T_Position currentPosition, T_ListeCoups listeCoups,int coupChoisi)

{  

		octet o, d; 
		T_Voisins voisins;
		int i;
		o = listeCoups.coups[coupChoisi].origine; 
		d = listeCoups.coups[coupChoisi].destination; 
		voisins =getVoisins(d);

		if((currentPosition.cols[o].couleur != currentPosition.trait)
		&& (currentPosition.cols[d].couleur == currentPosition.trait)
		&& (currentPosition.cols[o].nb+currentPosition.cols[d].nb == 5))

		{

			printf("COUP DEBILE \n");
			return 1;

		}
		

		if (currentPosition.cols[o].nb+currentPosition.cols[d].nb!=5)
		{

			for (i = 0; i < voisins.nb; ++i)

			{


				if ( (currentPosition.cols[voisins.cases[i]].nb+(currentPosition.cols[o].nb+currentPosition.cols[d].nb)==5 )
					&& (voisins.cases[i] != o) )
				{

					printf("COUP DEBILE \n");
					return 1;
					
				}
			}
		}
return 0;
}

void Boucle (int d,int o,T_ListeCoups listeCoups)
{
	int j;
	octet Org, Dest;

	for(j=0;j<listeCoups.nb;j++)
	{
		Org = listeCoups.coups[j].origine; 
		Dest = listeCoups.coups[j].destination;
		if(o==Org && d==Dest)
		ecrireIndexCoup(j);
	}
}


