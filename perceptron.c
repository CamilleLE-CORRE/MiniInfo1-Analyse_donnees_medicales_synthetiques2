#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "getData.h"


typedef struct ResTraining{
    float w1;
    float w2;
    float b;
    float nu;
    float accuracy;
} ResTraining;


float CharToFloat(const char* array, size_t length, int c) {

    //// ChatGPT

    // Vérification de validité du pointeur
    if (array == NULL || length == 0) {
        fprintf(stderr, "Erreur : tableau non valide.\n");
        return 0.0;
    }

    // Créer une copie locale modifiable
    char local_array[length + 1]; // +1 pour le '\0'
    strncpy(local_array, array, length); // Copier jusqu'à length caractères
    local_array[length] = '\0'; // S'assurer que c'est bien une chaîne terminée

    // Gestion du sexe M/F transforme en 0/1
    if (c == 2) {
        if (strcmp(local_array, "M") == 0) {
            local_array[0] = '1';
            local_array[1] = '\0';
        } else {
            local_array[0] = '0';
            local_array[1] = '\0';
        }
    }

    // Gestion du risque True/False
    if (c == 8) {
        if (local_array[0] == 'T') {
            local_array[0] = '1';
            local_array[1] = '\0';
        } else {
            local_array[0] = '0';
            local_array[1] = '\0';
        }
    }

    // Convertir la chaîne en flottant
    float value = strtof(local_array, NULL);
    return value;
}


int activation(float z){
    if (z>=0){
        return 1;
    }
    else {
        return 0;
    }
}

int countRisk(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data, int taille_data){

    /* Renvoie le nombre de patients de data pour lesquels le risque = 1 */

    int cpt_risque = 0;

    for (int i=0; i<taille_data; i++){
        int patient_id = data[i];
        int risque = CharToFloat(PatientsData[patient_id-1][8], strlen(PatientsData[patient_id-1][8]), 8);
        if (risque == 1){
            cpt_risque++;
        }
    }

    return cpt_risque;

}

float calculTauxErreur(float w1, float w2, float b, int c1, int c2, int* data, int taille_data, char* PatientsData[MAX_PATIENTS][MAX_FIELDS]){

    /* Calcul le risque des patients a partir des valeurs de w1, w2 et b determines par le perceptron,
        et renvoie le taux d'erreur de la classification des patients en comparant avec leur risque observe */

    // Declaration des variables
    // Declaration des variables
    float vect_x1[3500];
    float vect_x2[3500];
    float vect_risque[3500];    // vecteur contenant des 0 ou des 1
    float error_rate;
    int cpt_erreurs;
    int MAX_erreurs = 0;

    // Stocker les valeurs des champs c1 et c2 pour tous les patients, ainsi que leur risque respectif
    for (int i=1; i<3501; i++){
        vect_x1[i-1] = CharToFloat(PatientsData[i-1][c1], strlen(PatientsData[i-1][c1]), c1);
        vect_x2[i-1] = CharToFloat(PatientsData[i-1][c2], strlen(PatientsData[i-1][c2]), c2);
        vect_risque[i-1] = CharToFloat(PatientsData[i-1][8], strlen(PatientsData[i-1][8]), 8);
    }

    for (int i=0; i<taille_data; i++){  // pour chaque patient
        int patient = data[i];  // id du patient

        // Calcul du risque a partir de w1, w2, b et la fonction d'activation
        int risque_calcule = activation(vect_x1[patient-1]*w1 + vect_x2[patient-1]*w2 + b);

        if (risque_calcule != (int)vect_risque[patient-1]){    // s'il s'agit d'une erreur
                MAX_erreurs++;
        }
    }

    //printf("MAX erreurs dans calculTauxErreur = %d\n", MAX_erreurs);

    return (float)MAX_erreurs/(float)taille_data;

}

