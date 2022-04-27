/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

#define INDEX 0
#define OD 1


typedef struct {
	octet typeCoup;
	int index;
	int rentabilite;
	T_Coup od;
} T_Choix;



int verif5(octet o, octet d,T_Position currentPosition)
{

	T_Voisins voi;
	int i;

	currentPosition.cols[d].nb+=currentPosition.cols[o].nb;

	currentPosition.cols[o].nb=0;

	voi=getVoisins(d);

	for(i=0;i<voi.nb;i++)
	{
		if(currentPosition.cols[d].nb+currentPosition.cols[voi.cases[i]].nb <=5 )
		{
			if(currentPosition.cols[voi.cases[i]].couleur!=currentPosition.trait && currentPosition.cols[voi.cases[i]].couleur!=0)
			{
				return 0;
			}
		}
	}

return 1;
}






void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
T_Choix choix;
choix.typeCoup=INDEX;
choix.index=rand()%(listeCoups.nb-1);
choix.rentabilite=0;
int i,j,k,rentabilite,flag=0,nbcolsoccupee=0,LEvoisin;
octet macouleur=currentPosition.trait,o,d;
T_Coup Coup;
T_Voisins voisinsO,voisinsD;  // declaration de T_voisins pour stocker les voisins de destination et d'origine





/////////////////////////////////////////////////  PREMIERE BOUCLE QUI BALAYE LE TERRAIN PAR CASES ET NON VIA LA LISTE DE COUPS  //////////////////////////////////

