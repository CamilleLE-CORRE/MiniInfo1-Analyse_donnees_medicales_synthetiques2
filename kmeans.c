#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <float.h>
#include "getData.h"


// Structure représentant un point (centroïde)
typedef struct {
    float coordonnees[13];
} Point;

// Structure pour stocker les clusters (chaque cluster contient les indices des points)
typedef struct {
    int indicesPatient[5001];
    int size;
} Cluster;


void convertData(char* PatientsData[5001][13], double ConvertedData[5001][13]) {
    for (int i = 0; i < 5001; i++) {
        for (int j = 0; j < 13; j++) {
            if (PatientsData[i][j] == NULL) {
                // Si une case est vide, on ignore
                ConvertedData[i][j] = -1;
                continue;
            }

            if (j == 2) { // Colonne sexe
                // "M" => 1, "F" => 0
                if (strcmp(PatientsData[i][j], "M") == 0) {
                    ConvertedData[i][j] = 1;
                } else {
                    ConvertedData[i][j] = 0;
                }
            } else if (j == 8) { // Colonne risk
                // "True" => 1, "False" => 0
                if (strcmp(PatientsData[i][j], "True\r") == 0) {
                    ConvertedData[i][j] = 1;
                } else {
                    ConvertedData[i][j] = 0;
                }

            } else {
                // Conversion standard en nombre flottant
                ConvertedData[i][j] = atof(PatientsData[i][j]);
            }
        }
    }
}


