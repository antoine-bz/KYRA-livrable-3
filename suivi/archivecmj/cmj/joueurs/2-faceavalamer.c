/********* Moteur de tournoi : joueur ***************/
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int valider(T_Position currentPosition, octet o, octet d)
{
	int k, compteur = 0, somme1 = 0;
	octet myColor = currentPosition.trait; 		// notre couleur
	T_Voisins v = getVoisins(d);				// voisins de la destination
	
	//On prend le cas si on met une pile adverse sur la destination et que cela ne forme pas un roi
	if(currentPosition.cols[o].couleur != myColor && currentPosition.cols[d].nb + currentPosition.cols[o].nb <5)	
	{	
		for(k = 0; k < v.nb; k++)
		{
			somme1 = currentPosition.cols[d].nb + currentPosition.cols[o].nb;
			
			// Pile de 5 
			if(somme1==5)
				compteur++;
		}
			
		if (!compteur)	// Nickel, on valide
			return 1;
	}	
	
	//On prend le cas si on met une pile alliée sur la destination et que cela ne forme pas un roi
	if(currentPosition.cols[o].couleur ==myColor && currentPosition.cols[d].nb + currentPosition.cols[o].nb <5)
	{	
		for(k = 0; k < v.nb; k++)
		{
			somme1=currentPosition.cols[d].nb + currentPosition.cols[o].nb;
			
			//si autour on peut faire une pile de 5
			if(somme1==5 && currentPosition.cols[v.cases[k]].couleur !=myColor)
				compteur++;
		}
		
		if(compteur==0)	// On valide
			return 1;
	}	

	// On ne valide pas le coup
	return 0;
}


// Coups d'ouverture
// Tente de joueur une ouverture pour un coup donne. Les ouvertures sont des coups precis donc on utilise directemment des indexes connus.
// Renvoie 1 si le coup a ete joue, 0 sinon.
// c: colouleur, listeCoups: liste coup, i: index dans listeCoups, currentPosition: Position courante

octet ouverture(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	
 	int o, d;
	o = listeCoups.coups[i].origine; 
	d = listeCoups.coups[i].destination; 
	
	switch (c)
	{
		case 2:	// Rouges
			if ((o==18) && (d==19) && ((currentPosition.cols[29].nb==0) && (currentPosition.cols[22].nb==2 )))
			{
				ecrireIndexCoup(i);
				return 1; // coup joue
			}

			else if ((o==29) && (d==28))
			{
				ecrireIndexCoup(i);
				return 1; // coup joue
			}

			else if ((o==18) && (d==19) && ((currentPosition.cols[29].nb==0) && (currentPosition.cols[28].nb==2 )))
			{
				ecrireIndexCoup(i);
				return 1; // coup joue
			}
			
			return 0;
			
		case 1:
		
			if ((o==29) && (d==22))
			{
				ecrireIndexCoup(i);
				return 1; // coup joue
			}

			if ((o==18) && (d==25) && (currentPosition.cols[29].nb==0))
			{ 
				ecrireIndexCoup(i);
				return 1; // coup joue
			}
			if ((o==21) && (d==20) && (currentPosition.cols[29].nb==0) && (currentPosition.cols[22].nb==2 ) && (currentPosition.cols[18].nb==0) && (currentPosition.cols[25].nb==2 ))
			{
				ecrireIndexCoup(i);
				return 1; // coup joue
			}
			
			return 0;
			
		default:
			return 0;	// ????
	}
}

// Renvoie 1 si on peut faire un roi et joue le coup., sinon 0.
octet roi(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
 	int o, d, ComPion;
	o = listeCoups.coups[i].origine; 
	d = listeCoups.coups[i].destination; 
	ComPion=(currentPosition.cols[d].nb)+(currentPosition.cols[o].nb);
	
	if ((currentPosition.cols[o].couleur == c) && (ComPion== 5)) 
	{
		// On fait un roi 
		ecrireIndexCoup(i);
		return 1; // on quitte la fonction 
	}
	
	return 0;
	
}	


