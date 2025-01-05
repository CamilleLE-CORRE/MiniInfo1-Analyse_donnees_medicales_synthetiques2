#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getData.h"
#include "sampling.h"
#include "perceptron.h"
#include "perceptronBonus.h"

#define MAX_PATIENTS 5001
#define MAX_FIELDS 20
#define SAMPLING_RATIO 0.7 


int main(){

    //// INITIALISATION DU TABLEAU PATIENTSDATA ////
    char* PatientsData[MAX_PATIENTS][MAX_FIELDS];   // declaration du tableau

    memset(PatientsData, 0, sizeof(PatientsData));  // initialisation a 0

    // Remplir le tableau avec les donnees des fichiers patients et livestyle
    BrowseFilePatients("patients.pengu", PatientsData);
    BrowseFileLivestyle("lifestyle.pengu", PatientsData);

    // Affichage de quelques donnees de patients
    // for (int patient_id=1; patient_id<4; patient_id++){    

    //     printf("id : %s | age : %s | sexe : %s | weight : %s | blood_pressure : %s | average_heart_rate : %s | blood_sugar : %s | cholesterol : %s | risk : %s | physical_activity : %s | alcohol_consumption : %s | caffeine_consumption : %s | sleep_quality : %s\n", 
    //         PatientsData[patient_id-1][0], PatientsData[patient_id-1][1], PatientsData[patient_id-1][2], PatientsData[patient_id-1][3], PatientsData[patient_id-1][4], 
    //         PatientsData[patient_id-1][5], PatientsData[patient_id-1][6], PatientsData[patient_id-1][7], PatientsData[patient_id-1][8], 
    //         PatientsData[patient_id-1][9], PatientsData[patient_id-1][10], PatientsData[patient_id-1][11], PatientsData[patient_id-1][12]);
    // }


    //// ECHANTLLONAGE : division des donnees en deux jeux de donnees disjoints (donnees d'entrainement et donnees de test) ////
    ResSampling res = sampling(5000, SAMPLING_RATIO);    // mettre ensuite une varibale pour 0.7
    int* data_train = res.tab1;
    int* data_test = res.tab2;
    int length_data_train = SAMPLING_RATIO * 5000;
    int length_data_test = (1 - SAMPLING_RATIO) * 5000;

    //// ENTRAINEMENT DU PERCEPTRON ////
    ResTraining* tabResPerceptron = Training(PatientsData, data_train, 3500);

    // Visualisation des parametres calcules et des resultats de l'entrainement du perceptron
    // for (int i = 0; i < 21; i++) {
    //     printf("tabResPerceptron[%d] -> w1: %f, w2: %f, b: %f, accuracy: %f\n", 
    //             i, tabResPerceptron[i].w1, tabResPerceptron[i].w2, tabResPerceptron[i].b, tabResPerceptron[i].accuracy);
    // }

    //// TEST ////
    //float* tabAccuracyCouples = Test(PatientsData, tabResPerceptron, data_test, 1500);
    //explorationResultatsTest(meanAccuracy(100, PatientsData, tabResPerceptron, data_test, 1500));
    // for (int i = 0; i < 21; i++) {
    //     printf("tabAccuracyCouples[%d] -> acc=%f\n", i, tabAccuracyCouples[i]);
    // }
//     printf("Taille de tabAccuracyCouples : %lu éléments\n", sizeof(tabAccuracyCouples) / sizeof(tabAccuracyCouples[0]));

    //explorationResultatsTest(tabAccuracyCouples);

    //// BONUS ////
    ResTrainingBonus resPerceptronBonus = perceptronBonus(PatientsData, data_train, length_data_train);
    printf("w1=%f ; w2=%f ; w3=%f ; w4=%f ; w5=%f ; w6=%f ; w7=%f\n", resPerceptronBonus.w1, resPerceptronBonus.w2, resPerceptronBonus.w3, resPerceptronBonus.w4, resPerceptronBonus.w5, resPerceptronBonus.w6, resPerceptronBonus.w7);
    float accuracy = TestBonus(PatientsData, resPerceptronBonus, data_test, length_data_test);
    printf("Accuracy perceptron a 7 dimensions : %f\n", accuracy);


    // Libération de la mémoire allouée
    for (int i = 0; i < MAX_PATIENTS; i++) {
        for (int j = 0; j < MAX_FIELDS; j++) {
            if (PatientsData[i][j]) {
                free(PatientsData[i][j]);
            }
        }
    }

    return 0;
}