#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


typedef struct ResSampling {
        int* tab1;
        int* tab2;
    } ResSampling;


//// Fonctions pour la construction des deux jeux de donnees ////

int* creerTableauEntiers(int N){

    /* Retourne un tableau des entiers de 1 à N */


    int* tableau = (int*)malloc(N * sizeof(int));

    // Remplir le tableau avec les entiers de 1 à N
    for (int i = 0; i < N; i++) {
        tableau[i] = i + 1;
    }

    return tableau; // Retourne le pointeur vers le tableau
}


void melangerTableau(int* tableau, int taille){

    /* Melange le tableau donne en entree (algorithme de Fisher-Yates, melange de Knuth) */


    srand((unsigned int)time(NULL));

    for (int i = taille - 1; i > 0; i--){   // pour chaque i

        int j = rand() % (i + 1);   // choisir un j aleatoirement

        // Echanger tableau[i] et tableau[j]
        int temp = tableau[i];
        tableau[i] = tableau[j];
        tableau[j] = temp;
    }
}


ResSampling sampling(int N1, float p){

    /* Cette fonction permet de separer l'ensemble des donnees en deux : donnees d'entrainement et donnees de test.

        Elle prend en entree le plus grand element N1 (taille du tableau initial) et le pourcentage p que l'on veut piocher
        pour former le premier tableau.
        Retourne deux tableaux de taille reduite, avec respectivement N2 et N3 elements pioches aleatoirement entre 1 et N1,
        avec (N2 union N3 = N1) et (N2 inter N3 = ensemble vide) */


    // Cree un tableau d'entiers de 1 à N1
    int* tableau = creerTableauEntiers(N1);

    // Melange les elements de ce tableau
    melangerTableau(tableau, N1);

    // Calcul de la taille du premier tableau reduit (de taille N2) et initialisation de ce tableau
    int N2 = N1*p; // taille du tableau reduit
    int* tableau_reduit1 = (int*)malloc(N2 * sizeof(int));

    // Remplissage de tableau_reduit1 avec les premiers elements du tableau initial (melange)
    for (int i=0; i<N2; i++){
        tableau_reduit1[i] = tableau[i];
    }

    // Calcul de la taille du deuxieme tableau réduit (de taille N3) et initialisation de ce tableau
    int N3 = N1 - N2;
    int* tableau_reduit2 = (int*)malloc(N3 * sizeof(int));

    // Remplissage de tableau_reduit2 avec les derniers elements du tableau initial (melange)
    for (int i=N2; i<N1; i++){
        tableau_reduit2[i-N2] = tableau[i];
    }

    ResSampling res;
    res.tab1 = tableau_reduit1;
    res.tab2 = tableau_reduit2;

    free(tableau);

    return res;
}