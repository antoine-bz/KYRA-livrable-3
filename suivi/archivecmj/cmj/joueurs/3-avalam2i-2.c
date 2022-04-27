/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

#define NBSTRAT 5

int nbvoisin(T_Position * Position,octet pion);
int erreurcritique(T_Position * Position,int i,T_ListeCoups * listeCoups);
int isolement(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille);
int empileradverse(T_Position * Position,T_ListeCoups * listeCoups,int i,int * voisins);
int empileradverse2(T_Position * Position,T_ListeCoups * listeCoups,int i);
int gang(T_Position p, octet myColor);
int isolementsuicide(T_Position * Position,T_ListeCoups * listeCoups,int i /* index du coup voulent etre joué*/,int * taille);
int autoempileopti(T_Position * Position,T_ListeCoups * listeCoups,int i /* index du coup voulent etre joué*/,int * taille);


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 

	int tailleISOSUICIDE;
	int tailleAUTAMP;

	int taille,voisins;
	int i; 
	int isolement_coup;
	int coup_possible;
	int priorite[NBSTRAT];
	octet o, d;  //entier a valeur max d'octet (entre 0 et 255)
	octet myColor = currentPosition.trait; //on récupère ma couleur
	// afficherListeCoups(listeCoups);
	for(i=0;i<NBSTRAT;i++){
		priorite[i]=-1;
	}
	voisins=0;
	taille=5;

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));  // on l'affiche (avec la macro fonction colname, de avalam.h)
	for(i=0;i<listeCoups.nb; i++) {     //on parcours le nombre de coup possible
		o = listeCoups.coups[i].origine; 		//voir struct T coup de avalam.h
		d = listeCoups.coups[i].destination;  
		//printf("Coup %d : ", i);  //on affiche le numéro du coup
		//printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));  //(coup 0 (1(il y a un pion sur cette case), 1 ou 2la couleur de ce pion))
		//printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); // pareil que au dessus, casse de destiantion puis nombre de pions sur la case 
		//printf("test pour coup %d : %d ---> %d \n",i,listeCoups.coups[i].origine,listeCoups.coups[i].destination);	

	// Si je peux gagner une colonne, je la prends 
		if ( (currentPosition.cols[o].couleur == myColor) //si la colonne d'origine est de ma couleur
		&& (currentPosition.cols[d].nb+currentPosition.cols[o].nb == 5) && (currentPosition.cols[d].couleur != myColor)) { // 
			//printf("On choisit ce coup ! \n");
			ecrireIndexCoup(i);	
			printf("tour de 5 prise par le coup %d : %d ---> %d \n",i,o,d);			
			return; // on quitte la fonction, on pourrait la continuer pour trouver un "meilleur" choix
		}

		coup_possible=isolement(&currentPosition,&listeCoups,i,&taille);
		if(coup_possible!=1)priorite[0]=coup_possible;

		coup_possible=empileradverse(&currentPosition,&listeCoups,i,&voisins);
		if(coup_possible!=-1)priorite[1]=coup_possible;

		coup_possible=empileradverse2(&currentPosition,&listeCoups,i);
		if(coup_possible!=-1)priorite[2]=coup_possible;

		coup_possible=isolementsuicide(&currentPosition,&listeCoups,i,&tailleISOSUICIDE);
		if(coup_possible!=-1)priorite[3]=coup_possible;

		coup_possible=autoempileopti(&currentPosition,&listeCoups,i,&tailleAUTAMP);
		if(coup_possible!=-1)priorite[4]=coup_possible;


	} 


	for(i=0;i<NBSTRAT;i++){
		printf("%d\t",priorite[i]);
	}
	printf("\n");

	for(i=0;i<NBSTRAT;i++){

		if(priorite[i]!=-1){
			ecrireIndexCoup(priorite[i]);
			switch(i){
				case 0 : 
				printf("coup d'isolement la plus petite %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				case 2 : 
				printf("coup empilement  v1 %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				case 1 : 
				printf("coup empilement  v2 %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				case 3 : 
				printf("coup isolement suicide %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				case 4 : 
				printf("coup autoempileopti %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				default:
					printf("aucune strat choisi, coup par default\n");
					break;
			}

			return;
		}
	}
}

int erreurcritique(T_Position * Position,int i /* index du coup voulent etre joué*/,T_ListeCoups * listeCoups){
    T_Voisins VoisinD,VoisinD2; //voisin d: voisin de d, voisin d2: voisin des voisins de d
    int j,risque;
    octet myColor=Position->trait;
    octet o,d;
    o = listeCoups->coups[i].origine; 
    d = listeCoups->coups[i].destination;

    VoisinD=getVoisins(d);


    if(Position->cols[d].nb+Position->cols[o].nb != 5){    //si la tour obtenue par le coup est une tour de 5 on ne fait pas ce test car elle ne pourras pas etre deplacé
        for(j=0;j<VoisinD.nb;j++){        //pour tout les voisin de d
            if(o!=VoisinD.cases[j]){        //sauf le voisin qui est le pion qui vas etre deplacé
                if(Position->cols[o].nb+Position->cols[d].nb+Position->cols[VoisinD.cases[j]].nb==5 && (Position->cols[o].couleur!=myColor || Position->cols[VoisinD.cases[j]].couleur!=myColor))return 2;    //si l'un des nouveau voisins de cette nouvelle tour permet un coup critique en faveur de l'adversaire on return 2
                if(Position->cols[o].nb+Position->cols[d].nb+Position->cols[VoisinD.cases[j]].nb==5 && (Position->cols[o].couleur!=myColor && Position->cols[VoisinD.cases[j]].couleur!=myColor))return 1;    //si l'un des nouveau voisins de cette nouvelle tour permet un coup critique en faveur de l'adversaire on return 1
            }
        }
        if(nbvoisin(Position,d)==1){//si il n'y a qu'un seul voisin c'est donc le pion origine
            if(Position->cols[o].couleur!=myColor)return 2;//si on isole un pion advairse on return 2
        }
        if(nbvoisin(Position,d)==2){//si notre pion dest a 2 voisins
            for(j=0;j<VoisinD.nb;j++){//on trouve l'autre voisin
                if(o!=VoisinD.cases[j]){
                    VoisinD2=getVoisins(VoisinD.cases[j]);//on regarde si ce voisin a d'autre voisin que notre pion dest (on teste le risque d'isolement par un coup advairse)

                    if(nbvoisin(Position,VoisinD.cases[j])==1){
                        if(Position->cols[o].couleur!=myColor || Position->cols[VoisinD.cases[j]].couleur!=myColor) return 1;
                    }
                }
            }
        }
    }
    else{
        if(Position->cols[o].couleur!=myColor)return 2;//si le coup crée une tour de 5 énnemie on return 2
    }

return 0;
}

int isolement(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille){
    octet myColor=Position->trait;
    octet o,d;

	o = listeCoups->coups[i].origine; 
	d = listeCoups->coups[i].destination;

    if (Position->cols[o].couleur == myColor){ //si la colonne d'origine est de ma couleur
			if( (1==nbvoisin(Position,d)) && (erreurcritique(Position,i,listeCoups)<1) && (*(taille)<Position->cols[d].nb+Position->cols[o].nb)){
					*(taille)=Position->cols[d].nb+Position->cols[o].nb;	
					return i;
			}
			else return -1;
			
	}
	else return -1;


}

int nbvoisin(T_Position * Position,octet pion){
    int i,compteur=0;
    T_Voisins Voisin;
    Voisin=getVoisins(pion);

    for(i=0;i<Voisin.nb;i++){
        if(Position->cols[Voisin.cases[i]].nb!=0 && Position->cols[Voisin.cases[i]].nb!=5)compteur++;
    }
    return compteur;
}

//!!!!!!!!!!!faire que des tours de 2 pour essayerrrrr
int empileradverse(T_Position * Position,T_ListeCoups * listeCoups,int i,int * voisins){
    octet myColor=Position->trait;
    octet o,d;

		o = listeCoups->coups[i].origine; 		//voir struct T coup de avalam.h
		d = listeCoups->coups[i].destination;  
		//On empile pas les pions de l'adversaire si on crée une pile de 4 ou 5
		//couleur origine: adversaire
		//couleur destination: adverse
		// taille de la pile <4
		if ( (Position->cols[o].couleur != myColor) 
		&& (Position->cols[d].couleur != myColor)					//si la colonne d'origine est pas  de ma couleur
		&& (erreurcritique(Position,i,listeCoups)<1) && (*voisins>nbvoisin(Position,d))) { 
			*voisins=nbvoisin(Position,d);
			return i;				
		}
return -1;
}

int empileradverse2(T_Position * Position,T_ListeCoups * listeCoups,int i){
    int memoire=0;
    octet myColor=Position->trait;
    octet o,d;

		o = listeCoups->coups[i].origine; 		//voir struct T coup de avalam.h
		d = listeCoups->coups[i].destination;  
		//On empile pas les pions de l'adversaire si on crée une pile de 4 ou 5
		//couleur origine: adversaire
		//couleur destination: adverse
		// taille de la pile <4
		if ((Position->cols[d].couleur != myColor)					//si la colonne destination n' est pas  de ma couleur
		&& (erreurcritique(Position,i,listeCoups)<1) ) { 
			return i;				
			
		}
return -1;
}

int isolementsuicide(T_Position * Position,T_ListeCoups * listeCoups,int i /* index du coup voulent etre joué*/,int * taille){
    octet myColor=Position->trait;
    octet o,d;
//si on peut ce suicideisoler on le fait
		o = listeCoups->coups[i].origine; 		//voir struct T coup de avalam.h
		d = listeCoups->coups[i].destination;  	
	if(Position->cols[o].couleur == myColor  && nbvoisin(Position,d)==1 && (*taille>Position->cols[d].nb+Position->cols[o].nb)){
		*taille=Position->cols[d].nb+Position->cols[o].nb;
		return i;
	}
return -1;
}


int autoempileopti(T_Position * Position,T_ListeCoups * listeCoups,int i /* index du coup voulent etre joué*/,int * taille){
    octet myColor=Position->trait;
    octet o,d;

		o = listeCoups->coups[i].origine; 		//voir struct T coup de avalam.h
		d = listeCoups->coups[i].destination;  


		if ( (erreurcritique(Position,i,listeCoups)<1) && (*taille<Position->cols[d].nb+Position->cols[o].nb)) { 
			*taille=Position->cols[d].nb+Position->cols[o].nb;
			return i;				
		}
return -1;
}	
