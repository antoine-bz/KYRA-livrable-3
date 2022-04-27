/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "../include/avalam.h"
#include "../include/moteur.h"
#include "../include/topologie.h"
//livrable 3




int coin1fini(T_Position pos)
{
    printf("coinfini");
    if(pos.cols[0].couleur == 0 && pos.cols[1].couleur == 0) //pas de pion dur la case 0 et la case 1
    {
        return VRAI;
    }
    else if((pos.cols[0].couleur == 0 && nbVoisins(1) == 0) || (nbVoisins(0)==0 && pos.cols[1].couleur == 0)) 
    {
        //ici soit la case 0 est vide et la 1 n'a pas de voisins soit la case 1 est vide et la case 0 n'a pas de voisins
        //donc en gros s'il y a un seul pion isolé dans le coins haut gauche
        return VRAI;
    }
    else{
        return FAUX; //s'il reste un ou plusieurs pion non isolés dans le coins haut gauche
    }

}

int coin2fini(T_Position pos) //on repète pareil mais pour le coin haut droite
{
    if(pos.cols[19].couleur == 0 && pos.cols[27].couleur == 0) 
    {
        return VRAI;
    }
    else if((pos.cols[19].couleur == 0 && nbVoisins(27) == 0) || (nbVoisins(19)==0 && pos.cols[27].couleur == 0)) 
    {

        return VRAI;
    }
    else{
        return FAUX; 
    }

}

int coin3fini(T_Position pos) //on repète pareil mais pour le coin bas gauche
{

    if(pos.cols[20].couleur == 0 && pos.cols[28].couleur == 0) 
    {
        return VRAI;
    }
    else if((pos.cols[20].couleur == 0 && nbVoisins(28) == 0) || (nbVoisins(20)==0 && pos.cols[28].couleur == 0)) 
    {

        return VRAI;
    }
    else{
        return FAUX; 
    }

}

int coin4fini(T_Position pos) //on repète pareil mais pour le coin bas doite
{
    if(pos.cols[46].couleur == 0 && pos.cols[47].couleur == 0) 
    {
        return VRAI;
    }
    else if((pos.cols[46].couleur == 0 && nbVoisins(47) == 0) || (nbVoisins(46)==0 && pos.cols[47].couleur == 0)) 
    {

        return VRAI;
    }
    else{
        return FAUX; 
    }

}

