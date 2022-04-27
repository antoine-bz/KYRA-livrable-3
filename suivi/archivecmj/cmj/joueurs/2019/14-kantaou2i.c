/********* Moteur de tournoi : Kantaou2I ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

//PROTOTYPES
int max(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor);
int min(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor);
int eval(T_Position currentPosition, T_ListeCoups listeCoups, octet myColor);
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) ;
void afficher_tableau(int info_position_des_voisins[10][8]);
void fonction_enlever_les_voisins_com(int info_position_des_voisins[10][8],octet d, octet o);
//----------------------------------------------------------------------

int eval(T_Position currentPosition, T_ListeCoups listeCoups, octet myColor)
{
	// Declaration des variables 
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	int i,n,j=0,r=0;
	octet o, d, nbd=0, nbo=0; 
	T_Score scoreJoueur=evaluerScore(currentPosition);
	octet nbJ=scoreJoueur.nbJ;
	octet nbR=scoreJoueur.nbR;
	octet nbJ5=scoreJoueur.nbJ5;
	octet nbR5=scoreJoueur.nbR5;
	octet rouge=2;
	octet jaune=1;
	printf("\nMA COULEUR EST : %d",myColor); //DEBUG
	printf("\nMON SCORE J EST : %d",nbJ);
	printf("\nMON SCORE R EST : %d",nbR);
	printf("\nIL RESTE %d COUPS",listeCoups.nb);
	
	if (listeCoups.nb==0) //SI ON ARRIVE A LA FIN DU JEU (ON A PLUS DE COUP FAISABLE)
	{
			if (myColor == jaune) //Jaune
			{
				if (nbJ>nbR || nbJ==nbR && nbJ5>nbR5 ) //SI JE GAGNE EN TANT QUE JAUNE
				{
					printf("\n1000\n");
					return(1000); //win
				}
				else //SI JE PERD EN TANT QUE JAUNE
				{
					printf("\n-1000\n");
					return(-1000); //loose
				}
			}
			if (myColor == rouge) //Rouge
			{
				if (nbJ<nbR || nbJ==nbR && nbJ5<nbR5) 
				{
					printf("\n1000\n");
					return(1000); //win
				}
				else
				{
					printf("\n-1000\n");
					return(-1000); //loose
				}
			}
			if (nbJ==nbR && nbJ5==nbR5)
			{
				printf("\n0\n");
				return(0);
			}
	}
	printf("\n0\n");
	return(0);
}

int min(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor)
{
	T_Position oldPosition=currentPosition;
	octet o, d;
	T_ListeCoups l=listeCoups;
	int min_val = 10000,i,score,evalvalue;
	
	if (profondeur==0 || listeCoups.nb==0)
	{
		evalvalue=eval(currentPosition, listeCoups, myColor);
		printf("\nEVAL MIN: %d",evalvalue);
		return (evalvalue);
	}
	
	for(i=0;i<l.nb;i++)
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		currentPosition=jouerCoup(currentPosition, o, d);
		listeCoups=getCoupsLegaux(currentPosition);
		score = max(currentPosition, profondeur-1, listeCoups, myColor);
		if (score<min_val)
		{
			min_val=score;
		}
		listeCoups=l;
		currentPosition=oldPosition;
	}
	return (min_val);
}

int max(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor)
{
	T_Position oldPosition=currentPosition;
	octet o, d;
	T_ListeCoups l=listeCoups;
	int max_val = -10000,i,score,evalvalue;
	
	if (profondeur==0 || listeCoups.nb==0)
	{
		evalvalue=eval(currentPosition, listeCoups, myColor);
		printf("\nEVAL MAX: %d",evalvalue);
		return (evalvalue);
	}
	
	for(i=0;i<l.nb;i++)
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;
		currentPosition=jouerCoup(currentPosition, o, d);
		listeCoups=getCoupsLegaux(currentPosition);
		score = min(currentPosition, profondeur-1, listeCoups, myColor);
		if (score>max_val)
		{
			max_val=score;
		}
		listeCoups=l;
		currentPosition=oldPosition;
	}
	return (max_val);
}

// Fonction affichage tableau 2 dimensions 
void afficher_tableau(int info_position_des_voisins[10][8]){

int g,x=0;
	printf("Fonction afficher tableau\n");
	for (g = 0; g < 6; g++){
		for (int x = 0; x < 8; x++){
			printf("%d ",info_position_des_voisins[g][x]);
		}
		printf("\n");
	}

}

/* Fonction qui enleve les voisins 
En effet le pion d'origine posséde le pion destination 
dans ses voisins on l'enleve 
de même pour les voisins de destination 
il posséde le pion d'origine en voisin*/

