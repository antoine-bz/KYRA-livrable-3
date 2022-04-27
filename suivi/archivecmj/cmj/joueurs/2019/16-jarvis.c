												/*****************************************************************/
												/************** Moteur pour le tournoi : Jarvis v5.0  ************/
												/************** 	Groupe n°16 : Les Puissances 4	  ************/
												/*****************************************************************/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//include
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//structure
typedef struct 
	{
    int poidsCase[48];//on va stocker ici les poids de chaque case
    octet caseO[2];//dans la première case on met le meilleur poids du plateau et dans la deuxième le numéro de la case associé sur le plateau
   	octet caseD[2];//dans la première case on met le meilleur poids voisin à l'origine et dans la deuxième le numéro de la case associé sur le plateau
	} T_strat;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sous fonction

//fonction pour initialiser la structure servant à la stratégie
void initialisationStrat(T_strat *strat)
{
	//remplissage des poids des cases à 0
	for (int i=0;i<48;++i)
	{
		strat->poidsCase[i] = 0;
	}
	//remplissage des varaibles de contrôle de l'origine et de destination
	strat->caseO[0] = 100;
	strat->caseO[1] = -2;
	strat->caseD[0] = -1;
	strat->caseD[1] = -2;
}


//fonction récupérant le caractère allié ennemi d'une case
int caractereAllieEnnemi(T_Position *pp, int col)
{
	int allie;
	if(col == 0)	//si la case est vide on lui attribue la valeur 0
	{
		allie=0;
	}
	else	//sinon on compare la couleur avec la couleur du trait qui nous est associé
	{
		if(col == pp->trait)	//si les deux sont les mêmes alors on est sur une case allié on renvoie allie = 1
		{
			allie =1;
		}
		else	//sinon elle est ennemi on renvoie allie = 2 
		{
			allie =2;
		}
	}
	return allie; //on renvoie la valeur de allie
}

