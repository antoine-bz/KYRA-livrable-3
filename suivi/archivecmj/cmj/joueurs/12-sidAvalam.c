/********* Moteur de tournoi : joueur ***************/

#include "avalam.h"
#include "moteur.h"
#include <stdio.h>
#include <stdlib.h>

/*************** Déclartion des fonctions ***************/
int pile2(T_Coup coup, T_Position pos);
int pile3(T_Coup coup, T_Position pos);
int pile4(T_Coup coup, T_Position pos);
int pile5(T_Coup coup, T_Position pos);

int Coup_Futur_deplacer(T_Coup coup, T_Position pos);
int coup_isole(T_Coup coup, T_Position pos);
int pile_isole(T_Position pos, int tab[]);
int recherchecoup(T_ListeCoups ListeCoup, T_Coup coup);
T_Coup score(T_ListeCoups ListeCoup, T_Position pos);
int sid_couleur(T_Coup coup, T_Position pos);

void choisirCoup(T_Position currentPosition, T_ListeCoups listeCoups) {
  int i;
  i = recherchecoup(listeCoups, score(listeCoups, currentPosition)); 
  ecrireIndexCoup(i);
}

int Coup_Futur_deplacer(T_Coup coup, T_Position pos) {
  T_Position posbis;
  T_Voisins voisin;
  int compteuradv = 0;
  int compteurall = 0;

  posbis = pos; // initialisation de posbis
  voisin = getVoisins(
      coup.destination); // on recupère les voisins de notre destination

  posbis =
      jouerCoup(posbis, coup.origine, coup.destination); // on simule le coup

  for (int j = 0; j < voisin.nb; j++) { // on parcourt tous les voinsins
    if (posbis.cols[voisin.cases[j]].couleur ==
        posbis.trait) { // regarde si le pion est de couleur adverse
      compteuradv++;
    } else if (posbis.cols[voisin.cases[j]].couleur ==
               pos.trait) { // regarde si le pion est de couleur alliée.
      compteurall++;
    }
  }

  if (compteuradv >
      compteurall) // si il y a plus d'adversaires alors c'est défavorable
    return -40;    // on retourne un score negatif
  if (compteuradv <
      compteurall) // si il y a plus d'alliés alors c'est pfavorable
    return +40;    // on retourne un score positif

  return 0; // on retourne 0 si il n'y a pas plus d'alliés que d'adversaires
}

/*************** fonction pile isolée ***************/

int coup_isole(T_Coup coup, T_Position pos) {
  int tab[48];
  int taille = 0;
  int tabbis[48];
  int taillebis = 0;
  int score = 0;
  T_Position posbis;

  pile_isole(pos,
             tab); // on recherche les coups isolées avant simulation du coup

  while (tab[taille] != -1) { // Calcule le nombre de pile isolée
    taille++;
  }
  posbis = jouerCoup(pos, coup.origine, coup.destination); // on simule le coup
  posbis.trait = pos.trait; // on remodifie le trait pour le notre.

  pile_isole(posbis, tabbis); // on recherche de nouveau les piles isolées
  while (tabbis[taillebis] != -1) { // On recalcule le nombre de piles isolées
    taillebis++;
  }

  if (taillebis > taille) { // Condition qui regarde si avant le coup il y a
                            // plus de piles isolée qu'après
    for (int i = 0; i < taillebis; i++) {
      if (coup.destination == tabbis[i])
        return (taillebis - taille) * 50;
    }
    return (taillebis - taille) * 80; // score attribué pour un coup qui isole
  }
  return 0;
}

/*************** Sous fonction pile isolée ***************/

int pile_isole(T_Position pos, int tab[]) {

  T_Voisins voisins;
  int compteur = 0;
  int taille = 0;
  for (int i = 0; i < 48; i++) { // fonction qui parcourt tout le plateau

    if (pos.cols[i].couleur ==
        pos.trait) { // regarde si le pion qui est potentiellement isolé est de
                     // notre couleur
      voisins = getVoisins(i); // on récupère les voisins
      for (int j = 0; j < voisins.nb;
           j++) { // boucle qui parcourt tous les voisins
        if ((pos.cols[voisins.cases[j]].nb + pos.cols[i].nb) < 5 &&
            pos.cols[voisins.cases[j]].nb !=
                0) // si la pile ne peut être empilée ou si elle n'a pas de
                   // voisins
          compteur++;
      }
      if (compteur == 0) { // condition pour enregister le coup
        tab[taille] = i;
        taille++;
      }
      compteur = 0;
    }
  }
  tab[taille] = -1; // permet d'indiquer la fin du tableau
}

/*************** Fonctions Pile de taille X ***************/