T_Coup takeCorner(T_Position P)//matheo
{
    int i=0;
    T_Voisins lesVoisin1;
    T_Voisins lesVoisin2;
    T_Coup leCoup;


    if(coin1fini == FAUX)
    {
        lesVoisin1=getVoisins(0);
        lesVoisin2=getVoisins(1);
        if(P.cols[0].couleur != P.trait && lesVoisin1.nb >0) 
        {
            for(i=0 ;i<lesVoisin1.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],0) == FAUX) && ((P.cols[i].nb)+(P.cols[0].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin1.cases[i];
                    leCoup.destination=0;
                    return leCoup;
                    printf("take corner 1");
                }
            }
        }
        else if(P.cols[1].couleur != P.trait && lesVoisin2.nb >0) 
        {
            for(i=0 ;i<lesVoisin2.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],1) == FAUX) && ((P.cols[i].nb)+(P.cols[1].nb) < 6) ) 
                {
                    leCoup.origine=lesVoisin2.cases[i];
                    leCoup.destination=1;
                    return leCoup;
                    printf("take corner 2");
                }
            }
        }
    }
    //juste au dessus on regarde si l'ennemi possède la tour à l'emplacement 0 si oui et sinous possédons
    //une tour voisine a la tour 0 on joue cette tour sur la tour 0 ennemi 
    //on fait aussi de meme pour l'emplacement 1 ainsi on aura couvert le coin 1 puis on fera les autres coins 
    else if(coin2fini == FAUX) //on couvre mtn le coin 2
    {
        lesVoisin1=getVoisins(19);
        lesVoisin2=getVoisins(27);
        if(P.cols[19].couleur != P.trait && lesVoisin1.nb >0) 
        {
            for(i=0 ;i<lesVoisin1.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],19) == FAUX) && ((P.cols[i].nb)+(P.cols[19].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin1.cases[i];
                    leCoup.destination=19;
                    return leCoup;
                }
            }
        }
        else if(P.cols[27].couleur != P.trait && lesVoisin2.nb >0) 
        {
            for(i=0 ;i<lesVoisin2.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],27) == FAUX) && ((P.cols[i].nb)+(P.cols[27].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin2.cases[i];
                    leCoup.destination=27;
                    return leCoup;
                }
            }
        }
    }
    else if(coin3fini == FAUX) //coin 3
    {
        lesVoisin1=getVoisins(20);
        lesVoisin2=getVoisins(28);
        if(P.cols[20].couleur != P.trait && lesVoisin1.nb >0) 
        {
            for(i=0 ;i<lesVoisin1.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],20) == FAUX) && ((P.cols[i].nb)+(P.cols[20].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin1.cases[i];
                    leCoup.destination=20;
                    return leCoup;
                }
            }
        }
        else if(P.cols[28].couleur != P.trait && lesVoisin2.nb >0) 
        {
            for(i=0 ;i<lesVoisin2.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],28) == FAUX) && ((P.cols[i].nb)+(P.cols[28].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin2.cases[i];
                    leCoup.destination=28;
                    return leCoup;
                }
            }
        }
    }
    else if(coin4fini == FAUX)
    {
        lesVoisin1=getVoisins(46);
        lesVoisin2=getVoisins(47);
        if(P.cols[46].couleur != P.trait && lesVoisin1.nb >0) 
        {
            for(i=0 ;i<lesVoisin1.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],46) == FAUX) && ((P.cols[i].nb)+(P.cols[46].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin1.cases[i];
                    leCoup.destination=46;
                    return leCoup;
                }
            }
        }
        else if(P.cols[47].couleur != P.trait && lesVoisin2.nb >0) 
        {
            for(i=0 ;i<lesVoisin2.nb ;i++)
            {
                if((memeCouleur(P,lesVoisin1.cases[i],47) == FAUX) && ((P.cols[i].nb)+(P.cols[47].nb) < 6)) 
                {
                    leCoup.origine=lesVoisin2.cases[i];
                    leCoup.destination=47;
                    if(testGlobales(leCoup, P)==VRAI)
                    { 
                        return leCoup; //stratégie 7 
                    }
                }
            }
        }
    }
    else{
        leCoup.origine=48;
        leCoup.destination=48;
        return leCoup;
    }
}

T_Coup ErreurParDefaut(T_Position P)
{

    int i=0;
    int j=0;
    T_Voisins lesVoisin;
    T_Coup leCoup;
    printf("ERREUR DEB");
   	for(i=0;i<48;i++)
        {
            lesVoisin=getVoisins(i);
        
                if(P.cols[i].couleur = P.trait && P.cols[lesVoisin.cases[j]].couleur != P.trait)
                {
                    if((P.cols[i].nb + P.cols[lesVoisin.cases[j]].nb)<=5)
                    {
                        leCoup.origine=lesVoisin.cases[i];
                        leCoup.destination=j; 
                        if(testGlobales(leCoup,P)==VRAI)
                        { 
                            printf("ERREUR FIN");
                            return leCoup; //stratégie 7 
                        }                        
                    }
              
                }
        }
}

int memeCouleur(T_Position P,int pos1,int pos2) //on donne la position de 2 tout et le programme rnvoie s'il sont de meme couleur ou non
{
    printf("mmcouleur");
    if(P.cols[pos1].couleur == P.cols[pos2].couleur)
    {
        printf("take corner 2");
        return VRAI; 

    }
    else
    {
        return FAUX;
    }
}

int ecoTesCoups(T_Position P,octet numCase1,octet numCase2 )
{
    T_Voisins leVoisin;

    if(nbVoisins(numCase1)==1 && nbVoisins(numCase2)==1 && memeCouleur(P,numCase2,numCase1) == VRAI)
    {
        return VRAI;
    }
    else{
        return FAUX;
    }
}

