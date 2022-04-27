/********* Moteur de tournoi : joueur ***************/

// MANQUE UN ARGUMENT A SIMON
// AMBROISE MANQUE notefinal=0;

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


// proto

int verifToursDe5(T_ListeCoups l,T_Position pos,octet o,octet dest_int);
int chercherCoups(T_ListeCoups listeCoups, octet origine, octet destination);
int CoupsPossiblesPion(T_ListeCoups l,octet org,octet dest_interdit,T_ListeCoups* TabCoups);
int choisirMeilleurCoup(T_ListeCoups listeComplete,T_ListeCoups l,T_Position pos,octet* o1,octet* d1);
void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups);


int verifToursDe5(T_ListeCoups l,T_Position pos,octet o,octet dest_int){
	T_ListeCoups tab;
	int i;
	octet o1,d1;
	printf("ON TESTE LES COUPS A PARTIR DE %d\n",o);
	CoupsPossiblesPion(l,o,dest_int,&tab);
	printf("NOMBRE DE COUPS A PARTIR DE %d ==> %d\n",o,tab.nb);
	for(i=0;i<tab.nb;i++){
		/*Ici plusieurs possibilités: Le coup suivant peut amener à la création d'une tour de 5 (1) ou pas (2)  */
		/*(1) 	-Si les 2 pions sont de notre couleur ==> Pas grave, on joue quand meme le coup */
		/*  	-Si un des deux est de la couleur adverse ==> On ne joue pas le coup */
		/* 		- Si les deux sont de la couleur adverse ==> On ne joue pas le coup */
		/*(2) On joue le coup */
		o1=tab.coups[i].origine;
		d1=tab.coups[i].destination;
		printf("\tOn teste %d(%d) -> %d(%d)\n",o1,pos.cols[o1].nb+pos.cols[dest_int].nb,d1,pos.cols[d1].nb);
		if(pos.cols[o1].nb+pos.cols[d1].nb+pos.cols[dest_int].nb == 5){
			printf("\nATTENTION, L'ADVERSAIRE VA POUVOIR FORMER UNE TOUR DE 5!\n");
			if(pos.cols[o1].couleur != pos.trait || pos.cols[d1].couleur != pos.trait)return 0;
		}
	}
	return 1;

}
int chercherCoups(T_ListeCoups listeCoups, octet origine, octet destination) {
	int i;

	for (i=0;i<listeCoups.nb;i++) {
		// i, l.coups[i].origine, l.coups[i].destination
		if (listeCoups.coups[i].origine == origine && listeCoups.coups[i].destination == destination)
			return i;
	}
	return 0;
}

int CoupsPossiblesPion(T_ListeCoups l,octet org,octet dest_interdit,T_ListeCoups* TabCoups){
	int i; 
	TabCoups->nb=0;
	for(i=0;i<l.nb;i++) {
        if(l.coups[i].origine == org && l.coups[i].destination != dest_interdit){
			TabCoups->coups[TabCoups->nb]=l.coups[i];
			TabCoups->nb++;
        }
	}
    return 0;
}

