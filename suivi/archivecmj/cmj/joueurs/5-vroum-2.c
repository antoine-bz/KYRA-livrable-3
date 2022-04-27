
/*******************VROUM : version officielle 31/05/2019*********************/
/*MADE BY Adrien DECOCK, Mariette LELEU, Charlotte RIGAULT, Amandine DERAEDT*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avalam.h"
#include "moteur.h"
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "topologie.h"


void enregistrer(T_Position p, const char *ch);
T_ListeCoups getCoupsLegaux2(T_Position p, int AquiLeTour, int myColor);

void test();

#define MAX 300


//Structure pour les piles
//Les piles servent à stocker les coups lors du parcours d'arbre
typedef struct
{
        int pil;
        int depa;
        int arri;
        int couleur_depa;
        int couleur_arri;
        int nb_de_pion_deplace;
}T_info;

typedef struct
{
        int sommet;
        T_info pile[MAX];
}T_pile;




//***************FONCTIONS PILES*******************//

///////////////fonction pour empiler ///////////////////

int donner_couleur_arri(T_Position p,int arri)
{
    return p.cols[arri].couleur;
}

int donner_couleur_depa( T_Position p,int depa)
{
    return p.cols[depa].couleur;
}

int donner_nombre_depa( T_Position p,int depa)
{
    return p.cols[depa].nb;
}

//Empiler = ajouter le coup dans la pile de stockage des coups + jouer le coup
void empiler( T_pile * p, int i, T_ListeCoups * l, T_Position ** partie)
{

    if ( p->sommet < MAX)
    {
        p->pile[p->sommet].pil = i;
        p->pile[p->sommet].depa = l->coups[i].origine;
        p->pile[p->sommet].arri = l->coups[i].destination;
        
        p->pile[p->sommet].couleur_depa = donner_couleur_depa(**partie, l->coups[i].origine);
        
        p->pile[p->sommet].couleur_arri = donner_couleur_arri(**partie, l->coups[i].destination);
        
        p->pile[p->sommet].nb_de_pion_deplace = donner_nombre_depa(**partie, l->coups[i].origine);

        **partie = jouerCoup( **partie , p->pile[p->sommet].depa , p->pile[p->sommet].arri  );
        
        p->sommet++;
    }
}

///////////////////fonction pour depiler ////////////////

T_Position revenir_un_coup_en_arriere(T_Position p, int depa, int arri, int couleur_depa, int couleur_arri,int a)
{
    //printf(" depa = %d\n", depa);
    //printf(" arri = %d\n", arri);

    p.cols[depa].nb = p.cols[depa].nb + a;
    p.cols[arri].nb = p.cols[arri].nb - a;
    p.cols[depa].couleur = couleur_depa;
    p.cols[arri].couleur = couleur_arri;

    return p;
}

//Dépiler = enlever le coup de la pile de stockage + "déjouer" le coup
int depiler( T_pile * p, T_Position ** partie)
{
    int i = 0;
        
    if(p->sommet > 0)
    {
        p->sommet--;
        i = p->pile[ p->sommet ].pil;

        **partie = revenir_un_coup_en_arriere(**partie, p->pile[ p->sommet ].depa, p->pile[ p->sommet].arri, p->pile[ p->sommet].couleur_depa, p->pile[ p->sommet].couleur_arri, p->pile[ p->sommet].nb_de_pion_deplace );
    }

    return i;
}


//Initialiser la pile
void initialisation(T_pile * p)
{
    p->sommet = 0;
}


//Afficher les différents coups présents dans la pile
void afficher(T_pile p)
{
    for(int i = 0 ; i < p.sommet ; i++)
    {
        printf(" [%d] le coup a ete joue de %d vers %d \n", i , p.pile[i].depa , p.pile[i].arri );
    }
        
    printf("\n\n");
}


//Renvoit l'indice du sommet de la pile
int trouver_tete_de_pile(T_pile * p)
{
    int i = 0;
    if(p->sommet > 0)
    {
        i = p->pile[ p->sommet-1 ].pil;
    }
    
    return i;
}


