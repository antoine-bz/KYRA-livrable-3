/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"



void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	int choix=-1; 
	int i, o, d, oj, dj; 

	while(choix == -1) {
		//system("clear");

		printf("Votre couleur : %s\n", COLNAME(currentPosition.trait));
		printf("Votre choix (org dest) ? ");
		scanf("%d %d", &oj, &dj); 

		printf("Vous avez choisi %d->%d\n", oj, dj);
		printf("Recherche de l'indice du coup correspondant...\n");

		for(i=0;i<listeCoups.nb; i++) {
			o = listeCoups.coups[i].origine; 
			d = listeCoups.coups[i].destination;  
			if ((o ==oj) && (d ==dj)) {
				printf("Coup correspondant trouvé : %d\n", i);
				choix = i;
				break; 
			}
		}

		if (choix == -1) {
			printf("Erreur de saisie !\n"); 
		}

		ecrireIndexCoup(choix);
	}
}


