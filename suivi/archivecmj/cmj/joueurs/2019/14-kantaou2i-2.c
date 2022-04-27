/********* Moteur de tournoi : Kantaou2I ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

//PROTOTYPES------------------------------------------------------------
int eval(T_Position currentPosition, T_ListeCoups listeCoups, octet myColor);
int alphabeta(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor,int alpha, int beta);
void afficher_tableau(int info_position_des_voisins[10][8]);
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) ;
//----------------------------------------------------------------------

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
	int difference=0;
	printf("\nMA COULEUR EST : %d",myColor); //DEBUG
	printf("\nMON SCORE J EST : %d",nbJ);
	printf("\nMON SCORE R EST : %d",nbR);
	printf("\nIL RESTE %d COUPS",listeCoups.nb);
	
	if (listeCoups.nb==0) //SI ON ARRIVE A LA FIN DU JEU (ON A PLUS DE COUP FAISABLE)
	{
			if (myColor == jaune) //Si je suis jaune
			{
				difference=nbJ-nbR;
				if (nbJ>nbR || nbJ==nbR && nbJ5>nbR5 ) //SI JE GAGNE EN TANT QUE JAUNE
				{
					difference=difference+1000;
					printf("\n%d\n",difference);
					return(difference); //On indique qu'on gagne
				}
				else //SI JE PERD EN TANT QUE JAUNE
				{
					difference=difference-1000;
					printf("\n%d\n",difference);
					return(difference); //On indique qu'on perd
				}
			}
			if (myColor == rouge) //SI JE GAGNE EN TANT QUE ROUGE
			{
				difference=nbR-nbJ;
				if (nbJ<nbR || nbJ==nbR && nbJ5<nbR5) 
				{
					difference=difference+1000;
					printf("\n%d\n",difference);
					return(difference); //On indique qu'on gagne
				}
				else
				{
					difference=difference-1000;
					printf("\n%d\n",difference);
					return(difference); //On indique qu'on perd
				}
			}
			if (nbJ==nbR && nbJ5==nbR5) //SI EGALITE
			{
				printf("\n0\n");
				return(0); //Sinon on indique l'égalité
			}
	}
	printf("\n0\n");
	return(0);
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

// Fonction affichage tableau 2 dimensions 
void afficher_tableau(int info_position_des_voisins[10][8]){

int g,x=0;
	printf("Fonction afficher tableau\n");
	for (g = 0; g < 6; g++)
	{
		for (int x = 0; x < 8; x++)
		{
			printf("%d ",info_position_des_voisins[g][x]);
		}
		printf("\n");
	}

}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{
	// Declaration des variables 
	int i,n,r=0,alpha=-10000,beta=10000;
	octet o, d, nbd=0, nbo=0;
	octet myColor = currentPosition.trait; 
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	T_Position oldPosition=currentPosition;
	T_ListeCoups l=listeCoups;
	int choix, k, max_val=-10000,score=0, profondeur=99,meilleur_coup=0; //profondeur = profondeur du minimax

	// Pour le traitement des données sur un voisin 
	int info_position_des_voisins[10][8]={{0}};
	int score_coups[listeCoups.nb];
	int score_max=0;
	int score_max2=0;
	int flag=0,flagnb=0;
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
		score_coups[n]=0; //On reset le tableau des scores
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
	//------------------------------------------------------
		printf("\n---------COUP NUMERO %d------------\n",i);
		printf("On affiche les voisins\n");
		//afficher_tableau(info_position_des_voisins);

	/* Info position  est un tableau pour determiner un 
	score à un coups ! On regarde la liste de coup entierement
	Pour chaque coup donne un score et à la fin on prend le 
	score le plus elevé */


	//---------ZONE TRAITEMENT -----------

		nbd=nbVoisins(d); //On regarde le nombre de voisin de la case destination
		nbo=nbVoisins(o); //On regarde le nombre de voisin de la case origine
		
		if (listeCoups.nb<=30) //Si ENDGAME alors on MINIMAX ALPHABETA
		{
			flagnb=1;
		}
		
		// On va faire une pile 5 
		// Contraintes :
		// 1. Le pion d'origine est de notre couleur && l'addition de l'origine et la destination fait 5 et le
		// mode MINIMAX ALPHABETA n'est pas activé
			//	1. Si notre pion d'origine ou de destination a plus d'un voisin et que la destination n'est pas de notre couleur alors
			//	on met score = 99
			//	2. Si notre pion d'origine ou de destination a plus d'un voisin et que la destination est de notre couleur alors
			//	on met score = 98
		
		flag=0;
		for(r=0;r<8;r++)
		{			
			if (info_position_des_voisins[5][r]==myColor && flagnb==0)
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		
		if ( (currentPosition.cols[o].couleur == myColor)
			&& (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5) && flagnb==0)
		{
				if ((nbd>1 || nbo>1) & (currentPosition.cols[d].couleur != myColor)) //Si il y a plusieurs voisins et que la destination n'est pas de ma couleur
				{
					score_coups[i]=99;
					printf("\n\n99\n\n");
				}
				if ((nbd>1 || nbo>1) & (currentPosition.cols[d].couleur == myColor) && score_coups[i]<98 && flag==0) //Si il y a plusieurs voisins et que la destination est de ma couleur et il y a des pions rouges autour
				{
					score_coups[i]=98;
					printf("\n\n98\n\n");
				}	
		}
		
		// Faire une pile de 4 
		// Contraintes :
		
		// 1. L'addition de l'origine et de la destination fait une pile de 4, le pion d'origine 
		// est de ma couleur et le pion de destination ne possède pas de pions à 1 qui ne sont 
		// pas de ma couleur autour de lui et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 90
		
		// 2. l'addition de l'origine et la destination fait une pile de 4 et le pion de destination 
		// ne possède que des pions de ma couleur et un nombre de voisin supérieur à 1 et le pion de destination ne possède pas de pions à 1 et le mode MINIMAX n'est pas activé alors
		// on met score = 75
		
		flag=0;
		for(r=0;r<8;r++)
		{			
			if ((info_position_des_voisins[4][r]==1) && (info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) && flagnb==0)
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}

		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 4 && flag==0 && score_coups[i]<90 && flagnb==0)
		{
			printf("\n\n90\n\n");
			score_coups[i]=90; 
		}
		
		
		
		flag=0;		
		for(r=0;r<8;r++)
		{	
			if ((info_position_des_voisins[4][r]==1) && (info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) && flagnb==0)
			{
				flag=1;
			}
		}

		if ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 4 && flag==0 && score_coups[i]<75 && nbd>1 && flagnb==0)
		{
			score_coups[i]=75;
			printf("\n\n75\n\n"); 
		}
		
		/*
		// Faire une pile 3
		// Contraintes :
		
		// 1. L'addition de l'origine et de la destination fait une pile de 3, le pion d'origine 
		// est de ma couleur et le pion de destination ne possède pas de pions à 2 qui ne sont 
		// pas de ma couleur autour de lui et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 85
		
		// 2. l'addition de l'origine et la destination fait une pile de 3 et le pion de destination 
		// ne possède que des pions de ma couleur et un nombre de voisin supérieur à 1 et le pion de destination ne possède pas de pions à 2 et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 70

		*/
		
		flag=0;		
		for(r=0;r<8;r++)
		{
			if (info_position_des_voisins[4][r]==2 && (info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) && flagnb==0)
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}

		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb ) == 3 && flag==0 && score_coups[i]<85 && flagnb==0)
		{
			printf("\n\n85\n\n");
			score_coups[i]=85; 
		}
		
		flag=0;
		for(r=0;r<8;r++)
		{	
			if ((info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) && flagnb==0)
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		if ((info_position_des_voisins[4][r]==2) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 3 &&  flag==0 && score_coups[i]<70 && nbd>1 && flagnb==0)
		{
			printf("\n\n70\n\n");
			score_coups[i]=70; 
		}


		// Faire une pile de 2 
		// Contraintes :
		
		// 1. L'addition de l'origine et de la destination fait une pile de 2, le pion d'origine 
		// est de ma couleur et le pion de destination ne possède pas de pions à 3 qui ne sont 
		// pas de ma couleur autour de lui et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 80
		
		// 2. l'addition de l'origine et la destination fait une pile de 2 et le pion de destination 
		// ne possède que des pions de ma couleur et un nombre de voisin supérieur à 1 et le pion de destination ne possède pas de pions à 3 et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 65
		
		flag=0;		
		for(r=0;r<8;r++)
		{
			if (info_position_des_voisins[4][r]==3 && info_position_des_voisins[5][r]!=myColor && info_position_des_voisins[5][r]!=0 && flagnb==0)
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}

		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 2 && flag==0 && score_coups[i]<80 && flagnb==0)
		{
			printf("\n\n80\n\n");
			score_coups[i]=80; 
		}
		
		flag=0;
		for(r=0;r<8;r++)
		{	
			if ((info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) && flagnb==0)
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		if ((info_position_des_voisins[4][r]==3) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 2 && flag==0 && score_coups[i]<65 && nbd>1 && flagnb==0)
		{
			printf("\n\n65\n\n");
			score_coups[i]=65; 
		}
		
		
		// Stratégie ANNEXE :
		// Si le pion d'origine est de ma couleur et la destination non, et la somme des deux fait moins de 4, et il n'y a pas de pion permettant de faire une tour de 5 dans la zone de destination aprés coup qui ne sont pas de ma couleur et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 95
		
		flag=0;		
		for(r=0;r<8;r++)
		{	
			if ( (info_position_des_voisins[4][r]==(5-(currentPosition.cols[d].nb + currentPosition.cols[o].nb))) && flagnb==0 && (info_position_des_voisins[5][r]!=myColor) && (info_position_des_voisins[5][r]!=0) )
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		
		if ( (currentPosition.cols[o].couleur==myColor) && (currentPosition.cols[d].couleur!=myColor) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb < 4) && flag==0 && score_coups[i]<95 && flagnb==0)
		{		
			printf("\n\n95\n\n");
			score_coups[i]=95;
		}
		
		// Si le pion d'origine n'est pas de ma couleur et la destination non plus, et la somme des deux fait moins de 4, et il n'y a pas de pion permettant de faire une tour de 5 dans la zone de destination aprés coup qui ne sont pas de ma couleur et le mode MINIMAX ALPHABETA n'est pas activé alors
		// on met score = 50
		
		
		if ( (currentPosition.cols[o].couleur!=myColor) && (currentPosition.cols[d].couleur!=myColor) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb < 4) && flag==0 && score_coups[i]<95 && flagnb==0)
		{		
			printf("\n\n50\n\n");
			score_coups[i]=50;
		}
		
		//Si ENDGAME, on MINIMAX.
		if (flagnb==1)
		{
			printf("\n\nMINIMAX ALPHABETA\n\n");
			currentPosition=jouerCoup(currentPosition, o, d);
			listeCoups=getCoupsLegaux(currentPosition);
			score = alphabeta(currentPosition, profondeur-1, listeCoups, myColor, alpha, beta);
			if (score>max_val)
			{
				score_coups[i]=score;
			}
			listeCoups=l;
			currentPosition=oldPosition;
		}
		
		//Si le pion  adverse n'a pas de voisin HORMIS LE PION d'origine et sont d'une couleur différentes; on empile le notre sur l'adversaire et le mode MINIMAX ALPHABETA n'est pas activé.
		if (nbd==1 && (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && flagnb==0)
		{
			score_coups[i]=100; 
			printf("\n\n100\n\n");
		}
		
		//Si notre pion et celui souhaité n'ont pas de voisin HORMIS EUX MEME et sont d'une même couleur; on empile pas le notre sur l'adversaire; on le garde pour la fin de partie et le mode MINIMAX ALPHABETA n'est pas activé.
		if (nbd==1 && nbo==1 && (currentPosition.cols[o].couleur == currentPosition.cols[d].couleur) && flagnb==0)
		{
			score_coups[i]=0; 
			printf("\n\n0\n\n");
		}
		
		
		//--------------------------------------------------------------
		//SI IL N'Y A RIEN DE CHOISI ON MET 0
		
		
		if (score_coups[i]!=100 && score_coups[i]!=99 && score_coups[i]!=98 && score_coups[i]!=95 && score_coups[i]!=90 && score_coups[i]!=85 && score_coups[i]!=80 && score_coups[i]!=75 && score_coups[i]!=70 && score_coups[i]!=65 && score_coups[i]!=50 && score_coups[i]!=0 && flagnb==0)
		{
			printf("\n\n0\n\n");
			score_coups[i]=0;
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
	printf("\nTableau des scores des coups \n");
	for(n=l.nb-1;n>=0;n--)
	{
		printf("Coup numero %d : %d\n",n,score_coups[n]);
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
				printf("\nMEILLEUR SCORE ACTUEL: \n%d\n",score_coups[n]);
			}
		}
	}

	// On va jouer le coup avec le meilleur score 
	// Pour cela on a une variable score_max 
	// qui est l'indice du coup à jouer 
	printf("\n\nON CHOISIT CE COUP : %d\n\n",score_max);
	ecrireIndexCoup(score_max);

}// Fin de la fonction
