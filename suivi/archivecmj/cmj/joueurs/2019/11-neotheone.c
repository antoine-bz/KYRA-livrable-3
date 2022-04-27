/********* Moteur de tournoi : joueur ***************/
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int valider(T_Position currentPosition,octet o, octet d)
{
	int i,j,k,c=0,compteur=0,somme1=0;
	octet myColor = currentPosition.trait; 
	T_Voisins v = getVoisins(d);

	if(currentPosition.cols[o].couleur !=myColor && currentPosition.cols[d].nb + currentPosition.cols[o].nb <5)
		{//On prend le cas si on met une pile adverse sur la destination et que cela ne forme pas un roi
			
			for(k=0;k<v.nb;k++)
			{
				somme1=currentPosition.cols[d].nb + currentPosition.cols[o].nb;
				somme1 = somme1 + currentPosition.cols[v.cases[k]].nb;
				if(somme1==5)
				{//si autour on peut faire une pile de 5
					compteur++;
				}
			}
			
			if(compteur==0)
			{
				//printf("\nON VALIDE\n");
				return 1;
			}
		}
		
		if(currentPosition.cols[o].couleur ==myColor && currentPosition.cols[d].nb + currentPosition.cols[o].nb <5)
		{//On prend le cas si on met une pile alliée sur la destination et que cela ne forme pas un roi
			for(k=0;k<v.nb;k++)
			{
				somme1=currentPosition.cols[d].nb + currentPosition.cols[o].nb;
				somme1 = somme1 + currentPosition.cols[v.cases[k]].nb;
				if(somme1==5 && currentPosition.cols[v.cases[k]].couleur !=myColor)
				{//si autour on peut faire une pile de 5
					compteur++;
				}
			}
		
			if(compteur==0)
			{
				//printf("\nON VALIDE\n");
				return 1;
			
			}
	
		}
	//printf("\nON NE VALIDE PAS\n");
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
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		//printf("Coup %d : ", i); 
		//printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb,/*combien il y a de pions et quelle couleur*/ COLNAME(currentPosition.cols[o].couleur));
		//printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 


		//ouverture(currentPosition,listeCoups,o,d,myColor,i,&numcoup); 
		if (myColor==2)	//ROUGE
		{
			if ((o==18) && (d==19) && ((currentPosition.cols[29].nb==0) && (currentPosition.cols[22].nb==2 )))
			{
				//printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				numcoup+=1;
				return; // on quitte la fonction 
			}

			if ((o==29) && (d==28))
			{
				//printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				numcoup+=1;	
				return; // on quitte la fonction 
			}

			if ((o==18) && (d==19) && ((currentPosition.cols[29].nb==0) && (currentPosition.cols[28].nb==2 )))
			{
				//printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				numcoup+=1;	
				return; // on quitte la fonction 
			}
			

		}

		if (myColor==1)	//JAUNE
		{
			if ((o==29) && (d==22))
			{
				//printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				numcoup+=1;
				return; // on quitte la fonction 
			}

			if ((o==18) && (d==25) && (currentPosition.cols[29].nb==0))
			{ 
				//printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				numcoup+=1;
				return; // on quitte la fonction 
			}
			if ((o==21) && (d==20) && (currentPosition.cols[29].nb==0) && (currentPosition.cols[22].nb==2 ) && (currentPosition.cols[18].nb==0) && (currentPosition.cols[25].nb==2 ))
			{
				//printf("On choisit ce coup ! \n"); 
				ecrireIndexCoup(i);
				numcoup+=1;
				return; // on quitte la fonction 
			}

		}
	}
	
	



///// Stratégie roi
	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		ComPion=(currentPosition.cols[d].nb)+(currentPosition.cols[o].nb);
		if ( (currentPosition.cols[o].couleur == myColor) && (ComPion== 5) ) 
		{
			//printf("On fait un roi ! \n");    
			ecrireIndexCoup(i);
			numcoup+=1;
			return; // on quitte la fonction 
		}
	}


	///Stratégie 6
