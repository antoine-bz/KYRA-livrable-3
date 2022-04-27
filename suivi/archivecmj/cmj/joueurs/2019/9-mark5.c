/********* Moteur de tournoi : joueur ***************/
#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


int onSIsole(T_Position laPosition, octet laDestination, octet lOrigine);
int pasDebile(T_Position laPosition, octet laDestination, int nbChezLesVoisins);


//______________________________________________________________________________________________________________________________________________________________________________________________________________
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) 
{
int i, j, coupAJouer;
octet o, d, oTemp, dTemp;
octet myColor = currentPosition.trait;
T_Voisins lesVoisins;

printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
//*************************************************************************************************************************************************************// COUPS EVIDENTS
for(i=0;i<listeCoups.nb; i++){

	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination;

	//origine J1V? et destination A4V?
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 4)){ //pas besoin de verif l'origine
		coupAJouer=i;
		goto jouerLeCoup;
	}
	//origine J4V? et destination A1V?
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 4) && (currentPosition.cols[d].couleur != myColor)){
		coupAJouer=i;
		goto jouerLeCoup;
	}
	//origine J2V? et destination A3V?
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 3)){
		coupAJouer=i;
		goto jouerLeCoup;
	}
	//origine J3V? et destination A2V?
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 3) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 2)){
		coupAJouer=i;
		goto jouerLeCoup;
	}
}
//*************************************************************************************************************************************************************// FIN



//*************************************************************************************************************************************************************// ISOLEMENT
for(i=0;i<listeCoups.nb; i++){

	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination;
	lesVoisins=getVoisins(o);

	//origine J?V? et destination A?V? //verif qu'il y a que des cases dont somme ori+dest>5
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[d].couleur != myColor) && onSIsole(currentPosition, d, o)==VRAI){
		coupAJouer=i;
		goto jouerLeCoup;
	}
	//origine J?V? et destination #?V? //verif qu'il y a que des cases dont somme ori+dest>5
	if((currentPosition.cols[o].couleur == myColor) && onSIsole(currentPosition, d, o)==VRAI){
		coupAJouer=i;
		goto jouerLeCoup;
	}/*
	//origine J?V1 et destination #?V? //verif si on peut déplacer la destination pour isoler l'origine			ATTENTION QUAND TEMPSDECOUP<3s
	for(j=0;j<listeCoups.nb; j++){
		if(lesVoisins.nb==1 && listeCoups.coups[j].origine==d && listeCoups.coups[j].destination!=o && currentPosition.cols[listeCoups.coups[j].destination].couleur != myColor){
			coupAJouer=j;
			goto jouerLeCoup;
		}	
	}*/
}
//*************************************************************************************************************************************************************// FIN



//*************************************************************************************************************************************************************// CONTRE-COUPS
for(i=0;i<listeCoups.nb; i++){

	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination; 

	//contre-coup si origine A?V? et destination J?V? && A?+J?==5
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
//*************************************************************************************************************************************************************// FIN



//*************************************************************************************************************************************************************// COUPS NON ALEATOIRES ++
//origine J1V? et destination A1V?
for(i=0;i<listeCoups.nb; i++){
	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination;
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 1) && pasDebile(currentPosition, d, 3)==VRAI){
		coupAJouer=i;
		goto jouerLeCoup;
	}
}
//origine J1V? et destination A2V?
for(i=0;i<listeCoups.nb; i++){
	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination;
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 2) && pasDebile(currentPosition, d, 2)==VRAI){
		coupAJouer=i;
		goto jouerLeCoup;
	}
}
//origine J1V? et destination A3V?
for(i=0;i<listeCoups.nb; i++){
	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination;
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 1) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 3) && pasDebile(currentPosition, d, 1)==VRAI){
		coupAJouer=i;
		goto jouerLeCoup;
	}
}
//*************************************************************************************************************************************************************// FIN



//*************************************************************************************************************************************************************// COUPS NON ALEATOIRES --
//origine J2V? et destination A2V?
for(i=0;i<listeCoups.nb; i++){
	o = listeCoups.coups[i].origine;
	d = listeCoups.coups[i].destination; 
	if((currentPosition.cols[o].couleur == myColor) && (currentPosition.cols[o].nb == 2) && (currentPosition.cols[d].couleur != myColor) && (currentPosition.cols[d].nb == 2) && pasDebile(currentPosition, d, 1)==VRAI){
		coupAJouer=i;
		goto jouerLeCoup;
	}
}
//*************************************************************************************************************************************************************// FIN



//on a alors (peut-être [et si on n'a pas été trop long ou si le coup était dans les critères ci-dessus]) choisi le coup à jouer

jouerLeCoup:

o = listeCoups.coups[coupAJouer].origine;
d = listeCoups.coups[coupAJouer].destination; 

printf("Coup %d : ", coupAJouer); 
printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur));

printf("On choisit ce coup !");

ecrireIndexCoup(coupAJouer);
}
//______________________________________________________________________________________________________________________________________________________________________________________________________________



int onSIsole(T_Position laPosition, octet laDestination, octet lOrigine)
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