// Fonction pour générer un nombre flottant aléatoire dans un intervalle [min, max]
double random_double(double min, double max) {
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

// Fonction pour initialiser les K centroïdes avec 13 coordonnées
void init_centroides(Point *centroides, int K, double min[13], double max[13]) {
    for (int i = 0; i < K; i++) {
        // Commence à 1 pour ne pas prendre en compte l'id
        for (int j = 1; j < 13; j++) {
            // Chaque coordonnée est générée dans l'intervalle spécifique de la dimension j
            centroides[i].coordonnees[j] = random_double(min[j], max[j]);
        }
    }
}


void min_max_par_dimension(double data[5001][13], double min[13], double max[13]) {
    // Initialiser les min et max pour chaque dimension
    for (int j = 0; j < 13; j++) {
        min[j] = DBL_MAX;
        max[j] = -DBL_MAX;
    }

    // Parcours de chaque point et chaque dimension
    for (int i = 0; i < 5001; i++) {
        // Commence à 1 pour ne pas prendre en compte l'id
        for (int j = 1; j < 13; j++) { 
            // Si la valeur est valide
            if (data[i][j] != -1) {
                if (data[i][j] < min[j]) {
                    min[j] = data[i][j];
                }
                if (data[i][j] > max[j]) {
                    max[j] = data[i][j];
                }
            }
        }
    }
}


// Fonction pour calculer la distance Euclidienne entre un point et un centroïde
float distance_euclidienne(Point p, Point c){
    float sum = 0.0;
    int valid_dimensions = 0;

    for (int j = 1; j < 13; j++) {
        if (p.coordonnees[j] != -1 && c.coordonnees[j] != -1) { // Exclure les dimensions manquantes
            //printf("%f, %f\n", p.coordonnees[i], c.coordonnees[i]);
            sum += (p.coordonnees[j] - c.coordonnees[j]) * (p.coordonnees[j] - c.coordonnees[j]);
            valid_dimensions++;
        }
    }

    return sqrt(sum / valid_dimensions); // Moyenne pondérée sur les dimensions valides
}


void assigner_les_points(Point points[5001], Point centroids[5001], Cluster clusters[5001], int K) {
    // Initialiser les clusters
    for (int i = 0; i < K; i++) {
        clusters[i].size = 0;  // Réinitialiser la taille de chaque cluster
    }

    // Pour chaque point, on calcule la distance aux K centroïdes et on l'assigne au centroïde le plus proche
    for (int i = 0; i < 5001; i++) {
        Point p = points[i];
        int closest_centroid = 0;
        float min_distance = distance_euclidienne(p, centroids[0]);

        for (int j = 1; j < K; j++) {
            float dist = distance_euclidienne(p, centroids[j]);
            if (dist < min_distance) {
                min_distance = dist;
                closest_centroid = j;
            }
        }

        // Assigner le point au cluster du centroïde le plus proche
        clusters[closest_centroid].indicesPatient[clusters[closest_centroid].size] = i;
        clusters[closest_centroid].size++;
    }
}


int nouveaux_centroides(Point points[5001], Cluster clusters[], Point centroides[], int K, double seuil) {
    // Parcours de chaque cluster
    int stop_iteration = 1;
    for (int i = 0; i < K; i++) {
        // Initialiser la somme des coordonnées à zéro
        double somme[13] = {0}; // Somme pour chaque dimension
        int compteur[13] = {0}; // Compteur pour chaque dimension (pour éviter les -1)
        int nb_points = clusters[i].size; // Nombre de points dans le cluster

        if (nb_points == 0) {
            // Si le cluster est vide, on ne peut pas calculer un nouveau centroïde
            printf("Cluster %d est vide. Aucun nouveau centroïde calculé.\n", i + 1);
            continue;
        }

        // Ajouter les coordonnées des points au total
        for (int j = 0; j < nb_points; j++) {
            int indice_Patient = clusters[i].indicesPatient[j]; // Indice du point dans le tableau "points"
            for (int d = 1; d < 13; d++) {
                if (points[indice_Patient].coordonnees[d] != -1) { // Vérifier que la valeur est valide
                    somme[d] += points[indice_Patient].coordonnees[d];
                    compteur[d]++;
                }
            }
        }

        double moy_intermediaire;
        // Calculer la moyenne pour chaque dimension
        for (int d = 1; d < 13; d++) {
            if (compteur[d] > 0) {
                moy_intermediaire = somme[d] / compteur[d];
                if (fabs(centroides[i].coordonnees[d] - moy_intermediaire) > seuil) {
                    stop_iteration = 0;
                }
                centroides[i].coordonnees[d] = moy_intermediaire;
            } else {
                centroides[i].coordonnees[d] = -1; // Aucun point valide pour cette dimension
            }
        }
    }
    return stop_iteration;
}


void kmeans(char* PatientsData[5001][13]){

    // Initialisation de la structure de donnees

    double ConvertedData[5001][13];

    //memset(PatientsData, 0, sizeof(PatientsData));
    memset(ConvertedData, 0, sizeof(ConvertedData));

    // Conversion du tableau de données en un tableau de données contenant des valeurs numériques
    convertData(PatientsData, ConvertedData);

    /*
    // Afficher les premières lignes des données converties pour vérification
    for (int i = 0; i < 10; i++) { // Afficher 5 patients
        printf("Patient %d: ", i + 1);
        for (int j = 0; j < 13; j++) {
            printf("%.2f ", ConvertedData[i][j]);
        }
        printf("\n");
    }
    */

    //////// Initialisation : Point A /////////

    srand(time(NULL));  // Initialisation du générateur de nombres aléatoires

    int K = 2;

    // Recherche des bornes min et max pour chaque dimension
    double min[13], max[13];
    min_max_par_dimension(ConvertedData, min, max);

    // Initialisation des centroides
    Point centroides[K];
    init_centroides(centroides, K, min, max);

    // Affichage des centroïdes générés
    for (int i = 0; i < K; i++) {
        printf("Centroide %d: ( %f", i+1, centroides[i].coordonnees[1]);
        for (int j = 2; j < 13; j++) {
            printf(", %f", centroides[i].coordonnees[j]);
        }
        printf(")\n");
    }

    ///////// Initialisation : Point B /////////
    Point points[5001];
    for (int i = 0; i < 5001; i++) {
        for (int j = 0; j < 13; j++) {
            points[i].coordonnees[j] = ConvertedData[i][j];
        }
    }

    Cluster clusters[K];
    int indiceIteration = 1;
    int iterationMax = 12; //12 par défaut
    double seuil = 0.05;

    do {
        printf("Iteration : %d\n", indiceIteration);
        assigner_les_points(points, centroides, clusters, K);

        // Affichage des résultats
        for (int i = 0; i < K; i++) {
            printf("Cluster %d: ", i + 1);
            for (int j = 1; j < clusters[i].size; j++) {
                printf("%d ", clusters[i].indicesPatient[j]);  // Affiche les indices des points dans le cluster
            }
            printf("\n");
        }

        // nouveaux_centroides retourne 1 s'il n'y plus de mouvements significatifs 
        if (nouveaux_centroides(points, clusters, centroides, K, seuil)==1){
            printf("Plus de mouvements détectés au dessus du seuil %f lors de l'itération %d\n", seuil, indiceIteration);
            indiceIteration = iterationMax;
        } else {
            // Affichage des nouveaux centroïdes
            for (int i = 0; i < K; i++) {
                printf("Centroïde %d : ", i + 1);
                for (int j = 1; j < 13; j++) {
                    printf("%.2f ", centroides[i].coordonnees[j]);
                }
                printf("\n");
            }
            indiceIteration++;
        }
        
    } while (indiceIteration<iterationMax);
}