//on teste que la pile d'origine soit de notre couleurr et que la pile d'arrivée soit isolée de tout sauf de l'origine et qu'elle soit de la couleur adverse
//si il y a une pile adverse seulement isolée de sa couleur on la recouvre	(test?)
	for(i=0;i<listeCoups.nb;i++)
	{	

		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(d);
		c6=0;

		if(currentPosition.cols[o].couleur == myColor && currentPosition.cols[o].nb != 5 && currentPosition.cols[o].nb != 0)
		{//on teste que la pile d'origine soit de notre couleur
			if(currentPosition.cols[d].couleur != myColor && currentPosition.cols[d].nb != 5 && currentPosition.cols[d].nb != 0)
			{//on teste que la pile d'arrivée soit une pile ennemie
				for (k=0;k<v.nb;k++)
				{
					if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[d].nb != 5 && currentPosition.cols[d].nb != 0)
					{//on teste si la pile de destination est la pile isolée
						c6++;
					}
				}
				
				if(c6==0)
				{
					if(valider(currentPosition,o,d))
					{
						for(j=0;j<listeCoups.nb;j++)
						{
							
							if(o==listeCoups.coups[j].origine && d == listeCoups.coups[j].destination)
							{//On exécute le coup que l'on veut
								ecrireIndexCoup(j);
								//printf("\n\nstratégie 6\n\n");
								return; //changer avec les règles
							}	
						}
					}
				}
				
			}

		}
	}

////////
/*
8)Quand pile de 3 adverse à côté d’une pile de 2 adverse(pareil avec deux piles de 3 à côté de la pile de deux (actuellement on met la pile de un sur la pile de deux) et si il
	-1)si il y a un pion adverse à côté de la pile de 2 qui n’est lui-même pas à côté d’une pile de deux mettre: mettre la pile de 2 sur la pile de 1 ou la pile de 1 sur la pile de 2  selon les règles si il n'y a pas de pile de 1 adverse il prend une pile de 1 de notre couleur
-2)  si règles respectées mettre la pile de 3 sur une pile de 1// je ne l'ai pas faîte car je la trouve débile car au prochain tour il fait une pil de 5 de sa couleur avec une de nos pile de 5
-3) abandonner le district
*/

