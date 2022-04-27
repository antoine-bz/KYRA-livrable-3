/********* Moteur de jeu : BINKS ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


//le but de cette fonction est de trouver un coup dans la liste des coups 
//à partir de l'origine et de la destination du coup souhaité 
int trouverCoup(T_ListeCoups listeCoups, octet origineCoupSouhaitee, octet destinationCoupSouhaitee)
{
	int i;
	for(i=0;i<listeCoups.nb;i++)
	{
		printf("%d",i);
		if(listeCoups.coups[i].origine==origineCoupSouhaitee && 
			listeCoups.coups[i].destination==destinationCoupSouhaitee) 
			{
				return i;
			}
	}
	return -1;
}

//le but de cette fonction est d'annalyser les voisins de l'octet "destination"
//pour vérifier que le déplacement de "pileComparaison" sur "destination" ne va pas 
//permettre à l'adversaire de faire une pile de 5 en son avantage après le coup 
int analyserVoisins(T_Position currentPosition, octet myColor, octet pileComparaison, octet destination, int nombrePionsPile, int nombrePionsPileVoisine)
{
	int i,j; 
	int nombrePionsPile_PileVoisine;
	int nombrePionsLimite;
	T_Voisins tableauVoisins;
	tableauVoisins = getVoisins(destination);

	nombrePionsPile_PileVoisine = nombrePionsPile + nombrePionsPileVoisine;
	nombrePionsLimite = 5 - nombrePionsPile_PileVoisine;

	for(i=0;i<nbVoisins(destination);i++)
	{
        // si la pile qu'on souhaite déplacer est de notre couleur :
        // le voisin actuel ne doit pas être égal à "pileComparaison" 
            // (car cette pile est inclue dans le tableau des voisins de la destination)
        // le coup n'est pas correct (return 0) si :
        // -> le nombre de pions d'un des voisins est égal à la limite
        // -> ET la couleur du voisin est de la couleur adverse
		if(currentPosition.cols[pileComparaison].couleur == myColor
			&& tableauVoisins.cases[i] != pileComparaison 
			&& currentPosition.cols[tableauVoisins.cases[i]].nb == nombrePionsLimite
			&& currentPosition.cols[tableauVoisins.cases[i]].couleur != myColor)
				return 0;

        //si la pile qu'on souhaite déplacer est de la couleur adverse :
        // le voisin actuel ne doit pas être égal à "pileComparaison" 
            // (car cette pile est inclue dans le tableau des voisins de la destination)
        // le coup n'est pas correct (return 0) si :
        // -> le nombre de pions d'un des voisins est égal à la limite
            //puisque dans tous les cas si on fait le coup, l'adversaire pourra faire 
            //une pile de 5 
		if(currentPosition.cols[pileComparaison].couleur != myColor
			&& tableauVoisins.cases[i] != pileComparaison 
			&& currentPosition.cols[tableauVoisins.cases[i]].nb == nombrePionsLimite)
				return 0;	
	}
	return 1;
}



void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{

	int numCoupTrouve;
    T_Voisins pionVoisinsOrigine;
	T_Voisins pionVoisinsDestination;
    int nbPionsPile, nbPionsPileVoisine;
    int somme_ori_dest=0;

	int i,j; 
	octet o, d; 
	octet myColor = currentPosition.trait;

	
	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));

    // 6 correspond a notre nombre de "fonction" (en démarrant de 0)
    // (regroupement de if qui se font par ordre de priorité)
    // donc numero_du_tour = variable pour savoir dans quel if il faut rentrer 
    for(int numero_du_tour=0; numero_du_tour<6; numero_du_tour++)
    {

        for(i=0;i<listeCoups.nb; i++)
        {
            //on récupère l'octet d'origine et de destination 
            // par rapport au numéro du coup dans la liste des coups
            o = listeCoups.coups[i].origine; 
            d = listeCoups.coups[i].destination;

            //on récupère les voisins du pion d'origine et de destination 
            pionVoisinsOrigine = getVoisins(o);
            pionVoisinsDestination = getVoisins(d);

            //somme du nombre de pion de l'origine et de la destination
            somme_ori_dest=currentPosition.cols[o].nb+currentPosition.cols[d].nb;


//************  PREMIERE FONCTION  ******************//

            //************  PILES DE 5  ******************//
			//(coup sans risque direct avec la fonction analyserVoisin)
            if ( numero_du_tour == 0 
                && (currentPosition.cols[o].couleur == myColor)
                && ((currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 5)
                && (currentPosition.cols[d].couleur != myColor) ) 
            {
                printf("On choisit ce coup : formation d'une pile de 5 \n");
                ecrireIndexCoup(i);
                return; //quitte la fonction
            }

			//la fonction suivant a été mise en commentaire, car le if precedent prend en compte ce cas 
            /*
            if ( numero_du_tour == 0 
                && (currentPosition.cols[o].couleur != myColor)
                && ( (currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 5)
                && (currentPosition.cols[d].couleur == myColor) ) 
            {
                numCoupTrouve=trouverCoup(listeCoups, d, o);
                ecrireIndexCoup(numCoupTrouve);
                return;  
            }
            */

            if ( numero_du_tour == 0 
                && (currentPosition.cols[o].couleur == myColor)
                && ( (currentPosition.cols[o].nb + currentPosition.cols[d].nb) == 5)
                && (currentPosition.cols[d].couleur == myColor) ) 
                {
                    printf("On choisit ce coup : formation d'une pile de 5 \n");
                    ecrireIndexCoup(i);
                    return; //quitte la fonction
                }

            //************  ISOLEMENT  ******************//
			//(coup sans risque direct avec la fonction analyserVoisin)
            if (numero_du_tour == 0 
                && nbVoisins(o) == 1) //si le pion origine n'a qu'un seul voisin
            {
                //si destination de notre couleur et origine non 
                //Destination sur Origine
                if( currentPosition.cols[d].couleur == myColor 
                    && currentPosition.cols[o].couleur != myColor) 
                {
                    printf("On choisit ce coup : isolement d'une pile \n");
                    numCoupTrouve=trouverCoup(listeCoups, d, o);
                    ecrireIndexCoup(numCoupTrouve);
                    return; //quitte la fonction
                }

                //si Origine de notre couleur et Destination non
                //on doit éloigner la destination
                if( currentPosition.cols[o].couleur == myColor )
                {
                    //nombre de pions sur la pile de destination
                    nbPionsPile = currentPosition.cols[d].nb;

                    for(j=0;j<pionVoisinsDestination.nb;j++)
                    {
                        //nombre de pions à la pile voisine de la destination
                        nbPionsPileVoisine = currentPosition.cols[pionVoisinsDestination.cases[j]].nb;

                        //si la pile voisine de la destination est != de la pile d'origine
                        //et que le déplacement de la pile de destination sur un de ses voisins est possible
                        if( pionVoisinsDestination.cases[j] != o 
                            && currentPosition.cols[pionVoisinsDestination.cases[j]].couleur != myColor
                            && analyserVoisins(currentPosition, myColor, d, pionVoisinsDestination.cases[j], nbPionsPile, nbPionsPileVoisine) )
                        {
                            printf("On choisit ce coup : isolement d'une pile \n");
                            numCoupTrouve=trouverCoup(listeCoups, d, pionVoisinsDestination.cases[j]);
                            ecrireIndexCoup(numCoupTrouve);
                            break;
                        }
                    }
                    //si on ne peut déplacer le pion destination de notre couleur que sur l'origine
                    printf("On choisit ce coup : isolement d'une pile \n");
                    numCoupTrouve=trouverCoup(listeCoups, d, o);
                    ecrireIndexCoup(numCoupTrouve);
                    return;	//quitte la fonction 
                }
            }

            //************  PILE DE 4  ******************//
            //faire une pile de 4 avec pour origine un pile de notre couleur
			//(coup sans risque direct avec la fonction analyserVoisin)
            if( numero_du_tour == 0 
                && currentPosition.cols[o].couleur == myColor 
                && (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 4)
                && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb))
            {
                printf("On choisit ce coup : formation d'une pile de 4 pour nous \n"); 
                ecrireIndexCoup(i);
                return; //quitte la fonction
            }

            //la fonction suivant a été mise en commentaire, car le if precedent prend en compte ce cas 
            /*
            / ************  PILE DE 2 SUR PILE DE 2  ****************** /
            //pile de 2 (o) de notre couleur sur pile de 2 (d) couleur adverse 
            if(numero_du_tour == 0 
                && currentPosition.cols[o].couleur == myColor 
                && currentPosition.cols[d].couleur != myColor 
                && currentPosition.cols[o].nb == 2 
                && currentPosition.cols[d].nb == 2
                && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb))
            {
                printf("On choisit ce coup ! \n"); 
                ecrireIndexCoup(i);
                return; //quitte la fonction
            }
            */

