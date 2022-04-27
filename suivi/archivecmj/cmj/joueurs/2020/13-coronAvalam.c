#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"

#define NB 3		


int nbvoisin(T_Position * Position,octet pion);
int erreurcritique(T_Position * Position,int i,T_ListeCoups * listeCoups);
int isolement(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille);
int sixpremierstours(T_Position * currentPosition, T_ListeCoups *listeCoups,int i);
int empileradverse(T_Position * Position,T_ListeCoups * listeCoups,int i,int * voisins);
int empileradverse2(T_Position * Position,T_ListeCoups * listeCoups,int i);



/*On choisit ici le coup à jouer*/
/*On utilise un système de priorité (0 étant la priorité la plus importante) pour décider du coup*/
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	int taille,voisins;
	int i; 
	int coup_potentiel;
	int priorite[NB];	//On va y stocker les priorités de chaque coup trouvé
	octet o, d;  
	octet myColor = currentPosition.trait; //on récupère ma couleur
	for(i=0;i<NB;i++){		
		priorite[i]=-1;		//-1 est la valeur par défaut, elle correspond à la valeur de notre de notre tableau si aucun coup n'est trouvé
	}
	voisins=0;	
	taille=5;

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));  
	for(i=0;i<listeCoups.nb; i++) {     //on parcourt le nombre de coup possible
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  

	// Si la colonne d'origine esr de ma couleur et que je peux constituer une tour de 5, alors on choisit ce coup 
		if ( (currentPosition.cols[o].couleur == myColor)
		&& (currentPosition.cols[d].nb+currentPosition.cols[o].nb == 5) && (currentPosition.cols[d].couleur != myColor)) { 

			ecrireIndexCoup(i);	//on écrit le coup
			printf("tour de 5 prise par le coup %d : %d ---> %d \n",i,o,d);			
			return; //On return car il n'y a pas de meilleur coup
		}

		coup_potentiel=isolement(&currentPosition,&listeCoups,i,&taille);	//lancement de la fonction d'isolement 
		if(coup_potentiel!=1)priorite[0]=coup_potentiel; //on range le coup seulement si un coup n'a pas été trouvé

		coup_potentiel=empileradverse(&currentPosition,&listeCoups,i,&voisins); //lancement de la fonction d'empilement
		if(coup_potentiel!=-1)priorite[1]=coup_potentiel;

		coup_potentiel=empileradverse2(&currentPosition,&listeCoups,i);	//lancement de la seconde fonction 'empilement'
		if(coup_potentiel!=-1)priorite[2]=coup_potentiel;
	 }

	printf("%d\t%d\t%d\t%d\n",priorite[0],priorite[1],priorite[2],priorite[3]); // les differents coups rangés par priorité sont affichés

	for(i=0;i<NB;i++){

		if(priorite[i]!=-1){
			ecrireIndexCoup(priorite[i]);
			switch(i){
				case 0 : //le switch permet d'avoir un suivi des stratégies employées
					printf("coup d'isolement la plus petite %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				case 1 : 
				printf("coup empilement  v1 %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				case 2 : 
					printf("coup empilement  v2 %d : %d ---> %d \n",priorite[i],listeCoups.coups[priorite[i]].origine,listeCoups.coups[priorite[i]].destination);	
					break;
				
				default:
					printf("aucune strat choisi, coup par default\n");//ce cas n'a jamais été observé
					break;
			}

			return;
		}
	}
}



//Cette fonction permet de visualiser l'état du plateau après le coup qui est envisagé. On va donc pouvoir valider ou non ce coup en fonction
//Des possibilités d'action de l'adversaire. Par exemple s'assurer que le coup ne donnera pas de points à l'adversaire au tour suivant.
int erreurcritique(T_Position * Position,int i /* index du coup voulent etre joué*/,T_ListeCoups * listeCoups){
    T_Voisins VoisinD,VoisinD2; //voisin d: voisin de d, voisin d2: voisin des voisins de d
    int j,risque;
    octet myColor=Position->trait;
    octet o,d;
    o = listeCoups->coups[i].origine; 
    d = listeCoups->coups[i].destination;

    VoisinD=getVoisins(d);


    if(Position->cols[d].nb+Position->cols[o].nb != 5){    //si la tour obtenue par le coup est une tour de 5 on ne fait pas ce test car elle ne pourra pas etre deplacée
        for(j=0;j<VoisinD.nb;j++){          //pour tous les voisins de d
            if(o!=VoisinD.cases[j]){        //sauf le voisin qui est le pion qui va être déplacé
                if(Position->cols[o].nb+Position->cols[d].nb+Position->cols[VoisinD.cases[j]].nb==5 && (Position->cols[o].couleur!=myColor || Position->cols[VoisinD.cases[j]].couleur!=myColor))return 2;    //si l'un des nouveaux voisins de cette nouvelle tour permet un coup critique en faveur de l'adversaire on return 2
                if(Position->cols[o].nb+Position->cols[d].nb+Position->cols[VoisinD.cases[j]].nb==5 && (Position->cols[o].couleur!=myColor && Position->cols[VoisinD.cases[j]].couleur!=myColor))return 1;    //si l'un des nouveaux voisins de cette nouvelle tour permet un coup critique en faveur de l'adversaire on return 1
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
        if(Position->cols[o].couleur!=myColor)return 2;//si le coup crée une tour de 5 ennemie on return 2
    }

return 0;
}

//fonction "d'aide" comme erreur critique, détermine le nombre de voisins pour nous assister
int nbvoisin(T_Position * Position,octet pion){
    int i,compteur=0;
    T_Voisins Voisin;
    Voisin=getVoisins(pion);

    for(i=0;i<Voisin.nb;i++){		//on parcourt les voisins du pion soumis à la fonction
        if(Position->cols[Voisin.cases[i]].nb!=0 && Position->cols[Voisin.cases[i]].nb!=5)compteur++;
        //si la hauteur des tours est différente de 0(case vide) et différente de 5(tour immobile) on considère le pion comme un voisin
        //on incrémente le compteur
    }
    return compteur;
}

//Fonction proposant un coup d'isolement des pions de notre équipe
int isolement(T_Position * Position,T_ListeCoups * listeCoups,int i,int * taille){
    octet myColor=Position->trait;
    octet o,d;

	o = listeCoups->coups[i].origine; 
	d = listeCoups->coups[i].destination;

    if (Position->cols[o].couleur == myColor){ //si la colonne d'origine est de ma couleur
			if( (1==nbvoisin(Position,d)) && (erreurcritique(Position,i,listeCoups)<1) && (*(taille)<Position->cols[d].nb+Position->cols[o].nb)){
				//si la tour n'a qu'un seul voisin (pion de destination), qu'on ne fait pas d'erreur critique et que la taille de la tour isolée est la plus petite possible
					*(taille)=Position->cols[d].nb+Position->cols[o].nb;	
					return i;
			}
			else return -1;
			
	}
	else return -1;		//si le cas ne se présente pas, aucun coup n'est soumis


}





//empiler les adversaire pour limiter sa marge de manoeuvre
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
		&& (erreurcritique(Position,i,listeCoups)<1) && (*voisins>nbvoisin(Position,d))) { //et qu'on ne fait pas d'erreur critique et que le nombre de voisin est minimal
			*voisins=nbvoisin(Position,d);		//on enregistre ce nombre si une stratégie mieux est trouvée
			return i;				//on va venir soumettre ce coup
		}
return -1;
}

//dernier recours 
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
		&& (erreurcritique(Position,i,listeCoups)<1) ) { //et que l'on ne fait pa d'erreur critique
			return i;				//on soumet un coup
			
		}
return -1;
}


