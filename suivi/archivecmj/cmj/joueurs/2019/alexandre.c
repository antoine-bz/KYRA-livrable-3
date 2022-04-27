/********* Moteur de tournoi : Alexandre ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

//PROTOTYPES------------------------------------------------------------
int eval(T_Position currentPosition, T_ListeCoups listeCoups, octet myColor);
int alphabeta(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor,int alpha, int beta);
void afficher_tableau(int info_position_des_voisins[10][8]);
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups);
int nbvoisin(T_Position Position,octet pion);
//----------------------------------------------------------------------

// Fonction affichage tableau 2 dimensions 
void afficher_tableau(int info_position_des_voisins[10][8]){

int g,x=0;
	//printf("Fonction afficher tableau\n");
	for (g = 0; g < 3; g++)
	{
		for (int x = 0; x < 8; x++)
		{
			//printf("%d ",info_position_des_voisins[g][x]);
		}
		//printf("\n");
	}

}

int nbvoisin(T_Position Position,octet pion)
{
    int i,compteur=0;
    T_Voisins Voisin;
    Voisin=getVoisins(pion);
    for(i=0;i<Voisin.nb;i++)
    {
        if(Position.cols[Voisin.cases[i]].nb!=0)
        	compteur++;
    }
    return compteur;
}


int eval(T_Position currentPosition, T_ListeCoups listeCoups, octet myColor)
{
	// Declaration des variables 
	T_Score scoreJoueur=evaluerScore(currentPosition);
	octet nbJ=scoreJoueur.nbJ;
	octet nbR=scoreJoueur.nbR;
	octet nbJ5=scoreJoueur.nbJ5;
	octet nbR5=scoreJoueur.nbR5;
	octet rouge=2;
	octet jaune=1;
	octet o, d;
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	int i=0,r=0;
	int difference=0;
	int score=0,score2=0;
	int flag=0, flagbouge=0;
	int voisin=90;
	int info_position_des_voisins[10][8]={{0}};
	int nb_origine_voisins;
	int nb_destination_voisins;
	int compteur_voisins_o=0;
	int compteur_voisins_d=0;

	//printf("\nMA COULEUR EST : %d",myColor); //DEBUG
	//printf("\nMON SCORE J EST : %d",nbJ);
	//printf("\nMON SCORE R EST : %d",nbR);
	//printf("\nIL RESTE %d COUPS",listeCoups.nb);
	
	if (myColor==jaune)
	{
		score=nbJ-nbR;
		score2=nbJ-nbR;
	}
	if (myColor==rouge)
	{
		score=nbR-nbJ;
		score2=nbR-nbJ;
	}
	if (listeCoups.nb==0) //SI ON ARRIVE A LA FIN DU JEU (ON A PLUS DE COUP FAISABLE)
	{
			if (myColor == jaune) //Si je suis jaune
			{
				difference=nbJ-nbR;
				if (nbJ>nbR || nbJ==nbR && nbJ5>nbR5 ) //SI JE GAGNE EN TANT QUE JAUNE
				{
					difference=difference+1000;
					//printf("\n%d\n",difference);
					return(difference); //On indique qu'on gagne
				}
				else //SI JE PERD EN TANT QUE JAUNE
				{
					difference=difference-1000;
					//printf("\n%d\n",difference);
					return(difference); //On indique qu'on perd
				}
			}
			if (myColor == rouge) //SI JE GAGNE EN TANT QUE ROUGE
			{
				difference=nbR-nbJ;
				if (nbJ<nbR || nbJ==nbR && nbJ5<nbR5) 
				{
					difference=difference+1000;
					//printf("\n%d\n",difference);
					return(difference); //On indique qu'on gagne
				}
				else
				{
					difference=difference-1000;
					//printf("\n%d\n",difference);
					return(difference); //On indique qu'on perd
				}
			}
			if (nbJ==nbR && nbJ5==nbR5) //SI EGALITE
			{
				//printf("\n0\n");
				return(950); //Sinon on indique l'égalité
			}
	}
	else 
	{
		for(i=0;i<NBCASES;i++)
		{
			o=i;
			Tab_origine_voisins=getVoisins(o);
			compteur_voisins_o=Tab_origine_voisins.nb;
			voisin=nbvoisin(currentPosition,o);
			// Affiche voisins de l'origine 
			for (int k=0;k<compteur_voisins_o;k++)
			{
				info_position_des_voisins[0][k]=Tab_origine_voisins.cases[k];
				info_position_des_voisins[1][k]=currentPosition.cols[Tab_origine_voisins.cases[k]].nb;
				info_position_des_voisins[2][k]=currentPosition.cols[Tab_origine_voisins.cases[k]].couleur;			
			}// Fin du for
			if (currentPosition.cols[o].nb != VIDE) //Si la tour sélectionné est un pion
			{
				flagbouge=0;
				for(r=0;r<8;r++)
				{			
					if (info_position_des_voisins[1][r]<=(5-currentPosition.cols[o].nb) && info_position_des_voisins[1][r]!=0) // et que autour de celui ci il n'y a pas de pion pouvant s'additionner avec lui alors c'est une tour dites "fixe"
					{
								flagbouge=1;
					}
				}
				d = listeCoups.coups[i].destination;
				//printf("\n\nNBVOISIN:%d // COUP NUMERO: %d\n\n",voisin,o);
				if ((voisin==0) || (currentPosition.cols[o].nb == 5) || flagbouge==0) //Tour pas bougeable, dites fixe, si elle n'a pas de voisin ou si c'est une tour de 5 ou si elle remplit la condition du dessus
				{
					if (currentPosition.cols[o].couleur == JAU) //si la tour est jaune
					{
						if (myColor == jaune) //je suis jaune
						{ 
							score=score+currentPosition.cols[o].nb;
						}
						else // je suis pas jaune
						{
							score=score-currentPosition.cols[o].nb;
						}
					} 
					if (currentPosition.cols[o].couleur == ROU)
					{
						if (myColor == rouge) //je suis rouge
						{
							score=score+currentPosition.cols[o].nb;
						}
						else // je suis pas rouge
						{ 
							score=score-currentPosition.cols[o].nb;
						}
					}
				}
				else //tour movable qui n'est pas fixe
				{
					if (currentPosition.cols[o].nb==4) //Si la tour est de 4
					{
						flag=0;
						for(r=0;r<8;r++)
						{			
							if (info_position_des_voisins[1][r]==1 && info_position_des_voisins[2][r]!=myColor && info_position_des_voisins[2][r]!=0) //et qu'il y a des voisins de 1 ennemies autour
							{
								flag=1;
							}
						}
						if (flag==1)
						{
							score=score-10; // Alors on réduit le score
						}
						else if (flag==0)
						{
							score=score+11; // Sinon on ajoute...
						}
					}
					else if (currentPosition.cols[o].nb==3) //Si la tour est de 3
					{
						flag=0;
						for(r=0;r<8;r++)
						{			
							if (info_position_des_voisins[1][r]==2 && info_position_des_voisins[2][r]!=myColor && info_position_des_voisins[2][r]!=0) //et qu'il y a des voisins de 2 ennemies autour
							{
								flag=1;
							}
						}
						if (flag==1)
						{
							score=score-currentPosition.cols[o].nb;
						}
						else if (flag==0)
						{
							score=score+11;
						}
					}
					else if (currentPosition.cols[o].nb==2) //Si la tour est de 2
					{
						flag=0;
						for(r=0;r<8;r++)
						{			
							if (info_position_des_voisins[1][r]==3 && info_position_des_voisins[2][r]!=myColor && info_position_des_voisins[2][r]!=0) //et qu'il y a des voisins de 3 ennemies autour
							{
								flag=1;
							}
						}
						if (flag==1)
						{
							score=score-currentPosition.cols[o].nb;
						}
						else if (flag==0)
						{
							score=score+11;
						}
					}
					else if (currentPosition.cols[o].nb==1) //Si la tour est de 1
					{
						flag=0;
						for(r=0;r<8;r++)
						{			
							if (info_position_des_voisins[1][r]==4 && info_position_des_voisins[2][r]!=myColor && info_position_des_voisins[2][r]!=0) //et qu'il y a des voisins de 4 ennemies autour
							{
								flag=1;
							}
						}
						if (flag==1)
						{
							score=score-currentPosition.cols[o].nb;
						}
						else if (flag==0)
						{
							score=score+11;
						}
					}
				}
			}
		}
		if (score2==0) //Si en tant que JAU : nbJ-nbR == 0 ou en tant que ROU : nbR-nbJ == 0 alors
		{
			if (myColor==JAU) //Si ma couleur est jaune, on additionne au score nos tours de 5 et on soustrait celle de l'ennemi
			{
				score=score-nbR5;
				score=score+nbJ5;
			}
			if (myColor==ROU)
			{
				score=score-nbJ5;
				score=score+nbR5;
			}
		}
	return (score); //On retourne le score.
	}
}	

int alphabeta(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor,int alpha, int beta)
{
	T_Position oldPosition=currentPosition;
	octet o, d;
	T_ListeCoups l=listeCoups;
	int i,value,bestVal,evalvalue;
	
	if (profondeur==0 || listeCoups.nb==0)
	{
		evalvalue=eval(currentPosition, listeCoups, myColor);
		//printf("\nEVAL MAX: %d",evalvalue);
		return (evalvalue);
	}
	
	if (myColor==currentPosition.trait)
	{
		bestVal=-10000;
		for(i=l.nb-1;i>=0;i--)
		{
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination;
			currentPosition=jouerCoup(currentPosition, o, d);
			listeCoups=getCoupsLegaux(currentPosition);
			value = alphabeta(currentPosition, profondeur-1, listeCoups, myColor, alpha, beta);
			if (value>bestVal)
			{
				bestVal=value;
			}
			if (bestVal>alpha)
			{
				alpha=bestVal;
			}
			if (beta <= alpha)
			{
				break;
			}
			listeCoups=l;
			currentPosition=oldPosition;
		}
		return (bestVal);
	}
	
	else
	{
		bestVal=10000;
		for(i=l.nb-1;i>=0;i--)
		{
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination;
			currentPosition=jouerCoup(currentPosition, o, d);
			listeCoups=getCoupsLegaux(currentPosition);
			value = alphabeta(currentPosition, profondeur-1, listeCoups, myColor, alpha, beta);
			if (value<bestVal)
			{
				bestVal=value;
			}
			if (bestVal<beta)
			{
				beta=bestVal;
			}
			if (beta<=alpha)
			{
				break;
			}
			listeCoups=l;
			currentPosition=oldPosition;
		}
		return (bestVal);
	}
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{
	// Declaration des variables 
	int i,n,r=0,alpha=-10000,beta=10000;
	octet o, d, nbd=0, nbo=0;
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	T_Position oldPosition=currentPosition;
	octet myColor = oldPosition.trait; 
	T_ListeCoups l=listeCoups;
	int choix, k, max_val=-10000,score=0, profondeur=3,meilleur_coup=0; //profondeur = profondeur du minimax

	// Pour le traitement des données sur un voisin 
	int info_position_des_voisins[10][8]={{0}};
	int score_coups[listeCoups.nb];
	int score_max=0;
	int score_max2=0;
	int flag=0,flagnb=0,flag5=0;
	int nb_origine_voisins;
	int nb_destination_voisins;
	int compteur_voisins_o=0;
	int compteur_voisins_d=0;



	for (int a=0 ;a<10;a++)
	{
		for(int b=0 ;b<8;b++)
		{
			info_position_des_voisins[a][b]=0; //On reset le tableau des infos
		}
	}


	for(n=0;n<listeCoups.nb;n++)
	{
		score_coups[n]=-10000; //On reset le tableau des scores
	}

/* Les boucles ci-dessous produisent un tableau à 2 dimensions
Voici la typologie du tableau */

	//0 Voisins de l'origine      4 5 8 10 14 15 16 0 
	//1 Le nombre de jeton        0 0 5 1 1 1 1 0 
	//2 Le trait de la pile       0 0 1 1 2 1 2 0 
	//3 Voisins de destinationpos 5 9 11 15 16 17 0 0 
	//4 Le nombre de chaque jeton 0 1 1 1 1 1 0 0 
	//5 Le trait de la pile       0 2 2 1 2 1 0 0 	
	
	for(i=l.nb-1;i>=0;i--) 
	{

		// Dans la liste de coup on a l'origine et la destination
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		//afficherListeCoups(listeCoups);  
		 
		// On affiche les voisins de celui d'origine et de destination 
		Tab_origine_voisins=getVoisins(o);
		Tab_destination_voisins=getVoisins(d);
		compteur_voisins_o=Tab_origine_voisins.nb;
		compteur_voisins_d=Tab_destination_voisins.nb;
		
		// Affiche voisins de l'origine 
		for (int k=0;k<compteur_voisins_o;k++)
		{
			info_position_des_voisins[0][k]=Tab_origine_voisins.cases[k];
			info_position_des_voisins[1][k]=currentPosition.cols[Tab_origine_voisins.cases[k]].nb;
			info_position_des_voisins[2][k]=currentPosition.cols[Tab_origine_voisins.cases[k]].couleur;			
		}// Fin du for 

		// Affiche les voisins de la destination
		for (int k=0;k<compteur_voisins_d;k++)
		{		
			info_position_des_voisins[3][k]=Tab_destination_voisins.cases[k];
			info_position_des_voisins[4][k]=currentPosition.cols[Tab_destination_voisins.cases[k]].nb;
			info_position_des_voisins[5][k]=currentPosition.cols[Tab_destination_voisins.cases[k]].couleur;				
		}


	//---------ZONE TRAITEMENT -----------

		nbd=nbVoisins(d); //On regarde le nombre de voisin de la case destination
		nbo=nbVoisins(o); //On regarde le nombre de voisin de la case origine

		flagnb=1;
		if (listeCoups.nb<=65) //Si LATE MIDGAME alors on MINIMAX ALPHABETA avec une profondeur de 4
		{
			profondeur=4;
		}
		if (listeCoups.nb<=30) //Si ENDGAME alors on MINIMAX ALPHABETA jusqu'au bout
		{
			profondeur=99;
		}
		
		flag5=0;		
		for(r=0;r<8;r++)
		{	
			if ( (info_position_des_voisins[4][r]<=(5-(currentPosition.cols[d].nb + currentPosition.cols[o].nb))) && (info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) && (info_position_des_voisins[4][r]!=0))
			{
				//printf("Mauvais coup\n");
				flag5=1;
			}
		}
		if (nbd==1 && (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) || (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && flag5==0) //Si l'origine est de ma couleur et la destination non, alors qu'elle ne posséde qu'un voisin, c'est une tour gratuite! OU Si l'origine est de ma couleur et la destination non alors que la position de destination ne posséde pas de tour ayant un nombre pouvant s'ajouter à l'addition de l'origine et la destination qui ne soit pas de ma couleur alors c'est aussi une tour gratuite!
		{
				score_coups[i]=900;
		}
		
		//On MINIMAX par défaut.
		if (flagnb==1)
		{
			//printf("\n\nMINIMAX ALPHABETA\n\n");
			currentPosition=jouerCoup(currentPosition, o, d);
			listeCoups=getCoupsLegaux(currentPosition);
			score = alphabeta(currentPosition, profondeur-1, listeCoups, myColor, alpha, beta);
			if (score>max_val)
			{
				if (score>score_coups[i])
				{
					score_coups[i]=score;
				}
			}
			listeCoups=l;
			currentPosition=oldPosition;
		}
		
		//--------------------------------------------------------------
		
		// REPLACE DES 0 DANS LE TABLEAU 
		for (int a=0 ;a<10;a++)
		{
			for(int b=0 ;b<8;b++)
			{
				info_position_des_voisins[a][b]=0;
			}
		}
		//afficher_tableau(info_position_des_voisins);
	} // Fin du grand for !!!

	// Listage du/des meilleur coups
	//printf("\nTableau des scores des coups \n");
	for(n=l.nb-1;n>=0;n--)
	{
		//printf("Coup numero %d : %d\n",n,score_coups[n]);
	}

	//Prend le meilleur coup
	for(n=l.nb-1;n>=0;n--)
	{
		if (score_coups[n]>score_coups[n+1])
		{
			// On retient l'indice dans le tableau 
			if (score_max2<score_coups[n])
			{
				score_max2=score_coups[n];
				score_max=n;
				//printf("\nMEILLEUR SCORE ACTUEL: \n%d\n",score_coups[n]);
			}
		}
	}

	// On va jouer le coup avec le meilleur score 
	// Pour cela on a une variable score_max 
	// qui est l'indice du coup à jouer 
	printf("\n\nALEXANDRE CHOISIT CE COUP : %d \n\n",score_max);
	ecrireIndexCoup(score_max);

}// Fin de la fonction
