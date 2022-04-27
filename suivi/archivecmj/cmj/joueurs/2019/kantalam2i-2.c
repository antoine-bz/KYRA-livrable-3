/********* Moteur de tournoi : Kantalam2i ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

#define NBSTRAT 7

//PROTOTYPES------------------------------------------------------------
int nbvoisin(T_Position * Position,octet pion);
int erreurcritique(T_Position * Position,int i,T_ListeCoups * listeCoups);
int isolement(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille);
int empileradverse(T_Position * Position,T_ListeCoups * listeCoups,int i,int * voisins);
int empileradverse2(T_Position * Position,T_ListeCoups * listeCoups,int i);
int isolementsuicide(T_Position * Position,T_ListeCoups * listeCoups,int i /* index du coup joué*/,int * taille);
int autoempileopti(T_Position * Position,T_ListeCoups * listeCoups,int i /* index du coup joué*/,int * taille);
int alphabeta(T_Position currentPosition, int profondeur, T_ListeCoups listeCoups, octet myColor,int alpha, int beta);
int antisolement(T_Position * Position,T_ListeCoups * listeCoups,int i);
int eval(T_Position currentPosition, T_ListeCoups listeCoups, octet myColor);
int Kanta(T_Position currentPosition, T_ListeCoups listeCoups);
//----------------------------------------------------------------------

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{
	int n,alpha=-10000,beta=10000;
	octet myColor = currentPosition.trait; 
	T_Position oldPosition=currentPosition;
	T_ListeCoups l=listeCoups;
	int max_val=-10000,score=0, profondeur=99,meilleur_coup=0; //profondeur = profondeur du minimax
	int score_coups[listeCoups.nb];
	int score_max=0;
	int score_max2=0;
	int tailleISOSUICIDE=5;
	int tailleAUTAMP=0;
	int taille,voisins;
	int i; 
	int isolement_coup;
	int coup_possible;
	int priorite[NBSTRAT];
	octet o, d;

	if (listeCoups.nb<=30) //On active le minimax alpha-beta
	{
		for(i=0;i<listeCoups.nb; i++) 
		{   //on parcours le nombre de coup possible
			o = listeCoups.coups[i].origine; 	//voir struct T coup de avalam.h
			d = listeCoups.coups[i].destination;  
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
		return;
	}
	else //Sinon si il y a plus de X coups encore, on active la stratégie de base
	{
		for(i=0;i<NBSTRAT;i++)
		{
			priorite[i]=-1; //On initialise le tableau avec -1 suivant le nb de strat
		}
		voisins=0;
		taille=5;

		printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
		for(i=0;i<listeCoups.nb; i++) //Pour chaque coup possible
		{
			o = listeCoups.coups[i].origine;
			d = listeCoups.coups[i].destination;
			if ( (currentPosition.cols[o].couleur == myColor)
			&& (currentPosition.cols[d].nb+currentPosition.cols[o].nb == 5) && (currentPosition.cols[d].couleur != myColor)) //Si on peut prendre une tour de 5 avec une tour ennemi on prend.
			{
				ecrireIndexCoup(i);	
				printf("tour de 5 prise par le coup %d : %d ---> %d \n",i,o,d);			
				return;
			}
			if ( (currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && nbvoisin(&currentPosition,d)==1) //Si on peut prendre une tour isolé avec une tour ennemi on prend.
			{
				ecrireIndexCoup(i);	
				printf("tour isolé prise par le coup %d : %d ---> %d \n",i,o,d);			
				return;
			}
			coup_possible=isolement(&currentPosition,&listeCoups,i,&taille); //taille = 5 au début
			if(coup_possible!=-1)
				priorite[0]=coup_possible;
			coup_possible=antisolement(&currentPosition,&listeCoups,i);
			if(coup_possible!=-1)
				priorite[1]=coup_possible;
			coup_possible=empileradverse2(&currentPosition,&listeCoups,i);
			if(coup_possible!=-1)
				priorite[2]=coup_possible;
			coup_possible=empileradverse(&currentPosition,&listeCoups,i,&voisins);
			if(coup_possible!=-1)
				priorite[3]=coup_possible;
			if (priorite[0]==-1 && priorite[1]==-1 && priorite[2]==-1 && priorite[3]==-1) //On effectue Kanta si seulement les 3 premiéres ne donnent rien
			{
				coup_possible=Kanta(currentPosition,listeCoups);
				if(coup_possible!=-1)
					priorite[4]=coup_possible;
			}
			coup_possible=isolementsuicide(&currentPosition,&listeCoups,i,&tailleISOSUICIDE);
			if(coup_possible!=-1)
				priorite[5]=coup_possible;
			coup_possible=autoempileopti(&currentPosition,&listeCoups,i,&tailleAUTAMP);
			if(coup_possible!=-1)
				priorite[6]=coup_possible;
		} 
		for(i=0;i<NBSTRAT;i++)
		{
			printf("\n%d\n",priorite[i]);
		}
		for(i=0;i<NBSTRAT;i++)
		{
			if(priorite[i]!=-1)
			{
				ecrireIndexCoup(priorite[i]);
				return;
			}
		}
	}	
}

int erreurcritique(T_Position * Position,int i ,T_ListeCoups * listeCoups) //Fonction empêchant de faire des coups dit "erreur critique"
{
    T_Voisins VoisinD,VoisinD2;
    int j,risque;
    octet myColor=Position->trait;
    octet o,d;
    o = listeCoups->coups[i].origine; 
    d = listeCoups->coups[i].destination;
    VoisinD=getVoisins(d);

    if(Position->cols[d].nb+Position->cols[o].nb != 5) //Si la D et l'O ne font pas 5
    {
        for(j=0;j<VoisinD.nb;j++) //Pour tout les voisins de D
        {
            if(o!=VoisinD.cases[j]) //Si l'origine ne fait pas parti des voisins de D
            {
                if(Position->cols[o].nb+Position->cols[d].nb+Position->cols[VoisinD.cases[j]].nb==5 && (Position->cols[o].couleur!=myColor || Position->cols[VoisinD.cases[j]].couleur!=myColor))
                	return 2; //On retourne 2 si l'origine + la dest + un des voisins de dest = 5 et que l'o n'est pas de ma couleur OU les voisins de D ne sont pas de ma couleur
                if(Position->cols[o].nb+Position->cols[d].nb+Position->cols[VoisinD.cases[j]].nb==5 && (Position->cols[o].couleur!=myColor && Position->cols[VoisinD.cases[j]].couleur!=myColor))
                	return 1; //On retourne 1 si l'origine + la dest + un des voisins de dest = 5 et que l'o n'est pas de ma couleur ET les voisins de D ne sont pas de ma couleur
            }
        }
        if(nbvoisin(Position,d)==1) //Si d n'a qu'un voisin (donc O)
        {
            if(Position->cols[o].couleur!=myColor)
            	return 2; //On retourne 2 si O n'est pas de ma couleur
        }
        if(nbvoisin(Position,d)==2) //Si d a 2 voisins
        {
            for(j=0;j<VoisinD.nb;j++) //Pour chaque voisins de D
            {
                if(o!=VoisinD.cases[j]) //Si l'origine ne fait pas parti des voisins de D
                {
                    VoisinD2=getVoisins(VoisinD.cases[j]);//On regarde le voisin des voisins
                    if(nbvoisin(Position,VoisinD.cases[j])==1)//Si ils n'ont qu'un voisin
                    {
                        if(Position->cols[o].couleur!=myColor || Position->cols[VoisinD.cases[j]].couleur!=myColor)
                        	return 1; //On retourne 1 si O n'est pas de notre couleur ou si les voisins de d ne sont pas de ma couleur
                    }
                }
            }
        }
    }
    else
    {
        if(Position->cols[o].couleur!=myColor) //Si l'origine n'est pas de ma couleur
        	return 2;//on retourne 2
    }
	return 0;//sinon il n'y a pas d'erreur dites critique, on retourne 0
}

int isolement(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille) //Fonction cherchant à isoler nos pions
{
    octet myColor=Position->trait;
    octet o,d;
	o = listeCoups->coups[i].origine; 
	d = listeCoups->coups[i].destination;
	int flag=0,flag2=0;
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	// Pour le traitement des données sur un voisin 
	int info_position_des_voisins[10][8]={{0}};
	int nb_origine_voisins;
	int nb_destination_voisins;
	int compteur_voisins_o=0;
	int compteur_voisins_d=0;
	int addition = Position->cols[o].nb;
	int addition2=0;
	
	if (Position->cols[o].couleur == myColor && Position->cols[d].couleur == myColor) //Si le pion d'origine et de destination sont de ma couleur, pour isoler la destination au lieu d'empiler l'origine dessus, on va éloigner l'origine plus loin.
	{
		if( (1<nbvoisin(Position,o)) && (1==nbvoisin(Position,d)) && (erreurcritique(Position,i,listeCoups)<1) && (*(taille)>Position->cols[d].nb+Position->cols[o].nb))
		{
			for(int j=0;j<listeCoups->nb; j++) 
			{
				octet o2 = listeCoups->coups[j].origine; 
				octet d2 = listeCoups->coups[j].destination;  
				if ((o2 == o))
				{
						Tab_origine_voisins=getVoisins(o2);
						Tab_destination_voisins=getVoisins(d2);
						compteur_voisins_o=Tab_origine_voisins.nb;
						compteur_voisins_d=Tab_destination_voisins.nb;
						for (int k=0;k<compteur_voisins_o;k++)
						{
							info_position_des_voisins[0][k]=Tab_origine_voisins.cases[k];
							info_position_des_voisins[1][k]=Position->cols[Tab_origine_voisins.cases[k]].nb;
							info_position_des_voisins[2][k]=Position->cols[Tab_origine_voisins.cases[k]].couleur;		
						}// Fin du for 
						for(int r=0;r<8;r++)
						{	
							addition=addition+Position->cols[d2].nb;
							addition2=addition+info_position_des_voisins[1][r];
							//printf("ADDITION = %d\n",addition2);
							if (info_position_des_voisins[1][r]+addition>=5)
							{
								flag=1; //Si je le raméne on peut une tour de 5 donc nul
							}
							if (info_position_des_voisins[2][r]==myColor)
							{
								flag2=1; //Si je le raméne il y a des pions de ma couleur autour donc bien
							}
						}
					if (Position->cols[d2].couleur != myColor && flag==0 && flag2==1)
					{
						*(taille)=Position->cols[d2].nb+Position->cols[o2].nb; //Alors on retourne la taille par adresse	
						return j;
					}
				}
			}
		}
	}
    if (Position->cols[o].couleur == myColor) //Si origine de ma couleur
    {
			if( (1<nbvoisin(Position,o)) && (1==nbvoisin(Position,d)) && (erreurcritique(Position,i,listeCoups)<1) && (*(taille)>Position->cols[d].nb+Position->cols[o].nb)) //si le nb de voisin de la dest = 1 et il n'y a pas d'erreur critique avec la position i, et que la taille est inférieur à l'addition de la D et de l'O
			{
					*(taille)=Position->cols[d].nb+Position->cols[o].nb; //Alors on retourne la taille par adresse	
					return i; //Et on prend la position
			}
			else return -1; //Sinon -1
			
	}
	else return -1;//Sinon -1
}

int antisolement(T_Position * Position,T_ListeCoups * listeCoups,int i) //Fonction cherchant à empécher d'isoler les pions adverse
{
    octet myColor=Position->trait;
    octet o,d,nbd=nbVoisins(d);
	o = listeCoups->coups[i].origine; 
	d = listeCoups->coups[i].destination;
	T_Voisins Tab_origine_voisins;
	T_Voisins Tab_destination_voisins;
	int flag=0;
	int flag2=0;
	// Pour le traitement des données sur un voisin 
	int info_position_des_voisins[10][8]={{0}};
	int nb_origine_voisins;
	int nb_destination_voisins;
	int compteur_voisins_o=0;
	int compteur_voisins_d=0;
	int addition = Position->cols[d].nb + Position->cols[o].nb;
	int addition2=0;
	//printf("ADDITION = %d\n",addition);
    if (Position->cols[o].couleur != myColor) //Si origine pas de ma couleur
    {
			if((1==nbvoisin(Position,d)) && (nbvoisin(Position,o)>1) && (Position->cols[d].nb + Position->cols[o].nb != 5)) //si D a qu'un voisin (donc O) et que O a plusieurs voisins, et que l'addition des deux ne fait pas une tour de 5, on empêche l'ennemi d'isoler
			{
				for(int j=0;j<listeCoups->nb; j++) 
				{
					octet o2 = listeCoups->coups[j].origine; 
					octet d2 = listeCoups->coups[j].destination;  
					if ((o2 == d) && (d2 == o)) //On inverse le coup
					{
						Tab_origine_voisins=getVoisins(o2);
						Tab_destination_voisins=getVoisins(d2);
						compteur_voisins_o=Tab_origine_voisins.nb;
						compteur_voisins_d=Tab_destination_voisins.nb;
						for (int k=0;k<compteur_voisins_o;k++)
						{
							info_position_des_voisins[0][k]=Tab_origine_voisins.cases[k];
							info_position_des_voisins[1][k]=Position->cols[Tab_origine_voisins.cases[k]].nb;
							info_position_des_voisins[2][k]=Position->cols[Tab_origine_voisins.cases[k]].couleur;		
						}// Fin du for 
						for(int r=0;r<8;r++)
						{	
							addition2=addition+info_position_des_voisins[1][r];
							//printf("ADDITION = %d\n",addition2);
							if (info_position_des_voisins[1][r]+addition>=5)
							{
								flag=1; //Si je le raméne on peut une tour de 5 donc nul
							}
							if (info_position_des_voisins[2][r]==myColor)
							{
								flag2=1; //Si je le raméne il y a des pions de ma couleur autour donc bien
							}
						}
						if (flag==0 && flag2==1)
						{
							i=j; //Et on prend la position
						}
						else
						{
							i=-1;
						}
					}
				}
				return i;
			}
			else return -1; //Sinon -1	
	}
	else return -1;//Sinon -1
}

int nbvoisin(T_Position * Position,octet pion) //Fonction comptant le nb de voisin
{
    int i,compteur=0;
    T_Voisins Voisin;
    Voisin=getVoisins(pion);

    for(i=0;i<Voisin.nb;i++)
    {
        if(Position->cols[Voisin.cases[i]].nb!=0 && Position->cols[Voisin.cases[i]].nb!=5) //Si ce n'est pas une case vide ou une tour fixe de 5 alors on a un voisin de plus
        	compteur++;
    }
    return compteur;
}

int empileradverse(T_Position * Position,T_ListeCoups * listeCoups,int i,int * voisins)
{
    octet myColor=Position->trait;
    octet o,d;
	o = listeCoups->coups[i].origine;
	d = listeCoups->coups[i].destination;  
	
	if ( (Position->cols[o].couleur != myColor) && (Position->cols[d].couleur != myColor) && (erreurcritique(Position,i,listeCoups)<1) && (*voisins<nbvoisin(Position,d))) //Si l'O et la D ne sont pas de ma couleur et qu'il n'y a pas d'erreur critique sur ce coup, et si le nb de voisins est inférieur ou égal à 0
	{ 
		*voisins=nbvoisin(Position,d); 
		return i; //On retourne ce coup				
	}
	return -1; //On retourne -1
}

int empileradverse2(T_Position * Position,T_ListeCoups * listeCoups,int i)
{
    int memoire=0;
    octet myColor=Position->trait;
    octet o,d;
	o = listeCoups->coups[i].origine;
	d = listeCoups->coups[i].destination;  
	
	if ((Position->cols[d].couleur != myColor) && (erreurcritique(Position,i,listeCoups)<1) ) //Si la destination n'est pas de ma couleur et il n'y a pas d'erreur critique
	{ 
		return i;					
	}
	return -1;
}

int isolementsuicide(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille)
{
    octet myColor=Position->trait;
    octet o,d;
	o = listeCoups->coups[i].origine;
	d = listeCoups->coups[i].destination;  	
	
	if(Position->cols[o].couleur == myColor && nbvoisin(Position,d)==1 && (*taille>Position->cols[d].nb+Position->cols[o].nb)) //Si origine de ma couleur, et la destination n'a qu'un voisin, et que la taille de la D et de l'O est inférieur à 5
	{
		*taille=Position->cols[d].nb+Position->cols[o].nb;//On prend la nouvelle taille
		return i;//On retourne i
	}
	return -1;
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

int autoempileopti(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille)
{
    octet myColor=Position->trait;
    octet o,d;
	o = listeCoups->coups[i].origine;
	d = listeCoups->coups[i].destination;  

	if (Position->cols[o].couleur == myColor && Position->cols[d].couleur == myColor && (erreurcritique(Position,i,listeCoups)<1) && (*taille<Position->cols[d].nb+Position->cols[o].nb)) //Si il n'y a pas d'erreur critique et que la taille de la destination et l'origine est supérieur à la valeur de taille 
	{ 
		*taille=Position->cols[d].nb+Position->cols[o].nb;//Alors on prend la valeur de taille
		return i;//On retourne le coup
	}
	return -1;
}	

int Kanta(T_Position currentPosition, T_ListeCoups listeCoups) 
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
		
		if (listeCoups.nb<=25) //Si ENDGAME alors on MINIMAX ALPHABETA
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
					score_coups[i]=100;
					printf("\n\n100\n\n");
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
		
		if (nbd==1 && (currentPosition.cols[o].couleur == myColor) && flagnb==0 && score_coups[i]<99)
		{		
			printf("\n\n99\n\n");
			score_coups[i]=99;
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
	return(score_max);

}// Fin de la fonction