//************  DEUXIEME FONCTION  ******************//

        //Formation d'une pile de 3 de la couleur de l'adversaire
		//(coup sans risque direct avec la fonction analyserVoisin)
        if( numero_du_tour == 1 
            && (currentPosition.cols[o].couleur != myColor) 
            && (currentPosition.cols[d].couleur != myColor) 
            && (somme_ori_dest==3) 
            && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb) )
            {
                printf("On choisit ce coup : deuxieme fonction \n"); 
                ecrireIndexCoup(i);
                return; // on quitte la fonction 
            }
        

//************  TROISIEME FONCTION  ******************//

            //Formation d'une pile de 2 adverse dans le but de faire une pile de 3 par la suite
			//(coup sans risque direct avec la fonction analyserVoisin)
            if( numero_du_tour == 2 
                && (currentPosition.cols[o].couleur != myColor) 
                && (currentPosition.cols[d].couleur != myColor) 
                && (somme_ori_dest==2)
                && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb))
            {
                printf("On choisit ce coup : troisieme fonction \n");  
                ecrireIndexCoup(i);
                return; // on quitte la fonction 
            }


//************  QUATRIEME FONCTION  ******************//    

		//faire une pile de 3 
        //d'une pile de notre couleur sur une pile adverse
		//(coup sans risque direct avec la fonction analyserVoisin) 
        if(numero_du_tour == 3
            && currentPosition.cols[o].couleur == myColor
            && currentPosition.cols[d].couleur != myColor 
			&& (somme_ori_dest==3)
            && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb)) 
        {
            printf("On choisit ce coup : quatrieme fonction \n"); 
            ecrireIndexCoup(i);
            return; // on quitte la fonction 
        }
        