//fonction ajoutant ou retirant du poids selon les voisins puis sous certaines conditions
void ponderationAnnexe(T_Position *pp, T_strat *strat, int pos, int allie)
{
	int i,nbAllie=0,nbEnnemi=0,casPart=0,T5possible=0,flag=0; 
	octet col, haut, colV, hautV;

	//on récupère les infos de la case analysée
	col = pp->cols[pos].couleur;
	haut = pp->cols[pos].nb;
	//on commence par récupérer les voisins de la case analysée
	T_Voisins v = getVoisins(pos);
	//on boucle sur chaque voisin


	printf("->%d<-\n",pos );


	for(i=0;i<v.nb;i++) 
	{
		//on récupère les caractéristiques de la case voisines 
		colV = pp->cols[v.cases[i]].couleur;
		hautV = pp->cols[v.cases[i]].nb;
		//on commence par fair eun switch selon la couleur du voisin
		//on applique ensuite un poids selon la nature allié ennemi du voisin par rapport a la case d'origine
		switch (colV)
		{
			case 1 : //si la case est jaune
			//on regarde déjà si elle n'est pas de taille 5 sinon on ne si attarde pas
			if(hautV != 5)
			{
				//si la couleur et égale au caractère allié on augmente le poids de l'origine sinon on baisse
				if(col == colV)
				{
					strat->poidsCase[pos] = strat->poidsCase[pos]+2;
				}
				else
				{
					strat->poidsCase[pos] = strat->poidsCase[pos]-2;
				}
				//on regarde si la case voisine est allié ou ennemi et on compte
				if(pp->trait == colV)
				{
					nbAllie=nbAllie + 1;
				}
				else
				{
					nbEnnemi=nbEnnemi + 1;
				}

				//cas particulier si la somme des deux est égale à 4 on modifie leur valeur pour eviter le coup
				if(haut + hautV ==4)
				{
					strat->poidsCase[pos] = 3;
				}

				//cas particulier si la somme des deux est égale à 5 on recupere le type de cas
				if(haut + hautV == 5)
				{		
					//marqueur de tour de 5 possible en 3 valeurs 1 la plus forte config 2 la acceptable et 3 la si rien d'autres
					if((col == pp->trait)&&(colV != col))
					{
						T5possible = 1;
					}
					if(col == colV == pp->trait)
					{
						T5possible = 2;
					}
					if(col == colV != pp->trait)
					{
						T5possible = 3;
					}
				}
			}
			break;
			case 2 : //si la case est rouge
			//on regarde déjà si elle n'est pas de taille 5 sinon on ne si attarde pas
			if(hautV != 5)
			{
				//si la couleur et égale au caractère allié on augmente le poids de l'origine sinon on baisse
				if(col == colV)
				{
					strat->poidsCase[pos] = strat->poidsCase[pos]+2;
				}
				else
				{
					strat->poidsCase[pos] = strat->poidsCase[pos]-2;
					
				}
				//on regarde si la case voisine est allié ou ennemi et on compte
				if(pp->trait == colV)
				{
					nbAllie=nbAllie + 1;
				}
				else
				{
					nbEnnemi=nbEnnemi + 1;
				}

				//cas particulier si la somme des deux est égale à 4 on modifie leur valeur pour eviter le coup
				if(haut + hautV ==4)
				{
					strat->poidsCase[pos] = 3;
				}

				//cas particulier si la somme des deux est égale à 5 on recupere le type de cas
				if(haut + hautV == 5)
				{		
					//marqueur de tour de 5 possible en 3 valeurs 1 la plus forte config 2 la acceptable et 3 la si rien d'autres
					if((col == pp->trait)&&(colV != col))
					{
						T5possible = 1;
					}
					if(col == colV == pp->trait)
					{
						T5possible = 2;
					}
					if(col == colV != pp->trait)
					{
						T5possible = 3;
					}
				}
			}
			break;
			case 0 : //cas d'une case voisine vide
				strat->poidsCase[pos] = strat->poidsCase[pos]+2;
			break;
		}
	}

	//on passe maintenant au cas particulier
	//les cas en fonction des cases allié
	if(col == pp->trait)
	{
		//si il y a plus d'alliees que d'ennemis autour on augmente le poids de la piece pour privilégier des coups allie allie
		if(nbEnnemi <= nbAllie)
		{
			strat->poidsCase[pos] = strat->poidsCase[pos] - 3;
		}

		//si il n'y a pas d'ennemi on passe le poids de la case a 2 (une valeur faible) c'est une tour en théorie gagné
		if(nbEnnemi == 0 && casPart ==0)
		{
			printf("le pion isolé\n");
			strat->poidsCase[pos] = 2;
		}

		//si il y a un unique ennemi on baisse le poids de la case pour faire jouer ensuite l'ennemi plus loin
		if(nbEnnemi == 1)
		{
			printf("ennemi unique\n");
			strat->poidsCase[pos] = 30;
			for(i=0;i<v.nb;i++) 
			{
				//on récupère les caractéristiques de la case voisines 
				colV = pp->cols[v.cases[i]].couleur;
				hautV = pp->cols[v.cases[i]].nb;
				//on commence par faire un switch selon la couleur du voisin
				switch (colV)
				{
					case 1 : //si la case est jaune
					//on regarde déjà si elle n'est pas de taille 5 sinon on ne si attarde pas
					if(hautV != 5)
					{
						if(col == colV)
						{
							strat->poidsCase[v.cases[i]] = 5;
						}
						else
						{
							strat->poidsCase[v.cases[i]] = 20;
						}
					}
					break;
					case 2 : //si la case est rouge
					//on regarde déjà si elle n'est pas de taille 5 sinon on ne si attarde pas
					if(hautV != 5)
					{
						if(col == colV)
						{
							strat->poidsCase[v.cases[i]] = 5;
						}
						else
						{
							strat->poidsCase[v.cases[i]] = 20;
						}
					}
					break;
					case 0 : //cas d'une case voisine vide
					break;
				}
			}
			casPart = 1;
		}
	}

	//cas particulier en fonction des cases ennemis
	if((col != 0)&&(col != pp->trait))
	{
		//si il y a des ennemis autour on augmente le poids de la piece pour privilégier des coups ennemi ennemi
		if((nbEnnemi != 0))
		{
			strat->poidsCase[pos] = strat->poidsCase[pos] + 3;
		}

		//si il n'y a pas de case alliee autour la tour est considérée comme perdu
		if(nbAllie == 0)
		{
			printf("le pion isolé\n");
			strat->poidsCase[pos] = -1;
		}

		//si elle est isolé d'autres case ennemi on la récupère
		if(nbEnnemi == 0)
		{
			printf("un ennemi mais prenable\n");
			strat->poidsCase[pos] = 20;
			flag=0;
			for(i=0;i<v.nb;i++) 
			{
				//on récupère les caractéristiques de la case voisines 
				colV = pp->cols[v.cases[i]].couleur;
				hautV = pp->cols[v.cases[i]].nb;
				//on commence par faire un switch selon la couleur du voisin
				switch (colV)
				{
					case 1 : //si la case est jaune
					//on regarde déjà si elle n'est pas de taille 5 sinon on ne si attarde pas
					if(hautV != 5)
					{
						if(col != colV)
						{
							if(flag == 0)
							{
								strat->poidsCase[v.cases[i]] = 35;
								flag=1;
							}
							else
							{
								strat->poidsCase[v.cases[i]] = 8;
							}
						}
					}
					break;
					case 2 : //si la case est rouge
					//on regarde déjà si elle n'est pas de taille 5 sinon on ne si attarde pas
					if(hautV != 5)
					{
						if(col != colV)
						{
							if(flag == 0)
							{
								strat->poidsCase[v.cases[i]] = 35;
								flag=1;
							}
							else
							{
								strat->poidsCase[v.cases[i]] = 8;
							}
						}
					}
					break;
					case 0 : //cas d'une case voisine vide
					break;
				}
			}
		}
		casPart = 1;
	}

	//cas particulier tour de 5
	if((T5possible != 0))
	{
		switch (T5possible)
		{
			//un cas pour toutes les configurations de t5 possibles
			//soit allie sur ennemi
			//allie sur allie
			//ennemi sur ennemi 
			case 1:
				printf("tour de 5 cas 1\n");
				strat->poidsCase[pos] = 50;
				for(i=0;i<v.nb;i++) 
				{
					//on récupère les caractéristiques de la case voisines 
					colV = pp->cols[v.cases[i]].couleur;
					hautV = pp->cols[v.cases[i]].nb;
					//on commence par faire un switch selon la couleur du voisin
					switch (colV)
					{
						case 1 : //si la case est jaune
							if((haut + hautV == 5)&&(col != colV))
							{
								strat->poidsCase[v.cases[i]] = 45;
							}
						break;
						case 2 : //si la case est rouge
							if((haut + hautV == 5)&&(col != colV))
							{
								strat->poidsCase[v.cases[i]] = 45;
							}
						break;
						case 0 : //cas d'une case voisine vide
						break;
					}
				}
			break;
			case 2:
				if(casPart == 0)
				{
					printf("tour de 5 cas 2\n");
					strat->poidsCase[pos] = 15;
					for(i=0;i<v.nb;i++) 
					{
						//on récupère les caractéristiques de la case voisines 
						colV = pp->cols[v.cases[i]].couleur;
						hautV = pp->cols[v.cases[i]].nb;
						//on commence par faire un switch selon la couleur du voisin
						switch (colV)
						{
							case 1 : //si la case est jaune
								if((haut + hautV == 5)&&(col == colV))
								{
									strat->poidsCase[v.cases[i]] = 14;
								}
							break;
							case 2 : //si la case est rouge
								if((haut + hautV == 5)&&(col == colV))
								{
									strat->poidsCase[v.cases[i]] = 14;
								}
							break;
							case 0 : //cas d'une case voisine vide
							break;
						}
					}
				}
			break;
			case 3:
				if(casPart == 0)
				{
					printf("tour de 5 cas 3\n");
					strat->poidsCase[pos] = 7;
					for(i=0;i<v.nb;i++) 
					{
						//on récupère les caractéristiques de la case voisines 
						colV = pp->cols[v.cases[i]].couleur;
						hautV = pp->cols[v.cases[i]].nb;
						//on commence par faire un switch selon la couleur du voisin
						switch (colV)
						{
							case 1 : //si la case est jaune
								if((haut + hautV == 5)&&(col == colV))
								{
									strat->poidsCase[v.cases[i]] = 7;
								}
							break;
							case 2 : //si la case est rouge
								if((haut + hautV == 5)&&(col == colV))
								{
									strat->poidsCase[v.cases[i]] = 7;
								}
							break;
							case 0 : //cas d'une case voisine vide
							break;
						}
					}
				}
			break;
		}	
	}	
}