//Renvoit un si la pile est vide, 0 sinon
int pile_vide(T_pile * p)
{
        
    if( p->sommet == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//Donne la taille (nb de branches) de l'étage parent
int donner_taille( T_Position ** partie, T_ListeCoups * l, T_pile * p,int AquiLeTour, int myColor)
{
    int i;
    int taille;

    if(p->sommet != 0)
    {
        i = depiler(p, partie);
        AquiLeTour=-AquiLeTour;
        *l=getCoupsLegaux2(**partie, AquiLeTour, myColor);
        taille = l->nb;
        empiler( p ,i, l, partie);
        AquiLeTour=-AquiLeTour;
        *l=getCoupsLegaux2(**partie, AquiLeTour, myColor);
    }
    else
    {
        *l=getCoupsLegaux2(**partie, AquiLeTour, myColor);
        taille = l->nb;
    }

    return taille;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//***************FONCTIONS pour le parcours d'arbre : évaluer et réduire les branches*******************//


/*fonction pour évaluer le terrain :

En analysant le nombre de tours de nous et de l'adversaire, cette fonction va donner des iformations sur les tours.
On va séparer les tours gagnée de celles qui ne le sont pas.

Rappel : dans la fonction getVoisin 1=jaune et 2=rouge, dans celle que nous utilisons 1=jaune, -1=rouge*/

void NombreDeTour(T_Position p, int myColor, int * NbToursGagneeAdversaire, int * NbToursGagneeNous, int * NbToursAdversaire, int * NbToursNous)
{
    int petit_compteur = 0;
    int compteur;
    T_Voisins v;
    int NbRouge = 0;
    int NbJaune = 0;

    //**********Nombre de tours gagné : piles de 5 et piles définitives (sans voisins)**********
    //on parcours tout les pions du terrain
    for(int i = 0; i < 47; i++)
    {
        //On recupère les voisins du pion que l'on étudie
        v = getVoisins(i);

        //==>On compte les tours de 5 (piles définitives)
        if(p.cols[i].nb == 5 && p.cols[i].couleur == 1) {NbJaune++; }
        if(p.cols[i].nb == 5 && p.cols[i].couleur == 2) {NbRouge++; }
        
        
        petit_compteur = 0;
        compteur = 0;
        
        //==>On compte les tours sans voisins (piles définitives)
        
        //Pour les rouges
        //On regarde sur le plateau de jeu (p) si les voisin (v) d'indices "cD" (qui sont ceux du pions d'indice i que l'on étudie) est une tour compris entre 1 et 4
        //==>On compte les voisins qui ne sont pas une pile de 5 
        for(int cD = 0; cD < v.nb; cD++)//parcours et étude des voisins
        {   
            if( p.cols[ v.cases[cD] ].nb > 0 && p.cols[ v.cases[cD] ].nb < 5 ) compteur++;
        }
        //s'il n'y a pas de voisins autres (sauf les pile définitives de 5) ET que la pile étudier est rouge ET n'est pas une pile de 5
        if(compteur == 0 && p.cols[i].couleur == 2 && p.cols[i].nb < 5 && p.cols[i].nb > 0 ) 
        {
            NbRouge++; 
            petit_compteur++;
        }

        //Pour les jaunes
        compteur = 0;
        for(int cD = 0; cD < v.nb; cD++)//parcour des voisins
        {          
            if( p.cols[ v.cases[cD] ].nb > 0 && p.cols[ v.cases[cD] ].nb < 5 ) compteur++;
        }
        //s'il n'y a pas de voisins autres (sauf les pile définitives de 5) ET que la pile étudier est jaune ET n'est pas une pile de 5
        if(compteur == 0 && p.cols[i].couleur == 1 && p.cols[i].nb < 5 && p.cols[i].nb > 0 ) 
        {
            NbJaune++;  
            petit_compteur++;
        }
    }

    //Nombre de tours gagnées :
    //si on est jaune
    if( myColor == 1)
    { 
        *NbToursGagneeAdversaire = NbRouge;
        *NbToursGagneeNous = NbJaune;
    }
    //si on est rouge
    if( myColor == -1)
    {
        *NbToursGagneeAdversaire = NbJaune;
        *NbToursGagneeNous = NbRouge;        
    }


    //**********Nombre de piles de notre couleur**********
    NbRouge= 0;
    NbJaune= 0;
    
    //compte les piles jaunes et les piles rouges
    for(int i = 0; i < 47; i++)
    {
        if(p.cols[i].couleur == 1) NbJaune++;
        if(p.cols[i].couleur == 2) NbRouge++;
    }

    //si on est jaune
    if( myColor == 1)
    {
        *NbToursAdversaire = NbRouge;
        *NbToursNous = NbJaune;
    }
    //si on est rouge
    if( myColor == -1)
    {
        *NbToursAdversaire = NbJaune;
        *NbToursNous = NbRouge;        
    }

}



/*fonction pour réduire le nombre de branche et renvoyer une nouvelle liste de coups optimisée :

Réduire le nombre de coups à parcourir permet de faire gagner beaucoup de temps au porgramme.
La réduction du nombre de coups déccoule d'un choix arbitraire fait par notre groupe suite à l'étude du jeu Avalam.
Ce choix est détaillé ci-dessous.

En analysant les coups souvent joué et ceux qui ne sont que très rarement joué, nous avons fait le choix de supprimer ceux rarement joué de la liste de coup.
On suppose que l'adversaire ne va jamais jouer un pion de la couleur adversaire sur un de ses pions et très rarement jouer un pion de sa couleur sur un autre de sa couleur.
Pour notre bot, on peut dire la même chose, il ne devrait jamais jouer un pion de la couleur adverse sur l'un de ses pions.

Rappel : dans la fonction getVoisin 1=jaune et 2=rouge, dans celle que nous utilisons 1=jaune, -1=rouge*/

T_ListeCoups getCoupsLegaux2(T_Position p, int AquiLeTour, int myColor) 
{
    T_ListeCoups l ={0};
    int i;
    int compteur_tour_voisinne_non_vide;
    int cO, cD, iV; // case Origine, case Destination, indice Voisin
    T_Voisins v; 
    T_Voisins v2; 
    int cmp = 0;
    int compteur = 0;

    //Si on est jaune
    if( myColor == 1)
    {
    //Coup pour l'adversaire, càd pour les ROUGES
    /*On recherche et on liste les coups de l'adversaire, en commençant par les plus probables.
    Quand on estime qu'on a assez de coups "évidents", càd qu'on estime que le coups que va jouer l'adversaire est dans la liste
    ça ne sert à rien d'empiler les autres coups moins probables*/
       
        //On est jaune ET c'est à l'adversaire de jouer (l'adversaire est rouge soit couleur "2")
        if( AquiLeTour == -1)
        {
            //Dans un 1er temps, on suppose que s'il peut, l'adversaire va faire une tour de 5
            for(cO=0;cO<NBCASES;cO++) 
            {
                if (p.cols[cO].nb == VIDE ) {continue;}
                v = getVoisins(cO);
                for(iV=0;iV<v.nb;iV++) 
                {
                    cD = v.cases[iV]; 
                    if (p.cols[cD].nb == VIDE ) {continue;}
                    if (p.cols[cO].nb + p.cols[cD].nb == 5 && p.cols[cO].couleur == 2 )
                    {
                        addCoup(&l, cO,cD);
                        cmp++;
                    }
                }
            }
            
            //S'il ne peut pas faire de tour de 5, il va chercher la tour qu'il peut isoler
            if ( cmp == 0)
            {
                for(cO=0;cO<NBCASES;cO++) 
                {
                    if (p.cols[cO].nb == VIDE ) {continue;}
                    v = getVoisins(cO);
                    for(iV=0;iV<v.nb;iV++) 
                    {
                        cD = v.cases[iV]; 
                        v2 = getVoisins(cD);
                     
                        if (p.cols[cD].nb == VIDE ) {continue;}

                        //S'il trouve un pion isolé et que le seul voisin est un pion de la couleur de l'adversaire, je fais un addCoup a l'envers
                        if ( v2.nb == 1 && p.cols[cD].couleur == 2 && p.cols[cO].nb + p.cols[cD].nb <= 5)
                        {
                            addCoup(&l, cD,cO);
                            cmp++;
                        }
                    }
                }
            }
    
            //Sinon, il va faire un coups par defaut parmis les plus probables (on liste les coups des pions dont les voisins sont des tours de plus de 1 )
            if( cmp == 0)
            {
                for(cO=0;cO<NBCASES;cO++) 
                {                                           
                    if (p.cols[cO].nb == VIDE ) {printf0("vide!\n"); continue;}
                }
                    
                v = getVoisins(cO);       
                compteur=0;
              
                //Programme qui regarde si un pion a un voisin ou un voisin de voisin qui est une tour de plus de 1 pion empilé
                for(int cO2 = 0; cO2 < v.nb; cO2++)
                {
                    if(p.cols[ v.cases[cO2] ].nb > 1 && p.cols[ v.cases[cO2] ].nb < 5){ compteur++;}

                }
                // si le pion a des voisins je liste les coups
                if( compteur > 0)
                {
                    //le programme n'est pas interessé par des coups inutiles donc il trie
                    for(iV=0;iV<v.nb;iV++) 
                    {
                        cD = v.cases[iV]; 
                        if (p.cols[cD].nb == VIDE ) {printf0("vide!\n"); continue;}

                        if (p.cols[cO].nb + p.cols[cD].nb <= 5) 
                        {
                            if (p.cols[cO].couleur == 2 && p.cols[cD].couleur == 1 || p.cols[cO].couleur == 1 && p.cols[cD].couleur == 1|| p.cols[cO].couleur == 2 && p.cols[cD].couleur == 2 && p.cols[cO].nb + p.cols[cD].nb == 5 ) 
                            {
                                addCoup(&l, cO,cD);
                            }
                                    
                        }
                    }
                }  
            }
        }
    


        //On est jaune ET c'est à nous de jouer
        if( AquiLeTour == 1)
        {
            //On prend les coups par defaut (les plus probables) pour laisser au bot la liberté de pouvoir sacrifier des pions ou de prendre ceux adverse
            for(cO=0;cO<NBCASES;cO++)
            {
                if (p.cols[cO].nb == VIDE ) {printf0("vide!\n"); continue;}
                v = getVoisins(cO);
                compteur=0;
                //Programme qui regarde si un pion a un voisin ou un voisin de voisin qui est une tour de plus de 1 pion empilé
                if( p.cols[cO].nb < 5 && p.cols[cO].nb > 1) {compteur++; }     // je regarde si la tour est entre 4 et 2
                    
                for(int cO2 = 0; cO2 < v.nb; cO2++)
                {
                    //si le pion d'origine a un voisin de plus entre 2 et 4
                    if(p.cols[ v.cases[cO2] ].nb > 1 && p.cols[ v.cases[cO2] ].nb < 5){ compteur++;}
                }
                    
              
                if( compteur > 0)
                {     
                    //Le programme n'est pas interessé par des coups inutiles
                    for(iV=0;iV<v.nb;iV++)
                    {
                        cD = v.cases[iV]; 
                        if (p.cols[cD].nb == VIDE ) {printf0("vide!\n"); continue;}

                        if (p.cols[cO].nb + p.cols[cD].nb <= 5) 
                        {
                            if ( p.cols[cO].couleur == 1 && p.cols[cD].couleur == 2 || p.cols[cO].couleur == 2 && p.cols[cD].couleur == 2) 
                            {
                                addCoup(&l, cO,cD);
                            }

                        }
                    }
                }
            }
        }

    }
     
    //Si on est rouge
    if( myColor == -1)
    {
    //Coup pour l'adversaire, càd pour les JAUNES
    /*On recherche et on liste les coups de l'adversaire, en commençant par les plus probables.
    Quand on estime qu'on a assez de coups "évidents", càd qu'on estime que le coups que va jouer l'adversaire est dans la liste
    ça ne sert à rien d'empiler les autres coups moins probables*/

        //On est rouge ET c'est à l'adversaire de jouer (l'adversaire est jaune soit couleur "1")
        if( AquiLeTour == -1)
        {
                
                
            //Dans un 1er temps, on suppose que s'il veut, l'adversaire va faire une tour de 5 
            for(cO=0;cO<NBCASES;cO++) 
            {
                if (p.cols[cO].nb == VIDE ) {continue;}
                v = getVoisins(cO);
                for(iV=0;iV<v.nb;iV++) 
                {
                    cD = v.cases[iV]; 
                    if (p.cols[cD].nb == VIDE ) {continue;}
                    if (p.cols[cO].nb + p.cols[cD].nb == 5 && p.cols[cO].couleur == 1 ) 
                    {
                        addCoup(&l, cO,cD);
                        cmp++;
                    }
                }
            }
                
            //S'il ne peut pas faire de tour de 5, il va chercher la tour qu'il peut isoler
            if ( cmp == 0)
            {
                for(cO=0;cO<NBCASES;cO++) 
                {
                    if (p.cols[cO].nb == VIDE ) {continue;}
                    v = getVoisins(cO);
                    for(iV=0;iV<v.nb;iV++) 
                    {
                        cD = v.cases[iV]; 
                        v2 = getVoisins(cD);
                        if (p.cols[cD].nb == VIDE ) {continue;}
                            
                        // si il trouve un pion isolé et que la seule voisine est un pion de la couleur de l'adversaire. je fais un addCoup a l'envers.
                        if ( v2.nb == 1 && p.cols[cD].couleur == 1 && p.cols[cO].nb + p.cols[cD].nb <= 5) 
                        {  
                            addCoup(&l, cD,cO);
                            cmp++;
                        }
                    }
                }
            }
    
            //Sinon il va faire un coup par defaut parmis les plus probables (on liste les coups des pions dont les voisins sont de tours de plus de 1 )
            if( cmp == 0)
            {
                    for(cO=0;cO<NBCASES;cO++) 
                    {
                        if (p.cols[cO].nb == VIDE ) {printf0("vide!\n"); continue;}
                        v = getVoisins(cO);
                         compteur=0;

                        //Programme qui regarde si un pion a un voisin ou un voisin de voisin qui est une tour de plus de 1 pion empilé
                        for(int cO2 = 0; cO2 < v.nb; cO2++)
                        {
                            if(p.cols[ v.cases[cO2] ].nb > 1 && p.cols[ v.cases[cO2] ].nb < 5){ compteur++;}
                        }                   
                        //Si le pion a des voisins je liste les coups
                        if( compteur > 0)
                        {                  
                            // le programme n'est pas interessé par des coups inutiles
                            for(iV=0;iV<v.nb;iV++) 
                            {
                                cD = v.cases[iV]; 
                                if (p.cols[cD].nb == VIDE ) {printf0("vide!\n"); continue;}

                                if (p.cols[cO].nb + p.cols[cD].nb <= 5) 
                                {
                                    if (p.cols[cO].couleur == 1 && p.cols[cD].couleur == 2 || p.cols[cO].couleur == 2 && p.cols[cD].couleur == 2|| p.cols[cO].couleur == 1 && p.cols[cD].couleur == 1 && p.cols[cO].nb + p.cols[cD].nb == 5 )
                                    {
                                        addCoup(&l, cO,cD);
                                    }
                                }
                            }
                        }
                    }
            }
        }

        //On est Rouge ET c'est à nous de jouer
        if( AquiLeTour == 1)
        {
            //On prend les coups par défaut (les plus probables) pour laisser à notre bot la liberté de pouvoir sacrifier des pions ou de prendre ceux adverses
            for(cO=0;cO<NBCASES;cO++) 
            {
                if (p.cols[cO].nb == VIDE ) {printf0("vide!\n"); continue;}
                v = getVoisins(cO);
                compteur=0;
                //Programme qui regarde si un pion a un voisin ou un voisin de voisin qui est une tour de plus de 1 pion empilé
                if( p.cols[cO].nb < 5 && p.cols[cO].nb > 1) {compteur++; }     // je regarde si la tour est entre 4 et 2
                    
                for(int cO2 = 0; cO2 < v.nb; cO2++)
                {
                    if(p.cols[ v.cases[cO2] ].nb > 1 && p.cols[ v.cases[cO2] ].nb < 5){ compteur++;}   // si le pion d'origine a un voisin de plus entre 2 et 4
                }


                if( compteur > 0)
                {                   
                    //Le programme n'est pas interessé par des coups inutiles
                    for(iV=0;iV<v.nb;iV++) 
                    {
                        cD = v.cases[iV]; 
                        if (p.cols[cD].nb == VIDE ) {printf0("vide!\n"); continue;}

                        if (p.cols[cO].nb + p.cols[cD].nb <= 5) 
                        {
                            if ( p.cols[cO].couleur == 2 && p.cols[cD].couleur == 1 || p.cols[cO].couleur == 1 && p.cols[cD].couleur == 1) 
                            {
                                addCoup(&l, cO,cD);
                            }

                        }
                    }
                }
            }        
        }
    }
     
    //Cas où il n'y a aucun coup retenue par les cas énuméré ci-dessus :
    if( l.nb == 0 )
    {
        for(cO=0;cO<NBCASES;cO++) 
        {
            if (p.cols[cO].nb == VIDE  ) { printf0("vide!\n"); continue;}
            v = getVoisins(cO); 
            for(iV=0;iV<v.nb;iV++) 
            {
                cD = v.cases[iV]; 
                if (p.cols[cO].nb + p.cols[cD].nb <= 5)
                {   
                    if(p.cols[cD].nb != VIDE || p.cols[cD].nb != VIDE)
                    {
                        addCoup(&l, cO,cD); 
                    }
                }
            }
        }
    }
        
    //On retourne la nouvelle liste de coups optimisés
    return l;
}



/*fonction pour parcourir les branches de l'arbre :

Le parcour d'arbre a pour but de choisir un coup à jouer. 
Pour cela, il va parcourir toutes les branches de l'arbre des coups optimisé (récupéré par getCoupsLegaux2), évaluer le score de cette branche pour pouvoir trouver le meilleur coups.

Rappel : 
-dans la fonction 1=jaune, -1=rouge
-"AquiLeTour" permet de nous donner le tarit (si c'est à nous de jouer où à l'adversaire), quand le trait est nous : AquiLeTour=1 ; sinon il est égal à -1
*/

void parcourArbre( T_Position * partie, int * dep, int * arrive, T_ListeCoups * l, int myColor, int nb_de_coups_a_prevoir, int * test)
{
    int i = 0;
    T_pile p;

    initialisation(&p);

    //Variables pour le parcours d'arbre
    int taille = 0;
    int ind = 0;
    int sw;
    int cp1 = 0;
    int a = 0;
    int ind2;
    int saisie;
    int end;
    int AquiLeTour = 1;

    //Variables pour évaluer le terrain
    int pointsTerrainPourEvaluerPireTerrainDeLa1erBranche = 0;
    int JeSuisLaPirePartieDeLa1erBranche = 30;
    int Meilleure1erbrancheQuiProposeSaPirePartie = -30;

    int NbToursGagneeAdversaireAv;
    int NbToursAdversaireAv;
    int NbToursGagneeNousAv;
    int NbToursNousAv;

    int NbToursGagneeAdversaireAp;
    int NbToursAdversaireAp;
    int NbToursGagneeNousAp;
    int NbToursNousAp;

    int NbToursGagneeAdversairePendant;
    int NbToursAdversairePendantEnMoins;
    int NbToursGagneeNousPendant;
    int NbToursNousPendantEnMoins;

    //Evaluer le terrain initial
    NombreDeTour( *partie, myColor, &NbToursGagneeAdversaireAv, &NbToursGagneeNousAv, &NbToursAdversaireAv , &NbToursNousAv );

    //Récupérer la liste de coups optimisés
    *l=getCoupsLegaux2(*partie, AquiLeTour, myColor);
    taille = l->nb;

    //Empiler le premier coup (de la liste optimisé) avant le debut de le recherche (+changer "AquiLeTour")
    empiler( &p ,i, l, &partie);   
    AquiLeTour=-AquiLeTour;

    //récupérer la nouvelle liste de coups optimisés
    *l=getCoupsLegaux2(*partie, AquiLeTour, myColor);

    end = donner_taille(&partie, l, &p, AquiLeTour, myColor);

    //parcours de l'arbre
    while((p.pile[p.sommet-1].depa != 0 || p.pile[p.sommet-1].arri != 0 || cp1 !=0 ) && ( p.sommet != 0 || cp1 < end-1))
    {
        //printf(" cp1 = %d et end-1 = %d\n", cp1, end-1 );
        //printf(" p.sommet = %d\n", p.sommet );
        //printf("p.pile[p.sommet].depa = %d\n", p.pile[p.sommet-1].depa );
        //printf("p.pile[p.sommet].arri = %d\n", p.pile[p.sommet-1].arri );

        sw = 0;
        //afficher(p);
        //printf("\n");

        *l=getCoupsLegaux2(*partie, AquiLeTour, myColor);
        taille = donner_taille(&partie, l, &p, AquiLeTour, myColor);

        //printf(" nb_de_coups_a_prevoir =%d\n", nb_de_coups_a_prevoir);
        //printf("taille = %d", taille);

        //On va différencier différentes situation car on agira differement en fonction de la situation

        //case 2 : Si on a atteint la profondeur choisie ET qu'on est à la dernière branche
        if( p.sommet == nb_de_coups_a_prevoir && a == taille-1) sw = 2;

        //case 4 : Si on a atteint la profondeur choisie ET qu'on n'est PAS à la dernière branche
        if( p.sommet == nb_de_coups_a_prevoir && a < taille-1) sw = 4;

        //case 3 : Si on n'est PAS à la profondeur choisie ET PAS à la dernière branche
        if( p.sommet < nb_de_coups_a_prevoir && a <= taille-1 && ind == 0) sw= 3;

        //case 1 : Si on n'est PAS à la profondeur choisie ET à la dernière branche
        if( p.sommet < nb_de_coups_a_prevoir && a <= taille-1 && ind == 1) sw= 1;

        //Cas où il faut prévoir que 1 coup à l'avance DONC : Profondeur=1
        if( taille == 1 && nb_de_coups_a_prevoir == 1)
        {
            //si on est à la dernière branche ET qu'on est à la profondeur 1
            if( p.sommet == nb_de_coups_a_prevoir && a == taille-1) sw = 4;
            //si on n'est PAS à la dernière branche ET qu'on est à la profondeur 0 (pas encore de coup dans la pile)
            if( p.sommet < nb_de_coups_a_prevoir && a <= taille-1 && ind == 0) sw= 3;
            //si on est à la dernière branche ET qu'on est à la profondeur 0 (pas encore de coup dans la pile)
            if( p.sommet < nb_de_coups_a_prevoir && a <= taille-1 && ind == 1) sw= 1;
        }

        a = i;

        //Dans le cas où on a atteint la profondeur choisie : on évalue le terrain
        if( sw == 2 || sw == 4)
        {
        
            //points = points + EvaluerTerrain(*partie, Evaluation1, myColor);
            //printf("points 2 = %d\n", points);

            //Calcul du score et analyse des résultats obtenus
            NombreDeTour( *partie, myColor, &NbToursGagneeAdversaireAp, &NbToursGagneeNousAp, &NbToursAdversaireAp , &NbToursNousAp );
            //On décide qu'une tour gagnée (pile définitive) est plus importante qu'une tour à nous ==>donc multiplie leur nombre par 4 pour leur donner plus de poid
            NbToursGagneeAdversairePendant = (NbToursGagneeAdversaireAp - NbToursGagneeAdversaireAv)*4;//Idéal : doit tendre vers 0
            NbToursGagneeNousPendant = (NbToursGagneeNousAp - NbToursGagneeNousAv)*4;//Idéal : doit tendre vers l'infini
            NbToursAdversairePendantEnMoins = NbToursAdversaireAp - NbToursAdversaireAv;//Idéal : doit tendre vers l'infini
            NbToursNousPendantEnMoins = NbToursNousAp - NbToursNousAv;//Idéal  : doit tendre vers 0 
                
            //printf("NbToursGagneeAdversairePendant = %d\n", NbToursGagneeAdversairePendant);
            //printf("NbToursGagneeNousPendant = %d\n", NbToursGagneeNousPendant);                
            //printf("NbToursAdversairePendantEnMoins = %d\n", NbToursAdversairePendantEnMoins);
            //printf("NbToursNousPendantEnMoins = %d\n", NbToursNousPendantEnMoins);
                
            pointsTerrainPourEvaluerPireTerrainDeLa1erBranche = -NbToursGagneeAdversairePendant+NbToursGagneeNousPendant+NbToursAdversairePendantEnMoins-NbToursNousPendantEnMoins;
                
            //printf(" pointsTerrainPourEvaluerPireTerrainDeLa1erBranche = %d \n", pointsTerrainPourEvaluerPireTerrainDeLa1erBranche );
                
            if( JeSuisLaPirePartieDeLa1erBranche >= pointsTerrainPourEvaluerPireTerrainDeLa1erBranche )
            {
                JeSuisLaPirePartieDeLa1erBranche = pointsTerrainPourEvaluerPireTerrainDeLa1erBranche;
            }
        }


        switch(sw)
        {
            //case 1 : Si on n'est PAS à la profondeur choisie ET à la dernière branche
            //On empile un coup et on change le trait
            case 1: empiler( &p ,i, l, &partie);
                    AquiLeTour=-AquiLeTour;
                    i = 0;
                    ind = 0;
            break;

            //case 2 : Si on a atteint la profondeur choisie ET qu'on est à la dernière branche
            //On dépile le dernier coup (+changer le trait) ET on dépile jusqu'à ce qu'on soit revenue au premier noeud (càd qu'on a empilé et dépilé tout les coups)
            case 2: i = depiler(&p, &partie);
                    AquiLeTour=-AquiLeTour;
                    a = i;
                    ind2 = 0;

                    while(a >= taille-1)
                    {
                        i = depiler(&p, &partie);
                        AquiLeTour=-AquiLeTour;
                        a = i;
                        ind2++;
                        *l=getCoupsLegaux2(*partie, AquiLeTour, myColor);
                        taille = l->nb;

                        if(p.sommet == 0 )
                        {
                            //printf(" JeSuisLaPirePartieDeLa1erBranche = %d \n", JeSuisLaPirePartieDeLa1erBranche);
                            //printf(" Meilleure1erbrancheQuiProposeSaPirePartie = %d \n", Meilleure1erbrancheQuiProposeSaPirePartie);
                            
                            //permet de gérer le fait qu'il puisse y avoir dans une branche avec le meilleur coup mais AUSSI le pire coup (et on ne peux pas choisir le coups adverses)
                            if( JeSuisLaPirePartieDeLa1erBranche > Meilleure1erbrancheQuiProposeSaPirePartie && JeSuisLaPirePartieDeLa1erBranche !=30)
                            {
                                Meilleure1erbrancheQuiProposeSaPirePartie = JeSuisLaPirePartieDeLa1erBranche;
                                *dep = p.pile[0].depa;
                                *arrive = p.pile[0].arri;
                                printf(" Meilleure1erbrancheQuiProposeSaPirePartie = %d \n", Meilleure1erbrancheQuiProposeSaPirePartie);
                            }
                            JeSuisLaPirePartieDeLa1erBranche = 30;
                        }
                    }
                
                    i++;
                    if( ind2 > 0) ind = 1;
                    a = trouver_tete_de_pile(&p);//c'est le nb de coups stockés dans la pile
                    if( p.sommet == 0)  a = 0; 
            break;

            //case 3 : Si on n'est PAS à la profondeur choisie ET PAS à la dernière branche
            //On empile un coup et on change de trait
            case 3:
                    empiler( &p ,i, l, &partie);
                    AquiLeTour=-AquiLeTour;
                    ind = 1;
            break;
            
            //case 4 : Si on a atteint la profondeur choisie ET qu'on n'est PAS à la dernière branche
            //On dépile le dernier coup (+changer le trait) ET on regarde si on est revenue au premier noeud (càd qu'on a empilé et dépilé tout les coups)
            case 4:
                    i = depiler(&p, &partie);
                    AquiLeTour=-AquiLeTour;
                    i++;
                    a = trouver_tete_de_pile(&p);//c'est le nb de coups stockés dans la pile
                    //Si on est revenu au noeud de départ (càd qu'il n'y a plus de coups dans la pile)
                    if(p.sommet == 0 )
                    {
                        printf(" JeSuisLaPirePartieDeLa1erBranche = %d \n", JeSuisLaPirePartieDeLa1erBranche);
                        printf(" Meilleure1erbrancheQuiProposeSaPirePartie = %d \n", Meilleure1erbrancheQuiProposeSaPirePartie);
                        //scanf("%d", &saisie);
                        
                        //permet de gérer le fait qu'il puisse y avoir dans une branche avec le meilleur coup mais AUSSI le pire coup (et on ne peux pas choisir le coups adverses)
                        if( JeSuisLaPirePartieDeLa1erBranche > Meilleure1erbrancheQuiProposeSaPirePartie && JeSuisLaPirePartieDeLa1erBranche !=30)
                        {
                            Meilleure1erbrancheQuiProposeSaPirePartie = JeSuisLaPirePartieDeLa1erBranche;
                            *dep = p.pile[0].depa;
                            *arrive = p.pile[0].arri;
                        }
                        JeSuisLaPirePartieDeLa1erBranche = 30;
                    }    
            break;
        }
        
        //Si la pile de stockage des coups est vide ET qu'on a parcourus tout les coups (<=> empiler et dépiler tout les coups <=> nb de coups stockés dans la pile=0)
        if(p.sommet == 0 && a == 0)
        {
            cp1++;
        }
    }
}



//////////////////  en plus jusque 

void convertir_en_FEN(T_Position p, char * ch_partie){

char FEN[20] = "udtqcUDTQC";
int position;
int nb_vide;
int a = 0;
char char_nb[5];
int b;

   for(int i = 0; i < 48; i++){
        
        nb_vide = 0;
        
        while(p.cols[i].nb == 0){
                nb_vide++;
                i++;
        }
        
        if(nb_vide > 0){
                sprintf(char_nb,"%d",nb_vide);
                b = 0;
                while( char_nb[b] != '\0'){
                        ch_partie[a] = char_nb[b];
                        a++; b++;
                }
        }
        
        if(p.cols[i].nb > 0){
                position = p.cols[i].nb+ 5*(p.cols[i].couleur-1);
                position--;
                ch_partie[a] = FEN[position];
                a++;
        
        }
   }
        ch_partie[a-1] = '\0';
}
///////////////////////// la



/*fonction Principale qui va choisir le coup à jouer :

Cette fonction va choisir le coup à jouer, pour cela elle va faire appel aux fonctions du parcour d'arbre. 
C'est également dans cette fonction que se fait le premier coup correspondant à une de nos stratégies coups de défense face à une stratégie des autres joueurs.
Ainsi qu'une partie de l'analyse du meilleur coup.

Rappel : 
-dans la fonction nous considérons que 1=jaune, -1=rouge, dans les autres fonctions 1=jaune et 2=rouge (c'est pour cela qu'il y a quelques lignes en plus)
*/
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups)
{
    int i; 
    octet o, d; 
    int myColor = currentPosition.trait;//on récupère notre couleur
    int nb_de_coups_a_prevoir = 2;
    
    //Dans un premier temps on recupere les infos sur la partie et on l'integre a l'ancienne structure de données du livrable 1
    T_Position p; 
    T_ListeCoups l; 
    int arr;
    int dest;
    int test;
    
    int meilleurCoupALavance = -30;
    int meilleurArri;
    int meilleurDest;
    int meilleurNbcoupAPrevoir;
char ch_partie[50];   // en plus

    //on charge les données de la partie sur l'ancienne structure du livreble 1
    //Explication de ce choix : au départ on utilisait la structure du livrable 1, càd p au lieu de currentPosition
    //donc à chaque fois p<=>currentPosition donc pour ne pas changer tout le code et oublier des parties à changer, on copie la structure dans l'autre
    for(i = 0; i < 48 ; i++)
    {
        p.cols[i].nb = currentPosition.cols[i].nb;
        p.cols[i].couleur = currentPosition.cols[i].couleur;
    }

    //CHOIX FAIT SUR L'EXPRESSION DES COULEURS : -1==ROUGE et 1==JAUNE
    //Explication de ce choix : au départ, pour simplifier le parcour d'arbre, le joueur ne jouait que en étant jaune donc "jaune=notreBot=1", "rouge=adversaire=-1"
    //Cela permet de passer facilement d'une couleur à une autre ==> "rouge=-jaune"
    //La ligne permet donc d'adapter plus facilement le parcours d'arbre aux 2 couleurs
    if( myColor == 2) { myColor = -1; }

    //printf("myColor = %d\n", myColor);
        
    l = getCoupsLegaux( p);
    
        convertir_en_FEN(p, ch_partie);  // en plus jusque 
    
            int FEN_ori = 300;
            int FEN_des = 300;
            if( myColor == -1 ){
        
                
                        if( strcmp("UuUuUuuUuUuUUuUuUuUuUuUuuUuUuUuUuUuUUuUuUuuUuDu1", ch_partie ) == 0 ){
                                FEN_ori = 46;
                                FEN_des = 42;
                        }
                        
                        if( strcmp("UuUuUuuUuUuUUuUuUuUuUuUuuUuUuUuUuUuUUuUuTudUu3", ch_partie ) == 0 ){
                                FEN_ori = 42;
                                FEN_des = 37;
                        }
                        
                        if( strcmp("UuUuUuuUuUuUUuUuUuUuUuUuuUuUuUuUuUuUUtd1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 33;
                                FEN_des = 38;
                        }
                        
                        
                        if( strcmp("UuUuUuuUuUuUUuUuUuUuUuUuuUuUuDuUu1uU1tT1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 32;
                                FEN_des = 24;
                        }
                        
                        if( strcmp("UuUuUuuUuUuUUuUuUuUuUuUudUuDuDuU2u2tT1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 24;
                                FEN_des = 15;
                        }
                        
                        if( strcmp("UuUuUuuUuUuUUuUtUuUuUuUu1duDuDuU5tT1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 26;
                                FEN_des = 17;
                        }
                        
                        if( strcmp("UuUuUuuUuUuUUuUtUdUuUuDu1d1DuDu6tT1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 37;
                                FEN_des = 30;
                        }

                        if( strcmp("UuUuUuuUuUuUUuUtUdUuTuDu1d1Du1q7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 21;
                                FEN_des = 30;
                        }

                        if( strcmp("UuUuUuuUuUuUUuUttdUuT1Du3Du1c7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 17;
                                FEN_des = 16;
                        }
                        if( strcmp("UuUuUuuUuUuUUuUtc1TuT1Du4u1c7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 20;
                                FEN_des = 28;
                        }

                        if( strcmp("UuUuUuuUuUuUUuUtc1Tu2t5Q1c7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 13;
                                FEN_des = 6;
                        }

                        if( strcmp("UuUuUudDuUuUU2tc1Tu2t5Q1c7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 12;
                                FEN_des = 22;
                        }

                        if( strcmp("UuUuUudDud1U3tc1Tu2Q5Q1c7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 18;
                                FEN_des = 19;
                        }

                        if( strcmp("UuUuUudt1d1U3tc2Q2Q5Q1c7T1Tu1Uu3", ch_partie ) == 0 ){
                                FEN_ori = 43;
                                FEN_des = 44;
                        }

                        if( strcmp("UuUuUudt1d1U3tc2Q2Q5Q1c7T1q3D3", ch_partie ) == 0 ){
                                FEN_ori = 7;
                                FEN_des = 6;
                        }

                        if( strcmp("UuUutuc4U3tc2Q2Q5Q1c7T1q3D3", ch_partie ) == 0 ){
                                FEN_ori = 3;
                                FEN_des = 1;
                        }

                        if( strcmp("t1U1tuc4U3tc2Q2Q5Q1c7T1q3D3", ch_partie ) == 0 ){
                                FEN_ori = 2;
                                FEN_des = 0;
                        }

                arr = FEN_ori;
                dest = FEN_des;
               for(i=0;i<listeCoups.nb; i++) {
		        o = listeCoups.coups[i].origine; 
		        d = listeCoups.coups[i].destination;  
                	if ( o == arr && dest == d)  {
			        ecrireIndexCoup(i);
		        }
	        }

        }
   
   
   
     // là
    
 if( FEN_des == 300 && FEN_ori == 300){        // en plus
    
    //Si c'est le premier coup
    if( l.nb == 292)
    { 
        if( myColor == 1){
        arr = 29;
        dest = 22;
        
        for(i=0;i<listeCoups.nb; i++) 
        {
            o = listeCoups.coups[i].origine; 
            d = listeCoups.coups[i].destination;  
            
            if ( o == arr && dest == d)  
            {
                 ecrireIndexCoup(i);
            }
        }
    }
        //ne sert à rien...       
        if( myColor == -1)
        {
            arr = 3;
            dest = 8;
        
            for(i=0;i<listeCoups.nb; i++) 
            {
                o = listeCoups.coups[i].origine; 
                d = listeCoups.coups[i].destination;  
            
                if ( o == arr && dest == d)  
                {
                    ecrireIndexCoup(i);
                }
            }
                
        }
    }
    else
    {
        //Prédit n coups à l'avance : prédit des coups jusqu'à ce que le temps manque (limite fixer à 8 soit 7 coups)
        for(nb_de_coups_a_prevoir=1;nb_de_coups_a_prevoir<8;nb_de_coups_a_prevoir++)
        {
            arr = 100;
            dest = 100;

            parcourArbre(&p, &arr, &dest, &l, myColor,  nb_de_coups_a_prevoir, &test );

            printf("le meilleur coup est de %d -> %d et il peut faire rappoter au minimum %d points en prevoyant %d a l avance\n", arr, dest, test, nb_de_coups_a_prevoir);
        
            if( (test > meilleurCoupALavance || nb_de_coups_a_prevoir == 2 )&& arr >= 0 && arr <= 47 && dest >= 0 && dest <= 47 && arr != 100 && dest != 100)
            {
                meilleurCoupALavance = test;
                meilleurArri = arr;
                meilleurDest = dest;
                meilleurNbcoupAPrevoir = nb_de_coups_a_prevoir;

                        
                if(meilleurCoupALavance!=-30 && meilleurArri!=100 && meilleurDest!=100)
                {
                    printf("le meilleur coup est de %d -> %d et il peut faire rappoter au minimum %d points en prevoyant %d a l avance\n", meilleurArri, meilleurDest , meilleurCoupALavance, meilleurNbcoupAPrevoir);

                    //On cherche l'indice du coup dans la liste pour écrire le coup
                    for(i=0;i<listeCoups.nb; i++) 
                    {
                        o = listeCoups.coups[i].origine; 
                        d = listeCoups.coups[i].destination;  

                        if ( o == arr && dest == d)  
                        {
                            //coup choisi par le programme
                            ecrireIndexCoup(i);
                        }
                    }
                }
            }
        }
    }
}	// en plus
    return;
     
}
