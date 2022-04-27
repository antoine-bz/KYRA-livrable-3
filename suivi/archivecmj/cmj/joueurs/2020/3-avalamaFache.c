/********* Moteur de tournoi : joueur ***************/

#include <stdio.h>
#include <stdlib.h>
#include "avalam.h"
#include "moteur.h"


void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
	// Cette fonction peut appeler la fonction ecrireIndexCoup(coupChoisi);
	// Pour sélectionner l'index d'un coup à jouer dans la liste l 


	int i, j;
	int stop = 0; 
	int num, nbPions;
	int pionsSeuls;
	int contre;
	int porteeAdv, porteeAmi; // Variables testant si la portée ne contient que des pions adverses ou que des pions de notre couleur
	int tourPetite, tourDe5, tourGrande; // Variables testant la taille des tours ennemies de la portée du pion de destination
	int score = 0; // Score attribué à chacun des coups : l'algorithme choisira le coup valant le plus de points
	octet o, d;
	octet temp;
	octet o2, d2; // Variables utilisées lors des cas où l'on a besoin de regarder au tour suivant
	octet myColor = currentPosition.trait; 

	// afficherListeCoups(listeCoups);

	printf("Ma couleur : %s\n", COLNAME(currentPosition.trait));
	for(i=0;i<listeCoups.nb; i++) {

		tourPetite = 0, tourDe5 = 0, tourGrande = 0;
		porteeAmi = 1, porteeAdv = 1;
		nbPions = 0, pionsSeuls = 0;
		contre = 0;

		o = listeCoups.coups[i].origine; 
		d = listeCoups.coups[i].destination;  
		printf("Coup %d : ", i); 
		printf("%d (%d - %s) ->", o, currentPosition.cols[o].nb, COLNAME(currentPosition.cols[o].couleur));
		printf("%d (%d - %s) \n", d, currentPosition.cols[d].nb, COLNAME(currentPosition.cols[d].couleur)); 

		if (stop) {
			if (temp == o) goto FIN_BOUCLE;
			else stop = 0;
		}

		if (currentPosition.cols[o].couleur == myColor) {

			// Si on empile l'adversaire
			if (currentPosition.cols[d].couleur != myColor) {
				
				// Si on peut faire une tour de 5
				if (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) {
					printf("On choisit ce coup ! \n"); 
					ecrireIndexCoup(i); // Il ne sert ici à rien d'attribuer un nombre de points à ce coup puisqu'il s'agit du meilleur possible (selon nous)
					return; // on quitte la fonction 
				}

				// Si on ne peut pas, il faut faire attention à ce que l'adversaire ne puisse pas non plus
				// (si le score ne peut augmenter ici, alors cette boucle ne sert à rien)
				else if ((score < 90) && (currentPosition.cols[o].nb + currentPosition.cols[d].nb < 5)) {
						
					for (j=0; j<listeCoups.nb; j++) { // On va regarder chaque pion de la portée du pion de destination

						o2 = listeCoups.coups[j].origine; // Cette deuxième origine correspond à la destination du tour actuel
						d2 = listeCoups.coups[j].destination; // Cette deuxième destination correspond à la portée de la destination du tour actuel

						if ((d == o2) && (o != d2)) {

							if (currentPosition.cols[d2].couleur != myColor) 
							{
								nbPions++; // On a trouvé un pion dans la portée (celui que l'on déplace n'étant pas compris)
								porteeAmi = 0; // La portée ne contient pas que des pions de notre couleur
								if (currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb > 5) tourGrande++;
								else if (currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb == 5) tourDe5++;
								else if(currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb < 5) tourPetite++;
							}

							else if (currentPosition.cols[d2].couleur == myColor) {
								if (currentPosition.cols[o].nb + currentPosition.cols[d].nb + currentPosition.cols[d2].nb < 5) contre = 1;
								porteeAdv--; // Si la variable reste à 1 : il n'y a que des pions adverses
							}
						}

						if (nbPions == 7) break; // On économise des tours de boucle : il y a au maximum 8-1 pions dans la portée (car on ne compte pas l'origine)
					}

					// Si la portée contient des pions adverses + notre pion que l'on va déplacer, qui est le SEUL nous appartenant ici
					if (porteeAdv == 1) {

						// Si une tour de 5 est faisable pour l'adversaire 
						if (tourDe5 > 0) {
							stop = 1;
							temp = o;
							goto FIN_BOUCLE;
						}

						// Si le pion adverse est le seul pion de sa couleur : on l'empile (car il est isolé)
						if (!nbPions && score < 90 && currentPosition.cols[o].nb + currentPosition.cols[d].nb < 5) {

							score = 90;
							num = i;
						}

						// Sinon, il y en a plusieurs, donc il faut regarder leur taille
						else if (tourGrande == nbPions && score < 90 - currentPosition.cols[o].nb) { // S'il n'y a que des tours adverses trop grandes pour nous empiler
								score = 90 - currentPosition.cols[o].nb; // Pour départager les coups, on suppose qu'il est mieux moins l'on déplace de pions
								num = i;
						}

						else if (!score) {
      						score = 5;
     						num = i;
						}
					}
					
					else if (porteeAdv < 1) { // Sinon... (au moins 2 pions de notre couleur, en comptant l'origine)
						switch(porteeAmi) {
							
							// ... soit la portée contient des pions des 2 couleurs (sans compter celui que l'on va déplacer, donc au moins 2 Jaunes + 2 Rouges )
							case 0 : if (tourDe5 > 0) goto FIN_BOUCLE;
							
									if (tourGrande == nbPions) { // Si TOUS les pions adverses présents sont trop "grands" pour nous empiler en retour
										if (score < 80 - currentPosition.cols[o].nb) 
										{
											score = 80 - currentPosition.cols[o].nb;
											num = i;
										}
									}

									// S'il y a à la fois des tours trop grandes et des tours trop petites
									else if (tourGrande > 0  && tourPetite > 0 && score < 30) {
										score = 30;
										num = i;
									}

									// S'il n'y a que des tours trop petites
									else if (tourPetite == nbPions) {
										if (!contre && !score) {
											score = 10;
											num = i;
										}

										else if (score < 46) {
											score = 46;
											num = i;
										}
									}

									break;

							// ... soit la portée ne contient que des pions de notre couleur
							case 1 : if (score < 50) { 
										score = 50;
										num = i;
									}
						}
					}
				}
			}

			// Si on empile notre couleur
			else {
				
				if ((score < 20) && (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5)) {
					score = 20;
					num = i;
				}

				else {

					for (j=0; j<listeCoups.nb; j++) { // On va regarder chaque pion de la portée du pion de destination

						o2 = listeCoups.coups[j].origine; // Cette deuxième origine correspond à la destination du tour actuel
						d2 = listeCoups.coups[j].destination; // Cette deuxième destination correspond à la portée de la destination du tour actuel

						if ((d == o2) && (o != d2)) {

							if (currentPosition.cols[d2].couleur != myColor) porteeAmi = 0; // La portée ne contient pas que des pions de notre couleur	
							nbPions++;
						}

						if (nbPions == 7) break; // On économise des tours de boucle : il y a au maximum 8-1 pions dans la portée (car on ne compte pas l'origine)
					}

					if (!nbPions && score < 45) {
						score = 45;
						num = i;
					}
				}
			}
		}

		FIN_BOUCLE: ; // Instruction nulle
	}

	if (i == listeCoups.nb) {
		printf("\n On a parcouru toute la boucle !\n\n");
		ecrireIndexCoup(num);
		return;
	}
}