//fonction qui s'occupe d'attribuer une note à chaque case pour ensuite choisir la meilleur
void ponderationCase(T_Position *pp, T_strat *strat){
	int i,col,haut,allie=0,paspremier=0;
	//on commence par explorer toutes les cases du plateau
	for(i=0;i<48;i++)
		{
			//pour chaque case on récupère la couleur et la hauteur de la pile
			col = pp->cols[i].couleur;
			haut = pp->cols[i].nb;

			//on lance une fonction qui dis à chaque case si elle est alliée ou ennemie
			allie = caractereAllieEnnemi(pp,col);
			
			//on rentre ensuite dans un switch variant si la case et alliee ennemie ou vide
			switch (allie){
				case 1:	// la case est allie
					switch (haut) //on rentre alors dans un deuxième switch qui attribue une valeur à la case en fonction de son hauteur
					{
						//on donne ainsi plus de poids au petite tour sauf si elle fait 4 pour forcer une tour de 5, si elle fait deja 5 on lui attribue -2 car elle ne sert à rien
					 	//on applique aensuite un deuxième traitement pour les conditions supplémentaires
					 	case 1 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{
					 			strat->poidsCase[i] = strat->poidsCase[i] + 7;
						 		ponderationAnnexe(pp,strat,i,allie);
					 		}
					 	break;
					 	case 2 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{					 	
						 		strat->poidsCase[i] = strat->poidsCase[i] + 4;
						 		ponderationAnnexe(pp,strat,i,allie);
					 		}
					 	break;
					 	case 3 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{					 	
						 		strat->poidsCase[i] = strat->poidsCase[i] + 3;
						 		ponderationAnnexe(pp,strat,i,allie);
						 	}			 		
					 	break;
					 	case 4 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{					 	
						 		strat->poidsCase[i] = strat->poidsCase[i] + 10;
						 		ponderationAnnexe(pp,strat,i,allie);
						 	}					 		
					 	break;
					 	case 5 :
					 		strat->poidsCase[i] =  -2;
					 	break;
					 }
				break;
				case 2:	// la case est ennemi
					switch (haut) //on rentre alors dans un deuxième switch qui attribue une valeur à la case en fonction de son hauteur
					{
						//on donne ainsi plus de poids au petite tour sauf si elle fait 4 pour forcer une tour de 5, si elle fait deja 5 on lui attribue -1 car elle ne sert à rien
					 	//on applique aensuite un deuxième traitement pour les conditions supplémentaires
					 	case 1 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{					 	
						 		strat->poidsCase[i] = strat->poidsCase[i] + 8;
						 		ponderationAnnexe(pp,strat,i,allie);
						 	}			 		
					 	break;
					 	case 2 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{					 	
						 		strat->poidsCase[i] = strat->poidsCase[i] + 6;
						 		ponderationAnnexe(pp,strat,i,allie);
						 	}	
					 	break;
					 	case 3 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{
						 		strat->poidsCase[i] = strat->poidsCase[i] + 5;
						 		ponderationAnnexe(pp,strat,i,allie);
						 	}	
					 	break;
					 	case 4 :
					 		//on fait le traitement que si la case n'a pas déjà était modifié soit si elle est encore initialisé a 0
					 		if(strat->poidsCase[i] == 0)
					 		{
						 		strat->poidsCase[i] = strat->poidsCase[i] + 6;
						 		ponderationAnnexe(pp,strat,i,allie);
						 	}
					 	break;
					 	case 5 :
					 		strat->poidsCase[i] =  -2;
					 	break;
					}
				break;
				case 0:	//la case est vide
					//elle ne vaut rien donc on lui applique -2
					//si on rentre dans ce case c'est qu'au moins une case est vide donc que nous ne sommes pas au début de la partie
					strat->poidsCase[i] = -2;
					//on passe à 1 paspremier pour ne pas faire le if qui fait le coup d'origine jaune
					paspremier = 1;
				break;
			}
		}	
	//pour faire le premier coup (on est forcement jaune donc pas besoin de tester trait)
	if(paspremier == 0)
	{
		strat->poidsCase[0] = 22;
		strat->poidsCase[4] = 20;
	}
}


