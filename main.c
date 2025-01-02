#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getData.h"
#include "perceptron.h"

#define MAX_PATIENTS 5001
#define MAX_FIELDS 20


int main(){

    // Initialisation du tableau Patients Data
    char* PatientsData[MAX_PATIENTS][MAX_FIELDS];

    memset(PatientsData, 0, sizeof(PatientsData));

    // Remplir le tableau avec les donnees des fichiers patients et livestyle
    BrowseFilePatients("patients.pengu", PatientsData);
    BrowseFileLivestyle("lifestyle.pengu", PatientsData);

    // Affichage de quelques donnees de patients
    // for (int patient_id=4026; patient_id<4030; patient_id++){    

    //     printf("id : %s | age : %s | sexe : %s | weight : %s | blood_pressure : %s | average_heart_rate : %s | blood_sugar : %s | cholesterol : %s | risk : %s | physical_activity : %s | alcohol_consumption : %s | caffeine_consumption : %s | sleep_quality : %s\n", 
    //         PatientsData[patient_id-1][0], PatientsData[patient_id-1][1], PatientsData[patient_id-1][2], PatientsData[patient_id-1][3], PatientsData[patient_id-1][4], 
    //         PatientsData[patient_id-1][5], PatientsData[patient_id-1][6], PatientsData[patient_id-1][7], PatientsData[patient_id-1][8], 
    //         PatientsData[patient_id-1][9], PatientsData[patient_id-1][10], PatientsData[patient_id-1][11], PatientsData[patient_id-1][12]);
    // }

    // Echantillonage : division des donnees en deux jeux de donnees disjoints (donnees d'entrainement et donnees de test)
    Res res = echantillonage(5000, 0.7);    // mettre ensuite une varibale pour 0.7
    int* data_train = res.tab1;
    int* data_test = res.tab2;
    
    // printf("train: ");
    // for (int i=0; i<7; i++){
    //     printf("%d ", data_train[i]);
    // }
    // printf("\n");
    // printf("test: ");
    // for (int i=0; i<3; i++){
    //     printf("%d ", data_test[i]);
    // }
    // printf("\n");

    // Entrainement
    // int tab[20] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    // int* data_train2 = tab;
    //perceptron(1,2,PatientsData,data_train,3500);
    ResTraining* tabResPerceptron = Training(PatientsData, data_train, 3500);
    // for (int i = 0; i < 21; i++) {
    //     printf("tabResPerceptron[%d] -> w1: %f, w2: %f, b: %f, accuracy: %f\n", 
    //             i, tabResPerceptron[i].w1, tabResPerceptron[i].w2, tabResPerceptron[i].b, tabResPerceptron[i].accuracy);
    // }

    // Test
    //float* tabAccuracyCouples = Test(PatientsData, tabResPerceptron, data_test, 1500);
    explorationResultatsTest(meanAccuracy(10000, PatientsData, tabResPerceptron, data_test, 1500));
    // for (int i = 0; i < 21; i++) {
    //     printf("tabAccuracyCouples[%d] -> acc=%f\n", i, tabAccuracyCouples[i]);
    // }
//     printf("Taille de tabAccuracyCouples : %lu éléments\n", sizeof(tabAccuracyCouples) / sizeof(tabAccuracyCouples[0]));

    //explorationResultatsTest(tabAccuracyCouples);


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