float Accuracy(float w1, float w2, float b, int c1, int c2, int* data, int taille_data, char* PatientsData[MAX_PATIENTS][MAX_FIELDS]){

    /* Calcul le risque des patients a partir des valeurs de w1, w2 et b determines par le perceptron,
        et renvoie le taux d'erreur de la classification des patients en comparant avec leur risque observe */

    // Declaration des variables
    int TP=0;
    int TN=0;
    int FP=0;
    int FN=0;

    // Stocker les valeurs des champs c1 et c2 pour tous les patients, ainsi que leur risque respectif
    // for (int i=1; i<taille_data+1; i++){
    //     vect_x1[i-1] = CharToFloat(PatientsData[i-1][c1], strlen(PatientsData[i-1][c1]), c1);
    //     vect_x2[i-1] = CharToFloat(PatientsData[i-1][c2], strlen(PatientsData[i-1][c2]), c2);
    //     vect_risque[i-1] = CharToFloat(PatientsData[i-1][8], strlen(PatientsData[i-1][8]), 8);
    // }

    

    for (int i=0; i<taille_data; i++){  // pour chaque patient
        int patient = data[i];  // id du patient

        // Calcul du risque a partir de w1, w2, b et la fonction d'activation
        int risque_calcule = activation(CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1)*w1 + CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2)*w2 + b);
        if (risque_calcule != (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8)){    // s'il s'agit d'une erreur
            //MAX_erreurs++;
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
    float b=0;
    float nu=0.00001;   //prendre ensuite une valeur aléatoire ??
    int iteration=0;
    float accuracy = Accuracy(w1, w2, b, c1, c2, data_train, length_data_train, PatientsData);
    int flag_modif = 1;


    // Initialisation de w1 et w2 par un flottant aleatoire dans [-1;1]
    srand(time(NULL));
    // w1 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    // w2 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

    float previous_accuracy = 0;

    while (iteration < 100 && fabs(accuracy - previous_accuracy) > 0.001){ //&& (previous_accuracy - accuracy) < 0.01){    // au maximum 100 itérations

        previous_accuracy = accuracy;
        flag_modif = 0;

        //printf("iteration %d :\n", iteration);

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
        accuracy = Accuracy(w1, w2, b, c1, c2, data_train, length_data_train, PatientsData);
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

    int best_c1=1;
    int best_c2=2;
    float best_accuracy = Accuracy(tabResPerceptron[0].w1, tabResPerceptron[0].w2, tabResPerceptron[0].b, 1, 2, data_test, length_data_test, PatientsData);
    float* accuracy_couples = (float*)malloc(21 * sizeof(float));

    int i_couple = 0;

    // Pour chaque couple (c1, c2) des champs de patients.pengu
    for (int c1 = 1; c1 < 8; c1++){
        for (int c2 = c1+1; c2 < 8; c2++){
            if (c1 != c2){
                
                float accuracy = Accuracy(tabResPerceptron[i_couple].w1, tabResPerceptron[i_couple].w2, tabResPerceptron[i_couple].b, c1, c2, data_test, length_data_test, PatientsData);
                accuracy_couples[i_couple] = accuracy;
                i_couple++;

                if (accuracy > best_accuracy){ // Memoriser les meilleurs champs et leur accuracy
                    best_c1 = c1;
                    best_c2 = c2;
                    best_accuracy = accuracy;
                }
            }
        }
    }
    //printf("meilleur accuracy = %f (%d ; %d)\n", best_accuracy, best_c1, best_c2);

    return accuracy_couples;
}

float* meanAccuracy(int N, char* PatientsData[MAX_PATIENTS][MAX_FIELDS], ResTraining* tabResPerceptron, int* data_test, int length_data_test){

    float* meanAccuracy = (float*)malloc(21 * sizeof(float));
    memset(meanAccuracy, 0, 21 * sizeof(float));

    // Somme
    for (int i=0; i<N; i++){
        float* accuracies = Test(PatientsData, tabResPerceptron, data_test, length_data_test);
        for (int k=0; k<21; k++){
            meanAccuracy[k] += accuracies[k];
        }  
    }

    // Division
    for (int k=0; k<21; k++){
            meanAccuracy[k] /= N;
    }

    return meanAccuracy;
}

void explorationResultatsTest(float* tabAccuracyCouples){
    //printf("Taille du tableau : %lu éléments\n", sizeof(tabAccuracyCouples) / sizeof(tabAccuracyCouples[0]));
    
    int i_couple=0;

    for (int c1 = 1; c1 < 8; c1++){
        for (int c2 = c1+1; c2 < 8; c2++){
            if (c1 != c2){
                printf("(%d ; %d) -> %f\n", c1, c2, tabAccuracyCouples[i_couple]);
                i_couple++;
            }
        }
    }
}