//Il faut bien faire vérifier que l'ennemi ne puisse pas faire une tour de 5 grâce à notre coup

	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(d);
		c8=0;



		if (currentPosition.cols[d].couleur != myColor && currentPosition.cols[d].nb==2)
		{//On teste que la destination est un fantassin adverse
			for (k=0;k<v.nb;k++)
			{
				if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb==3)
				{//On teste qu'il y ait un prince adverse à côté
					for(r=0;r<v.nb;r++)
					{
						if (currentPosition.cols[v.cases[r]].couleur != myColor && currentPosition.cols[v.cases[r]].nb==1)
						{//On prend une pile de 1 adverse voisine
							if(valider(currentPosition,v.cases[r],d))
							{
		
								for(j=0;j<listeCoups.nb;j++)
								{
							
									if(v.cases[r]==listeCoups.coups[j].origine && d == listeCoups.coups[j].destination)
									{//On exécute le coup que l'on veut
										ecrireIndexCoup(j);
										//printf("\n\nstratégie 8----1\n\n");
										return; //changer avec les règles
									}	
								}
							}	
						}
					}

					for(r=0;r<v.nb;r++)
					{
						if (currentPosition.cols[v.cases[r]].couleur == myColor && currentPosition.cols[v.cases[r]].nb==1)
						{//On prend une pile de 1 adverse voisine
							if(valider(currentPosition,v.cases[r],d))
							{
								for(j=0;j<listeCoups.nb;j++)
								{
							
									if(v.cases[r]==listeCoups.coups[j].origine && d == listeCoups.coups[j].destination)
									{//On exécute le coup que l'on veut
										ecrireIndexCoup(j);
										//printf("\n\nstratégie 8----2\n\n");
										return; //changer avec les règles
									}	
								}
							}	
						}
					}

				}
	
			}

		}

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

	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(o);
		T_Voisins v1;
		c4bis=0;
			
		if(currentPosition.cols[o].couleur == myColor && currentPosition.cols[o].nb !=5 && currentPosition.cols[o].nb !=0)//teste que la pile soit de notre couleur
		{
			for (k=0;k<v.nb;k++)
			{
				if(currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				{//On regarde combien il y a de voisins quelle que soit leur couleur (est de notre couleur ou est une pile de 0 ou un roi)
					c4bis++;
				}
			}
		
			if(c4bis==1)//on vérifie qu'il n'y a qu'un seul voisin
			{//si il y a une pile non isolée à côté on l'éloigne
				for (k=0;k<v.nb;k++)
				{
					if(currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
					{//on prend la pile voisine de notre pile isolée
					
						v1=getVoisins(v.cases[k]);
						for(r=0;r<v1.nb;r++)
						{
							 if(currentPosition.cols[v1.cases[r]].couleur != myColor && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5 && currentPosition.cols[v.cases[k]].nb + currentPosition.cols[v1.cases[r]].nb <=5)
							{//On éloigne la pile voisine sur une pile adverse (attention de bien vérifier que cela ne fait pas une pile de 5 adverse)
								if(valider(currentPosition,v.cases[k],v1.cases[r]))
								{
									for(j=0;j<listeCoups.nb;j++)
									{
							
										if(v.cases[k]==listeCoups.coups[j].origine && v1.cases[r] == listeCoups.coups[j].destination)
										{//On exécute le coup que l'on veut
											ecrireIndexCoup(j);
											printf("\n\n\t %d\n\n",o);
											//printf("\n\nstratégie 4-5bis ennemie\n\n");
											return; //changer avec les règles
										}	
									}
								}	
							}
								
						}
						for(r=0;r<v1.nb;r++)
						{
							 if(currentPosition.cols[v1.cases[r]].couleur == myColor && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5 && currentPosition.cols[v.cases[k]].nb + currentPosition.cols[v1.cases[r]].nb <=5 && o != v1.cases[r])
							{//On éloigne la pile voisine sur une pile alliée car on ne peut pas l'éloigner sur une pile  adverse
								if(valider(currentPosition,v.cases[k],v1.cases[r]))
								{
									for(j=0;j<listeCoups.nb;j++)
									{
							
										if(v.cases[k]==listeCoups.coups[j].origine && v1.cases[r] == listeCoups.coups[j].destination)
										{//On exécute le coup que l'on veut
											ecrireIndexCoup(j);
											printf("\n\n\t %d\n\n",o);
											//printf("\n\nstratégie 4-5bis alliée\n\n");
											return; //changer avec les règles
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

////////////// Stratégie 3.1

		//  • Quand un fantassin adverse n’est entouré pour les pions adverses que de fantassins adverses
//1- Si on a un fantassin de notre couleur autour on le met dessus
//2- Si on peut faire un fantassin de notre couleur entouré d’au moins une pile de 1 de notre couleur on le fait pour après placer notre fantassin sur un des fantassins adverses au prochain tour
//3- Sinon on abandonne le district
		
	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(o);
		T_Voisins v1;
		
		if(currentPosition.cols[d].couleur !=myColor &&  currentPosition.cols[d].nb==2)//On teste pour rentrer dans le cas de la stratégie
		{
			for(k=0;k<v.nb;k++) 
			{
				if(currentPosition.cols[v.cases[k]].nb==2 || currentPosition.cols[v.cases[k]].nb==0 || 	currentPosition.cols[v.cases[k]].nb==5 || currentPosition.cols[v.cases[k]].couleur == myColor)//teste si les voisins sont tous une pile de deux ou une pile vide ou une pile de 5
				{
					c++;	//On incrémente un compteur
				}	
			}
			printf("\n\n%d\n\n",c);
			if(c==v.nb)
			{
				//printf("\n\n arrivé !!!!\n\n");
				for(k=0;k<v.nb;k++) 
				{
					if(currentPosition.cols[v.cases[k]].couleur == myColor && currentPosition.cols[v.cases[k]].nb==2)//Cas 1
					{//test avec toutes les origines et destinations
						//printf("Arrivé 2ème");
						if(valider(currentPosition,v.cases[k],d))
						{
							for(j=0;j<listeCoups.nb;j++)
							{
							
								if(d==listeCoups.coups[j].destination && v.cases[k] == listeCoups.coups[j].origine)
								{//On exécute le coup que l'on veut
									ecrireIndexCoup(j);
									//printf("\n\nstratégie 3.1\n\n");
									return; //changer avec les règles
								}
							}
						}
					}
				
					


				}
				for(k=0;k<v.nb;k++)//Cas 3.2
				{
					if(currentPosition.cols[v.cases[k]].couleur == myColor && currentPosition.cols[v.cases[k]].nb==1)//Si il y a une pile de 1 alliée autour de la pile adverse
					{
						v1=getVoisins(v.cases[k]);
						c3=0;
						for(r=0;r<v1.nb;r++)
						{
							if(currentPosition.cols[v1.cases[r]].nb==1 && currentPosition.cols[v1.cases[r]].couleur==myColor)
							{
								c3++;
							}
						}
						if(c3>=2)//si il y a deux cases autour étant une pile de 1 alliée
						{
							for(r=0;r<v1.nb;r++)
							{
								if(currentPosition.cols[v1.cases[r]].nb==1 && currentPosition.cols[v1.cases[r]].couleur==myColor)
								{
									if(valider(currentPosition,v1.cases[r],v.cases[k]))
									{
										for(j=0;j<listeCoups.nb;j++)
										{
										
											if(v.cases[k]==listeCoups.coups[j].destination && 												v1.cases[r] == listeCoups.coups[j].origine)
											{//On exécute le coup que l'on veut
												ecrireIndexCoup(j);
												//printf("\n\nstratégie 3.2\n\n");
												return; //changer avec les règles
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
	}


////////////////////
/*
4)quand il y a une pile adverse qui est isolée de notre couleur 
	-si elle forme avec d’autres piles adverses un bloc de 3 on pose une des piles extérieures sur la pile en contact avec les deux autres piles sauf si la somme des jetons des piles est supérieure à 5 ( ne le fait pas quand les 3 piles du block sont toutes voisines entre elles)
-si il y a une ou des pile adverse autour non isolées, on empile la pile isolée sur une des piles autour(voir règles)
*/
//Cas 1

	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(d);
		T_Voisins v1;
		c4=0;
		c411=0;
		c412=0;
		c413=0;
		c414=0;
		somme=0;
		
		if (currentPosition.cols[d].couleur != myColor && currentPosition.cols[d].nb !=5 && currentPosition.cols[d].nb !=0)//teste que la pile ne soit pas de notre couleur 
		{
			for (k=0;k<v.nb;k++)
			{
				if(currentPosition.cols[v.cases[k]].couleur == myColor && currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				{//on teste si la pile est bien isolée de notre couleur(est de la couleur adverse ou est une pile de 0 ou un roi
					c4++;
				}
			}
			
			if (c4==0)
			{//on rentre dans le cas 4.1
				for (k=0;k<v.nb;k++)
				{
					if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
					{//teste le nombre de piles adverses autour
						c411++;
							
					}
				
				}
				
				if (c411==2)//on rentre si la pile n'a que deux voisins
				{//doit tester que les voisins n'ont qu'un seul voisin ou deux (bloc de 3) (pour l'instant juste 1)
					for (k=0;k<v.nb;k++)
					{
						c414=0;
						
						if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
						{//Nous permet de choisir le voisin qui est une pile adverse non vide et non roi
							v1=getVoisins(v.cases[k]);
							for(r=0;r<v1.nb;r++)
							{
							 	if(currentPosition.cols[v1.cases[r]].couleur == myColor && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5)
								{//on teste que le voisin de la case de destination est isolée de notre couleur
									c412++;	
								}
								
							}
							
							if(c412==0)
							{//on sait que le voisin est isolé et on veut tester si il n'a qu'un voisin, la pile de destination adverse
								for(r=0;r<v1.nb;r++)
								{
									if(currentPosition.cols[v1.cases[r]].couleur != myColor && currentPosition.cols[v1.cases[r]].nb !=0 && currentPosition.cols[v1.cases[r]].nb !=5)
									{//teste le nombre de piles adverses autour
										c413++;
							
									}

								}
							}
						}
				
						if(c413==1)
						{//On peut maintenant appliquer la régle Attention il faut voir si cela teste pour les deux

							c414++;
						}
							
							
						
					}
					
					if(c414==2)
					{//si les deux voisins ont chacun un seul voisin la case de destination on teste si la somme des 3 est inférieure à 5
						somme=currentPosition.cols[d].nb;
						for(r=0;r<v1.nb;r++)
						{
							if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
							{
								somme =somme + currentPosition.cols[v.cases[k]].nb;
							}

						}
						
						if (somme<5)//Si la somme des 3 piles est inférieure à 5
						{
							for (k=0;k<v.nb;k++)
							{
								if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
								{//on choisit une des deux cases

									if(valider(currentPosition,v.cases[k],d))
									{
										for(j=0;j<listeCoups.nb;j++)
										{
							
											if(d==listeCoups.coups[j].destination && v.cases[k] == listeCoups.coups[j].origine)
											{//On exécute le coup que l'on veut
												ecrireIndexCoup(j);
												//printf("\n\nstratégie 4.1\n\n");
												return; //changer avec les règles
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

	}

//Cas 2 -si il y a une ou des pile adverse autour non isolées, on empile la pile isolée sur une des piles autour(voir règles)


	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(d);
		T_Voisins v1;
		c4=0;
		c411=0;
		c412=0;
		
		
		if (currentPosition.cols[d].couleur != myColor && currentPosition.cols[d].nb !=5 && currentPosition.cols[d].nb !=0)//teste que la pile ne soit pas de notre couleur 
		{
			for (k=0;k<v.nb;k++)
			{
				if(currentPosition.cols[v.cases[k]].couleur == myColor && currentPosition.cols[v.cases[k]].nb!=0 && currentPosition.cols[v.cases[k]].nb!=5)
				{//on teste si la pile est isolée de notre couleur(est de la couleur adverse ou est une pile de 0 ou un roi)
					c4++;
				}
			}
		
			if (c4==0)
			{//on rentre dans le cas 4.2
				for (k=0;k<v.nb;k++)
				{
					if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
					{//teste le nombre de piles adverses autour
						c411++;
							
					}
				}
					
				if(c411>0)//on teste que la destination ait au moins un voisin de couleur adverse pour qu'elle ne soit pas totalement isolée
				{
					for (k=0;k<v.nb;k++)
					{	
					
						if(currentPosition.cols[v.cases[k]].couleur != myColor && currentPosition.cols[v.cases[k]].nb !=0 && currentPosition.cols[v.cases[k]].nb !=5)
						{//Nous permet de choisir le voisin qui est une pile adverse non vide et non roi
							v1=getVoisins(v.cases[k]);
							for(r=0;r<v1.nb;r++)
							{
							 	if(currentPosition.cols[v1.cases[r]].couleur == myColor && currentPosition.cols[v1.cases[r]].nb!=0 && currentPosition.cols[v1.cases[r]].nb!=5)
								{//on teste que le voisin de la case de destination ne soit pas isolée de notre couleur

									if(valider(currentPosition,d,v.cases[k]))
									{
										for(j=0;j<listeCoups.nb;j++)
										{
							
											if(d==listeCoups.coups[j].origine && v.cases[k] == listeCoups.coups[j].destination)
											{//On exécute le coup que l'on veut
												ecrireIndexCoup(j);
												//printf("\n\nstratégie 4.2\n\n");
												return; //changer avec les règles
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
	}
	

		//7)-quand une pile de 2 adverse si pas d’autre pile de 2 adverse autour mais qu’il y a une pile de 1 adverse autour 
//-on rassemble les deux piles adverses en une seule pile si règles respectées (Dans la variante actuelle on met la pile de 1 sur la pile de deux)
//-sinon on abandonne le district
	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		c71=0;
		c72=0;
		T_Voisins v = getVoisins(d);
		if(currentPosition.cols[d].couleur !=myColor &&  currentPosition.cols[d].nb==2)
		{
			for(k=0;k<v.nb;k++) 
			{
				if(currentPosition.cols[v.cases[k]].nb==2 && currentPosition.cols[v.cases[k]].couleur !=myColor )//teste si les voisins de la couleur adverse sont de taille 2
				{
					c71++;	//On incrémente un compteur
				}
				if(currentPosition.cols[v.cases[k]].nb==1 &&  currentPosition.cols[v.cases[k]].couleur !=myColor)//teste si les voisins de la couleur adverse sont de taille 1
				{
					c72++;
				}
			}

			if(c71==0 && c72==1)//on teste qu'autour de la pile il n'y ait aucune pile adverse et une seule de pile 1
			{
				for(k=0;k<v.nb;k++) 
				{
					if(currentPosition.cols[v.cases[k]].nb==1 &&  currentPosition.cols[v.cases[k]].couleur !=myColor)//on sélectionne la pile de 1 pour la mettre en origine
					{	
						if(valider(currentPosition,v.cases[k],d))
						{
							for(j=0;j<listeCoups.nb;j++)
							{
							
								if(d==listeCoups.coups[j].destination && v.cases[k] == listeCoups.coups[j].origine)
								{//On exécute le coup que l'on veut
									ecrireIndexCoup(j);
									//printf("\n\nstratégie 7\n\n");
									return; //changer avec les règles
								}	
							}
						}	
					}
				}
			}
			


		}
	}



/////Pile de deux adverses 
	
		//On fait des piles de deux avec les piles adverses Validé
	for(i=0;i<listeCoups.nb;i++)
	{	
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		T_Voisins v = getVoisins(d);
		
		if ((currentPosition.cols[o].couleur !=myColor && currentPosition.cols[d].couleur!=myColor) && (currentPosition.cols[o].nb==1 && currentPosition.cols[d].nb==1))
		{	
			if(valider(currentPosition,o,d))
			{
				//printf("On fait une pile de deux \n");
				ecrireIndexCoup(i);
				return; // on quitte la fonction
			}


		}
	}
	
}

