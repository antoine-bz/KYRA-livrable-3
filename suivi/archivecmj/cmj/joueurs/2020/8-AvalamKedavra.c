/********* Moteur de tournoi : Avalam Kedavra ***************/

#include "avalam.h"
#include "moteur.h"
#include <stdio.h>
#include <stdlib.h>

#define COUPS_POSSIBLES_MAX 50

int priorite_max = 0;

void empiler(T_ListeCoups listeCoups, octet origine, octet destination,
             int priorite);
void empilerIndex(int index, int priorite);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
  int i;
  octet o, d;
  octet myColor = currentPosition.trait;
  octet enemyColor = (myColor == 1) ? 2 : 1;

  int case_vide = 0;
  int coup_special_1 = 0;

  for (int j = 0; j < 48; j++) { // nb case vide
    if (currentPosition.cols[j].nb == 0) {
      case_vide++;
    }
  }

  if (myColor == 1) { // si on est jaune
    if ((currentPosition.cols[4].nb == 2) &&
        (currentPosition.cols[4].couleur == myColor) &&
        (currentPosition.cols[8].nb == 0) && (case_vide == 2)) {
      coup_special_1 = 2;
    } else {
      int j = 0;
      while (currentPosition.cols[j].nb == 1 && j < 9) {
        j++;
      }

      (j == 9) ? (coup_special_1 = 1) : (coup_special_1 = 0);
    }
  }

  // coup special
  if (coup_special_1 > 0) {
    switch (coup_special_1) {
    case 1:
      empiler(listeCoups, 8, 4, 18);
      break;
    case 2:
      empiler(listeCoups, 9, 16, 18);
      break;
    }
  }

  // coups normaux
  for (i = 0; i < listeCoups.nb;
       i++) { // parcours tous les deplacements possibles
    o = listeCoups.coups[i].origine;
    d = listeCoups.coups[i].destination;

    int nb_nos_voisins = 0;
    int nb_voisins_ennemis = 0;

    for (int j = 0; j < 8; j++) {
      T_Voisins voisins = getVoisins(d);

      if (currentPosition.cols[voisins.cases[j]].couleur == myColor) {
        nb_nos_voisins++;
      } else if (currentPosition.cols[voisins.cases[j]].couleur == enemyColor) {
        nb_voisins_ennemis++;
      }
    }

    // pile de 5
    if ((currentPosition.cols[o].couleur == myColor) &&
        (currentPosition.cols[o].nb == 4) && (nb_voisins_ennemis > 0)) {
      int condition = 0;
      for (int j = 0; j < 8; j++) { // on parcours les voisins
        T_Voisins voisins = getVoisins(o);

        if ((currentPosition.cols[voisins.cases[j]].couleur == enemyColor) &&
            (currentPosition.cols[voisins.cases[j]].nb == 1)) {

          for (int k = 0; k < 8; k++) { // on parcours les voisins
            T_Voisins voisins = getVoisins(d);

            if ((currentPosition.cols[voisins.cases[k]].couleur ==
                 enemyColor)) {

              condition = 1;
            }
          }
        }
      }

      if (condition == 1) {
        empilerIndex(i, 21);
      }
    } else {
      if ((currentPosition.cols[o].nb == 4) && // j4 -> 1
          (currentPosition.cols[o].couleur == myColor) &&
          (currentPosition.cols[d].nb == 1)) {
        if (currentPosition.cols[d].couleur == enemyColor) {
          empilerIndex(i, 20);
        } else {
          empilerIndex(i, 19);
        }
      }

      if ((currentPosition.cols[o].nb == 1) && // j1 -> 4
          (currentPosition.cols[o].couleur == myColor) &&
          (currentPosition.cols[d].nb == 4)) {
        if (currentPosition.cols[d].couleur == enemyColor) {
          empilerIndex(i, 20);
        } else {
          empilerIndex(i, 19);
        }
      }

      if ((currentPosition.cols[o].nb == 3) && // j3 -> 2
          (currentPosition.cols[o].couleur == myColor) &&
          (currentPosition.cols[d].nb == 2)) {
        if (currentPosition.cols[d].couleur == enemyColor) {
          empilerIndex(i, 20);
        } else {
          empilerIndex(i, 19);
        }
      }

      if ((currentPosition.cols[o].nb == 2) && // j2 -> 3
          (currentPosition.cols[o].couleur == myColor) &&
          (currentPosition.cols[d].nb == 3)) {
        if (currentPosition.cols[d].couleur == enemyColor) {
          empilerIndex(i, 20);
        } else {
          empilerIndex(i, 19);
        }
      }
    }

    if ((currentPosition.cols[o].couleur == myColor) &&
        (nb_voisins_ennemis == 0)) {
      empilerIndex(i, 18);
    }

    if ((currentPosition.cols[o].nb + currentPosition.cols[d].nb == 5) &&
        (currentPosition.cols[o].couleur == enemyColor) &&
        (currentPosition.cols[d].couleur == enemyColor)) {
      int condition = 0;

      for (int j = 0; j < 8; j++) { // on parcours les voisins
        T_Voisins voisins = getVoisins(o);

        if ((currentPosition.cols[voisins.cases[j]].nb <
             (5 - (currentPosition.cols[o].nb)))) {
          condition = 1;
        }
      }

      if (condition == 1) {
        empilerIndex(i, 17);
      }
    }

    if (currentPosition.cols[o].couleur == myColor) {
      int condition = 1;

      for (int j = 0; j < 8; j++) { // on parcours les voisins
        T_Voisins voisins = getVoisins(d);

        if ((currentPosition.cols[voisins.cases[j]].nb >
             (5 - (currentPosition.cols[o].nb)))) {
          condition = 0;
        }
      }

      if ((condition == 1) && (currentPosition.cols[d].couleur == enemyColor)) {
        empilerIndex(i, 17);
      } else if ((condition == 1) &&
                 (currentPosition.cols[d].couleur == myColor)) {
        empilerIndex(i, 16);
      }
    }

    if ((currentPosition.cols[o].couleur == myColor) &&
        (nb_voisins_ennemis > 0)) { // si jaune
      int condition = 1;

      for (int j = 0; j < 8; j++) { // on parcours les voisins
        T_Voisins voisins = getVoisins(d);
        int nb_empilement =
            (currentPosition.cols[o].nb + currentPosition.cols[d].nb);

        if ((currentPosition.cols[voisins.cases[j]].couleur == enemyColor) &&
            (currentPosition.cols[voisins.cases[j]].nb + nb_empilement <= 5)) {
          condition = 0;
        }
      }

      if (condition == 1) {
        empilerIndex(i, 15);
      }
    }

    if ((currentPosition.cols[o].couleur == myColor) &&
        (currentPosition.cols[o].nb + currentPosition.cols[d].nb == 3) &&
        (nb_nos_voisins >= 4)) {
      int condition = 1;

      for (int j = 0; j < 8; j++) {
        T_Voisins voisins = getVoisins(d);

        if ((currentPosition.cols[voisins.cases[j]].nb + 3 <= 5) &&
            (currentPosition.cols[voisins.cases[j]].couleur == enemyColor)) {
          condition = 0;
        }

        if (condition == 1) {
          if ((currentPosition.cols[d].couleur == enemyColor) &&
              (nb_voisins_ennemis == 0)) {
            empilerIndex(i, 14);
          } else if ((currentPosition.cols[d].couleur == enemyColor) &&
                     (nb_voisins_ennemis == 1)) {
            empilerIndex(i, 13);
          } else if ((currentPosition.cols[d].couleur == enemyColor) &&
                     (nb_voisins_ennemis == 2)) {
            empilerIndex(i, 12);
          } else if ((currentPosition.cols[d].couleur == myColor) &&
                     (nb_voisins_ennemis == 0)) {
            empilerIndex(i, 11);
          } else {
            empilerIndex(i, 10);
          }
        }
      }
    }

    if ((currentPosition.cols[o].couleur ==
         enemyColor) && // r -> r si entoure d'au moins 4 jaunes
        (currentPosition.cols[o].nb == 1) &&
        (currentPosition.cols[d].couleur == enemyColor) &&
        (currentPosition.cols[d].nb == 1) && (nb_nos_voisins >= 4)) {
      if (nb_voisins_ennemis < 4) {
        empilerIndex(i, 9);
      } else {
        empilerIndex(i, 8);
      }
    }

    if ((currentPosition.cols[o].nb == 1) && // r1 -> r1
        (currentPosition.cols[o].couleur == enemyColor) &&
        (currentPosition.cols[d].nb == 1) &&
        (currentPosition.cols[d].couleur == enemyColor)) {
      empilerIndex(i, 6);
    }

    if ((currentPosition.cols[o].couleur == myColor) && // j -> r
        (currentPosition.cols[d].couleur == enemyColor)) {
      empilerIndex(i, 5);
    }

    if ((currentPosition.cols[o].couleur == enemyColor) && // r -> r
        (currentPosition.cols[d].couleur == enemyColor)) {
      empilerIndex(i, 4);
    }

    if ((currentPosition.cols[o].couleur == myColor) && // j -> j
        (currentPosition.cols[d].couleur == myColor)) {
      empilerIndex(i, 3);
    }

    if ((currentPosition.cols[o].couleur == enemyColor) && // r -> j
        (currentPosition.cols[d].couleur == myColor)) {
      empilerIndex(i, 2);
    }

    empilerIndex((rand() % (listeCoups.nb - 1)),
                 1); // si aucun coup alors aleatoire
  }
}

void empiler(T_ListeCoups listeCoups, octet origine, octet destination,
             int priorite) {
  octet o, d;

  if (priorite >= priorite_max) {
    for (int j = 0; j < listeCoups.nb; j++) {
      o = listeCoups.coups[j].origine;
      d = listeCoups.coups[j].destination;

      if ((o == origine) && (d == destination)) {
        priorite_max = priorite;
        ecrireIndexCoup(j);
        break;
      }
    }
  }
  // printf("\n>>> Empiler : %d -> %d (priorite = %d)\n\n", origine,
  // destination,
  //       priorite);
}

void empilerIndex(int index, int priorite) {
  if (priorite >= priorite_max) {
    priorite_max = priorite;
    ecrireIndexCoup(index);
  }
}
