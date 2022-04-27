/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l
	int i,k;
	int pred = 0;
	octet o, d;
	octet myColor = currentPosition.trait;
	T_Voisins voisins;


	for (i = 0; i < listeCoups.nb; i++)
	{

		o = listeCoups.coups[i].origine;
		d = listeCoups.coups[i].destination;


		//1 sur 4
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].nb == 4))
		{
			ecrireIndexCoup(i);
		}
		//4 sur 1
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 4) && (currentPosition.cols[d].nb == 1))
		{
			ecrireIndexCoup(i);
		}
		//2 sur 3
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d].nb == 3))
		{
			ecrireIndexCoup(i);
		}
		//3 sur 2
		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 3) && (currentPosition.cols[d].nb == 2))
		{
			ecrireIndexCoup(i);
		}



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


			//ON AVALAM
			if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb != 3))
			{
				ecrireIndexCoup(i);
			}

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


	}


}