T_Coup lonely(T_Position P)
{
    int i=0;
    T_Voisins leVoisin;
    int j=0;
    T_Coup leCoup;
    printf("lonely 1");
    for(i=0;i<48;i++)
    { 
        if(nbVoisins(i)==1)
        {
            leVoisin=getVoisins(i);

            if(P.cols[i].couleur != P.trait && P.cols[leVoisin.cases[0]].couleur == P.trait && (P.cols[i].nb + P.cols[leVoisin.cases[0]].nb)<6)
            {
                leCoup.origine=leVoisin.cases[0];
                leCoup.destination=i;
                if(testGlobales(leCoup, P)==VRAI)
                { 
                    printf("lonely 2");
                    return leCoup; //stratégie 7 
                }
            }
            else if(P.cols[i].couleur == P.trait && P.cols[leVoisin.cases[0]].couleur != P.trait)
            {
                if(leVoisin.nb>1)
                {
                    for(j=1;j<=leVoisin.nb;j++)
                    {
                        if(leVoisin.cases[j] != i && (P.cols[leVoisin.cases[j]].nb + P.cols[i].nb) <6 )
                        {
                            leCoup.origine=leVoisin.cases[j];
                            leCoup.destination=i; 
                            if(testGlobales(leCoup, P)==VRAI)
                            { 
                                printf("lonely 3");
                                return leCoup; //stratégie 7 
                            }
                        }
                    }
                }
                else 
                {
                    leCoup.origine=48;
                    leCoup.destination=48;
                    return leCoup;
                }
            }
        }
    }
}

/*
T_Coup notThisOne(T_position P,STR_COULEUR macouleur)
{
    int i=0;
    int j=0;
    int k=0;
    T_Voisins lesVoisin;
    T_Voisins lesVoisin2;

    for(i=0; i<48 ;i++)
    {
        if(P.cols[i].couleur != maCouleur)
        { 
            lesVoisin=getVoisins(i);
            for(j=0; j<=lesVoisin.nb; j++)
            {
                if(P.cols[i].nb+P.cols[lesVoisin.cases[j]].nb = 5 && P.cols[lesVoisin.cases[j]].couleur != ma couleur)//la  tour est dangereuse
                {
                    for(k=0; k<=lesVoisin.nb; k++)
                    {   
                        lesVoisin2=getVoisins(lesVoisin.cases[j]);
                        
                        if(lesVoisin.cases[j]=!lesVoisin2.cases[k])//un voisin de latour
                        {   //dangereuse n'est' pas dans le voisinage de l'autre tour dangereuse et que le coup est possible, on retiens le coup
                            if(estValide(P, i, P.cols[lesVoisin2.cases[k]]) == VRAI)
                            {
                                leCoup.origine=i;
                                leCoup.destination=lesVoisin2.cases[k];
                                return leCoup;
                                //après réflexion nous pensons que cette fonction est bcp trop gourmande en
                                //temps surtout que son éfficacité est moindre je pense que nous la passerons surement en commentaire
                                //de plus je ne suis pas sur de l'avoir bien réalisée du tout
                            }
                        }
                    }
                    
                }

            }

        }

    }
    leCoup.origine=48;
    leCoup.destination=48;
    return leCoup;
}
*/

int pasPointGratuitAdv(T_Position P,octet numCase1,octet numCase2)
{
    if(P.cols[numCase1].couleur != P.trait && P.cols[numCase2].couleur != P.trait && P.cols[numCase1].nb + P.cols[numCase2].nb == 5)
    {
        return VRAI;
    }
    else{
        return FAUX;
    }
}


T_Coup SimpleEfficace(T_Position P)
{

    int i=0;
    int j=0;

    T_Voisins lesVoisin;
    T_Coup leCoup;

    printf("smple 1");
    for(i=0;i<NBCASES;i++)
    {
        lesVoisin=getVoisins(i);

        for(j=0;j<=lesVoisin.nb;j++) 
        {

                if(P.cols[i].couleur == P.trait && P.cols[lesVoisin.cases[j]].couleur == P.trait)
                {
                    if((P.cols[i].nb + P.cols[lesVoisin.cases[j]].nb)<=5)
                    {
			            if(P.cols[i].couleur == P.trait && P.cols[lesVoisin.cases[j]].couleur != P.trait)
			            {
				            if((P.cols[i].nb + P.cols[lesVoisin.cases[j]].nb)==5)
                       		{		
					            leCoup.origine=lesVoisin.cases[i];
                      		    leCoup.destination=j; 
                                
                                if(testGlobales(leCoup, P)==VRAI)
                                { 
                                    printf("smple 2");
                                    return leCoup; 
                                }
                            }
              			    else
             			    {
                                 printf("smple 3");
                  		        leCoup.origine=48;
                		        leCoup.destination=48; 
               		            return leCoup;
                            }
                        }
                    }
                }
        }
    }
}