// Pile adverse isolee de tout sauf de notre pion, c'est un coup gratuit et sans risque, on le prend!
octet isole(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
 	int o, d, c6, k, j;
	
	o = listeCoups.coups[i].origine; 
	d = listeCoups.coups[i].destination; 
	
	T_Voisins v = getVoisins(d);
	
	//on teste que la pile d'origine soit de notre couleur
	if(currentPosition.cols[o].couleur == c && currentPosition.cols[o].nb != 5 && currentPosition.cols[o].nb != 0)
	{
		//on teste que la pile d'arrivée soit une pile ennemie
		if(currentPosition.cols[d].couleur != c && currentPosition.cols[d].nb != 5 && currentPosition.cols[d].nb != 0)
		{
			for (k=0;k<v.nb;k++)
				//on teste si la pile de destination est la pile isolée
				if(currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[d].nb != 5 && currentPosition.cols[d].nb != 0)
					c6++;
				
			if(c6==0 && valider(currentPosition, o, d))	// Il faut etre sur que ce coup soit tout de meme jouable
			{
				for(j=0;j<listeCoups.nb;j++)
				{
					//On exécute le coup que l'on veut	
					if(o==listeCoups.coups[j].origine && d == listeCoups.coups[j].destination)
					{
						ecrireIndexCoup(j);
						return 1; //changer avec les règles
					}
				}
			}	
		}
	}
	return 0;
}

// Empecher l'adversaire de former un roi.
// Pour se faire on va empiler ses pions de la sorte a ce qu'il ne puisse pas faire une tour de 5: (Eg: tour de 3 sur 1 en etant sur qu'aucune tour de 1 soit a cote)

octet cockblock(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	
	int o, d, c8 = 0, k, r, j;
	o = listeCoups.coups[i].origine; 
	d = listeCoups.coups[i].destination; 
	
	T_Voisins v = getVoisins(d);
	
	// Destination = fantassin adverse?
	if (currentPosition.cols[d].couleur != c && currentPosition.cols[d].nb==2)
	{
		for (k=0;k<v.nb;k++)
		{
			// Prince adverse a cote?
			if (currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[v.cases[k]].nb==3)
			{
				for(r = 0; r < v.nb; r++)
				{
					//On prend une pile de 1 adverse voisine
					if (currentPosition.cols[v.cases[r]].couleur != c && currentPosition.cols[v.cases[r]].nb==1 && (valider(currentPosition,v.cases[r],d)))
					{
						for(j = 0; j < listeCoups.nb; j++)
						{
							//On exécute le coup que l'on veut
							if (v.cases[r]==listeCoups.coups[j].origine && d == listeCoups.coups[j].destination)
							{
								ecrireIndexCoup(j);
								return 1;	// On le joue
							}	
						}
					}	
				}


				for(r = 0; r < v.nb; r++)
				{
					//On prend une pile de 1 adverse voisine
					if (currentPosition.cols[v.cases[r]].couleur == c && currentPosition.cols[v.cases[r]].nb==1 && valider(currentPosition,v.cases[r],d))
					{
						for(j=0;j<listeCoups.nb;j++)
						{
							//On exécute le coup que l'on veut
							if (v.cases[r]==listeCoups.coups[j].origine && d == listeCoups.coups[j].destination)
							{
								ecrireIndexCoup(j);
								return 1;	// On le joue
							}	
						}
					}
				}
			}
		}	
	}
	return 0;
}	

