#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "getData.h"

// Declaration du type ResTraining, permettant de renvoyer les différents paramètres
// utilises et calcules a l'issu de l'entrainement du perceptron
typedef struct ResTraining{
    float w1;
    float w2;
    float b;
    float nu;
    float accuracy;
} ResTraining;


float CharToFloat(const char* str, size_t length, int c){

    /* Convertit un char* donne en entree en flottant.
        Pour les donnees quantitatives, la conversion est directe. En revanche, pour les donnees
        qualitatives (sexe et risque), les valeurs sont transformees en 0 et 1. 
        'c' est l'indice du champs du char* (permet de modifier ces donnees qualitatives) */


    // Copie 
    char copy_str[length + 1];   // declaration de la copie
    strncpy(copy_str, str, length); // copier tous les caracteres
    copy_str[length] = '\0'; // fin de la chaine de caracteres

    // Gestion du sexe M/F transforme en 0/1
    if (c == 2) {
        if (strcmp(copy_str, "M") == 0) {   // version ou M -> 1 et F -> 0
            copy_str[0] = '1';
            copy_str[1] = '\0';
        } else {
            copy_str[0] = '0';
            copy_str[1] = '\0';
        }
    }

    // Gestion du risque True/False (deviennent 1 et 0 respectivement)
    if (c == 8) {
        if (copy_str[0] == 'T') {
            copy_str[0] = '1';
            copy_str[1] = '\0';
        } else {
            copy_str[0] = '0';
            copy_str[1] = '\0';
        }
    }

    // Convertir la chaîne en flottant et retourner ce flottant
    float value = strtof(copy_str, NULL);
    return value;
}


int activation(float z){

    /* Fonction d'activation du perceptron (seuil = 0) */

    if (z>=0){
        return 1;
    }
    else {
        return 0;
    }
}


int countRisk(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data, int length_data){

    /* Renvoie le nombre de patients de data pour lesquels le risque = 1 */


    int cpt_risk = 0;

    for (int i=0; i<length_data; i++){
        int patient_id = data[i];
        int risque = CharToFloat(PatientsData[patient_id-1][8], strlen(PatientsData[patient_id-1][8]), 8);
        if (risque == 1){
            cpt_risk++;
        }
    }

    return cpt_risk;

}


float Accuracy(float w1, float w2, float b, int c1, int c2, int* data, int taille_data, char* PatientsData[MAX_PATIENTS][MAX_FIELDS]){

    /* Calcul le risque des patients a partir des valeurs de w1, w2 et b determines par le perceptron,
        et renvoie le taux d'erreur de la classification des patients en comparant avec leur risque observe */

    // Declaration des variables
    int TP=0;
    int TN=0;
    int FP=0;
    int FN=0;  

    for (int i=0; i<taille_data; i++){  // pour chaque patient
        int patient = data[i];  // id du patient

        // Calcul du risque a partir de w1, w2, b et la fonction d'activation
        int risque_calcule = activation(CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1)*w1 + CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2)*w2 + b);
        if (risque_calcule != (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8)){    // s'il s'agit d'une erreur

            if ((int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) == 0){  // si la valeur reelle est Faux
                FP++;
            }
            else{
                FN++;
            }
        }
        else{
            if ((int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) == 0){  // si la valeur reelle est Faux
                TN++;
            }
            else{
                TP++;        
            }
        }
    }

    float accuracy = (float)(TP+TN)/(float)(TP+TN+FP+FN);

    return accuracy;
}


ResTraining perceptron(int c1, int c2, char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data_train, int length_data_train){

    /* Effectue l'entrainement du perceptron pour les champs c1 et c2 et retourne les valeurs de w1, w2 et b calculees,
        ainsi que le taux d'apprentissage nu utilise et l'accuracy obtenue sur les donnees d'entrainement */


    // Declaration des variables
    float w1=0;
    float w2=0;
    // les poids w1 et w2 sont initialises a 0 mais pourraient prendre une valeur aleatoire entre -1 et 1 par exemple
    // cela donnerait la commande suivante : w1 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;         idem pour w2
    float b=0;
    float nu=0.001;
    int iteration=0;
    float accuracy = Accuracy(w1, w2, b, c1, c2, data_train, length_data_train, PatientsData);
    int flag_modif = 1;
    float previous_accuracy = 0;

    // Tant que l'ecart d'accuracy entre 2 iterations est suffisament grand, et au maximum 100 itérations
    while (iteration < 100 && fabs(accuracy - previous_accuracy) > 0.001){

        previous_accuracy = accuracy;
        flag_modif = 0;

        for (int i=0; i<length_data_train; i++){   // pour chaque patient

            int patient = data_train[i];
          
            // Utilisation de la fonction d'activation pour estimer le risque du patient
            int risque = activation(CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1)*w1 + CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2)*w2 + b);

            // Comparaison du risque predit avec le risque attendu
            if (risque != (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8)){    // s'il s'agit d'une erreur

                // Modification des poids w1 et w2, ainsi que du biais b
                w1 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1);
                w2 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2);
                b += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque);

                flag_modif=1;
            }
        }
        iteration++;
        accuracy = Accuracy(w1, w2, b, c1, c2, data_train, length_data_train, PatientsData);    // calcul de l'accuracy pour ces nouvelles valeurs w1, w2 et b
    }

    // Resultat en sortie
    ResTraining res;
    res.w1=w1;
    res.w2=w2;
    res.b=b;
    res.nu=nu;
    res.accuracy=accuracy;

    return res;
}


