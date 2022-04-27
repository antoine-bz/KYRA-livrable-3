/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l
	int i,k,rentabilite=0;
	int pred = 0;
	int test = 0;
	octet o, d;
	octet myColor = currentPosition.trait;
	T_Voisins voisins;


	for (i = 0; i < listeCoups.nb; i++)
	{

		o = listeCoups.coups[i].origine;
		d = listeCoups.coups[i].destination;


		//Coup de fin
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && ((currentPosition.cols[o].nb + currentPosition.cols[d].nb) <= 5))
		{
			if(rentabilite ==0)
			{
				ecrireIndexCoup(i);
				printf("Coup fin %d\n",i);
				rentabilite =0;
			}
		}

		//ON AVALAM
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb != 3))
		{
			if(rentabilite<=1)
			{
				ecrireIndexCoup(i);
				printf("Coup Avalam %d\n",i);
				rentabilite =1;
			}
		}


			//1 sur 4
			if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].nb == 4))
			{
				if(rentabilite <=2)
				{
					ecrireIndexCoup(i);
					printf("Coup 1sur4 %d\n",i);
					rentabilite =2;
				}
			}
			//4 sur 1
			if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 4) && (currentPosition.cols[d].nb == 1))
			{
				if(rentabilite <=2)
				{
					ecrireIndexCoup(i);
					printf("Coup 1sur4 %d\n",i);
					rentabilite =2;
				}
			}
			//2 sur 3
			if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d].nb == 3))
			{
				if(rentabilite <=2)
				{
					ecrireIndexCoup(i);
					printf("Coup 1sur4 %d\n",i);
					rentabilite =2;
				}
			}
			//3 sur 2
			if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 3) && (currentPosition.cols[d].nb == 2))
			{
				if(rentabilite <=2)
				{
					ecrireIndexCoup(i);
					printf("Coup 1sur4 %d\n",i);
					rentabilite =2;
				}
			}

	}

//JAUNE
/*
	if (myColor == 1) //je suis jaune
		{
			if (currentPosition.cols[21].couleur == myColor && currentPosition.cols[12].couleur ==2	)//1
			{
				test=1;
				o=21;
				d=12;
				printf("%d 17 if\n",i);
				}
				if (currentPosition.cols[20].couleur != myColor && currentPosition.cols[12].couleur != myColor && currentPosition.cols[12].nb == 1	)//1
				{
					test=1;
					o=20;
					d=12;
					printf("%d 18 if\n",i);
					}
				if (currentPosition.cols[29].couleur != myColor && currentPosition.cols[22].couleur != myColor && currentPosition.cols[22].nb == 1	)//1
				{
					test=1;
					o=29;
					d=22;
					printf("%d 19 if\n",i);
					}
				if (currentPosition.cols[36].couleur != myColor && currentPosition.cols[31].couleur != myColor && currentPosition.cols[31].nb == 1	)//1
				{
					test=1;
					o=36;
					d=31;
					printf("%d 20 if\n",i);
					}




				if (currentPosition.cols[11].couleur != myColor && currentPosition.cols[16].couleur != myColor && currentPosition.cols[16].nb == 1	)//1
				{
					test=1;
					o=11;
					d=16;
					printf("%d 22 if\n",i);
					}
				if (currentPosition.cols[18].couleur != myColor && currentPosition.cols[25].couleur != myColor && currentPosition.cols[25].nb == 1	)//1
				{
					test=1;
					o=18;
					d=25;
					printf("%d 23 if\n",i);
					}
				if (currentPosition.cols[27].couleur == myColor 	)//1
				{
					test=1;
					o=27;
					d=35;
					printf("%d 26 if\n",i);
					}
		}
*/
//ROUGE
	if (myColor==2)// si je suis rouge
		{

			if (currentPosition.cols[26].couleur==myColor && currentPosition.cols[34].couleur ==1 )//4
			{
				test=1;
				o=26;
				d=34;
				printf("%d 44\n",i);
			}
			if (currentPosition.cols[18].couleur==myColor  && currentPosition.cols[26].couleur ==1 )//3
			{
				test=1;
				o=18;
				d=26;
				printf("%d 43\n",i);
			}
			if (currentPosition.cols[19].couleur!=myColor && currentPosition.cols[19].couleur ==1)//2
			{
				test=1;
				o=18;
				d=19;
				printf("%d 42\n",i);
			}
				if (currentPosition.cols[27].couleur == myColor )//1
				{
					test=1;
					o=27;
					d=19;
					printf("%d 41 if\n",i);
					}

				if (currentPosition.cols[21].couleur==myColor && currentPosition.cols[30].couleur ==1 )//4
				{
					test=1;
					o=21;
					d=30;
					printf("%d 34\n",i);
				}
				if (currentPosition.cols[28].couleur==myColor && currentPosition.cols[21].couleur ==1)//3
				{
					test=1;
					o=20;
					d=21;
					printf("%d 33 if\n",i);
				}
				if (currentPosition.cols[28].couleur!=myColor && currentPosition.cols[21].nb != 0 && currentPosition.cols[21].nb == 1)//2
				{
					test=1;
					o=20;
					d=28;
					printf("%d 32 \n",i);
				}
				if (currentPosition.cols[29].couleur == myColor	)//1
				{
					test=1;
					o=29;
					d=28;
					printf("%d 31 if\n",i);
					}
		}

		if(test == 1){
			for (i = 0; i < listeCoups.nb; i++)
			{
				if(listeCoups.coups[i].origine == o && listeCoups.coups[i].destination == d)
				ecrireIndexCoup(i);
			}

		}
}