//fonction qui récupère le meilleur couple origine destination jouable
void recupCaseOD(T_Position *pp,T_strat *strat){
	int i,origine,poids,trouve=0,poidsO,posO,meilleurO,posD,meilleurD;
	
	do{
		//on initialise le meilleurO a -2 pour ne pas prendre les cases vides ou les tours de 5
		meilleurO = -2;
		for(i=0;i<48;i++)
		{
			poidsO = strat->poidsCase[i];
			//on prend le poids qui est juste en dessous du O actuel est qui est le meilleur du reste
			if((poidsO <= strat->caseO[0])&&(poidsO > -2)&&(i != strat->caseO[1]))
			{
				if(poidsO >= meilleurO)
				{
					meilleurO = poidsO;
					posO = i;
				}
			}
		}
		//on modifie alors dans la structure les valeurs liées à l'origine
		strat->caseO[0] = meilleurO;
		strat->caseO[1] = posO;

		//on initialise meilleurD
		meilleurD = -2;
		//on récupère la liste des voisins autour de la meilleur case d'origine
		T_Voisins v = getVoisins(strat->caseO[1]);
		//on parcourt les voisins
		for(i=0;i<v.nb;i++) 
		{
			//on récupère le poids associé au voisin i
			poids = strat->poidsCase[v.cases[i]];
			//si le poids et supérieur à celui de base ou le dernier entré on change et si la somme des hauteurs entre l'origine et le voisin i ne dépasse pas 5
			if ((poids > -2)&&(((pp->cols[strat->caseO[1]].nb)+(pp->cols[v.cases[i]].nb))<6))
			{
				if(poids >= meilleurD)
				{
					trouve = 1;
					meilleurD = poids;
					posD = v.cases[i];
				}
			}
		}
		//si on trouve une case on change les valeurs dans la partie de la structure s'occupant de la case de destination
		if(trouve == 1)
		{
			strat->caseD[0] = meilleurD;
			strat->caseD[1] = posD;
		}
		if(trouve ==0)
		{
			//pour éviter des boucles infinies si on a deux très fortes de mêmes poids ne proposant aucun coup disponible
			strat->poidsCase[posO] = strat->poidsCase[posO]+1;
		}
	//si on ne trouve pas on relance la recherche
	}while(trouve == 0);
}