ResTraining* Training(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data_train, int length_data_train){

    /* Effectue l'entrainement du perceptron pour obtenir les valeurs de w1, w2 et b pour chaque couple de champs */


    // Initialisation d'un tableau pour stocker pour chaque couple de champs, les valeurs de w1, w2 et b obtenues
    ResTraining* tabResPerceptron = (ResTraining*)malloc(21 * sizeof(ResTraining));

    int i_couple = 0;

    // Pour chaque couple (c1, c2) des champs de patients.pengu
    for (int c1 = 1; c1 < 8; c1++){
        for (int c2 = c1+1; c2 < 8; c2++){
            if (c1 != c2){
                
                // Entrainement du perceptron pour un couple
                ResTraining res = perceptron(c1, c2, PatientsData, data_train, length_data_train);
                tabResPerceptron[i_couple] = res;   // ajout des resultats au tableau
                
                i_couple++;
            }
        }
    }
    return tabResPerceptron;
}


float* Test(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], ResTraining* tabResPerceptron, int* data_test, int length_data_test){

    /* Renvoie l'accuracy pour chaque couple de champs */


    // Initialisation des variables pour se souvenir du meilleur couple
    int best_c1=1;
    int best_c2=2;
    float best_accuracy = Accuracy(tabResPerceptron[0].w1, tabResPerceptron[0].w2, tabResPerceptron[0].b, 1, 2, data_test, length_data_test, PatientsData);
    // Tableau pour stocker l'accuracy de chaque couple
    float* accuracy_couples = (float*)malloc(21 * sizeof(float));
    int i_couple = 0;

    // Pour chaque couple (c1, c2) des champs de patients.pengu
    for (int c1 = 1; c1 < 8; c1++){
        for (int c2 = c1+1; c2 < 8; c2++){
            if (c1 != c2){
                
                // Calcul de l'accuracy
                float accuracy = Accuracy(tabResPerceptron[i_couple].w1, tabResPerceptron[i_couple].w2, tabResPerceptron[i_couple].b, c1, c2, data_test, length_data_test, PatientsData);
                accuracy_couples[i_couple] = accuracy;      // ajout au tableau
                i_couple++;

                if (accuracy > best_accuracy){ // Memoriser les meilleurs champs et leur accuracy
                    best_c1 = c1;
                    best_c2 = c2;
                    best_accuracy = accuracy;
                }
            }
        }
    }
    //printf("meilleure accuracy = %f (%d ; %d)\n", best_accuracy, best_c1, best_c2);

    return accuracy_couples;
}


float* meanAccuracy(int N, char* PatientsData[MAX_PATIENTS][MAX_FIELDS], ResTraining* tabResPerceptron, int* data_test, int length_data_test){

    /* Effectue le test N fois et retourne la moyenne des accuracies de chaque couple de champs

        Cette fonction est utilisee pour avoir une vision plus globale des accuracies calculees,
        etant donnee que 2 executions ne donnent jamais exactement les memes resultats. */


    // Tableau de stockages des moyennes des accuracies
    float* meanAccuracy = (float*)malloc(21 * sizeof(float));
    memset(meanAccuracy, 0, 21 * sizeof(float));    // initialisation avec des 0

    // Somme des N accuracies calculees
    for (int i=0; i<N; i++){
        float* accuracies = Test(PatientsData, tabResPerceptron, data_test, length_data_test);
        for (int k=0; k<21; k++){
            meanAccuracy[k] += accuracies[k];
        }  
    }

    // Division de la somme pour obtenir des moyennes arithmetiques
    for (int k=0; k<21; k++){
            meanAccuracy[k] /= N;
    }

    return meanAccuracy;
}


void explorationResultatsTest(float* tabAccuracyCouples){

    /* Affiche les accuracies de chaque couple, stockees dans le tableau donne en entree */


    int i_couple=0;

    // Parcourir tous les couples de champs
    for (int c1 = 1; c1 < 8; c1++){
        for (int c2 = c1+1; c2 < 8; c2++){
            if (c1 != c2){
                printf("(%d ; %d) -> %f\n", c1, c2, tabAccuracyCouples[i_couple]);
                i_couple++;
            }
        }
    }


}