/*
			//VOISIN
			pred = 0;
			voisins = getVoisins(d);

			for (k = 0; k < voisins.nb; k++)
			{
				if((currentPosition.cols[voisins.cases[k]].couleur != myColor) && (currentPosition.cols[voisins.cases[k]].couleur != VIDE))
				{
					pred ++;
				}
			}
				 if(pred==0) ecrireIndexCoup(i);
		}
*/


			/*
					for (i = 0; i < listeCoups.nb; i++)
					{
						o = listeCoups.coups[i].origine;
						d = listeCoups.coups[i].destination;

						pred = 0;
						voisins = getVoisins(d);

						for (k = 0; k < voisins.nb; k++)
						{
							if(currentPosition.cols[voisins.cases[k]].couleur != myColor && currentPosition.cols[voisins.cases[k]].couleur != VIDE)
							{
							  pred ++;
							}
						}
				       if(pred==0) ecrireIndexCoup(i);
					}
			*/


/*
//pile oklm
if (myColor == 1) //je suis jaune
	{
		if ((currentPosition.cols[28].nb > 1) && (currentPosition.cols[19].nb > 1))
		{
		if ((o == 29) &&  (d == 22))
		ecrireIndexCoup(i);

		if ((o == 20) &&  (d == 12))
		ecrireIndexCoup(i);

		if ((o == 21) &&  (d == 22))
		ecrireIndexCoup(i);


		if ((o == 27) &&  (d == 35))
		ecrireIndexCoup(i);

		if ((o == 18) &&  (d == 25))
		ecrireIndexCoup(i);

		if ((o == 26) &&  (d == 25))
		ecrireIndexCoup(i);

		if ((o == 26) &&  (d == 35))
		ecrireIndexCoup(i);

		}
	}

	if (myColor==2)// si je suis rouge
	{
			if (currentPosition.cols[13].nb == 1)
			{
				if ( (o == 21) &&  (d == 13))//3
				ecrireIndexCoup(i);
			}
			if (currentPosition.cols[23].nb == 1)
			{
				if ( (o == 30) &&  (d == 23))//4s
				ecrireIndexCoup(i);
			}
			if (currentPosition.cols[32].nb == 1)
			{
				if ( (o == 37) &&  (d == 32))//3
				ecrireIndexCoup(i);
			}
			if (currentPosition.cols[39].nb == 1)
			{
				if ( (o == 42) &&  (d == 39))//2
				ecrireIndexCoup(i);
			}
			if (currentPosition.cols[44].nb == 1)
			{
				if ( (o == 46) &&  (d == 44))//1
				ecrireIndexCoup(i);
			}
	}
	*/