//fonction pour récupérer le numéro du coup
int recupCoup(T_ListeCoups *listeCoups,T_strat *strat)
{
	int i,o,d,numCoup=0;
	for(i=0;i<listeCoups->nb; i++) //pour trouver le numéro on boucle sur tous les coups 
		{
			//à chaque coup on récupère les cases d'origine et de destination
			o = listeCoups->coups[i].origine; 
			d = listeCoups->coups[i].destination;  

			//on les compare avec les valeurs que l'on a dans notre structure 
			//l'index de la case d'origne étant ranger dans la deuxième case du tableau gérant le poid et l'index de la meilleur case d'origine
			//de même pour la destination
			if ((o == strat->caseO[1]) && (d == strat->caseD[1]))
			 {
			 	//si c'est bon on stocke l'index du coup et on quitte la boucle
				printf("Coup correspondant trouvé : %d\n", i);
				printf("o %d d %d\n",o,d);
				numCoup = i;
				break; 
			}
		}
	//on renvoie alors le numéro du coup
	return numCoup;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//zone du main

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {

	T_strat strat;
	int numCoup=0;

	//fonction qui font la stratégie
	initialisationStrat(&strat);
	ponderationCase(&currentPosition,&strat);
	recupCaseOD(&currentPosition,&strat);

	//on termine par récupéré le numéro du coup
	numCoup = recupCoup(&listeCoups,&strat);

	//on l'envoie et on termine
	ecrireIndexCoup(numCoup);
	return;
}

//Copyright Baptiste Behelle alias Iron Man 