T_Coup Prudence(T_Position P)
{

    int i=0;
    int j=0;

    T_Voisins lesVoisin;
    T_Coup leCoup;
    printf("prud 1");
   	for(i=0;i<48;i++)
    {
        lesVoisin=getVoisins(i);

        if(P.cols[i].couleur = P.trait && P.cols[lesVoisin.cases[j]].couleur != P.trait)
        {
            if((P.cols[i].nb + P.cols[lesVoisin.cases[j]].nb)<=5)
            {
                for(j=1;j<=lesVoisin.nb;j++)
              	{  	  
                    if(lesVoisin.cases[j] != i)
                    {
                        leCoup.origine=lesVoisin.cases[i];
                        leCoup.destination=j; 
                        if(testGlobales(leCoup,P)==VRAI)
                        { 
                        printf("prud 2");
                        return leCoup; 
                        }
                    }
                }
            }  
        }
    }
        
}


T_Coup unEtunVautDeux(T_Position P)
{

    int i=0;
    int j=0;

    T_Voisins lesVoisin;
    T_Coup leCoup;
    printf("undeux 1");
    for(j=0;j<=lesVoisin.nb ;j++) //metrre la condition
    {
        for(i=0;i<NBCASES;i++)
        {
            lesVoisin=getVoisins(i);
        
                if(P.cols[i].couleur != P.trait && P.cols[lesVoisin.cases[j]].couleur != P.trait)
                {
                    if((P.cols[i].nb + P.cols[lesVoisin.cases[j]].nb)==2)
                    {
                        leCoup.origine=lesVoisin.cases[i];
                        leCoup.destination=j; 
                        printf("undeux 2");
                        return leCoup; 
                    }
                }
                else
                {
                    leCoup.origine=48;
                    leCoup.destination=48; 
                    printf("undeux 3");
                    return leCoup;
                }
                
        }
    }
}

int testGlobales(T_Coup leCoup, T_Position P)
{
    printf("testglob 1");
    if(pasPointGratuitAdv(P,leCoup.origine,leCoup.destination)== VRAI)
    {
        if(ecoTesCoups(P,leCoup.origine,leCoup.destination)==VRAI)
        {
            printf("testglob 2");
            return VRAI;
        }
    }
    else{
        return FAUX;
    }
}

void envoyerCoup(T_Position P,octet org,octet dest,T_ListeCoups listeCoups)
{
    int i;
    printf("send 1");
    for(i=0;i<=listeCoups.nb; i++)
    {
        if(listeCoups.coups[i].origine == org && listeCoups.coups[i].destination == dest)
        {
            printf("send 2");
            ecrireIndexCoup(i);
        }
    }
}



void choisirCoup(T_Position P,T_ListeCoups listeCoups)
{
    int numStrat=1;
    T_Coup leCoup;
    
     printf("choos1");
    //on prend les coins
    leCoup=takeCorner(P);

    if(leCoup.origine != 48 )
    {
        envoyerCoup(P, leCoup.origine, leCoup.destination,listeCoups);
        printf("choos2");
    }
    else
    {
        leCoup=SimpleEfficace(P); //strat 7/4 (les deux dans la meme fonctions)
                printf("choos3");
    }

    if(leCoup.origine != 48 ) //on a 48 lorsqu'une stratégie n'est pas possible ainsi on est sur de ne pas renvoyer de coup possible car il n'y a pas de case 48 de plus on teste les stratégies globales avant de lancer le coup
    {
        envoyerCoup(P,leCoup.origine,leCoup.destination,listeCoups);
    }
    else
    {
        leCoup=Prudence(P);
    }


//  
    if(leCoup.origine )
    {
        envoyerCoup(P, leCoup.origine, leCoup.destination,listeCoups);
    }
    else
    {
        leCoup=lonely(P); //strat 7/4 (les deux dans la meme fonctions)
    }

//
    if(leCoup.origine != 48  )
    {
        envoyerCoup(P, leCoup.origine, leCoup.destination,listeCoups);
    }



// stratégies par défauts en dessous (on ne vérifie pas les stratégies globales ici)

    leCoup=unEtunVautDeux(P);
    if(leCoup.origine =!48)
    {
        envoyerCoup(P,leCoup.origine , leCoup.destination,listeCoups);
    }
    else
    {
        leCoup=ErreurParDefaut(P) ;
        envoyerCoup(P,leCoup.origine,leCoup.destination,listeCoups);
    }

}







