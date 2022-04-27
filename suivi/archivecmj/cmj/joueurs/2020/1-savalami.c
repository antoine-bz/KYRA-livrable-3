/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"
    

T_Voisins getNbVoisins(T_Position p,octet cO) { // 

    int iV,cD; // indice Voisin
    T_Voisins v; 
    T_Voisins aux; 
    aux.nb = 0 ; 
        v = getVoisins(cO); 

        for(iV=0;iV<v.nb;iV++) {
            cD = v.cases[iV]; 

            if (p.cols[cD].nb == VIDE && (p.cols[cD].nb !=5)){
                //printf("vide!\n");
                //aux.cases[iV]= 100 ; 
            }
            else {
                //printf("possible !\n");
                aux.cases[aux.nb]=cD; 
                aux.nb = aux.nb + 1;
            }
        }
    return aux; 
}
void afficherVoisins(T_Voisins v){
    int i; 
    for(i =0 ; i<v.nb; i++){
        //if(v.cases[i] != 100 )
            printf("\nvois num %d => %d",i,v.cases[i]);
    }
}

int calculerNbPilesPlateau(T_Position p)
{
    int j, nb = 0;

    for (j = 0 ; j < NBCASES ; j++)
        if (p.cols[j].nb != 0)
            nb++;

    return nb;
}

int calculerVoisins(T_Position p, T_Voisins v,int Color, int taillePile,octet myColor){
    
    int nb =0;
    for (int j = 0 ; j < v.nb ; j++)
        {
                     if (p.cols[v.cases[j]].couleur != myColor && p.cols[v.cases[j]].nb == taillePile && Color==0)
                nb++;
                     else if (p.cols[v.cases[j]].couleur == myColor && p.cols[v.cases[j]].nb == taillePile && Color==1)
                nb++;
                     else if (p.cols[v.cases[j]].nb == taillePile && Color==2)
                nb++;
        }
    return nb;
}


int calculerNbVoisinsAmis(T_Position p, T_Voisins v, octet myColor)
{
    int j, nb;

    for (j = 0 ; j < v.nb ; j++)
        {
            if ((p.cols[v.cases[j]].couleur == myColor) && (p.cols[v.cases[j]].nb!=5))     // calcul des piles voisines possédées
            nb++;
        }

    return nb;
}

int calculerNbVoisinsAdverses(T_Position p, T_Voisins v, octet myColor)
{
    int j, nb;

    for (j = 0 ; j < v.nb ; j++)
        {
            if ((p.cols[v.cases[j]].couleur != myColor) && (p.cols[v.cases[j]].nb!=5))    // calcul des piles voisines adverses
            nb++;
        }

    return nb;
}

typedef struct {
    int indice ;
    char sousType[25]; //offensif,defensif,standby...
    int valeur; //potentiel du coup (dans sa catégorie) 
    } TCoup;

typedef struct {
    int offensif ;  
    int deffensif ; 
    int standby ; 
    int eviter; 
    } T_Nombre_Coups;