void fonction_enlever_les_voisins_com(int info_position_des_voisins[10][8],octet d, octet o){
	int k=0;
	printf("Fonction qui enleve les voisins commun\n");
	printf("origine %d - destination %d \n",o,d);
	
	for (int g = 0; g < 8; g++)
	{
		if (d==info_position_des_voisins[0][g])
		{
			// On fait une boucle pour tout decaler 
			for(k=g;k<7;k++)
			{
				info_position_des_voisins[0][k]=info_position_des_voisins[0][k+1];
				info_position_des_voisins[1][k]=info_position_des_voisins[1][k+1];
				info_position_des_voisins[2][k]=info_position_des_voisins[2][k+1];
			}// Fin for decalage
			if(k==7)
			{
				info_position_des_voisins[0][k]=0;
				info_position_des_voisins[1][k]=0;
				info_position_des_voisins[2][k]=0;
			}
		}// Fin du if
		if (o==info_position_des_voisins[3][g])
		{
			// On fait une boucle pour tout decaler 
			for(k=g;k<7;k++)
			{
				info_position_des_voisins[3][k]=info_position_des_voisins[3][k+1];
				info_position_des_voisins[4][k]=info_position_des_voisins[4][k+1];
				info_position_des_voisins[5][k]=info_position_des_voisins[5][k+1];
			}// Fin for decalage
			if(k==7 )
			{
				info_position_des_voisins[3][k]=0;
				info_position_des_voisins[4][k]=0;
				info_position_des_voisins[5][k]=0;
			}
		}// Fin du if
	}// Fin du for du tri
}

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {

	// Declaration des variables 
	int i,n,j=0,r=0;
	octet o, d, nbd=0, nbo=0;
	octet myColor = currentPosition.trait; 
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	T_Position oldPosition=currentPosition;
	T_ListeCoups l=listeCoups;
	int choix, k, max_val=-10000,score=0, profondeur=99,meilleur_coup=0;

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



	for (int a=0 ;a<10;a++){
		for(int b=0 ;b<8;b++){
			info_position_des_voisins[a][b]=0;
		}
	}


	for(n=0;n<listeCoups.nb;n++){
		score_coups[n]=0;
		}
	
	// Affichage liste de coups 
	//afficherListeCoups(listeCoups); 
	
	// Affichage de ma couleur 
	//printf("Ma couleur est : %d\n",myColor);

/* Les boucles ci-dessous produisent un tableau à 2 dimensions
Voici la typologie du tableau */

	//0 Voisins de l'origine      4 5 8 10 14 15 16 0 
	//1 Le nombre de jeton        0 0 5 1 1 1 1 0 
	//2 Le trait de la pile       0 0 1 1 2 1 2 0 
	//3 Voisins de destinationpos 5 9 11 15 16 17 0 0 
	//4 Le nombre de chaque jeton 0 1 1 1 1 1 0 0 
	//5 Le trait de la pile       0 2 2 1 2 1 0 0 	
	
	for(i=0;i<l.nb; i++) {

		// Dans la liste de coup on a l'origine et la destination
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		 
		// On affiche les voisins de celui d'origine et de destination 
		Tab_origine_voisins=getVoisins(o);
		Tab_destination_voisins=getVoisins(d);
		//printf("\n--Ma couleur : %d\n", currentPosition.trait);
		//printf("Le coups %d - %d \n",o,d);
		//printf("Le nombre de voisin autour de l'origine :%d\n",Tab_origine_voisins.nb); 
		//printf("Le nombre de voisin autour de destination :%d\n",Tab_destination_voisins.nb); 

		compteur_voisins_o=Tab_origine_voisins.nb;
		compteur_voisins_d=Tab_destination_voisins.nb;
		
		// Affiche voisins de l'origine 
		for (int k=0;k<compteur_voisins_o;k++){

			info_position_des_voisins[0][k]=Tab_origine_voisins.cases[k];
			info_position_des_voisins[1][k]=currentPosition.cols[Tab_origine_voisins.cases[k]].nb;
			info_position_des_voisins[2][k]=currentPosition.cols[Tab_origine_voisins.cases[k]].couleur;			

		}// Fin du for 

		// Affiche les voisins de la destination
		for (int k=0;k<compteur_voisins_d;k++){
		
			info_position_des_voisins[3][k]=Tab_destination_voisins.cases[k];
			info_position_des_voisins[4][k]=currentPosition.cols[Tab_destination_voisins.cases[k]].nb;
			info_position_des_voisins[5][k]=currentPosition.cols[Tab_destination_voisins.cases[k]].couleur;			
		
		}
	//------------------------------------------------------
		printf("\n---------COUP NUMERO %d------------\n",i);
		printf("On affiche les voisins\n");
		afficher_tableau(info_position_des_voisins);

	/* Info position  est un tableau pour determiner un 
	score à un coups ! On regarde la liste de coup entierement
	Pour chaque coup donne un score et à la fin on prend le 
	score le plus elevé */

	//-----Gestion du tableau ---------------

	fonction_enlever_les_voisins_com(info_position_des_voisins,d,o);
	
	afficher_tableau(info_position_des_voisins);
	
	//printf("Case origine : %d\n",o); 
	//printf("Case destination : %d\n",d); 

	//---------ZONE TRAITEMENT -----------

		nbd=nbVoisins(d); //On regarde le nombre de voisin de la case destination
		nbo=nbVoisins(o); //On regarde le nombre de voisin de la case origine
		
		if (listeCoups.nb<26)
		{
			flagnb=1;
		}
		
		// On va faire une pile 5 
		// Contraintes :
		// 1. Le pion d'origine est de notre couleur && l'addition de l'origine et la destination fait 5
			//	1. Si notre pion d'origine ou de destination a plus d'un voisin et que la destination n'est pas de notre couleur
			//	on met score = 99
			//	2. Si notre pion d'origine ou de destination a plus d'un voisin et que la destination est de notre couleur
			//	on met score = 98
		
		if ( (currentPosition.cols[o].couleur == myColor)
			&& (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5) && flagnb==0)
		{
				if ((nbd>1 || nbo>1) & (currentPosition.cols[d].couleur != myColor)) //Si il y a plusieurs voisins et que la destination n'est pas de ma couleur
				{
					score_coups[i]=99;
					printf("\n\n99\n\n");
				}
				if ((nbd>1 || nbo>1) & (currentPosition.cols[d].couleur == myColor) && score_coups[i]<98) //Si il y a plusieurs voisins et que la destination est de ma couleur
				{
					score_coups[i]=98;
					printf("\n\n98\n\n");
				}	
		}
		
		// Faire une pile de 4 
		// Contraintes :
		
		// 1. l'addition de l'origine et la destination fait 4 && le pion d'origine est de mon trait 
		// && le pion de destination ne posséde pas de pions à 1 qui ne sont pas de ma couleur autour de lui
		// on met score = 90
		
		// 2. l'addition de l'origine et la destination fait 4
		// && le pion de destination ne posséde que des pions de ma couleur && nombre de voisin > 1.
		// on met score = 85
		
		flag=0;
		for(r=0;r<8;r++){
					
			if ((info_position_des_voisins[4][r]==1) && info_position_des_voisins[5][r]!=myColor && flagnb==0){
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
		for(r=0;r<8;r++){
					
			if (info_position_des_voisins[5][r]!=myColor && flagnb==0){
				flag=1;
			}
		}

		if ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 4 && flag==0 && score_coups[i]<85 && nbd>1 && flagnb==0)
		{
			score_coups[i]=85;
			printf("\n\n85\n\n"); 
		}
		
		/*
		// Faire une pile 3
		// Contraintes :
		
		// 1: La somme des deux vaut 3 && il n'y a pas de pion de 2 autour
		// de la destination qui ne sont pas de ma couleur && le pion origine est de ma couleur
		// on met score = 80
		
		// 2. L'addition de l'origine et la destination fait 3
		// && le pion de destination ne posséde que des pions de ma couleur && nombre de voisin > 1.
		// on met score = 75

		*/
		
		flag=0;		
		for(r=0;r<8;r++){
					
			if (info_position_des_voisins[4][r]==2 && info_position_des_voisins[5][r]!=myColor && flagnb==0){
				//printf("Mauvais coup \n");
				flag=1;
			}
		}

		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb ) == 3 && flag==0 && score_coups[i]<80 && flagnb==0)
		{
			printf("\n\n80\n\n");
			score_coups[i]=80; 
		}
		
		flag=0;
		for(r=0;r<8;r++){
					
			if (info_position_des_voisins[5][r]!=myColor && flagnb==0){
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		if ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 3 &&  flag==0 && score_coups[i]<75 && nbd>1 && flagnb==0)
		{
			printf("\n\n75\n\n");
			score_coups[i]=75; 
		}


		// Faire une pile de 2 
		// Contraintes :
		
		// 1: La somme des deux vaut 2 && il n'y a pas de pion de 3 autour
		// de la destination qui ne sont pas de ma couleur && le pion origine est de ma couleur
		// on met score = 70
		
		// 2. L'addition de l'origine et la destination fait 2
		// && le pion de destination ne posséde que des pions de ma couleur && nombre de voisin > 1.
		// on met score = 65
		
		flag=0;		
		for(r=0;r<8;r++){
					
			if (info_position_des_voisins[4][r]==3 && info_position_des_voisins[5][r]!=myColor && flagnb==0){
				//printf("Mauvais coup \n");
				flag=1;
			}
		}

		if ((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 2 && flag==0 && score_coups[i]<70 && flagnb==0)
		{
			printf("\n\n70\n\n");
			score_coups[i]=70; 
		}
		
		flag=0;
		for(r=0;r<8;r++){
					
			if (info_position_des_voisins[5][r]!=myColor && flagnb==0){
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		if ((currentPosition.cols[o].nb+currentPosition.cols[d].nb) == 2 && flag==0 && score_coups[i]<65 && nbd>1 && flagnb==0)
		{
			printf("\n\n65\n\n");
			score_coups[i]=65; 
		}
		
		
		// Stratégie ANNEXE :
		// Si le pion d'origine n'est pas de ma couleur && la destination non plus && la somme des deux fait moins de 4 && il n'y a pas de pion permettant de faire une tour de 5 dans la zone de destination aprés coup alors
		// on met score = 92
		
		flag=0;		
		for(r=0;r<8;r++){
					
			if ( (info_position_des_voisins[4][r]==(5-(currentPosition.cols[d].nb + currentPosition.cols[o].nb))) && flagnb==0 )
			{
				//printf("Mauvais coup \n");
				flag=1;
			}
		}
		
		if ( (currentPosition.cols[o].couleur!=myColor) && (currentPosition.cols[d].couleur!=myColor) && (currentPosition.cols[d].nb + currentPosition.cols[o].nb < 4) && flag==0 && score_coups[i]<95 && flagnb==0)
		{		
			printf("\n\n95\n\n");
			score_coups[i]=95;
		}
		
		//ENDGAME, on MINIMAX.
		if (flagnb==1)
		{
			printf("\n\nMINIMAX\n\n");
			currentPosition=jouerCoup(currentPosition, o, d);
			listeCoups=getCoupsLegaux(currentPosition);
			score = min(currentPosition, profondeur-1, listeCoups, myColor);
			if (score>max_val)
			{
				score_coups[i]=score;
			}
			listeCoups=l;
			currentPosition=oldPosition;
		}
		
		//Si le pion  adverse n'a pas de voisin HORMIS LE PION O et sont d'une couleur diff; on empile le notre sur l'adversaire
		if (nbd==1 && (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && flagnb==0)
		{
			score_coups[i]=100; 
			printf("\n\n100\n\n");
		}
		
		//Si notre pion et celui souhaité n'ont pas de voisin HORMIS EUX MEME et sont d'une même couleur; on empile pas le notre sur l'adversaire; on le garde pour la fin de partie
		if (nbd==1 && nbo==1 && (currentPosition.cols[o].couleur == currentPosition.cols[d].couleur) && flagnb==0)
		{
			score_coups[i]=0; 
			printf("\n\n0\n\n");
		}
		
		
		//--------------------------------------------------------------
		//SI IL N'Y A RIEN DE CHOISI ON MET 0
		
		
		if (score_coups[i]!=100 && score_coups[i]!=99 && score_coups[i]!=98 && score_coups[i]!=95 && score_coups[i]!=92 && score_coups[i]!=90 && score_coups[i]!=85 && score_coups[i]!=80 && score_coups[i]!=75 && score_coups[i]!=70 && score_coups[i]!=65 && score_coups[i]!=0 && flagnb==0)
		{
			printf("\n\n0\n\n");
			score_coups[i]=0;
		}
		
		//--------------------------------------------------------------
		
		// REPLACE DES 0 DANS LE TABLEAU 
		for (int a=0 ;a<10;a++){
			for(int b=0 ;b<8;b++){
			info_position_des_voisins[a][b]=0;
			}
		}
		//afficher_tableau(info_position_des_voisins);
	} // Fin du grand for !!!

	// Listage du/des meilleur coups
	printf("\nTableau des scores des coups \n");
	for(n=0;n<l.nb;n++){
		printf("Coup numero %d : %d\n",n,score_coups[n]);
		}

	//Prend le meilleur coup
	for(n=0;n<l.nb;n++){
		if (score_coups[n]>score_coups[n+1]){
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
