/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int coupVoisins(octet dBis,octet oBis,T_Position P);

int testCoup(int pot,int max,int i,T_Position P)
{
	if(P.trait==2) //On test la couleur du trait pour effectuer de quelconques modifications  
	{
		if(pot>max) // On échange la valeur de max si elle est inférieur au potentiel du coup
		{
			max=pot;
		 
			ecrireIndexCoup(i); // On écrit le coup dans ce cas
		}
	}
	else
	{
		if(pot>max)
		{
			max=pot;
		 
			ecrireIndexCoup(i);
		}
	}
	
	return max;
}

int calculMax(int pot,int max)
{
	if(pot>max) // On échange la valeur de max si elle est inférieur au potentiel du coup
	{
		max=pot;
	}
}

int calculMin(int pot,int min)
{
	if(pot<min) // On échange la valeur de min si elle est supérieur au potentiel du coup
	{
		min=pot;
	}
	return min;
}

int test(octet dBis,octet oBis,T_Position P)/*Dans cette fonction on teste le meilleur coup que l'on peut faire après avoir regardé le pire coup de l'énnemi*/
{
	T_Voisins voisins; // Voisins de la tour jouée
	T_ListeCoups l;  // Nouvelle liste des coups possibles
	voisins = getVoisins(dBis);

	int pot;		/*Le potentiel d'un coup*/
	int max = -50;	/*Le potentiel maximum*/
	int cptAm =0;	/*Le nombre de tour amies voisine de la tour jouée*/
	int cptEn =0;	/*Le nombre de tour ennemies voisine de la tour jouée*/

	octet o,d;		/*Octet d'origine et de destinations*/

	l.nb = 0;		/*On initialise le nombre de voisins à 0*/
	listerVoisins(dBis);
	P = jouerCoup(P,oBis,dBis); //On simule le coup à jouer

	for (int j = 0; j < voisins.nb; ++j) //  Recalcul de la liste des coups possible sur la tour jouée
	{
		printf("nb = %d\n",P.cols[voisins.cases[j]].nb);
		if(((P.cols[voisins.cases[j]].nb)!=0) && ((P.cols[voisins.cases[j]].nb + P.cols[dBis].nb )<= 5))
		{
			/*Coup tour voisins sur tour jouée*/
			l.coups[l.nb].origine = voisins.cases[j];
			l.coups[l.nb].destination = dBis;	
			l.nb++;

			/*Coup tour jouée sur tour voisins*/
			l.coups[l.nb].origine = dBis;
			l.coups[l.nb].destination = voisins.cases[j];
			l.nb++;

			if(P.cols[voisins.cases[j]].couleur == P.trait)
			{
				cptAm++; //Nombre de tour amies de la tour jouée
			}
			else
			{
				cptEn++; // Nombre de tour ennemies de la tour jouée
			}
		}
	}
	afficherListeCoups(l);

	if((l.nb == 0) || (cptAm==0 && cptEn > 0))// Ne pas jouée le coup si plus aucun coup n'est possible après le coup énnemi
	{
		max = -10;
	}
	else
	{
		for(int i=0;i<l.nb; i++)
		{
			o = l.coups[i].origine;    // position origine du coup
			d = l.coups[i].destination;// position destination du coup  

			//Ami sur ami
			if(P.cols[o].couleur == P.trait && P.cols[d].couleur == P.trait)
			{
				switch(P.cols[o].nb + P.cols[d].nb)
				{
					//tour de 2
					case 2: pot = 3 + coupVoisins(d,o,P); //On appel la fonction tour voisin
					break;
					//tour de 3
					case 3: pot = 3 + coupVoisins(d,o,P); // On appel la fonction tour voisin
					break;
					//tour de 4
					case 4:
							pot = 2 + coupVoisins(d,o,P); // On appel la ...
					break;
					//tour de 5
					case 5:pot = 10;
					break;
				}
				max = calculMax(pot,max); // Recalcul du meilleur coup
				
			}
			// Ami sur ennemi
			else if(P.cols[o].couleur == P.trait && P.cols[d].couleur != P.trait)
			{
				if(P.cols[o].nb < P.cols[d].nb)// Il vaut mieux jouer un ami sur 2 ennemi
				{
					switch(P.cols[o].nb + P.cols[d].nb)
					{
						//tour de 2
						case 2: pot = 6 + coupVoisins(d,o,P);
						break;

						//tour de 3
						case 3: pot = 6 + coupVoisins(d,o,P);
						break;

						//tour de 4
						case 4:
								pot = 5 + coupVoisins(d,o,P);
						break;

						//tour de 6
						case 5:pot = 15; //Pas de test à faire ici normalement
						break;
					}
				}
				else // Plutôt que 2 ami sur un ennemi
				{
					switch(P.cols[o].nb + P.cols[d].nb)
					{
						//tour de 2
						case 2: pot = 4 + coupVoisins(d,o,P);
						break;

						//tour de 3
						case 3: pot = 4 + coupVoisins(d,o,P);
						break;

						//tour de 4
						case 4:
								pot = 3 + coupVoisins(d,o,P);
						break;

						//tour de 5
						case 5:pot = 13;
						break;
					}
				}
				max = calculMax(pot,max);// Recalcul du meilleur coup
			}
			// Ennemi sur ennemi
			else if(P.cols[o].couleur != P.trait && P.cols[d].couleur != P.trait)//TODO
			{
				switch(P.cols[o].nb + P.cols[d].nb)
				{
					//tour de 2
					case 2: pot = 4+coupVoisins(d,o,P);
					break;

					//tour de 3
					case 3: pot = 4+coupVoisins(d,o,P);
					break;

					//tour de 4
					case 4: pot = -4;
					break;

					//tour de 5
					case 5:pot = 0;
					break;
				}
				max = calculMax(pot,max);// Recalcul du meilleur coup
			}
			//Ennemi sur ami (à ne pas faire)
			else
			{
				switch(P.cols[o].nb + P.cols[d].nb)
				{
					//tour de 2
		            case 2: pot = -20;
		            break;

		            //tour de 3
		            case 3: pot = -30;
		            break;

		            //tour de 4
		            case 4: pot = -40;
		            break;

		            //tour de 5
		            case 5:pot = -50;
		            break;
		        }
			}

		}
	}
	
	return max; // On retourne le potentiel maximum qui est récupéré dans la fonction coupVoisins
}