int pile2(T_Coup coup, T_Position pos) {
  int score = 0;
  int nbVoisinAdverse = 0;
  int nbVoisinAllie = 0;
  int pile3Voisine = 0;
  T_Position posbis;
  T_Voisins voisin;
  posbis = pos;
  posbis = jouerCoup(posbis, coup.origine, coup.destination);
  voisin = getVoisins(coup.destination);

  for (int j = 0; j < voisin.nb; j++) {
    if ((posbis.cols[voisin.cases[j]].nb == 1) &&
        (posbis.cols[voisin.cases[j]].couleur == posbis.trait))
      nbVoisinAdverse += 1; // Voisin de taille 1 et couleur adverse

    else if ((posbis.cols[voisin.cases[j]].nb ==
              3)) { // pile de 3 proche => coup non viable
      if (posbis.cols[voisin.cases[j]].couleur !=
          pos.cols[coup.origine]
              .couleur) { // pile3 et pile2 de couleurs differentes
        pile3Voisine = 1;
      } else if (pos.cols[coup.origine].couleur ==
                 posbis.trait) { // pile2 et pile3 de couleur adverse
        pile3Voisine = 1;
      }

    }

    else if ((posbis.cols[voisin.cases[j]].nb == 1) &&
             (posbis.cols[voisin.cases[j]].couleur != posbis.trait))
      nbVoisinAllie += 1; // Voisin de taille 1 et de notre couleur -> utile
                          // pour "défendre" la pile
  }

  if (pos.cols[coup.destination].nb + pos.cols[coup.origine].nb ==
      2) { // pile de 2
    if (pile3Voisine == 1) {
      score = -300;
    } else if ((pos.cols[coup.origine].couleur == posbis.trait) &&
               (pos.cols[coup.destination].couleur ==
                posbis.trait)) { // pion adverse sur adverse
      score = 25 + nbVoisinAllie - nbVoisinAdverse;
    } else if ((pos.cols[coup.origine].couleur == pos.trait) &&
               (pos.cols[coup.destination].couleur ==
                posbis.trait)) { // pion allié sur adverse
      score = 20 + nbVoisinAllie - nbVoisinAdverse;
    } else if ((pos.cols[coup.origine].couleur == pos.trait) &&
               (pos.cols[coup.destination].couleur ==
                pos.trait)) { // pion allié sur allié
      score = 5 + nbVoisinAllie - nbVoisinAdverse;
    }

  } else
    score = 0;

  return score;
}

int pile3(T_Coup coup, T_Position pos) {
  int score = 0;
  int nbvoisinAdverse = 0;
  int nbvoisinAllie = 0;
  int pile2Adverse = 0;
  T_Position posbis;
  T_Voisins voisin;
  posbis = pos;
  posbis = jouerCoup(posbis, coup.origine, coup.destination);
  voisin = getVoisins(coup.destination);

  for (int j = 0; j < voisin.nb; j++) {
    if ((posbis.cols[voisin.cases[j]].nb == 1) &&
        (posbis.cols[voisin.cases[j]].couleur == posbis.trait))
      nbvoisinAdverse += 1; // Voisin de taille 1 et couleur adverse
    else if ((posbis.cols[voisin.cases[j]].nb == 2) &&
             (posbis.cols[voisin.cases[j]].couleur == posbis.trait))
      pile2Adverse =
          1; // Voisin de taille 2 et couleur adverse -> coup non viable
    else if ((posbis.cols[voisin.cases[j]].nb == 1) &&
             (posbis.cols[voisin.cases[j]].couleur == pos.trait))
      nbvoisinAllie += 1; // Voisin de taille 1 et de notre couleur -> utile
                          // pour "défendre" la pile
  }

  if ((pos.cols[coup.origine].couleur == pos.trait) &&
      (pos.cols[coup.destination].nb + pos.cols[coup.origine].nb ==
       3)) { // pile de 3 de notre couleur
    if (pile2Adverse ==
        0) { // Pas de pile de 2 adverse pouvant prendre notre pile de 3
             // directement et au moins un pion de notre couleur au cas où
             // l'adversaire recouvre notre pile de 3
      if ((nbvoisinAdverse >= 1) &&
          (nbvoisinAllie >=
           1)) { // Au moins un voisin de notre couleur pour défendre
        if (pos.cols[coup.destination].couleur != pos.trait)
          score = 40 + (nbvoisinAllie - nbvoisinAdverse) *
                           2; // Score en fonction du risque
        else
          score = 35 + (nbvoisinAllie - nbvoisinAdverse) * 2;
      } else if (nbvoisinAdverse == 0)
        score = 45 + (nbvoisinAllie - nbvoisinAdverse) * 2;
    } else
      score = -200;
  } else
    score = 0;

  return score;
}