/////////////////////
/*
4bis) Quand on a une pile isolée de la couleur adverse(pour notre couleur)
	-Si il y a des piles de notre couleur autour on les éloigne pour isoler totalement la pile et en faire une tour
	//il faudrait peut-être pas le faire si il y a trop de piles autour:pour l'instant on le fait que s'il y a juste une pile autour

5bis) Quand on a une pile isolée de notre couleur 
	-si plusieurs piles adverses autour on abandonne le district
	-si une seule pile adverse autour on déplace la pile adverse sur une de ses piles de préférence en suivant la règle

Actuellement je suis en train de regrouper la 4bis et la 5bis pour quand il n’y a qu’une seule pile autour de notre pile on l’éloigne de préférence en la mettant sur une pile adverse si le voisin est lui aussi isolée, si c'est une pile adverse c'est la stratégie 8 sinon on ne fait rien
*/		
//4-5bis ça isolée sur des pile isolables 
//s'occupe des piles adverses isolables alors que c'est la stratégie 6
octet distanciation_sociale(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	
	int k, j, r;
	int o = listeCoups.coups[i].origine; 
	int d = listeCoups.coups[i].destination;
	T_Voisins v = getVoisins(o);
	T_Voisins v1;
	int c4bis=0;
	

	if (currentPosition.cols[o].couleur == c && currentPosition.cols[o].nb !=5 && currentPosition.cols[o].nb !=0)//teste que la pile soit de notre couleur
	{
		for (k = 0; k < v.nb; k++)
		{
			//On regarde combien il y a de voisins quelle que soit leur couleur (est de notre couleur ou est une pile de 0 ou un roi)
			if(currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				c4bis++;
		}
		
		
		if (c4bis==1)//on vérifie qu'il n'y a qu'un seul voisin
		{
			//si il y a une pile non isolée à côté on l'éloigne
			for (k=0;k<v.nb;k++)
			{
				if (currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				{//on prend la pile voisine de notre pile isolée
					
					v1=getVoisins(v.cases[k]);
					for(r = 0; r < v1.nb; r++)
					{
						//On éloigne la pile voisine sur une pile adverse (attention de bien vérifier que cela ne fait pas une pile de 5 adverse)
						if (currentPosition.cols[v1.cases[r]].couleur != c && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5 && currentPosition.cols[v.cases[k]].nb + currentPosition.cols[v1.cases[r]].nb <=5)
						{
							if(valider(currentPosition,v.cases[k],v1.cases[r]))
							{
								for(j=0;j<listeCoups.nb;j++)
								{
									//On exécute le coup que l'on veut
									if(v.cases[k]==listeCoups.coups[j].origine && v1.cases[r] == listeCoups.coups[j].destination)
									{
										ecrireIndexCoup(j);
										return 1; //changer avec les règles
									}	
								}
							}	
						}		
					}
					
					for(r = 0; r < v1.nb; r++)
					{
						//On éloigne la pile voisine sur une pile alliée car on ne peut pas l'éloigner sur une pile  adverse
						if (currentPosition.cols[v1.cases[r]].couleur == c && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5 && currentPosition.cols[v.cases[k]].nb + currentPosition.cols[v1.cases[r]].nb <=5 && o != v1.cases[r])
						{
							if (valider(currentPosition,v.cases[k],v1.cases[r]))
							{
								for (j = 0; j < listeCoups.nb; j++)
								{
									//On exécute le coup que l'on veut
									if (v.cases[k]==listeCoups.coups[j].origine && v1.cases[r] == listeCoups.coups[j].destination)
									{
										ecrireIndexCoup(j);
										return 1; //changer avec les règles
									}		
								}	
							}
						}		
					}
				}
			}
		}
	}	
	
	
	return 0;
}	

////////////// Stratégie 3.1

// Quand un fantassin adverse n’est entouré pour les pions adverses que de fantassins adverses
//1- Si on a un fantassin de notre couleur autour on le met dessus
//2- Si on peut faire un fantassin de notre couleur entouré d’au moins une pile de 1 de notre couleur on le fait pour après placer notre fantassin sur un des fantassins adverses au prochain tour
//3- Sinon on abandonne le district

octet pile_fantassin(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	
	int k, j, r;
	int o = listeCoups.coups[i].origine; 
	int d = listeCoups.coups[i].destination;
	T_Voisins v = getVoisins(o);
	T_Voisins v1;
	int c2 = 0, c3 = 0;
	

	if (currentPosition.cols[d].couleur !=c && currentPosition.cols[d].nb==2)//On teste pour rentrer dans le cas de la stratégie
	{
		for (k=0;k<v.nb;k++) 
		{
			if (currentPosition.cols[v.cases[k]].nb==2 || currentPosition.cols[v.cases[k]].nb==0 || 	currentPosition.cols[v.cases[k]].nb==5 || currentPosition.cols[v.cases[k]].couleur == c)//teste si les voisins sont tous une pile de deux ou une pile vide ou une pile de 5
				c2++;	//On incrémente un compteur
			
			if(c2==v.nb)
			{

				for(k=0;k<v.nb;k++) 
				{
					//test avec toutes les origines et destinations
					if(currentPosition.cols[v.cases[k]].couleur == c && currentPosition.cols[v.cases[k]].nb==2 && valider(currentPosition,v.cases[k],d))//Cas 1
					{

						for(j=0;j<listeCoups.nb;j++)
						{
							//On exécute le coup que l'on veut
							if(d==listeCoups.coups[j].destination && v.cases[k] == listeCoups.coups[j].origine)
							{
								ecrireIndexCoup(j);
								return 1; //changer avec les règles
							}
						}
					}
				}
			}
			
			for(k=0;k<v.nb;k++)//Cas 3.2
			{
				if (currentPosition.cols[v.cases[k]].couleur == c && currentPosition.cols[v.cases[k]].nb==1)//Si il y a une pile de 1 alliée autour de la pile adverse
				{
					v1=getVoisins(v.cases[k]);
					c3 = 0;
					for(r=0;r<v1.nb;r++)
					{
						if (currentPosition.cols[v1.cases[r]].nb==1 && currentPosition.cols[v1.cases[r]].couleur==c)
							c3++;

					}
					
					if (c3>=2)//si il y a deux cases autour étant une pile de 1 alliée
					{
						for(r=0;r<v1.nb;r++)
						{
							if (currentPosition.cols[v1.cases[r]].nb==1 && currentPosition.cols[v1.cases[r]].couleur==c && valider(currentPosition,v1.cases[r],v.cases[k]))
							{

								for(j=0;j<listeCoups.nb;j++)
								{
									//On exécute le coup que l'on veut
									if (v.cases[k]==listeCoups.coups[j].destination && v1.cases[r] == listeCoups.coups[j].origine)
									{
										ecrireIndexCoup(j);
										return 1; //changer avec les règles
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

octet strat4(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{

	int k, j, r;
	int o = listeCoups.coups[i].origine; 
	int d = listeCoups.coups[i].destination;
	T_Voisins v = getVoisins(o);
	T_Voisins v1;
	int c411 = 0, c412 = 0, c413 = 0, c414 = 0, c4 = 0;
	int somme = 0;
	
	if (currentPosition.cols[d].couleur != c && currentPosition.cols[d].nb !=5 && currentPosition.cols[d].nb !=0)//teste que la pile ne soit pas de notre couleur 
	{
		for (k=0;k<v.nb;k++)
		{
			//on teste si la pile est bien isolée de notre couleur(est de la couleur adverse ou est une pile de 0 ou un roi
			if (currentPosition.cols[v.cases[k]].couleur == c && currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				c4++;
		}
		
		//on rentre dans le cas 4.1
		if (c4==0)
		{
			for (k=0;k<v.nb;k++)
			{
				//teste le nombre de piles adverses autour
				if(currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
					c411++;			
			}
			
			//doit tester que les voisins n'ont qu'un seul voisin ou deux (bloc de 3) (pour l'instant juste 1)
			if (c411==2)//on rentre si la pile n'a que deux voisins
			{
				for (k=0;k<v.nb;k++)
				{
					c414=0;
					
					//Nous permet de choisir le voisin qui est une pile adverse non vide et non roi
					if (currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
					{
						v1=getVoisins(v.cases[k]);
						for(r=0;r<v1.nb;r++)
						{
							//on teste que le voisin de la case de destination est isolée de notre couleur
							if (currentPosition.cols[v1.cases[r]].couleur == c && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5)
								c412++;	
								
						}
							
						if (c412==0)
						{//on sait que le voisin est isolé et on veut tester si il n'a qu'un voisin, la pile de destination adverse
							for(r=0;r<v1.nb;r++)
							{
								//teste le nombre de piles adverses autour
								if (currentPosition.cols[v1.cases[r]].couleur != c && currentPosition.cols[v1.cases[r]].nb !=0 && currentPosition.cols[v1.cases[r]].nb !=5)
										c413++;

							}
						}
					}
					
					//On peut maintenant appliquer la régle Attention il faut voir si cela teste pour les deux
					if(c413==1)
						c414++;		
				}
				
				//si les deux voisins ont chacun un seul voisin la case de destination on teste si la somme des 3 est inférieure à 5
				if (c414==2)
				{
					somme=currentPosition.cols[d].nb;
					for(r=0;r<v1.nb;r++)
					{
						if (currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
							somme += currentPosition.cols[v.cases[k]].nb;
					}
						
					if (somme<5)//Si la somme des 3 piles est inférieure à 5
					{
						for (k=0;k<v.nb;k++)
						{
							if (currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
							{//on choisit une des deux cases

								if(valider(currentPosition,v.cases[k],d))
								{
									for(j=0;j<listeCoups.nb;j++)
									{
										//On exécute le coup que l'on veut
										if (d==listeCoups.coups[j].destination && v.cases[k] == listeCoups.coups[j].origine)
										{
											ecrireIndexCoup(j);
											return 1; //changer avec les règles		
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return 0;
}	


//Cas 2 -si il y a une ou des pile adverse autour non isolées, on empile la pile isolée sur une des piles autour(voir règles)
octet strat4_2(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	int k, j, r;
	int o = listeCoups.coups[i].origine; 
	int d = listeCoups.coups[i].destination;
	T_Voisins v = getVoisins(o);
	T_Voisins v1;
	int c411 = 0, c412 = 0, c413 = 0, c414 = 0, c4 = 0;
	int somme = 0;
		
		
	if (currentPosition.cols[d].couleur != c && currentPosition.cols[d].nb !=5 && currentPosition.cols[d].nb !=0)//teste que la pile ne soit pas de notre couleur 
	{
		for (k=0;k<v.nb;k++)
		{
			if(currentPosition.cols[v.cases[k]].couleur == c && currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				c4++;
		}
		
		//on rentre dans le cas 4.2
		if (c4==0)
		{
			for (k=0;k<v.nb;k++)
			{
				//teste le nombre de piles adverses autour
				if(currentPosition.cols[v.cases[k]].couleur != c && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
					c411++;
			}
					
			if (c411>0)//on teste que la destination ait au moins un voisin de couleur adverse pour qu'elle ne soit pas totalement isolée
			{
				for (k=0;k<v.nb;k++)
				{	
					//Nous permet de choisir le voisin qui est une pile adverse non vide et non roi
					if(currentPosition.cols[v.cases[k]].couleur !=c && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
					{
						v1=getVoisins(v.cases[k]);
						for(r=0;r<v1.nb;r++)
						{
							if(currentPosition.cols[v1.cases[r]].couleur == c && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5)
							{//on teste que le voisin de la case de destination ne soit pas isolée de notre couleur

								if(valider(currentPosition,d,v.cases[k]))
								{
									for(j=0;j<listeCoups.nb;j++)
									{
										//On exécute le coup que l'on veut
										if(d==listeCoups.coups[j].origine && v.cases[k] == listeCoups.coups[j].destination)
										{
											ecrireIndexCoup(j); 
											return 1;
										}		
									}
								}	
							}	
						}
					}					
				}
			}
		}	
	}	
	return 0;
}

//-quand une pile de 2 adverse si pas d’autre pile de 2 adverse autour mais qu’il y a une pile de 1 adverse autour 
//-on rassemble les deux piles adverses en une seule pile si règles respectées (Dans la variante actuelle on met la pile de 1 sur la pile de deux)

octet anti_fantassin(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	int o = listeCoups.coups[i].origine; 
	int d = listeCoups.coups[i].destination;
	int c71=0;
	int c72=0;
	int k, j;
	T_Voisins v = getVoisins(d);
	
	if (currentPosition.cols[d].couleur != c &&  currentPosition.cols[d].nb==2)
	{
		for(k=0;k<v.nb;k++) 
		{
			if(currentPosition.cols[v.cases[k]].nb==2 && currentPosition.cols[v.cases[k]].couleur !=c )//teste si les voisins de la couleur adverse sont de taille 2
				c71++;	//On incrémente un compteur

			if(currentPosition.cols[v.cases[k]].nb==1 &&  currentPosition.cols[v.cases[k]].couleur !=c)//teste si les voisins de la couleur adverse sont de taille 1
					c72++;
		}

		if(c71==0 && c72==1)//on teste qu'autour de la pile il n'y ait aucune pile adverse et une seule de pile 1
		{
			for(k=0;k<v.nb;k++) 
			{
				if(currentPosition.cols[v.cases[k]].nb==1 &&  currentPosition.cols[v.cases[k]].couleur !=c && valider(currentPosition,v.cases[k],d))//on sélectionne la pile de 1 pour la mettre en origine
				{	
					for(j=0;j<listeCoups.nb;j++)
					{
						//On exécute le coup que l'on veut
						if(d==listeCoups.coups[j].destination && v.cases[k] == listeCoups.coups[j].origine)
						{
							ecrireIndexCoup(j);
							return 1; //changer avec les règles
						}	
					}
				}	
			}
		}
	}
	return 0;	
}	

octet empiler2(octet c, T_ListeCoups listeCoups, int i, T_Position currentPosition)
{
	int o = listeCoups.coups[i].origine; 
	int d = listeCoups.coups[i].destination;
	T_Voisins v = getVoisins(d);
		
	if ((currentPosition.cols[o].couleur !=c && currentPosition.cols[d].couleur!=c) && (currentPosition.cols[o].nb==1 && currentPosition.cols[d].nb==1) && valider(currentPosition,o,d))
	{	
		ecrireIndexCoup(i);
		return 1;
	}
	return 0;
}	

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	//Ce bot fait des piles de deux avec les pions adverses

	int i,j,k,r,c,c3,c4,c411,c412,c413,c414,c4bis,somme,c6,c71,c72,c8,sortie,compteur,numcoup=0,ComPion;
	octet o,d;
	octet myColor = currentPosition.trait; 

	
	for(i=0;i<listeCoups.nb; i++) 
	{
		
		// Ici on a des strategies de "points gratuits"
		// En theorie, c'est meme plus important qu'une ouverture bien qu'on ne pourra appliquer aucune des strategies presentes ici en debut de partie.
		
		// Faire un roi des que possible
		if (roi(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue

		
		// pion adverse isole
		// Pile d'origine de notre couleur et une pile de destination enemie isolee de tout sauf de notre origine, un coup tres safe et gratuit s'il se presente!		
		if (isole(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue
			
	}	

	for(i=0;i<listeCoups.nb; i++) 
	{	
		// Ouverture: Tres importante car elle donne un avantage tactique non negligeable, donc on va regarder *tout* nos coups seulement pour savoir si on peut la faire.
		if (ouverture(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue
				
	}
	
	for(i=0;i<listeCoups.nb; i++) 
	{
		
		// Ici, on a le reste de nos strategies placees par importance.
		
		// cockblock: Empecher l'adversaire de faire un roi en empilant ses pions de sorte a ce qu'il perde des points et se trouve incappable de faire un roi.
		if (cockblock(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue
		
		// distanciation sociale: Eloigner nos piles de l'adversaire pour faire une tour
		if (distanciation_sociale(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue
		
		// Placer des fantassins allies sur des fantassins adverses ou tenter en second lieux de se positionner de sorte a pouvoir le faire au prochain tour.
		if (pile_fantassin(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue	
		
		// Pile adverse isolee de nous:
		// - Si d'autres piles sont autours, on va empiler dessus de sorte a ce que l'adversaire perde des tours.
		// - Sinon, on empile la pile isolee sur une autre pile autour
		if (strat4(myColor, listeCoups, i, currentPosition) || strat4_2(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue
		
		//-quand une pile de 2 adverse si pas d’autre pile de 2 adverse autour mais qu’il y a une pile de 1 adverse autour 
		//-on rassemble les deux piles adverses en une seule pile si règles respectées (Dans la variante actuelle on met la pile de 1 sur la pile de deux)
		if (anti_fantassin(myColor, listeCoups, i, currentPosition))
			return;	// Coup joue			
	}	
}
