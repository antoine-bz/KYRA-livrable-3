/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

typedef struct 
{
int numcoup;
octet enfant;
octet parent;
int libelle;
int advr;
int ordi;
int rapport;
int testsupp;
} T_tableau;

typedef T_tableau T_TableauDeTableaux[ 8*NBCASES];

int estValideint(T_Position p, octet origine, octet destination);
//Fonction qui vérifie si le coup selectionné est valide
//Fonction identique à celle dans libavalam.c mais renvoie un int car sinon cela renvoyait des warnings, et sans les printf()
int marquerPoint(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int couleur);
//Fonction qui vérifie si des coups peuvent permettre de marquer des points.
//Si couleur est égal à 1, on vérifie les coups de notre couleur, sinon on vérifie les coups de la couleur adverse
void afficherTab(T_TableauDeTableaux tabCoups, int taille);
//Fonction qui affiche le contenu d'un tableau de coup
//--------------------------------------------------------------------------------------------
//Vérification des tests supplémentaires
//--------------------------------------------------------------------------------------------
//Fonctions de test supplementaire qu'on appelle lorsque l'on marque un point
int contrerAdvr1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
int pointPlusTard1(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int i);
int contrerOrdi1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi);
int donnerAdrv1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
int empiler1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i);
void calculerTestSupp1(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int k, int ordi, int advr);
//Fonctions de test supplementaire qu'on appelle lorsque l'on bloque un coup
int donnerOrdi2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi, int advr);
int pileQuatre2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
int contrerOrdi2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi);
int donnerAdrv2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
int contrerAdvr2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
void calculerTestSupp2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int k, int ordi, int advr);
//Fonctions de test supplementaire qu'on appelle lorsque l'on joue un coup qui ne marque ou ne bloque pas de point
int donnerOrdi3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi, int advr);
int donnerAdrv3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
int marquerOrdi3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i);
int empilerCouleur3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i);
int pileQuatre3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr);
void calculerTestSupp3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int k, int ordi, int advr);
//--------------------------------------------------------------------------------------------
void prevoirCoupAvance(T_Position currentPosition, T_TableauDeTableaux tabCoups, int k, int ordi, int advr);
//On simule les coups possibles et on calcule leur rapport pour savoir lequel est le meilleur
void classer(T_TableauDeTableaux tabCoups, int taille);
//On classe par ordre décroissant le tableau de coup en fonction de sa valeur testsupp
T_tableau trouverCoup(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int ordi, int advr);
//On cherche lequel des coups pouvant nous marquer un point est le meilleur selon plusieurs critères tels que leur libellé 
//et leur rapport
//--------------------------------------------------------------------------------------------
//Fonctions pour bloquer un coup de l'adversaire
//--------------------------------------------------------------------------------------------
int eloignerPile(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int pile, int k);
//On éloigne une pile de 2 adverse d'une pile de 3 adverse pour l'empêcher de faire une pile de 5
int fairePile3(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int pile, int k);
//On met un pion sur une pile de 2 adverse à côté d'une pile de 3 adverse pour l'empêcher de faire une pile de 5
int bloquerLib1(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int enfant, int parent, int k);
//On prend en compte tout les coups possibles pour bloquer un libellé 1, soit faire une pile de 5
int bloquerLib23(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int enfant, int parent, int k);
//On prend en compte tout les coups possibles pour bloquer un libellé 2 et 3, soit faire des piles isolées
T_tableau bloquerAdvr(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int ordi, int advr);
//On cherche le meilleur coup possible pour bloquer un coup adverse selon plusieurs critères tels que le libelle et le rapport
//--------------------------------------------------------------------------------------------
int coupAvantageux(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int ordi, int advr);
//On cherche un coup optimal quand on ne peut ni assurer un point ni bloquer l'adversaire.
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups);
//Fonction qui renvoie le coup à jouer


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonction qui vérifie si le coup selectionné est valide
//Fonction identique à celle dans libavalam.c mais renvoie un int car sinon cela renvoyait des warnings, et sans les printf()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int estValideint(T_Position p, octet origine, octet destination) {

	T_Voisins v; int i;


	// vérifie la légalité d'un coup 
	if (p.cols[origine].nb == VIDE) { 
		return FAUX;  
	} 

	if (p.cols[destination].nb == VIDE) { 
		return FAUX;  
	} 

	if (p.cols[origine].nb + p.cols[destination].nb > 5) {
		return FAUX;  		
	}

	// Il faut aussi vérifier accessibilité !!
	v= getVoisins(origine);
	for (i=0;i<v.nb;i++)
		if (v.cases[i]==destination) return VRAI;
	
	return FAUX;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonction qui vérifie si des coups peuvent permettre de marquer des points.