//************  CINQUIEME FONCTION  ******************//    

		//faire une pile de 2
        //d'une pile de notre couleur sur une pile adverse 
		//(coup sans risque direct avec la fonction analyserVoisin)
        if(numero_du_tour == 4
            && currentPosition.cols[o].couleur == myColor
            && currentPosition.cols[d].couleur != myColor 
			&& (somme_ori_dest==2)
            && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb)) 
        {
            printf("On choisit ce coup : cinquieme fonction \n");
            ecrireIndexCoup(i);
            return; // on quitte la fonction 
        }
      

//************  SIXIEME FONCTION  ******************//

		//en dernier recours : on empile une pile de notre couleur sur une de notre couleur
		//(coup sans risque direct avec la fonction analyserVoisin)
        if(numero_du_tour == 5
            && currentPosition.cols[o].couleur == myColor
            && currentPosition.cols[d].couleur == myColor
            && analyserVoisins(currentPosition, myColor, o, d, currentPosition.cols[o].nb, currentPosition.cols[d].nb))
            {
                printf("On choisit ce coup : sixieme fonction \n");
                ecrireIndexCoup(i);
                return; //quitte la fonction
            }
        }

    }

}


//efficace et pas cher (en temps)
//c'est avalam que j'préfère
//c'est avalam !