for(i=0;i<=47;i++)
{

	/****************************************************************STRATEGIE VISANT A ISOLER UNE COLONNE ALIEE****************************************************************/

	if(currentPosition.cols[i].nb != 5) // VERIF QUE COLONNE CIBLE SOIT PLUS PETITE QUE 5
	{

		voisinsO=getVoisins(i);

		nbcolsoccupee=0;
		for(j=0;j<voisinsO.nb;j++)
		{
			if(currentPosition.cols[voisinsO.cases[j]].couleur != VIDE && currentPosition.cols[voisinsO.cases[j]].nb !=5)
			{
				 nbcolsoccupee = nbcolsoccupee + 1;
				 LEvoisin = voisinsO.cases[j];
			}
		}


		if(nbcolsoccupee == 1) // VERIFI QUE CETTE COLONNE N'A QU'UN VOISIN
		{

			nbcolsoccupee = 0;


			if(currentPosition.cols[i].couleur == macouleur) // SI CIBLE EST DE NOTRE COULEUR
			{

				if(currentPosition.cols[LEvoisin].couleur == macouleur) // SI VOISIN DE NOTRE COULEUR
				{

					voisinsD=getVoisins(LEvoisin);

					for(j=0;j<voisinsD.nb;j++)
					{
						if(currentPosition.cols[voisinsD.cases[j]].couleur != VIDE && currentPosition.cols[voisinsO.cases[j]].nb !=5) nbcolsoccupee++;
					}

					if(nbcolsoccupee>=2)   // VERIF QUE VOISIN AIT BIEN 2 VOISINS OU PLUS
					{
						nbcolsoccupee = 0;
						for(j=0;j<voisinsD.nb;j++)
						{
							if(currentPosition.cols[voisinsD.cases[j]].couleur != macouleur && currentPosition.cols[voisinsD.cases[j]].couleur != 0 && choix.rentabilite < 19 && currentPosition.cols[voisinsD.cases[j]].nb + currentPosition.cols[LEvoisin].nb <= 5) // PRIORITE A METTRE LE VOISIN SUR UN VOISIN MECHANT
							{
								flag=1;
								choix.typeCoup = OD;
								choix.od.destination = voisinsD.cases[j];
								choix.od.origine = LEvoisin;
								choix.rentabilite = 19;
								printf("----19----\n");
								printf("\n%u -> %u\n",choix.od.origine,choix.od.destination);
							}
						}

						if(flag!=1)
						{
							for(j=0;j<voisinsD.nb;j++)
							{
								if(currentPosition.cols[voisinsD.cases[j]].couleur == macouleur && voisinsD.cases[j] != i && choix.rentabilite < 18 && currentPosition.cols[voisinsD.cases[j]].nb + currentPosition.cols[LEvoisin].nb <= 5) // SI PAS DE VOISINS ENNEMIS, METTRE SUR ALLIE ET QUE LE VOISIN A ISOLER NE SOIT PAS LE NOTRE
								{
									choix.typeCoup = OD;
									choix.od.destination = voisinsD.cases[j];
									choix.od.origine = LEvoisin;
									choix.rentabilite = 18;
									printf("----18  1----\n");
									printf("\n%u -> %u\n",choix.od.origine,choix.od.destination);
								}
							}
						}

						flag=0;

					}
				}

				else if(currentPosition.cols[LEvoisin].couleur != macouleur) // SI VOISIN PAS DE NOTRE COULEUR
				{
					voisinsD=getVoisins(LEvoisin);
					nbcolsoccupee=0;

					for(j=0;j<voisinsD.nb;j++)
					{
						if(currentPosition.cols[voisinsD.cases[j]].couleur != VIDE) nbcolsoccupee++;
					}

					if(nbcolsoccupee>=2) // VERIF QUE VOISIN AIT BIEN 2 VOISINS OU PLUS
					{
						nbcolsoccupee=0;
						for(j=0;j<voisinsD.nb;j++)  // BOUCLE POUR PARCOURIR LES VOISINS DU VOISIN
						{
							if(currentPosition.cols[voisinsD.cases[j]].couleur != macouleur && currentPosition.cols[voisinsD.cases[j]].couleur != VIDE && currentPosition.cols[voisinsD.cases[j]].nb + currentPosition.cols[LEvoisin].nb <= 5 && choix.rentabilite < 18) // PRIORITE A METTRE LE VOISIN SUR UN VOISIN MECHANT
							{
								flag=1;
								choix.typeCoup = OD;
								choix.od.destination = voisinsD.cases[j];
								choix.od.origine = LEvoisin;
								choix.rentabilite = 18;
								printf("----18   2----\n");
								printf("\n%u -> %u\n",choix.od.origine,choix.od.destination);
							}
						}

						if(flag!=1)
						{
							for(j=0;j<voisinsD.nb;j++)
							{
								if(currentPosition.cols[voisinsD.cases[j]].couleur == macouleur && voisinsD.cases[j] != i && currentPosition.cols[voisinsD.cases[j]].nb + currentPosition.cols[LEvoisin].nb <= 5 && choix.rentabilite < 17) // SI PAS MECHANTS, FAUT METTRE SUR GENTIL ET QUE LE VOISIN A ISOLER NE SOIT PAS LE NOTRE
								{
									choix.typeCoup = OD;
									choix.od.destination = voisinsD.cases[j];
									choix.od.origine = LEvoisin;
									choix.rentabilite = 17;
									printf("----17----\n");
									printf("\n%u -> %u\n",choix.od.origine,choix.od.destination);
								}
							}
						}

					flag=0;

					}
				}
			}


			else if(currentPosition.cols[i].couleur != macouleur && currentPosition.cols[i].couleur != VIDE) // SI CIBLE EST PAS DE NOTRE COULEUR
			{

			 	if(currentPosition.cols[LEvoisin].couleur == macouleur && choix.rentabilite < 21 && currentPosition.cols[LEvoisin].nb + currentPosition.cols[i].nb <= 5) // SI LE VOISIN EST DE NOTRE COULEUR
				{
					choix.typeCoup = OD;
					choix.od.origine = LEvoisin;
					choix.od.destination = i;
					choix.rentabilite = 21;
					printf("----21----\n");
					printf("\n%u -> %u\n",choix.od.origine,choix.od.destination);
				}
			}
		}
	}
}










////////////////////////////////////////////////  DEUXIEME BOUCLE QUI BALAYE LE TERRAIN VIA LA LISTE DE COUPS  ////////////////////////////////////////////////////