typedef struct {
    TCoup coupsOffensif[48]; 
    TCoup coupsDeffensif[48]; 
    TCoup coupsStandby[48]; 
    TCoup coupsEviter[48]; 
    T_Nombre_Coups nb ; 
    //TCoup coupsSauverMeubles[MAXCOUPS]; 
    } T_listes_Coups;

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups ) {
    // Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
    // Pour sélectionner l'index d'un coup à jouer dans la liste l 


    int i; 
    octet o,d; 
    octet myColor = currentPosition.trait;
    T_Voisins voisD,voisO ; 

    T_listes_Coups liste_coups;
    liste_coups.nb.offensif = 0;
    liste_coups.nb.deffensif = 0;
    liste_coups.nb.standby = 0;
    liste_coups.nb.eviter =0; 
    int ouverture = 0 ; 

    // afficherListeCoups(listeCoups);

    printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
    for(i=0;i<listeCoups.nb; i++) {
        o = listeCoups.coups[i].origine; 
        d = listeCoups.coups[i].destination;
        voisD = getNbVoisins(currentPosition,d);
        voisO = getNbVoisins(currentPosition,o); 
//-----------------------------------------------------------------------------//
        
        

//---------------------------------------------------------------------------------//
        printf("\n\nCoup %d : ", i); 
        printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
        printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 
        //printf("\n==========>Voisins de %d: %d <========\n",d,voisD);
        printf("\nliste des voisins de la case %d:",d); 
        //printf("\n==========>Voisins de %d: %d <========\n",d,voisD.nb);
        afficherVoisins(voisD); 

                    /*******************************************************************/
                    //-----------------------VARIABLESS--------------------------------//
                    /*******************************************************************/   


                    /*******************************************************************/
                    //-----------------------STRATÉGIES--------------------------------//
                    /*******************************************************************/

//--------EVITER DE LAISSER L'ADVERSAIRE FAIRE UNE PILE DE 5 (II. 3)------------------------------//

            int x = currentPosition.cols[o].nb + currentPosition.cols[d].nb;
            if((currentPosition.cols[o].couleur != myColor) && (calculerVoisins(currentPosition,voisD,2,5-x,myColor)>=1)){
                 i++;
            }

           else if((currentPosition.cols[o].couleur == myColor) && (calculerVoisins(currentPosition,voisD,0,5-x,myColor)>=1)){
                 i++ ;
            }
           

//-----COUP INUTILE VERSION ISOLEMENT -----//
if( (currentPosition.cols[o].couleur == myColor)&&(voisO.nb ==1) 
 && (currentPosition.cols[d].couleur == myColor)&&(voisD.nb ==1) ){

            i++ ; // on passe au coup suivant 
        liste_coups.coupsEviter[liste_coups.nb.eviter].indice = i ; 
        liste_coups.coupsEviter[liste_coups.nb.eviter].valeur = 5;
        liste_coups.nb.eviter = liste_coups.nb.eviter +1;
        }

//---------STRATEGIE OUVERTURE ------------//

int nbPiles = calculerNbPilesPlateau(currentPosition);

if (nbPiles > NBCASES - 1)// si c'est le premier tour (aucun empilement)
{
    if (myColor == JAU)
            if( (o == 18 && d == 27) || (o == 29 && d == 20) ){ 
                 //ecrireIndexCoup(i);
                ouverture = i ; }

        if (myColor == ROU)
                if( (o == 26 && d == 19) || (o == 21 && d == 28) ){ 
                //ecrireIndexCoup(i);
                     ouverture = i ; }
}

//--------STRATEGIE POUR CRÉER PILE DE 5 DS N'IMPORTE QUEL CAS------------------------------//
        if ((currentPosition.cols[o].couleur == myColor) 
         && (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) ) {
           
            liste_coups.coupsOffensif[liste_coups.nb.offensif].indice = i ; 
            liste_coups.coupsOffensif[liste_coups.nb.offensif].valeur = 5;
            liste_coups.nb.offensif = liste_coups.nb.offensif+1;
            }


//--------STRATEGIE POUR ISOLER UNE PILE V1.1------------------------------//
       
        int nb = currentPosition.cols[o].nb +currentPosition.cols[d].nb ; 
        if((currentPosition.cols[o].couleur == myColor) && 
           (nb !=5) ){
            
            T_Voisins Aux; 
            for (int j=0; j<voisO.nb; ++j)
             {
                 Aux = getNbVoisins(currentPosition,voisO.cases[j]); 
                 if(Aux.nb == 1 
                    && (calculerVoisins(currentPosition,voisD,1,5-nb,myColor)))
                 {  liste_coups.coupsOffensif[liste_coups.nb.offensif].indice = i; 
                    liste_coups.coupsOffensif[liste_coups.nb.offensif].valeur = 5;
                    liste_coups.nb.offensif = liste_coups.nb.offensif+1;
                 }
             }    
        }

//--------STRATEGIE POUR ISOLER UNE PILE V1.2------------------------------//
// le pion destination ne possède q'un voisin qui est le pion origine 
        if((currentPosition.cols[o].couleur == myColor) 
        && (voisD.nb ==1)){
            
            liste_coups.coupsOffensif[liste_coups.nb.offensif].indice = i ; 
            liste_coups.coupsOffensif[liste_coups.nb.offensif].valeur = 5;
            liste_coups.nb.offensif = liste_coups.nb.offensif+1;
        }

       
//---------------STRATEGIE POUR ISOLER UNE PILE V1.3------------------------------//
//isoler une pile qui a des voisins de 5 
        if(currentPosition.cols[o].couleur == myColor)
        {    
            int voisinSafe = 0; 
            for (int j=0; j < voisD.nb ; ++j)
            {
                if(currentPosition.cols[voisD.cases[j]].nb != 5)
                     voisinSafe = voisinSafe + 1 ;  
            }

            if(voisinSafe == 1){
               
                liste_coups.coupsOffensif[liste_coups.nb.offensif].indice = i ; 
                liste_coups.coupsOffensif[liste_coups.nb.offensif].valeur = 5;
                liste_coups.nb.offensif = liste_coups.nb.offensif+1;
            }
        }
//--------- STRATEGIE CREER PILE DE 4 SECURISEE (II. 4 .1.) ------------//

int voisNoSafe = calculerVoisins(currentPosition,voisD,0,1,myColor); 
//printf("voisNoSafe => %d\n",voisNoSafe);
if ( (currentPosition.cols[d].nb + currentPosition.cols[o].nb  == 4)
    && (voisNoSafe == 0) 
    && (currentPosition.cols[o].couleur == myColor)
    && (currentPosition.cols[d].couleur != myColor)){
        
        liste_coups.coupsOffensif[liste_coups.nb.offensif].indice = i ; 
        liste_coups.coupsOffensif[liste_coups.nb.offensif].valeur = 4;
        liste_coups.nb.offensif = liste_coups.nb.offensif+1;
    } 

//--------EMPECHER L'ADVERSAIRE DE FAIRE UNE PILE DE 5 (II. 2)------------------------------//
 if ((currentPosition.cols[o].couleur == myColor)) {

    for(int j=0 ; j<voisD.nb ; j++)
    { 
        if(currentPosition.cols[d].nb + currentPosition.cols[voisD.cases[j]].nb == 5
        &&(currentPosition.cols[d].couleur !=myColor || currentPosition.cols[voisD.cases[j]].couleur != myColor) )
            { 
            //printf("On fait une empêche une pile de 5 ds ton cul l'adversaire! \n"); 
            //ecrireIndexCoup(i);
            T_Voisins aux ;
            aux = getNbVoisins(currentPosition,voisD.cases[j]);
            int Nosafe = calculerVoisins(currentPosition,aux,0,
                currentPosition.cols[d].nb,myColor);
            
            if( Nosafe < 2){ 
            liste_coups.coupsDeffensif[liste_coups.nb.deffensif].indice = i ; 
            liste_coups.coupsDeffensif[liste_coups.nb.deffensif].valeur = 5;
            liste_coups.nb.deffensif++;}
           }
    }
}

// STRATEGIE CREER PILE DE 3 POUR BLOQUER L'ADVERSAIRE SECURISEE (II. 4. 2.) // 

int voisNoSafe2 = calculerVoisins(currentPosition,voisD,0,2,myColor); 
int voisNoSafe1 = calculerVoisins(currentPosition,voisD,0,1,myColor); 
int voisSafe = calculerVoisins(currentPosition,voisD,1,1,myColor); 


if (   (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 3)
    && (currentPosition.cols[o].couleur == myColor)
    && (currentPosition.cols[d].couleur != myColor)
    && (voisSafe > voisNoSafe1) 
    && (voisNoSafe2 == 0)){
        
    liste_coups.coupsStandby[liste_coups.nb.standby].indice = i ; 
    liste_coups.coupsStandby[liste_coups.nb.standby].valeur = 4;
    liste_coups.nb.standby++ ;  
    }

//--------STRATEGIE POUR MINIMISER LE NB DE PILES ADVERSES-------------------//
    voisNoSafe1 = calculerVoisins(currentPosition,voisD,0,1,myColor);
    voisSafe = calculerVoisins(currentPosition,voisD,1,1,myColor); 
    int voisNoSafe3 = calculerVoisins(currentPosition, voisD,0,3,myColor);

if((currentPosition.cols[o].couleur==myColor) &&(currentPosition.cols[d].couleur !=myColor)
    &&(currentPosition.cols[d].nb + currentPosition.cols[d].nb == 2 )){
    if((voisSafe>voisNoSafe1) &&(voisNoSafe3==0)){
   
    liste_coups.coupsStandby[liste_coups.nb.standby].indice = i ; 
    liste_coups.coupsStandby[liste_coups.nb.standby].valeur = 5;
    liste_coups.nb.standby++ ;  
    }
}
//--------- STRATEGIE AVANTAGEUSE 2------------//
// ATTENTION CETTE STARTAEGIE S EXECUTE SOUVENT !!!!//
    int NbVoisinsAmis2 = calculerVoisins(currentPosition,voisD,1,2,myColor);
    int NbVoisinsAdverse3 = calculerVoisins(currentPosition,voisD,0,3,myColor);

      if((currentPosition.cols[o].couleur == myColor) 
        && (currentPosition.cols[d].couleur!=myColor)
        && (NbVoisinsAmis2 == 1)
        &&(NbVoisinsAdverse3 == 0)
        && (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 2))
    {
        liste_coups.coupsStandby[liste_coups.nb.standby].indice = i ; 
        liste_coups.coupsStandby[liste_coups.nb.standby].valeur = 3;
        liste_coups.nb.standby++ ;   
    }

    }
/*******************************************************************/
//-----------------------HIÉRARCHISATION-------------------------//
/*******************************************************************/
if ( (liste_coups.nb.offensif == 0) && (ouverture != 0) ){
    printf("\n-----COUP D OUVERTURE---\n");
    ecrireIndexCoup(ouverture);
    return ; 
}

    
    if ((liste_coups.nb.offensif != 0))
        {    
        for(int k=0 ;k<liste_coups.nb.offensif;k++)
            {
                if(liste_coups.coupsOffensif[k].valeur == 5){
                    ecrireIndexCoup(liste_coups.coupsOffensif[k].indice);
                    return ; 
                }}
        if ((liste_coups.nb.deffensif !=0 ))
            {            
                    for(int k=0 ;k<liste_coups.nb.deffensif;k++)
                    {
                        if(liste_coups.coupsDeffensif[k].valeur == 4){
                            ecrireIndexCoup(liste_coups.coupsDeffensif[k].indice);
                            return ; 
                        }  
                    }
            }
        for(int k=0 ;k<liste_coups.nb.offensif;k++)
            {
                 if(liste_coups.coupsOffensif[k].valeur == 4){
                    ecrireIndexCoup(liste_coups.coupsOffensif[k].indice);
                    return ; 
                }}
     }


    else if ( (liste_coups.nb.offensif ==0 ) && (liste_coups.nb.standby !=0) )
    { 
            for(int k=0 ;k<liste_coups.nb.standby;k++)
                {
                    if(liste_coups.coupsStandby[k].valeur == 5){
                        ecrireIndexCoup(liste_coups.coupsStandby[k].indice);
                        return ; 
                    }}
            for(int k=0 ;k<liste_coups.nb.standby;k++)
                {
                    if(liste_coups.coupsStandby[k].valeur == 4){
                        ecrireIndexCoup(liste_coups.coupsStandby[k].indice);
                        return ; 
                    }}
            for(int k=0 ;k<liste_coups.nb.standby;k++)
                {
                    if(liste_coups.coupsStandby[k].valeur == 3){ 
                        ecrireIndexCoup(liste_coups.coupsStandby[k].indice);
                        return ; 
                    }}  
            
    }
}