int coupVoisins(octet dBis,octet oBis,T_Position P)
{
	T_Voisins voisins;  //Voisins une fois le coup jouée
	T_Voisins voisinsoBis; //Voisins de l'origine avant de jouer le coup
	T_Voisins voisVois;

	T_ListeCoups l; // Liste de coup possible sur le coup que l'on va simuler

	int pot;		//Le potentiel d'un coup
	int min = 0;	//Le potentiel minimum d'un coup
	int cptEn = 0;	//Le nombre de tour ennemies voisines de la tour jouée
	int cptAm = 0;	//Le nombre de tour ennemies voisines de la tour jouée
	int voisEnoBis =0;// Le nombre de tour ennemies voisines de notre tour origine Bis
	int voisAmoBis =0;// Le nombre de tour amies voisines de notre tour origine Bis
	int coloroBis = P.cols[oBis].couleur; // La couleur de la tour origine Bis
	int colordBis = P.cols[dBis].couleur; // La couleur de la tour destination Bis
	int voisVoisAm=0,voisVoisEn=0; // Le nombre de tour voisines amies/ennemies de d'une tour voisine de la tour origine Bis
	int danger=0;	// Danger est à 1 si une tour voisine de la tour origine Bis va se retrouver seule face à l'ennemi après le coup

	octet o,d; // Origine et destination des nouveaux coups possibles ennemi

	voisins = getVoisins(dBis);    // On récupère les voisins de la tour jouée
	voisinsoBis = getVoisins(oBis); // On récupère les voisins de l'origine Bis

	l.nb = 0;  // On initialise le nombre de coup possible à 0

	listerVoisins(dBis); // Printf de debug


	for (int j = 0; j < voisinsoBis.nb; ++j)
	{
		if(((P.cols[voisinsoBis.cases[j]].nb)!=0) && (P.cols[voisinsoBis.cases[j]].couleur != P.trait) && ((P.cols[voisinsoBis.cases[j]].nb + P.cols[oBis].nb )<= 5))
		{
			voisEnoBis++;  // Calcul du nombre de tours voisines ennemies de l'origine Bis
		}
		else if(((P.cols[voisinsoBis.cases[j]].nb)!=0) && (P.cols[voisinsoBis.cases[j]].couleur == P.trait)&& ((P.cols[voisinsoBis.cases[j]].nb + P.cols[oBis].nb )<= 5))
		{
			voisAmoBis++; // Calcul du nombre de tours voisines amies de l'origine Bis
			voisVois = getVoisins(voisinsoBis.cases[j]); // On récupère les voisins de la tour voisine amie de l'origine Bis

			for (int k = 0; k < voisVois.nb; ++k)
			{
				if(((P.cols[voisVois.cases[k]].nb)!=0) && (P.cols[voisVois.cases[k]].couleur != P.trait) && ((P.cols[voisVois.cases[k]].nb + P.cols[voisinsoBis.cases[j]].nb )<= 5))
				{
					voisVoisEn++;  // Calcul du nombre de tours ennemies qui mettent potentiellement en danger notre tour voisine après le coup
				}
				else if(((P.cols[voisVois.cases[k]].nb)!=0) && (P.cols[voisVois.cases[k]].couleur == P.trait)&& ((P.cols[voisVois.cases[k]].nb + P.cols[voisinsoBis.cases[j]].nb )<= 5))
				{
					voisVoisAm++;// Calcul du nombre de tours amies qui sécurisent notre tour voisine
				}
			}
			if(voisVoisAm == 1 && voisVoisEn != 0) // Si la tour voisine possède une unique tour voisine ami(origine Bis) et au moins un ennemi
			{
				danger=1;  // On fixe danger à 1 pour indiquer qu'il ne faut pas augmenter le potentiel pour le reste de la fonction
				min = -10; // On place min à -10 par sécurité
			}
		}
	}
	printf("\ndanger = %d",danger);

	P = jouerCoup(P,oBis,dBis);  // On joue le coup pour simuler la position du plateau si le coup était jouée


	/*Cette boucle for calcul la liste des coup possible entre la position de la tour jouée	*/
	/*et les voisins qu'elle possède.														*/

	for (int j = 0; j < voisins.nb; ++j)
	{
		if(((P.cols[voisins.cases[j]].nb)!=0) && ((P.cols[voisins.cases[j]].nb + P.cols[dBis].nb )<= 5))//On test si le coup est possible
		{
			/*Coup tour voisins sur tour jouée*/
			l.coups[l.nb].origine = voisins.cases[j];
			l.coups[l.nb].destination = dBis;	
			l.nb++;

			/*Coup tour jouée sur tour voisins*/
			l.coups[l.nb].origine = dBis;
			l.coups[l.nb].destination = voisins.cases[j];
			l.nb++;

			if(P.cols[voisins.cases[j]].couleur != P.trait)
			{
				cptAm++; //Nombre de tour amis de la tour jouée
			}
			else
			{
				cptEn++; // Nombre de tour ennemis de la tour jouée
			}
		}
	}
	afficherListeCoups(l);

/*Si il n'y a plus de coups possible et que l'on n'a jouée ami sur ennemi et qu'il n'y a pas de tour voisines en danger*/
	if(l.nb == 0 && coloroBis !=P.trait && colordBis == P.trait && danger ==0)
	{
		if(voisAmoBis != 0 && voisEnoBis==0) // Si la tour origine possède au moins une tour vosine ami et aucune tour ennemi 
		{
			min = 12;
		}
		else // Sinon on diminue légèrement le potentiel
		{
			min = 10;
		}
	}
	else if(l.nb==0 && colordBis !=P.trait)//Si on joue ami sur ami et qu'il n'y a plus de coups possible
	{
		min=-7; // Ce coup n'est pas à jouée, c'est un coup inutile, mais pas dangereux
	}
	else if(l.nb==0 && coloroBis == P.trait && colordBis==P.trait && voisAmoBis>=1 && voisEnoBis==1)// On joue ennemi sur ennemi pour sauver une tour ami
	{
		min=10;
	}
	else // On calcul le pire coup que l'adversaire peut nous faire après avoir jouée
	{
		for (int j = 0; j < l.nb; ++j)
		{

			o = l.coups[j].origine;    // Nouvelle origine du coup adverse
			d = l.coups[j].destination; // Nouvelle origine du coup adverse

			if(P.cols[o].couleur != P.trait) //Si la tour obtenue de notre couleur
			{
				switch(P.cols[o].nb + P.cols[d].nb)
				{
					//tour de 3
					case 3: pot = 3;
					break;

					//tour de 4
					case 4: pot = 4;
					break;

					//tour de 5
					case 5:pot = 5;
					break;
				}
				min = calculMin(pot,min); //Recalcul du pire coup
			}
			else
			{
				switch(P.cols[o].nb + P.cols[d].nb) //Si la tour obtenue est de couleur ennemi
				{
					//tour de 3
					case 3: pot = 3 + test(d,o,P);
					break;

					//tour de 4
					case 4: pot = 3 + test(d,o,P);
					break;

					//tour de 5
					case 5:pot = -10;
					break;
				}
				min = calculMin(pot,min); //Recalcul du pire coup
			}
		}
		if(cptAm == 0 && cptEn >= 1) //Pas d'ami et au moins un ennemi autour
		{
			min = -15;
		}
		else if(cptEn == 0 && colordBis == P.trait && danger==0) //Si on joue sur une tour ennemi et qu'il n'y a plus d'ennemi autour
		{
			if(coloroBis != P.trait) // Si la tour origine Bis est de notre couleur
			{
				if(voisAmoBis != 0 && voisEnoBis==1)// Si la tour origine possède au moins une tour vosine ami et aucune tour ennemi après coup
				{  
					min = 9;
				}
				else // Sinon on dimunu légèrement le potentiel
				{
					min = 7;
				}
			}
			else if(cptAm>=2 && min!=-10 && danger==0)// Si au moins deux tour voisines amis et que l'ennemi ne peut pas faire de tour de 5 //TODO:modifier !=
			{
				min = 7;
			}
			else
			{
				min = -7;
			}
		}
	}

	if(voisEnoBis == 0) //Si la tour oBis possède aucun voisins ennemis
	{
		min=-7;
	}
	printf("\n\nmini = %d\n\n",min);
	
	return min;
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i;
	int pot;		//Potentiel d'un coup
	int max=-50;	// Potentiel maximum des coups
	octet o, d;     // Origine et destination du coup
	octet myColor = currentPosition.trait; 

	//afficherListeCoups(listeCoups);

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));

	for(i=0;i<listeCoups.nb; i++)
	{
		o = listeCoups.coups[i].origine;    // position origine du coup
		d = listeCoups.coups[i].destination;// position destination du coup  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

		//Ami sur ami
		if(currentPosition.cols[o].couleur == myColor && currentPosition.cols[d].couleur == myColor)
		{
			switch(currentPosition.cols[o].nb + currentPosition.cols[d].nb)
			{
				//tour de 2
				case 2: pot = 3 + coupVoisins(d,o,currentPosition);
						printf("pot = %d",pot);
				break;
				//tour de 3
				case 3: pot = 3 + coupVoisins(d,o,currentPosition);
				break;
				//tour de 4
				case 4:
						pot = 2 + coupVoisins(d,o,currentPosition);
				break;
				//tour de 5
				case 5:pot = 5 +coupVoisins(d,o,currentPosition); // Pas de test à faire ici
				break;
			}
			max = testCoup(pot,max,i,currentPosition); // Recalcul du meilleur coup
			
		}
		// Ami sur ennemi
		else if(currentPosition.cols[o].couleur == myColor && currentPosition.cols[d].couleur != myColor)
		{
			if(currentPosition.cols[o].nb < currentPosition.cols[d].nb)// Il vaut mieux jouer un ami sur 2 ennemi
			{
				switch(currentPosition.cols[o].nb + currentPosition.cols[d].nb)
				{
					//tour de 2
					case 2: pot = 6 + coupVoisins(d,o,currentPosition);
							printf("pot = %d",pot);
					break;

					//tour de 3
					case 3: pot = 6 + coupVoisins(d,o,currentPosition);
					break;

					//tour de 4
					case 4:
							pot = 5 + coupVoisins(d,o,currentPosition);
					break;

					//tour de 6
					case 5:pot = 15; //Pas de test à faire ici
					break;
				}
			}
			else // Plutôt que 2 ami sur un ennemi
			{
				switch(currentPosition.cols[o].nb + currentPosition.cols[d].nb)
				{
					//tour de 2
					case 2: pot = 4 + coupVoisins(d,o,currentPosition);
							printf("pot = %d",pot);
					break;

					//tour de 3
					case 3: pot = 4 + coupVoisins(d,o,currentPosition);
					break;

					//tour de 4
					case 4:
							pot = 3 + coupVoisins(d,o,currentPosition);
					break;

					//tour de 5
					case 5:pot = 13; // Pas de test à faire ici
					break;
				}
			}
			max = testCoup(pot,max,i,currentPosition); // Recalcul du meilleur coup
		}
		// Ennemi sur ennemi
		else if(currentPosition.cols[o].couleur != myColor && currentPosition.cols[d].couleur != myColor)//TODO
		{
			switch(currentPosition.cols[o].nb + currentPosition.cols[d].nb)
			{
				//tour de 2
				case 2: pot = 4+coupVoisins(d,o,currentPosition);
				break;

				//tour de 3
				case 3: pot = 4+coupVoisins(d,o,currentPosition);
				break;

				//tour de 4
				case 4: pot = -4;
				break;

				//tour de 5
				case 5:pot = 12;
				break;
			}
			max = testCoup(pot,max,i,currentPosition); // Recalcul du meilleur coup
		}
		//Ennemi sur ami (à ne pas faire)
		else
		{
			switch(currentPosition.cols[o].nb + currentPosition.cols[d].nb)
			{
				//tour de 2
	            case 2: pot = -20;
	            break;

	            //tour de 3
	            case 3: pot = -30;
	            break;

	            //tour de 4
	            case 4: pot = -40;
	            break;

	            //tour de 5
	            case 5:pot = -50;
	            break;
	        }
		}
		max = testCoup(pot,max,i,currentPosition); // Recalcul du meilleur coup

	}
}