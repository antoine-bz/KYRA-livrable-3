/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void copiestruc(T_Position init, T_Position * dest)
{
	int i;
	dest->trait=init.trait;
	for (i=0;i<NBCASES;i++)
	{
		dest->cols[i].nb=init.cols[i].nb;
		dest->cols[i].couleur=init.cols[i].couleur;
	}
}

int bordplateau(int pion)
{
	int i;
	int PionsBordPlateau[22]={0,1,5,10,11,18,19,27,35,41,45,47,46,42,37,36,29,28,20,12,6,2};
	for (i=0;i<=22;i++)
	{
		if (pion==PionsBordPlateau[i])
			return 1;
	}
	return 0;
}

int evaluationJeu(T_Position jeu,int joueurActuel)
{
	int scoreMOI,scoreADV,scoreMOI5,scoreADV5,nbrPion,coulPion;//SCORE ET TAILLE ET COULEUR DES PIONS
	int poidsIA=0,poidsVoisins=0,tauxPionIsole=12,poidsEVBP=0;//AFFECTE UN "SCORE" A CHAQUE SITUATION
	int coeff=10,tauxVoisins=8,taux5Pions=4,tauxPionsBP=4,taux5PionsVoisin=3;
	int ADV,MOI=joueurActuel,compteurMOI=0,compteurADV=0;
	int i,j;
	
	T_Voisins v;
	T_Score score=evaluerScore(jeu);
	T_ListeCoups l=getCoupsLegaux(jeu);
	if (MOI==JAU)
	{
		ADV=ROU;
		scoreMOI=score.nbJ;
		scoreADV=score.nbR;
		scoreMOI5=score.nbJ5;
		scoreADV5=score.nbR5;

	}
	else
	{
		ADV=JAU;
		scoreADV=score.nbJ;
		scoreMOI=score.nbR;
		scoreMOI5=score.nbR5;
		scoreADV5=score.nbJ5;
	}

	
	
	
	for (i=0;i<48;i++)
	{
			if (jeu.cols[i].nb != VIDE )
			{
				nbrPion=jeu.cols[i].nb;
				coulPion=jeu.cols[i].couleur;
				if (nbVoisins(i)==0 && nbrPion<5) // UN PION ISOLE <5
				{
					if (coulPion==MOI)
					{
						poidsIA+=tauxPionIsole;
					}
					else
					{
						poidsIA-=tauxPionIsole;
					}
					
				}

				if (nbrPion==5) // UNE TOUR DE 5
				{
					if (coulPion==MOI)
					{
						poidsIA+=taux5Pions;
					}
					else
					{
						poidsIA-=taux5Pions;
					}
					
				}
				poidsVoisins=0;
				v=getVoisins(i); //VOISIN
				for (j=0;j<nbVoisins(i);j++)
				{
					if (bordplateau(v.cases[j])==0)
					{
						if (nbrPion+jeu.cols[v.cases[j]].nb==5 && coulPion==MOI)
						{
							if (jeu.cols[v.cases[j]].couleur==MOI)
							{

								poidsIA +=taux5PionsVoisin;
								compteurMOI++;

							}
								
							else if (jeu.cols[v.cases[j]].couleur==ADV)
							{
								poidsIA-=taux5PionsVoisin;
								compteurADV++;
							}
							
						}

						if (coulPion==MOI && jeu.cols[v.cases[j]].couleur==MOI)
						{
							if (nbrPion==5)
								poidsVoisins--;
							else
								poidsVoisins++;
							
						}
						else if(jeu.cols[v.cases[j]].couleur==ADV && coulPion==ADV)
							poidsVoisins--;
						/*else if (jeu.cols[v.cases[j]].couleur==ADV && coulPion==MOI)
							poidsVoisins--;*/                        
					}
					else
					{
						if (coulPion==MOI)
							poidsEVBP++;
						else if (coulPion==ADV)
						{
							poidsEVBP--;
						}

						if (jeu.cols[v.cases[j]].couleur==MOI)
							{
								compteurMOI++;

							}
								
							else if (jeu.cols[v.cases[j]].couleur==ADV)
							{
								compteurADV++;
							}
					}

				}//FIN VOISIN


				compteurMOI=0;
				compteurADV=0;

				if (poidsVoisins>1)
					poidsIA +=tauxVoisins;
				else
				{
					poidsIA-=(tauxVoisins);
				}
				
				if (poidsEVBP>1)
					poidsIA +=tauxPionsBP;
				else
				{
					poidsIA-=tauxPionsBP;
				}
			}
	}/*
	if (l.nb==0)
	{
		if(scoreADV>scoreMOI)
			return (10000 + scoreMOI);
		else if (scoreMOI > scoreADV)
			return (-10000 - scoreADV);
		else if (scoreMOI==scoreADV)
		{
			if (scoreMOI5>scoreADV5)
			{
				return (-10000 - scoreADV);
			}
			else if (scoreADV5>scoreMOI5)
			{
				return (10000 + scoreMOI);
			}
			else
			{
				return 0;
			}
		}	
	}*/
	if (joueurActuel==JAU)
	{
		return poidsIA -(score.nbJ-score.nbR)*coeff; //On accorde une grande importance aux nombres de nos pions.
	}
	else
	{
		return poidsIA +(score.nbR-score.nbJ)*coeff;
	}
}