for(i=0;i<listeCoups.nb;i++)
{
	Coup=listeCoups.coups[i];
	o=Coup.origine;
	d=Coup.destination;
	voisinsO=getVoisins(o); // on récupere les voisins de l'origine et on les stockes dans un T_Voisins
	voisinsD=getVoisins(d); // on récupere les voisins et on les stockes dans un T_Voisins

	if(choix.rentabilite <= 1)
	{
		if(macouleur == currentPosition.cols[o].couleur && macouleur != currentPosition.cols[d].couleur && (currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 2 ) {
			choix.typeCoup=INDEX;
			choix.index=i;
			choix.rentabilite=1;
			printf("----1----\n");
		}
	}


	/****************************************************************STRATEGIE VISANT A ANNIHILER LA POSSIBILITE QU'UN PION ENNEMI FASSE UNE TOUR DE 5 EN UTILISANT UNIQUEMENT SA COULEUR****************************************************************/



	if(macouleur!=currentPosition.cols[o].couleur && macouleur!=currentPosition.cols[d].couleur && (currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 5) //check si il y a cette possibilité
	{


		// on analyse les nb voisins de L'ORIGINE


		for(j=0;j<voisinsO.nb;j++)
		{
			if(currentPosition.cols[voisinsO.cases[j]].couleur == macouleur && (currentPosition.cols[voisinsO.cases[j]].nb + currentPosition.cols[o].nb) <= 5) // si le voisin est de ma couleur et que le fait de mettre le voisin sur l'origine du coup est possible (<=5)
			{
				if(verif5(voisinsO.cases[j],o,currentPosition)==1)
				{
					if(choix.rentabilite < 7) // si coup est plus rentable que l'ancien en mémoire
					{
						printf("\nSTRATEGIE 7 POINTS STOCKEE\n");
						choix.typeCoup = OD; // type du coup est changé
						choix.od.origine = voisinsO.cases[j];  // on actualise origine
						choix.od.destination = o; // on actualise destination
						choix.rentabilite = 7; // on actualise la rentabilite en memoire
					}
				}
			}
		}


		 // on analyse les nb voisins de la DESTINATION


		for(j=0;j<voisinsD.nb;j++)
		{
			if(currentPosition.cols[voisinsD.cases[j]].couleur == macouleur && (currentPosition.cols[voisinsD.cases[j]].nb + currentPosition.cols[d].nb) <= 5) // si le voisin est de ma couleur et que le fait de mettre le voisin sur l'origine du coup
			{
				if(verif5(voisinsD.cases[j],d,currentPosition) == 1)
				{
					if(choix.rentabilite < 7) // si coup est plus rentable que l'ancien en mémoire
					{
						choix.typeCoup = OD;
						choix.od.origine = voisinsD.cases[j];  // on change de coup
						choix.od.destination = d;
						choix.rentabilite = 7; // on actualise la rentabilite en memoire
						printf("\nSTRATEGIE 7 POINTS STOCKEE\n");
					}
				}
			}
		}
	}



	/****************************************************************STRATEGIE VISANT A REALISER UNE TOUR DE 5 DE NOTRE COULEUR SI IL Y A POSSIBILITE****************************************************************/


	if(macouleur == currentPosition.cols[o].couleur)
	{
	//	printf("premier if\n");
		if(currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5)
		{


			//	REALISER TOUR DE 5 SUR UNE COLONNE ADVERSE


			if(currentPosition.cols[d].couleur != macouleur && currentPosition.cols[d].couleur != VIDE)
			{
				if(choix.rentabilite < 20)
				{
					choix.typeCoup = INDEX;
					choix.index = i;
					choix.rentabilite = 20;
					printf("c'est la 20\n");
				}
			}


			//	REALISER TOUR DE 5 SUR UNE COLONNE ALLIEE


			if(currentPosition.cols[d].couleur == macouleur)
			{
				if(choix.rentabilite < 19)
				{
					choix.typeCoup = INDEX;
					choix.index = i;
					choix.rentabilite = 19;
					printf("c'est la 19\n");
				}
			}
		}
	}

	if(currentPosition.cols[d].couleur != VIDE)
	{
		//printf("encore un if 1\n");
		j=currentPosition.cols[o].nb + currentPosition.cols[d].nb;

		if(j < 5)
		{
		//	printf("encore un if 2\n");
			voisinsD = getVoisins(d);
			flag = 0;

			for (k = 0; k < voisinsD.nb; k++)
			{
				if(currentPosition.cols[voisinsD.cases[k]].couleur != macouleur && currentPosition.cols[voisinsD.cases[k]].couleur != VIDE)
				{
				//	printf("encore un if 4\n");
					if(currentPosition.cols[voisinsD.cases[k]].nb + j <= 5)
					{
						flag = 1;
					}
				}
			}
			if(flag!=1)
			{
				if(choix.rentabilite < 5)
				{
					printf("c'est la 5\n");
					choix.typeCoup=INDEX;
					choix.index=i;
					choix.rentabilite=5;
				}
			}
		}
	}
}




				if(choix.typeCoup==OD)
				{
					for(i=0;i<listeCoups.nb;i++)
					{
						if(listeCoups.coups[i].origine==choix.od.origine && listeCoups.coups[i].destination==choix.od.destination)
						{
							ecrireIndexCoup(i);
							printf("\n\t%u -> %u\n",choix.od.origine,choix.od.destination);
						}
					}

				//	ecrireCoup(choix.od.origine,choix.od.destination);
				}

				if(choix.typeCoup==INDEX)
				{
					printf("%d\n",choix.index);
					ecrireIndexCoup(choix.index);
				}
		}