int pile4(T_Coup coup, T_Position pos) {
  int score = 0;
  int voisinAdverse = 0;
  T_Position posbis;
  T_Voisins voisin;
  posbis = pos;
  posbis = jouerCoup(posbis, coup.origine, coup.destination);
  voisin = getVoisins(coup.destination);
  for (int j = 0; j < voisin.nb; j++) {
    if ((posbis.cols[voisin.cases[j]].nb == 1) &&
        (posbis.cols[voisin.cases[j]].couleur !=
         pos.cols[coup.origine].couleur)) {
      voisinAdverse +=
          1; // Si on a des voisins posant problème (de couleur adverse)
    }
  }
  if (pos.cols[coup.destination].nb + pos.cols[coup.origine].nb ==
      4) {                    // Pile de 4
    if (voisinAdverse == 0) { // Pile isolée des voisins adverses
      if (pos.cols[coup.origine].couleur ==
          pos.trait) { // pile 4 de notre couleur
        if (pos.cols[coup.destination].couleur !=
            pos.trait) { // recouvre pion(s) adverse(s)
          score = 60;
        }

        else // recouvre pion(s) allié(s)
          score = 50;
      }

    } else
      score = -300;

  } else
    score = 0;

  return score;
}

int pile5(T_Coup coup,
          T_Position pos) { // teste si le coup permet de former une pile de 5
  int score = 0;
  if ((pos.cols[coup.origine].couleur == pos.trait) &&
      (pos.cols[coup.destination].nb + pos.cols[coup.origine].nb == 5)) {
    // si on forme une pile de 5 de notre couleur
    if (pos.cols[coup.destination].couleur !=
        pos.trait) { // on recouvre des pions adverses
      score = 100;
    } else
      score = 80; // on recouvre nos pions
  } else if (pos.cols[coup.destination].nb + pos.cols[coup.origine].nb == 5 &&
             pos.cols[coup.destination].couleur != pos.trait) {
    score = 5; // on fait une pile de 5 uniquement avec des pions adverses
  }
  return score;
}

/*************** Fonction d'observation des couleurs ***************/

int sid_couleur(T_Coup coup, T_Position pos) {
  if (pos.cols[coup.origine].couleur != pos.trait &&
      pos.cols[coup.destination].couleur != pos.trait) {
    return 40; // empiler un adverse sur adverse
  } else if (pos.cols[coup.origine].couleur == pos.trait &&
             pos.cols[coup.destination].couleur != pos.trait) {
    return 50; // pion allié sur adverse
  } else if (pos.cols[coup.origine].couleur == pos.trait &&
             pos.cols[coup.destination].couleur == pos.trait) {
    return 10; // pion allié sur allié
  } else if (pos.cols[coup.origine].couleur != pos.trait &&
             pos.cols[coup.destination].couleur == pos.trait) {
    return -50; // pion adverse sur allié
  }
}

T_Coup score(T_ListeCoups ListeCoup, T_Position pos) {
  int score[ListeCoup.nb];
  T_Coup Coup_sortant; // variable qui contient le coup le plus optimisé selon
                       // nos fonctions
  int best = 0;        // contient le score du meilleur coup
  int coup; // contient la position dans la liste rentrant du coup choisi

  for (int i = 0; i < ListeCoup.nb;
       i++) { // Cette boucle nous permmet de regarder tout les coups

    // On va maintenant appeler toutes les fonctions qui vont donner un score au
    // coup
    score[i] = pile5(ListeCoup.coups[i], pos);
    score[i] += pile4(ListeCoup.coups[i], pos);
    score[i] += pile3(ListeCoup.coups[i], pos);
    score[i] += pile2(ListeCoup.coups[i], pos);
    score[i] += coup_isole(ListeCoup.coups[i], pos);
    score[i] += Coup_Futur_deplacer(ListeCoup.coups[i], pos);
    score[i] += sid_couleur(ListeCoup.coups[i], pos);
    if (best < score[i]) { // si le score du nouveau coup est meilleur que
                           // l'ancien on le sauvegarde
      best = score[i];
      coup = i;
    }
  }
  Coup_sortant = ListeCoup.coups[coup];
  return Coup_sortant;
}

int recherchecoup(T_ListeCoups ListeCoup, T_Coup coup) {
  for (int i = 0; i < ListeCoup.nb; i++) { // on parcourt la liste
    if (coup.origine == ListeCoup.coups[i].origine &&
        coup.destination == ListeCoup.coups[i].destination)
      return i; // si le coup de la liste est identique à celui recherché, on
                // retourne sa position dans la liste de tous les coups
  }
  return 0; // On retourne 0 si il n'existe pas
}