int minimax (int profondeur, T_Position jeu,int alpha,int beta, int maxi)
{
	int meilleurScore,score;
	T_Position jeuTemporaire;
	T_ListeCoups listeCoups=getCoupsLegaux(jeu);
	if (profondeur==0||listeCoups.nb==0)
	{
		if (maxi==1)
			return evaluationJeu(jeu,JAU);
		else if (maxi==0)
			return evaluationJeu(jeu,ROU);
	}
	if (maxi==1)
	{
		meilleurScore = alpha;
				for (int i=0;i<listeCoups.nb;i++)
		{

			copiestruc(jeu,&jeuTemporaire);
			jeuTemporaire=jouerCoup(jeuTemporaire,listeCoups.coups[i].origine,listeCoups.coups[i].destination);
			score=minimax(profondeur-1,jeuTemporaire,meilleurScore,beta,0);
			if (score > meilleurScore)
			{
				meilleurScore=score;
			}
			if (meilleurScore >=beta)
			{
					return beta;
			}
		}
		return meilleurScore;
	}
	else {
		meilleurScore=beta;
		for (int i=0;i<listeCoups.nb;i++)
		{
			
			copiestruc(jeu,&jeuTemporaire);
			jeuTemporaire=jouerCoup(jeuTemporaire,listeCoups.coups[i].origine,listeCoups.coups[i].destination);
			score=minimax(profondeur-1,jeuTemporaire,alpha,meilleurScore,1);
			if (meilleurScore > score)
				meilleurScore=score;
			if (meilleurScore <= alpha)
			{
				return alpha;
			}
		}
		return meilleurScore;
	}
	
}


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 
	octet myColor = currentPosition.trait;
	int index,i,score,max,MAX_DEPTH;
	T_Position jeuTemporaire;
	

	if (myColor==JAU)
	{
		max=1000000;
		MAX_DEPTH=2;
	}
	else
	{
		max=-1000000;
		MAX_DEPTH=3;
	}
	
	
	for (i=0;i<listeCoups.nb;i++)
	{
		if (myColor== JAU)
		{
			copiestruc(currentPosition,&jeuTemporaire);
			jeuTemporaire=jouerCoup(jeuTemporaire,listeCoups.coups[i].origine,listeCoups.coups[i].destination);
			score=minimax(MAX_DEPTH-1,jeuTemporaire,-10000,10000,1);
			if (score <= max)
			{
				max=score;
				index=i;
				ecrireIndexCoup(index);

			}
		}
		else
		{
			copiestruc(currentPosition,&jeuTemporaire);
			jeuTemporaire=jouerCoup(jeuTemporaire,listeCoups.coups[i].origine,listeCoups.coups[i].destination);
			score=minimax(MAX_DEPTH-1,jeuTemporaire,-10000,10000,0);
			if (score >= max)
			{
				max=score;
				index=i;
				ecrireIndexCoup(index);

			}
		}
	}
	ecrireIndexCoup(index);
	
}
