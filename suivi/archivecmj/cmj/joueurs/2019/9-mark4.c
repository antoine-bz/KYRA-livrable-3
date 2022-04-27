/********* Moteur de tournoi : joueur ***************/
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

int isole(T_Position laPosition, octet laDestination, octet lOrigine);
int pasDebile(T_Position laPosition, octet laDestination, int nbChezLesVoisins);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{
int i, j, coupAJouer;
octet o, d, oTemp, dTemp;
octet myColor = currentPosition.trait;
T_Voisins lesVoisins;

printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
for(i=0;i<listeCoups.nb; i++){ //----------------- FOR -----------------------------

	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination; 

	printf("Coup %d : ", i); 
	printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
	printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur));

	//On trouve le coup à la plus haute priorité
	//DEBUT DU PROCESSUS

	


	//origine J1V? et destination A4V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 4)){ //pas besoin de verif l'origine
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}
	//origine J4V? et destination A1V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 4) && (currentPosition.cols[d].couleur != myColor)){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}


	//origine J2V? et destination A3V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 3)){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}
	//origine J3V? et destination A2V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 3) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 2)){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}

	//origine J?V? et destination A?V? //verif qu'il y a que des cases dont somme ori+dest>5
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && isole(currentPosition, d, o)==VRAI){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}
	//origine J?V? et destination #?V? //verif qu'il y a que des cases dont somme ori+dest>5
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && isole(currentPosition, d, o)==VRAI){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}

	//FAUDRA VERIF LES COUPS DE L'ADV

	//contre-coup si origine A?V? et destination J?V? && A?+J?==5
	for(j=0;j<listeCoups.nb; j++){
		if(currentPosition.cols[o].couleur != myColor && currentPosition.cols[d].couleur == myColor && currentPosition.cols[o].nb+currentPosition.cols[d].nb==5){
			dTemp=o;
			oTemp=d;
			for(j=0;j<listeCoups.nb; j++){
				if(listeCoups.coups[j].origine==oTemp && listeCoups.coups[j].destination==dTemp){
					coupAJouer=j;
					goto jouerLeCoup;
				}
			}
		}
	}



	//QUAND ON SAIT PAS QUOI FAIRE
	//origine J1V? et destination A1V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 1) && pasDebile(currentPosition, d, 3)==VRAI){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}
	//origine J1V? et destination A2V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 2) && pasDebile(currentPosition, d, 2)==VRAI){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}
	//origine J1V? et destination A3V?
	for(j=0;j<listeCoups.nb; j++){
		if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 3) && pasDebile(currentPosition, d, 1)==VRAI){
			coupAJouer=i;
			goto jouerLeCoup;
		}
	}
	
}// -------------- FIN DU FOR ------------------ on a alors choisi le coup à jouer
jouerLeCoup:

o = listeCoups.coups[coupAJouer].origine;
d = listeCoups.coups[coupAJouer].destination; 

printf("Coup %d : ", coupAJouer); 
printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur));

printf("On choisit ce coup !");

ecrireIndexCoup(coupAJouer);
}// -------------------------------------------- FIN FONCTION --------------------------------------------



int isole(T_Position laPosition, octet laDestination, octet lOrigine)
{
int i;
T_Voisins lesVoisins;

lesVoisins=getVoisins(laDestination);

for(i=0; i<lesVoisins.nb; i++){ //i<nb de cases voisines (dont les cases vides)
	if(laPosition.cols[lesVoisins.cases[i]].nb + laPosition.cols[laDestination].nb + laPosition.cols[lOrigine].nb <= 5 && laPosition.cols[lesVoisins.cases[i]].couleur!=laPosition.trait){//verif que o+d+v <=5
		return FAUX;
	}
}
return VRAI;
}



int pasDebile(T_Position laPosition, octet laDestination, int nbChezLesVoisins)
{
int i;
T_Voisins lesVoisins;

lesVoisins=getVoisins(laDestination);

for(i=0; i<lesVoisins.nb; i++){ //i<nb de cases voisines (dont les cases vides)
	if(laPosition.cols[lesVoisins.cases[i]].nb==nbChezLesVoisins && laPosition.cols[lesVoisins.cases[i]].couleur!=laPosition.trait){//verif que o+d+v ==5
		return FAUX;
	}
}
return VRAI;
}