//Si couleur est égal à 1, on vérifie les coups de notre couleur, sinon on vérifie les coups de la couleur adverse
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int marquerPoint(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int couleur) //On cherche les coups qui peuvent marquer un point
{
	octet o, d;
	int i, flag = 0, ordi = 0, nbvoisins;
	octet myColor = currentPosition.trait; 
	T_Voisins voisins, voisins2;
	if (couleur==1) //On cherche ceux de notre couleur
	{
		for(i=0;i<listeCoups.nb; i++) {
			flag = 0;
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination;  

			if (currentPosition.cols[o].couleur == myColor) { //On ne prend en compte que les coups qui ont pour origine un pion de notre couleur


				if (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) //Si le coup fait une pile de 5, on le note dans le tableau de coup en tant que libelle 1
				{
					tabCoups[ordi].numcoup = i;
					tabCoups[ordi].enfant = o;
					tabCoups[ordi].parent = d;
					tabCoups[ordi].libelle = 1;
					tabCoups[ordi].advr = 0;
					tabCoups[ordi].ordi = 0;
					tabCoups[ordi].rapport = 0;
					tabCoups[ordi].testsupp = 0;
					ordi++; //On incrémente ordi qui compte le nombre de coup trouvé
				}
				else 
				{
					nbvoisins = 0;
					voisins = getVoisins(d);

					for (int j = 0; j < voisins.nb; j++)
					{
						if ( currentPosition.cols[voisins.cases[j]].nb!=0) nbvoisins++; //On compte le nombre de voisin non vide
					}


					if (nbvoisins == 1) //Si le pion destination n'a qu'un seul voisin, ce qui veut dire son seul voisin est le pion d'origine et que le déplacer nous donnerait un point. On note donc ce coup sous le libellé 3
					{
						voisins2 = getVoisins(o);
						for (int j = 0; j < voisins2.nb; ++j) //On regarde les voisins du pion d'origine
						{
							if (estValideint(currentPosition, o, voisins2.cases[j])==1) // Si le coup pion d'origine vers voisin du pion est valide, donc que le voisin n'est pas vide et que la somme des deux est inférieur ou égale à 5
							{
								voisins = getVoisins(voisins2.cases[j]); //On prend les voisins du voisin du pion d'origine
								for (int k = 0; k < voisins.nb; k++)
								{
									if ( estValideint(currentPosition, o, voisins2.cases[j])==1 && !(currentPosition.cols[o].nb + currentPosition.cols[voisins2.cases[j]].nb + currentPosition.cols[voisins.cases[j]].nb < 5) ) flag++; //Pour tout les voisins, on vérifie si lorsque l'on déplace le pion d'origine vers son voisin, cela ne créer pas de coup à prendre pour l'adversaire
								}
								if (estValideint(currentPosition, o, voisins2.cases[j])==1 && flag==0) //Si le coup du pion d'origine vers son voisin est valide et que cela ne créer pas de coup pour l'adversaire
								{
									tabCoups[ordi].enfant = o;
									tabCoups[ordi].parent = voisins2.cases[j];
									for( int l = 0; l < listeCoups.nb; l++) //On cherche le numéro de coup correspondant
									{
										o = listeCoups.coups[l].origine; 
										d = listeCoups.coups[l].destination;  
										if ((o == tabCoups[ordi].enfant) && (d == tabCoups[ordi].parent)) {
											tabCoups[ordi].numcoup = l;
											break; 
										}
									}
									tabCoups[ordi].libelle = 3;
									tabCoups[ordi].advr = 0;
									tabCoups[ordi].ordi = 0;
									tabCoups[ordi].rapport = 0;
									tabCoups[ordi].testsupp = 0;
									ordi++; //On incrémente ordi qui compte le nombre de coup trouvé
								}
							}
						}
					}

					else //Si le pion de destination a plusieurs voisins
					{
						for (int j = 0; j < voisins.nb; j++)
						{
							if ( o != voisins.cases[j] && currentPosition.cols[voisins.cases[j]].nb!=0 && !(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[voisins.cases[j]].nb > 5) ) flag++; //On vérifie s'il y a des coups valides parmis les coups qui vont du pion de destination vers ses voisins, s'il n'y en a pas flag est égal à 0
						}
						
						if (flag==0) //S'il n'y a pas de coup valide parmis les coups qui vont du pion de destination vers ses voisins autre que la destination vers l'origine, alors on note ce coup dans le tableau sous le libellé 2
						{
							tabCoups[ordi].numcoup = i;
							tabCoups[ordi].enfant = o;
							tabCoups[ordi].parent = d;
							tabCoups[ordi].libelle = 2;
							tabCoups[ordi].advr = 0;
							tabCoups[ordi].ordi = 0;
							tabCoups[ordi].rapport = 0;
							tabCoups[ordi].testsupp = 0;
							ordi++; //On incrémente ordi qui compte le nombre de coup trouvé
						}
					}
				}
			}
		}
		return ordi;
	}
	else //On fait les mêmes vérifications mais pour les pions de la couleur adverse
	{
		for(i=0;i<listeCoups.nb; i++) {
			flag = 0;
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination;  

			if (currentPosition.cols[o].couleur != myColor) {


				if (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) 
				{
					tabCoups[ordi].numcoup = i;
					tabCoups[ordi].enfant = o;
					tabCoups[ordi].parent = d;
					tabCoups[ordi].libelle = 1;
					tabCoups[ordi].advr = 0;
					tabCoups[ordi].ordi = 0;
					tabCoups[ordi].rapport = 0;
					tabCoups[ordi].testsupp = 0;
					ordi++;
				}
				else 
				{
					nbvoisins = 0;
					voisins = getVoisins(d);

					for (int j = 0; j < voisins.nb; j++)
					{
						if ( currentPosition.cols[voisins.cases[j]].nb!=0) nbvoisins++;
					}


					if (nbvoisins == 1)
					{
						voisins2 = getVoisins(o);
						for (int j = 0; j < voisins2.nb; ++j)							
						{
							if (estValideint(currentPosition, o, voisins2.cases[j])==1)
							{
								voisins = getVoisins(voisins2.cases[j]);
								for (int k = 0; k < voisins.nb; k++)
								{
									if ( estValideint(currentPosition, o, voisins2.cases[j])==1 && !(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[voisins.cases[j]].nb < 5) ) flag++;
								}
								if (estValideint(currentPosition, o, voisins2.cases[j])==1 && flag==0)
								{
									tabCoups[ordi].enfant = o;
									tabCoups[ordi].parent = voisins2.cases[j];
									for( int l = 0; l < listeCoups.nb; l++) 
									{
										o = listeCoups.coups[l].origine; 
										d = listeCoups.coups[l].destination;  
										if ((o == tabCoups[ordi].enfant) && (d == tabCoups[ordi].parent)) {
											tabCoups[ordi].numcoup = l;
											break; 
										}
									}
									tabCoups[ordi].libelle = 3;
									tabCoups[ordi].advr = 0;
									tabCoups[ordi].ordi = 0;
									tabCoups[ordi].rapport = 0;
									tabCoups[ordi].testsupp = 0;
									ordi++;
								}
							}
						}
					}

					else
					{
						for (int j = 0; j < voisins.nb; j++)
						{
							if ( o != voisins.cases[j] && currentPosition.cols[voisins.cases[j]].nb!=0 && !(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[voisins.cases[j]].nb > 5) ) flag++; 
						}
						
						if (flag==0)
						{
							tabCoups[ordi].numcoup = i;
							tabCoups[ordi].enfant = o;
							tabCoups[ordi].parent = d;
							tabCoups[ordi].libelle = 2;
							tabCoups[ordi].advr = 0;
							tabCoups[ordi].ordi = 0;
							tabCoups[ordi].rapport = 0;
							tabCoups[ordi].testsupp = 0;
							ordi++;
						}
					}
				}
			}
		}
		return ordi;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonction qui affiche le contenu d'un tableau de coup
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void afficherTab(T_TableauDeTableaux tabCoups, int taille)
{
	for (int i = 0; i < taille; ++i)
	{
		for (int i = 0; i < taille+1; ++i)
		{
			printf("Position %d : coup numéro %d rapport : %d testsupp : %d\n", i, tabCoups[i].numcoup, tabCoups[i].rapport, tabCoups[i].testsupp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Vérification des tests supplémentaires
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonctions de test supplementaire qu'on appelle lorsque l'on marque un point
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int contrerAdvr1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
   if (tabCoups[i].advr - advr < 0) return 2;
   else return 0;
}

	// On vérifie s'il est possible de prendre un point plus tard sans risque que l'adversaire le prenne p.31
int pointPlusTard1(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int i)
{ // A refaire
	octet o, d, myColor = currentPosition.trait;
	T_TableauDeTableaux tabTemp;
	o = tabCoups[i].enfant; 
	d = tabCoups[i].parent;
	if ( (currentPosition.cols[o].couleur == myColor) && currentPosition.cols[d].couleur == myColor	&& (currentPosition.cols[d].nb+currentPosition.cols[o].nb == 5) 
		&& bloquerLib1(currentPosition, listeCoups, tabTemp, o, d, 0) == 0 )
	{
		return -999;
	}
	else return 0;
}

int contrerOrdi1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi)
{
   if (tabCoups[i].ordi - ordi + 1 < 0) return -2;
   else return 0;
}

	// On vérifie si ce coup permet à l'adversaire peut gagner un point p.33
int donnerAdrv1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
	if (tabCoups[i].advr - advr > 0) return -4;
	else return 0;
}

	// Si on empile sur un pion de notre couleur p.34
int empiler1(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i)
{
	octet myColor = currentPosition.trait;
	if (currentPosition.cols[tabCoups[i].parent].couleur == myColor) return -1;
	else return 0;
}

void calculerTestSupp1(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int k, int ordi, int advr)
{
	for (int i = 0; i < k; ++i)
	{
		tabCoups[i].testsupp = tabCoups[i].testsupp + contrerAdvr1(currentPosition, tabCoups, i, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + pointPlusTard1(currentPosition, listeCoups, tabCoups, i);
		tabCoups[i].testsupp = tabCoups[i].testsupp + contrerOrdi1(currentPosition, tabCoups, i, ordi);
		tabCoups[i].testsupp = tabCoups[i].testsupp + donnerAdrv1(currentPosition, tabCoups, i, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + empiler1(currentPosition, tabCoups, i);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonctions de test supplementaire qu'on appelle lorsque l'on bloque un coup
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int donnerOrdi2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi, int advr)
{
	if (tabCoups[i].ordi - ordi > 0 && tabCoups[i].advr==advr) return 4;
	else return 0;
}

	// Si on fait une pile de 4 p.39
int pileQuatre2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
	octet o = tabCoups[i].enfant, d = tabCoups[i].parent, myColor = currentPosition.trait;
	if (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 4 && !(tabCoups[i].advr == advr)) return -999;
	else return 0;
}

	//p.38
int contrerOrdi2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi)
{
   if (tabCoups[i].ordi - ordi < 0) return -4;
   else return 0;
}

int donnerAdrv2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
	if (tabCoups[i].advr - advr > 0) return -4;
	else return 0;
}

int contrerAdvr2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
   if (tabCoups[i].advr - advr < 0) return 4;
   else return 0;
}

void calculerTestSupp2(T_Position currentPosition, T_TableauDeTableaux tabCoups, int k, int ordi, int advr)
{
	for (int i = 0; i < k; ++i)
	{
		tabCoups[i].testsupp = tabCoups[i].testsupp + donnerOrdi2(currentPosition, tabCoups, i, ordi, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + pileQuatre2(currentPosition, tabCoups, i, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + contrerOrdi2(currentPosition, tabCoups, i, ordi);
		tabCoups[i].testsupp = tabCoups[i].testsupp + donnerAdrv2(currentPosition, tabCoups, i, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + contrerAdvr2(currentPosition, tabCoups, i, advr);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonctions de test supplementaire qu'on appelle lorsque l'on joue un coup qui ne marque ou ne bloque pas de point
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int donnerOrdi3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int ordi, int advr)
{
	if (tabCoups[i].ordi - ordi > 0 && tabCoups[i].advr==advr) return +10;
	else return 0;
}

int donnerAdrv3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
	if (tabCoups[i].advr - advr > 0) return -20;
	else return 0;
}

int marquerOrdi3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i)
{
	if(currentPosition.cols[tabCoups[i].enfant].nb+currentPosition.cols[tabCoups[i].parent].nb == 5) return -1;
	else return 0;
}

int empilerCouleur3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i)
{
	octet o = tabCoups[i].enfant, d = tabCoups[i].parent, myColor = currentPosition.trait;
	if (currentPosition.cols[o].couleur != myColor && currentPosition.cols[d].couleur != myColor) return +6;
	if (currentPosition.cols[o].couleur == myColor && currentPosition.cols[d].couleur == myColor) return -4;
	if (currentPosition.cols[o].couleur != myColor && currentPosition.cols[d].couleur == myColor) return -8;
	if (currentPosition.cols[o].couleur == myColor && currentPosition.cols[d].couleur != myColor) return +4;
}

int pileQuatre3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int i, int advr)
{
	octet o = tabCoups[i].enfant, d = tabCoups[i].parent, myColor = currentPosition.trait;
	if (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 4 && !(tabCoups[i].advr == advr)) return -999;
	else return 0;
}

void calculerTestSupp3(T_Position currentPosition, T_TableauDeTableaux tabCoups, int k, int ordi, int advr)
{
	for (int i = 0; i < k; ++i)
	{
		tabCoups[i].testsupp = tabCoups[i].testsupp + donnerOrdi3(currentPosition, tabCoups, i, ordi, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + donnerAdrv3(currentPosition, tabCoups, i, advr);
		tabCoups[i].testsupp = tabCoups[i].testsupp + marquerOrdi3(currentPosition, tabCoups, i);
		tabCoups[i].testsupp = tabCoups[i].testsupp + empilerCouleur3(currentPosition, tabCoups, i);
		tabCoups[i].testsupp = tabCoups[i].testsupp + pileQuatre3(currentPosition, tabCoups, i, advr);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On simule les coups possibles et on calcule leur rapport pour savoir lequel est le meilleur
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void prevoirCoupAvance(T_Position currentPosition, T_TableauDeTableaux tabCoups, int k, int ordi, int advr) //On prévoit un coup à l'avance
{
	T_Position coupAvance;
	T_ListeCoups listeCoups;
	T_TableauDeTableaux testAvance;
	octet origine, destination;
	for (int i = 0; i < k; ++i) //Pour tout les coups possibles
	{
		coupAvance = currentPosition; //On créer un autre T_Position et on lui donne la valeur de la position actuelle
		origine = tabCoups[i].enfant; 
		destination = tabCoups[i].parent;  
		coupAvance.cols[destination].nb += coupAvance.cols[origine].nb; 			//On rajoute le nombre de pion de la colonne d'origine dans la colonne destination dans la position simulée
		coupAvance.cols[destination].couleur = coupAvance.cols[origine].couleur; 	//On met la couleur du pion de la colonne d'origine sur la colonne destination dans la position simulée
		coupAvance.cols[origine].nb = 0; 											//On vide la colonne d'origine dans la position simulée
		coupAvance.cols[origine].couleur = 0;										//On retire la couleur de la colonne d'origine dans la position simulée
		listeCoups = getCoupsLegaux(coupAvance); 									//On cherche les coups possibles de la position simulée
		tabCoups[i].ordi = marquerPoint(coupAvance, listeCoups, testAvance, 1); 	//On calcule ordi de la position simulée
		tabCoups[i].advr = marquerPoint(coupAvance, listeCoups, testAvance, 0);		//On calcule advr de la position simulée
		tabCoups[i].rapport = (tabCoups[i].ordi - ordi) - (tabCoups[i].advr - advr); //On calcule le rapport
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On classe par ordre décroissant le tableau de coup en fonction de sa valeur rapport
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void classerrapport(T_TableauDeTableaux tabCoups, int taille) //On classe en fonction de testsupp
{
	T_tableau tabTemps;
	int i, j;
	for(i=0;i<taille;i++)
	{
		for(j=0;j<taille-1;j++)
		{
		if(tabCoups[j].rapport<tabCoups[j+1].rapport)
			{
				tabTemps = tabCoups[j];
				tabCoups[j] = tabCoups[j+1];
				tabCoups[j+1] = tabTemps;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On classe par ordre décroissant le tableau de coup en fonction de sa valeur testsupp
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void classertestsupp(T_TableauDeTableaux tabCoups, int taille) //On classe en fonction de testsupp
{
	T_tableau tabTemps;
	int i, j;
	for(i=0;i<taille;i++)
	{
		for(j=0;j<taille-1;j++)
		{
		if(tabCoups[j].testsupp<tabCoups[j+1].testsupp)
			{
				tabTemps = tabCoups[j];
				tabCoups[j] = tabCoups[j+1];
				tabCoups[j+1] = tabTemps;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On cherche lequel des coups pouvant nous marquer un point est le meilleur selon plusieurs critères tels que leur libellé 
//et leur rapport
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

T_tableau trouverCoup(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int ordi, int advr) //On cherche le meilleur coup possible pou marquer un point
{

	T_TableauDeTableaux resultTab, tabTemp;
	int nbtrouve = 0, k = 0, l = 0;
	if(ordi>2)
	{
		for (int i = 0; i < NBCASES; ++i) // On vérifie si une case revient plus de trois fois
		{
			nbtrouve=0;
			for (int j = 0; j < ordi; ++j)
			{
				if (i==tabCoups[j].enfant || i==tabCoups[j].parent) //On vérifie si la case intervient dans un coup en tant qu'enfant ou parent
				{
					tabTemp[nbtrouve]=tabCoups[j]; //On retient où la case apparaît
					nbtrouve++; //On retient le nombre de fois où la case apparaît
				}
			}
			if (nbtrouve>2) //Si on trouve la case au moins 3 fois on la retient dans le tableau résultat
			{
				for (int j = 0; j < nbtrouve; ++j)
				{
					resultTab[k] = tabTemp[j]; // On note les coups trouvés dans le tableau résultat
					k++; //Le nombre de coups que l'on a trouvé
				}
			}
		}
		if (k!=0) //Si des cases sont citées plus de trois fois
		{
			prevoirCoupAvance(currentPosition, resultTab, k, ordi, advr); //On fait une simulation un coup en avance
			calculerTestSupp1(currentPosition, listeCoups, resultTab, k, ordi, advr);
			classertestsupp(resultTab, k); 										  //On classe les coups en fonction du rapport ordi advr
			return resultTab[0]; //On renvoie le premier coup du tableau, soit celui qui a la plus haute valeur testsupp, pour le jouer
		}
	}
	nbtrouve=0;
	for (int i = 0; i < ordi; ++i) //On vérifie tout les coups trouvés dans le tableau de coups
	{
		if (tabCoups[i].libelle==1) // On vérifie si le coup fait une pile de 5
		{
			resultTab[nbtrouve]=tabCoups[i]; // On range le coup dans un nouveau tableau de coup
			nbtrouve++; //On compte le nombre de coups trouvés
		}
	}
	if (nbtrouve==1) //Si un seul coup fait une pile de 5
	{
		prevoirCoupAvance(currentPosition, resultTab, k, ordi, advr); //On fait une simulation un coup en avance
		calculerTestSupp1(currentPosition, listeCoups, resultTab, k, ordi, advr);
		if (resultTab[0].testsupp < -800)
		{
			nbtrouve = 0; //On met nbtrouve à 0 pour passer aux libellés 2 et 3
		}
		else return resultTab[0]; //On la joue
	}
	if (nbtrouve>1) //Si plusieurs coups font une pile de 5
	{
		prevoirCoupAvance(currentPosition, resultTab, nbtrouve, ordi, advr); //On fait une simulation d'un coup en avance
		calculerTestSupp1(currentPosition, listeCoups, resultTab, k, ordi, advr);
		classertestsupp(resultTab, nbtrouve); 										//On classe les coups en fonction du rapport ordi advr
		if (resultTab[0].testsupp < -800)
		{
			nbtrouve = 0; //On met nbtrouve à 0 pour passer aux libellés 2 et 3
		}
		return resultTab[0]; //On renvoie le premier coup du tableau, soit celui qui a la plus haute valeur testsupp, pour le jouer
	}
	if (nbtrouve==0) //Si aucun pion ne fait de pile de 5, on utilise les libellé 2 et 3
	{
		prevoirCoupAvance(currentPosition, tabCoups, nbtrouve, ordi, advr); //On fait une simulation d'un coup en avance
		calculerTestSupp1(currentPosition, listeCoups, resultTab, k, ordi, advr);
		classertestsupp(tabCoups, nbtrouve); //On classe les coups en fonction du rapport ordi advr
		return tabCoups[0]; //On renvoie le premier coup du tableau, soit celui qui a la plus haute valeur testsupp, pour le jouer
	}
	return resultTab[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonctions pour bloquer un coup de l'adversaire
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On éloigne une pile de 2 adverse d'une pile de 3 adverse pour l'empêcher de faire une pile de 5
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int eloignerPile(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int pile, int k)
{
	T_Voisins voisins, voisins2;
	voisins = getVoisins(pile);
	octet o, d;
	int flag = 0;
	for (int i = 0; i < voisins.nb; ++i) //Pour tout les voisins de la pile
	{
		voisins2 = getVoisins(voisins.cases[i]);
		if (currentPosition.cols[voisins.cases[i]].nb==1) //Si le voisin est une pile de 1
		{
			for (int j = 0; j < voisins2.nb; ++j) //On parcours les voisins du voisin
			{
				if (currentPosition.cols[voisins2.cases[i]].nb==2) flag++; //Si un voisin est une pile de 2, on incrémente flag
			}
			if (flag==0) //Si aucun voisin de la position qu'on vérifie n'est une pile de 2, alors on déplace la pile de 2 voisine de la pile de 3 dessus, ce qui bloque le coup de l'adversaire
			{
				for( int l = 0; l < listeCoups.nb; l++) //On cherche le numéro de coup correspondant à l'origine et la destination
				{
					o = listeCoups.coups[l].origine; 
					d = listeCoups.coups[l].destination;
					if ((o == pile) && (d == voisins.cases[i])) {
						tabCoups[k].numcoup = l;
						break; 
					}
				}
				tabCoups[k].enfant = pile;
				tabCoups[k].parent = voisins.cases[i];
				tabCoups[k].libelle = 1; //On ne se sert plus du libelle, on met donc une valeur égale à 1 pour vérifier que ce coup a bien été selectionné et que ce n'est pas un coup vide par défaut
				tabCoups[k].advr = 0;
				tabCoups[k].ordi = 0;
				tabCoups[k].rapport = 0;
				tabCoups[k].testsupp = 0;
				k++; //On compte le nombre de coup rajouté dans le tableau
			}
		}
	}
	return k;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On met un pion sur une pile de 2 adverse à côté d'une pile de 3 adverse pour l'empêcher de faire une pile de 5
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fairePile3(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int pile, int k)
{
	T_Voisins voisins, voisins2;
	voisins = getVoisins(pile);
	int flag = 0;
	octet o, d;
	for (int i = 0; i < voisins.nb; ++i) //Pour tout les voisins de la pile
	{
		if (currentPosition.cols[voisins.cases[i]].nb==2) flag++; //Si un voisin est une pile de 2, on incrémente flag
	}
	if (flag==0) //Si aucun voisin n'est une pile de 2, alors on déplace la pile de 2 voisine de la pile de 3 dessus, ce qui bloque le coup de l'adversaire
	{
		for (int i = 0; i < voisins.nb; ++i) //Si aucun voisin de la position qu'on vérifie n'est une pile de 2, alors on met un pion sur la pile de 2, ce qui bloque le coup de l'adversaire
		{
			if (currentPosition.cols[voisins.cases[i]].nb==1)
			{
				for( int l = 0; l < listeCoups.nb; l++) 
					{
						o = listeCoups.coups[l].origine; 
						d = listeCoups.coups[l].destination;  
						if ((o == voisins.cases[i]) && (d == pile)) {
							tabCoups[k].numcoup = l;
							break; 
						}
					}
					tabCoups[k].enfant = pile;
					tabCoups[k].parent = voisins.cases[i];
					tabCoups[k].libelle = 1; //On ne se sert plus du libelle, on met donc une valeur égale à 1 pour vérifier que ce coup a bien été selectionné et que ce n'est pas un coup vide par défaut
					tabCoups[k].advr = 0;
					tabCoups[k].ordi = 0;
					tabCoups[k].rapport = 0;
					tabCoups[k].testsupp = 0;
					k++;
			}
		}
	}
	return k;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On prend en compte tout les coups possibles pour bloquer un libellé 1, soit faire une pile de 5
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int bloquerLib1(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int enfant, int parent, int k)
{
	if (currentPosition.cols[enfant].nb==2) //Si c'est le pile enfant qui est une pile de deux, on lance les fonctions sur la pile enfant
	{
		k = fairePile3(currentPosition, listeCoups, tabCoups, enfant, k);
		k = eloignerPile(currentPosition, listeCoups, tabCoups, enfant, k);
	
	}
	if (currentPosition.cols[parent].nb==2) //Si c'est le pile parent qui est une pile de deux, on lance les fonctions sur la pile parent
	{
		k = fairePile3(currentPosition, listeCoups, tabCoups, parent, k);
		k = eloignerPile(currentPosition, listeCoups, tabCoups, parent, k);
	}
	return k;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On prend en compte tout les coups possibles pour bloquer un libellé 2 et 3, soit faire des piles isolées
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int bloquerLib23(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int enfant, int parent, int k)
{
	octet o, d;
	for (int i = 0; i < listeCoups.nb; ++i) //Pour tout les coups, on cherche celui qui déplace le pion qui peut être isolé et le met sur le pion à côté
		{
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination;
			if ((o == parent) && (d == enfant)) {
				tabCoups[k].numcoup = i;
				break; 
			}
		}
		tabCoups[k].enfant = parent;
		tabCoups[k].parent = enfant;
		tabCoups[k].libelle = 1; //On ne se sert plus du libelle, on met donc une valeur égale à 1 pour vérifier que ce coup a bien été selectionné et que ce n'est pas un coup vide par défaut
		tabCoups[k].advr = 0;
		tabCoups[k].ordi = 0;
		tabCoups[k].rapport = 0;
		tabCoups[k].testsupp = 0;
		k++;
	return k;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On cherche le meilleur coup possible pour bloquer un coup adverse selon plusieurs critères tels que le libelle et le rapport
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

T_tableau bloquerAdvr(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int ordi, int advr) //On cherche le meilleur coup possible pou marquer un point
{
	T_TableauDeTableaux resultTab, tabTemp, tabBlock;
	int nbtrouve = 0, k = 0, l = 0;
	if(advr>2)
	{
		for (int i = 0; i < NBCASES; ++i) // On vérifie si une case revient plus de trois fois
		{
			nbtrouve=0;
			for (int j = 0; j < advr; ++j)
			{
				if (i==tabCoups[j].enfant || i==tabCoups[j].parent) //On vérifie si la case intervient dans un coup en tant qu'enfant ou parent
				{
					tabTemp[nbtrouve]=tabCoups[j]; //On retient où la case apparaît
					nbtrouve++; //On retient le nombre de fois où la case apparaît
				}
			}
			if (nbtrouve>2) //Si on trouve la case au moins 3 fois on la retient dans le tableau résultat
			{
				for (int j = 0; j < nbtrouve; ++j)
				{
					resultTab[k] = tabTemp[j]; // On note les coups trouvés dans le tableau résultat
					k++; //Le nombre de coups que l'on a trouvé
				}
			}
		}
		if (k!=0) //Si au moins une case est citée au moins trois fois
		{	
			nbtrouve=k;
			k=0;
			for (int i = 0; i < nbtrouve; ++i) //On cherche les coups qu'ils permettent en fonction de leur libelle
			{
				if (resultTab[i].libelle==1) k = bloquerLib1(currentPosition, listeCoups, tabBlock, resultTab[0].enfant, resultTab[0].parent, k);
				else if(currentPosition.cols[tabCoups[i].enfant].nb + currentPosition.cols[tabCoups[i].parent].nb <4) k = bloquerLib23(currentPosition, listeCoups, tabBlock, resultTab[0].enfant, resultTab[0].parent, k);
			}
			prevoirCoupAvance(currentPosition, tabBlock, k, advr, advr); //On fait une simulation d'un coup en avance
			calculerTestSupp2(currentPosition, resultTab, k, ordi, advr);
			// afficherTab(tabBlock, k);
			classertestsupp(tabBlock, k); //On classe les coups en fonction du rapport ordi advr
			// afficherTab(tabBlock, k);		
			return tabBlock[0]; //On renvoie le premier coup du tableau, soit celui qui a le plus haut rapport ordi advr, pour le jouer
		}
	}
	nbtrouve=0;
	for (int i = 0; i < advr; ++i) // On vérifie si un coup fait une pile de 5
	{
		if (tabCoups[i].libelle==1)
		{
			resultTab[nbtrouve]=tabCoups[i];
			nbtrouve++;
		}
	}
	if (nbtrouve==1) //Si un seul coup fait une pile de 5
	{
		k = bloquerLib1(currentPosition, listeCoups, tabBlock, resultTab[0].enfant, resultTab[0].parent, k); //On regarde les 
		prevoirCoupAvance(currentPosition, tabBlock, k, ordi, advr); //On fait une simulation d'un coup en avance
		calculerTestSupp2(currentPosition, resultTab, k, ordi, advr);
		classertestsupp(tabBlock, k); //On classe les coups en fonction du rapport ordi advr
		// afficherTab(tabBlock, k);
		if (tabBlock[0].testsupp < -800 || k==0)
		{
			nbtrouve=0;
		}
		return tabBlock[0]; //On renvoie le premier coup du tableau, soit celui qui a le plus haut rapport ordi advr, pour le jouer
	}
	if (nbtrouve>1)
	{
		for (int i = 0; i < nbtrouve; ++i)
		{
			k = bloquerLib1(currentPosition, listeCoups, tabBlock, resultTab[i].enfant, resultTab[i].parent, k);
		}
		prevoirCoupAvance(currentPosition, tabBlock, k, ordi, advr); //On fait une simulation un coup en avance
		calculerTestSupp2(currentPosition, resultTab, k, ordi, advr);
		// afficherTab(tabBlock, k);
		classertestsupp(tabBlock, k); //On classe les coups en fonction du rapport ordi advr
		// afficherTab(tabBlock, k);
		if (tabBlock[0].testsupp < -800 || k==0)
		{
			nbtrouve=0;
		}
		return tabBlock[0]; //On renvoie le numéro de coup du premier coup du tableau, soit celui qui a le plus haut rapport ordi advr, pour le jouer
	}
	if (nbtrouve==0)
	{
		for (int i = 0; i < advr; ++i)
		{
			if(currentPosition.cols[tabCoups[i].enfant].nb + currentPosition.cols[tabCoups[i].parent].nb <4) {
						k = bloquerLib23(currentPosition, listeCoups, tabBlock, tabCoups[i].enfant, tabCoups[i].parent, k);}
		}
		prevoirCoupAvance(currentPosition, tabBlock, k, ordi, advr); //On fait une simulation un coup en avance
		calculerTestSupp2(currentPosition, resultTab, k, ordi, advr);
		// afficherTab(tabBlock, k);
		classertestsupp(tabBlock, k); //On classe les coups en fonction du rapport ordi advr
		// afficherTab(tabBlock, k);
		return tabBlock[0]; //On renvoie le premier coup du tableau, soit celui qui a le plus haut rapport ordi advr, pour le jouer
	}
	return tabBlock[0];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//On cherche un coup optimal quand on ne peut ni assurer un point ni bloquer l'adversaire.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int coupAvantageux(T_Position currentPosition, T_ListeCoups listeCoups, T_TableauDeTableaux tabCoups, int ordi, int advr)
{	int coupjoue, flag = 0, k = 0, i = 0;
	int numtab, nbvoisins, nbvoisinsmin = 8, nbvoisinsmax = 0;
	octet o, d; 
	octet myColor = currentPosition.trait; 
	T_Voisins voisin;

	for(i=0;i<listeCoups.nb; i++) 
	{
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		tabCoups[i].numcoup = i;
		tabCoups[i].enfant = o;
		tabCoups[i].parent = d;
		tabCoups[i].libelle = 0;
		tabCoups[i].advr = 0;
		tabCoups[i].ordi = 0;
		tabCoups[i].rapport = 0;
		tabCoups[i].testsupp = 0;
	}
	prevoirCoupAvance(currentPosition, tabCoups, i, ordi, advr); //On fait une simulation un coup en avance
	calculerTestSupp3(currentPosition, tabCoups, i, ordi, advr);
	classertestsupp(tabCoups, i);
	return tabCoups[0].numcoup;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Fonction qui renvoie le coup à jouer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int ordi, advr, coupjoue = -1, i = 0; 
	octet o, d; 
	T_TableauDeTableaux tabCoups, tabCoupsAdvr;
	T_tableau coupAJouer;

	// afficherListeCoups(listeCoups);

	// Si je peux gagner une colonne, je la prends 

	advr = marquerPoint(currentPosition, listeCoups, tabCoupsAdvr, 0);
	ordi = marquerPoint(currentPosition, listeCoups, tabCoups, 1);

	if (ordi == 1)
	{
		prevoirCoupAvance(currentPosition, tabCoups, 1, ordi, advr); //On fait une simulation d'un coup en avance
		calculerTestSupp1(currentPosition, listeCoups, tabCoups, 1, ordi, advr);
		if (tabCoups[0].testsupp<-800)
		{
			coupjoue = -1;
		}
	}
	if (ordi > 1)
	{
		coupAJouer = trouverCoup(currentPosition, listeCoups, tabCoups, ordi, advr);
		if (coupAJouer.testsupp < -800)
		{
			coupjoue = -1;
		}
		else coupjoue = coupAJouer.numcoup;
	}
	if (ordi==0)
	{
		if (advr != 0)
		{
			coupAJouer = bloquerAdvr(currentPosition, listeCoups, tabCoupsAdvr, ordi, advr);//On trouve le meilleur coup à jouer pour bloquer l'adversaire
			if (coupAJouer.testsupp < -800 || coupAJouer.libelle == 0) coupjoue = -1;
			else coupjoue = coupAJouer.numcoup;
		}
	}


	if (coupjoue == -1)
	{
		coupjoue = coupAvantageux(currentPosition, listeCoups, tabCoups, ordi, advr);
		ecrireIndexCoup(coupjoue);
	}
	else ecrireIndexCoup(coupjoue);

}