int choisirMeilleurCoup(T_ListeCoups listeComplete,T_ListeCoups l,T_Position pos,octet* o1,octet* d1){
	octet notemax;
	int i,k;
	octet Tab[l.nb][3];
	octet temp[l.nb][3];
	printf("ON CHERCHE LE MEILLEUR COUP POSSIBLE PARMI %d POSSIBILITES\n",l.nb);
	for(i=0;i<l.nb;i++){
		*o1 = l.coups[i].origine;
		*d1 = l.coups[i].destination;
		printf("LE PION EST %s\n",COLNAME(pos.cols[*o1].couleur));
		if(pos.cols[*o1].couleur==pos.trait){
			//Le pion est de notre couleur
			if(pos.cols[*d1].couleur == pos.trait){
				//Le pion et le voisin sont de notre couleur, il n'est pas judicieux de les empiler: 1
				printf("\t%d) LE PION ET LE VOISIN SONT DE MA COULEUR\n",i);
				Tab[i][0]=1;
			}
			else{
				//Le pion est de notre couleur, mais pas le voisin. Les empiler ne fera pas avancer le jeu: 2
				printf("\t%d) LE PION EST DE NOTRE COULEUR, MAIS PAS LE VOISIN\n",i);
				Tab[i][0]=2;
			}
		}
		else{
			//Le pion est de la couleur adverse
			if(pos.cols[*d1].couleur == pos.trait){
				//Le pion est de la couleur adverse, mais le voisin est de notre couleur, il n'est pas judicieux de les empiler: 1
				printf("\t%d) LE PION EST DE LA COULEUR ADVERSE, MAIS PAS LE VOISIN\n",i);
				Tab[i][0]=1;
			}
			else{
				//Le pion et le voisin sont de la couleur adverse, on l'empile: 2
				printf("\t%d) LE PION ET LE VOISIN SONT DE LA COULEUR ADVERSE\n",i);
				Tab[i][0]=2;
			}
		}
		Tab[i][1]=*o1;
		Tab[i][2]=*d1;
	}
	notemax=0;
	/* Tri à bulles des coups possibles */
	k=0;
	do{
		k=0;
		for(i=0;i<(l.nb)-1;i++){
			if(Tab[i][0]<Tab[i+1][0]){
				temp[i][0]=Tab[i+1][0];
				temp[i][1]=Tab[i+1][1];
				temp[i][2]=Tab[i+1][2];
				Tab[i+1][0]=Tab[i][0];
				Tab[i+1][1]=Tab[i][1];
				Tab[i+1][2]=Tab[i][2];
				Tab[i][0]=temp[i][0];
				Tab[i][1]=temp[i][1];
				Tab[i][2]=temp[i][2];
				k++;
			}
		}
	}while(k!=0);
	k=-1;
	for(i=0;i<l.nb;i++){
		if(verifToursDe5(listeComplete,pos,Tab[i][2],Tab[i][1])){
			printf("LE MEILLEUR COUP EST %d -> %d\n",Tab[i][1],Tab[i][2]);
			k=i;
			i=l.nb;
		}
		else{
			printf("%d -> %d PERMETTRAIT A L'ADVERSAIRE DE GAGNER UNE TOUR DE 5.ON ABANDONNE DONC CE COUP.\n",Tab[i][1],Tab[i][2]);
		}
	}
	if(k!=-1){
		*o1 = Tab[k][1];
		*d1 = Tab[k][2];
	}
	return 1;
}


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i,j,note,note_final=0,a;
	octet o, d,o1,d1; 
	octet myColor = currentPosition.trait;
	T_ListeCoups TabCoups,tabVoisins,tabVoisins2;
	T_Voisins voisins;
	int val;
	int k=0,taille_pile;
	int aux1,aux2;

	//afficherListeCoups(listeCoups);

	//printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	//printf("Nombre de coups possibles %d\n",listeCoups.nb );
	for(i=0;i<listeCoups.nb; i++) {
		TabCoups.nb=0;
		tabVoisins.nb=0;
		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		//printf("Coup %d : ", i); 
		//printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		//printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 


		// inferiorité numérique
		if ((currentPosition.cols[o].couleur != myColor) && (currentPosition.cols[d].couleur != myColor))
			if (currentPosition.cols[d].nb + currentPosition.cols[o].nb < 3) {
				voisins = getVoisins(d);
				for (j=0;j<nbVoisins(d);j++) {
					if (currentPosition.cols[voisins.cases[j]].nb < 3 && currentPosition.cols[voisins.cases[j]].nb !=0 )
						ecrireIndexCoup(i);
				}
			}
			

		if (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 3) {
			if (currentPosition.cols[o].couleur == myColor){
				k=0;
				CoupsPossiblesPion(listeCoups,d,o,&tabVoisins);
				printf("ON REGARDE SI IL Y A DES POINTS DE 2 OU 1 ADVERSE AUTOUR\n");
				for (j=0;j<tabVoisins.nb;j++){
					if (currentPosition.cols[tabVoisins.coups[j].destination].couleur != myColor
						&& currentPosition.cols[tabVoisins.coups[j].destination].nb <=2 ) {
						k++;
					}
				}
				if (k==0) {
					printf("AUCUN POINT DE 2 OU 1 ADVERSE AUTOUR, ON GAGNE LA TOUR\n");
					printf("LE MEILLEUR COUP EST %d => %d\n",o,d);
					ecrireIndexCoup(i);
					return;  
				}
			}	
		}


		if (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 4) {
			if (currentPosition.cols[o].couleur == myColor){
				k=0;
				CoupsPossiblesPion(listeCoups,d,o,&tabVoisins);
				printf("ON REGARDE SI IL Y A DES POINTS DE 1 ADVERSE AUTOUR\n");
				for (j=0;j<tabVoisins.nb;j++){
					if (currentPosition.cols[tabVoisins.coups[j].destination].couleur != myColor
						&& currentPosition.cols[tabVoisins.coups[j].destination].nb <=1 ) {
						k++;
					}
				}
				if (k==0) {
					printf("AUCUN POINT DE 1 ADVERSE AUTOUR, ON GAGNE LA TOUR\n");
					printf("LE MEILLEUR COUP EST %d => %d\n",o,d);
					ecrireIndexCoup(i);
					return;  
				}
			}	
		}



		 if (currentPosition.cols[d].nb + currentPosition.cols[o].nb == 5)
			if (currentPosition.cols[o].couleur == myColor) {
				if (currentPosition.cols[d].couleur != myColor){
					printf("ICI UNE TOUR DE 5 DOIT ETRE FAITE CAR SI ON NE L'A FAIT PAS L'ADVERSAIRE VA S'EN CHARGER\n");
					printf("MEILLEUR COUP : %d => %d\n",o,d );
					ecrireIndexCoup(i);
					return;
				}
				else {
					printf("LE PION %d EST DE NOTRE COULEUR \n",d );
					printf("REGARDONS SES VOISINS\n");
					CoupsPossiblesPion(listeCoups,d,o,&tabVoisins);
					for (j=0;j<tabVoisins.nb;j++){
						aux1=currentPosition.cols[tabVoisins.coups[j].destination].nb+d;
						if (currentPosition.cols[tabVoisins.coups[j].destination].couleur != myColor && aux1<=5){
							printf("ON PEUT PERDRE UNE TOUR SI ON NE JOUE PAS\n");
							printf("ATTENTION LE PION %d possède un voisin de la couleur adverse\n",d);
							printf("CE PION EST %d\n",tabVoisins.coups[j].destination);
							printf("CE PION PEUT POTENTIELLEMENT ETRE POSE SUR LA DESTINATION\n");
							printf("ON INVERSE DESTINATION ET ORIGINE\n");
							o1=d;
							d1=o;
							printf("ON REGARDE SI L'ON A LE MEME PROBLEME DANS L'AUTRE SENS\n");
							CoupsPossiblesPion(listeCoups,d1,o1,&tabVoisins2);
							for (a=0;a<tabVoisins2.nb;a++){
								aux2=currentPosition.cols[tabVoisins2.coups[a].destination].nb+d1;
								if (currentPosition.cols[tabVoisins2.coups[a].destination].couleur != myColor && aux2<=5){
									printf("ON SE TROUVE SUR LE PION %d\n",d1 );
									printf("ICI, IL Y A ENCORE UN PION ADVERSE AUX ALENTOURS \n");
									printf("C'EST LE PION %d\n",tabVoisins2.coups[a].destination);
									printf("CE PION PEUT EGALEMENT ETRE POSE SUR LE PION %d\n",d1 );
									printf("ON JOUE LA TOUR, ON PREND AUCUN RISQUE\n");
									printf("MEILLEUR COUP : %d => %d",o,d);
									ecrireIndexCoup(i);
									return;
								}
							}	
						}
					}
				}
			}
		

		taille_pile = currentPosition.cols[o].nb + currentPosition.cols[d].nb;
        if(currentPosition.cols[o].couleur == myColor){
            CoupsPossiblesPion(listeCoups,d,o,&tabVoisins);
            k=0;
            
            printf("ON REGARDE SI UNE TOUR PEUT ETRE PRISE FACILEMENT\n");
            for(j=0;j<tabVoisins.nb;j++){
                if(taille_pile+currentPosition.cols[tabVoisins.coups[j].destination].nb <=5 && currentPosition.cols[tabVoisins.coups[j].destination].couleur != myColor){
                    k++;
                }
            }
            if(k==0){
            	if (currentPosition.cols[d].couleur != myColor ){
                	printf("On peut former une tour qui ne pourra pas être prise! %d -> %d\n",o,d);
                	ecrireIndexCoup(i);
                	return;
                }
            }  
        }
        
		
		CoupsPossiblesPion(listeCoups,o,d,&tabVoisins); 
        if(tabVoisins.nb==0){ //Si on trouve un pion qui n'a qu'un seul voisin (=0 car on enleve le coup actuellement étudié)
	   		printf("UN PION N'A QU'UN SEUL VOISIN ==> %d\n",o);
            if(currentPosition.cols[o].couleur == myColor){ //Si il est de notre couleur
                //ON ECARTE SON VOISIN ==> ON GAGNE LA TOUR
				printf("IL EST DE NOTRE COULEUR\n");
				CoupsPossiblesPion(listeCoups,d,o,&tabVoisins); //Prend en argument la liste complete des coups, 
				//l'origine à ne pas compter dans les coups possibles (on ne veut pas empiler le voisin sur le pion o), le voisin, et une &liste de coups possibles du voisin.
				printf("LE VOISIN (%d) A %d AUTRES VOISINS\n",d,tabVoisins.nb);
				if(tabVoisins.nb==0){
					//Le voisin n'a pas d'autres voisins.
					printf("LE VOISIN N'A PAS D'AUTRES VOISINS\n");
					for(j=0;j<tabVoisins.nb;j++){
						o1 = tabVoisins.coups[j].origine;
						d1 = tabVoisins.coups[j].destination;
						if(currentPosition.cols[d1].couleur == myColor){
							//Si il  est de notre couleur, on ne fait rien, on aura forcemment le point
							printf("IL EST DE NOTRE COULEUR, ON NE FAIT RIEN\n");
						}
						else{
							//Si il n'est pas de notre couleur, on l'empile
							printf("ON JOUE LE COUP\n");
							val = chercherCoups(listeCoups,o1,d1);
							ecrireIndexCoup(val);
						}
					}
				}
				else{ // Si le pion est de notre couleur
					printf("LE VOISIN PEUT ETRE DEPLACE\n");
					choisirMeilleurCoup(listeCoups,tabVoisins,currentPosition,&o1,&d1);
					printf("LE MEILLEUR COUP EST %d -> %d\n",o1,d1);
					val = chercherCoups(listeCoups,o1,d1);
					ecrireIndexCoup(val);
					
				}
            }
            else{ //Si il est à l'adversaire
				printf("IL N'EST PAS DE NOTRE COULEUR\n");
                if(currentPosition.cols[d].couleur == myColor){ //Si le voisin est de notre couleur
                    //ON EMPILE LE VOISIN SUR LE PION D'ORIGINE ==> ON GAGNE LA TOUR
					printf("SON VOISIN EST DE NOTRE COULEUR, ON PEUT RECUPERER LA TOUR\n");
                    val = chercherCoups(listeCoups,d,o);
                    ecrireIndexCoup(val);
                }
                else{ //Le voisin est de la couleur adverse
                    //ON EMPILE LE PION D'ORIGINE SUR LE VOISIN ==> ON REMET LE PION DANS LE JEU
					printf("LE VOISIN EST AUSSI A L'ADVERSAIRE, ON REMET LE PION EN JEU POUR L'EMPECHER DE RECUPERER UNE TOUR\n");
					ecrireIndexCoup(i);
                }
            }
        }

		

        // fonction de démarrage
		if (myColor == 1) {
			if (listeCoups.nb == 292){
			printf("ON DEMARRE TRANQUILLEMENT SI L'ON EST JAUNE\n");
			printf("MEILLEUR COUP : %d => %d\n",o,d );
			// on regarde si c'est le début de partie 
			val = chercherCoups(listeCoups,47,45);
			ecrireIndexCoup(val);
			return; // on quitte la fonction 
			}
		}
	}


 


}
