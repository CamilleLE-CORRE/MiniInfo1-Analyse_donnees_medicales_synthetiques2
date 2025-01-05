#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "getData.h"
#include "perceptron.h"

typedef struct ResTrainingBonus{
    float w1;
    float w2;
    float w3;
    float w4;
    float w5;
    float w6;
    float w7;
    float b;
    float accuracy;
} ResTrainingBonus;


float AccuracyBonus(float w1, float w2, float w3, float w4, float w5, float w6, float w7, float b, int* data, int taille_data, char* PatientsData[5001][13]){

    /* Calcul le risque des patients determine par le perceptron,
        et renvoie le taux d'erreur de la classification des patients en comparant avec leur risque observe */

    // Declaration des variables
    int TP=0;
    int TN=0;
    int FP=0;
    int FN=0;  

    for (int i=0; i<taille_data; i++){  // pour chaque patient
        int patient = data[i];  // id du patient

        int risque_calcule = activation(CharToFloat(PatientsData[patient-1][1], strlen(PatientsData[patient-1][1]), 1)*w1
                                    + CharToFloat(PatientsData[patient-1][2], strlen(PatientsData[patient-1][2]), 2)*w2
                                    + CharToFloat(PatientsData[patient-1][3], strlen(PatientsData[patient-1][3]), 3)*w3
                                    + CharToFloat(PatientsData[patient-1][4], strlen(PatientsData[patient-1][4]), 4)*w4
                                    + CharToFloat(PatientsData[patient-1][5], strlen(PatientsData[patient-1][5]), 5)*w5
                                    + CharToFloat(PatientsData[patient-1][6], strlen(PatientsData[patient-1][6]), 6)*w6
                                    + CharToFloat(PatientsData[patient-1][7], strlen(PatientsData[patient-1][7]), 7)*w7
                                    + b);   // pourrait etre optimise avec une boucle for

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


ResTrainingBonus perceptronBonus(char* PatientsData[5001][13], int* data_train, int length_data_train){

    /* Effectue l'entrainement du perceptron pour toutes les caracteristiques et retourne les valeurs calculees,
        ainsi que  l'accuracy obtenue sur les donnees d'entrainement */


    // Declaration des variables
    float w1=0;
    float w2=0;
    float w3=0;
    float w4=0;
    float w5=0;
    float w6=0;
    float w7=0;
    float b=0;
    float nu=0.001;
    int iteration=0;
    float accuracy = AccuracyBonus(w1, w2, w3, w4, w5, w6, w7, b, data_train, length_data_train, PatientsData);
    int flag_modif = 1;
    float previous_accuracy = 0;

    // Tant que l'ecart d'accuracy entre 2 iterations est suffisament grand, et au maximum 100 itérations
    while (iteration < 100 && fabs(accuracy - previous_accuracy) > 0.001){

        previous_accuracy = accuracy;
        flag_modif = 0;

        for (int i=0; i<length_data_train; i++){   // pour chaque patient

            int patient = data_train[i];
          
            // Utilisation de la fonction d'activation pour estimer le risque du patient
            int risque = activation(CharToFloat(PatientsData[patient-1][1], strlen(PatientsData[patient-1][1]), 1)*w1
                                    + CharToFloat(PatientsData[patient-1][2], strlen(PatientsData[patient-1][2]), 2)*w2
                                    + CharToFloat(PatientsData[patient-1][3], strlen(PatientsData[patient-1][3]), 3)*w3
                                    + CharToFloat(PatientsData[patient-1][4], strlen(PatientsData[patient-1][4]), 4)*w4
                                    + CharToFloat(PatientsData[patient-1][5], strlen(PatientsData[patient-1][5]), 5)*w5
                                    + CharToFloat(PatientsData[patient-1][6], strlen(PatientsData[patient-1][6]), 6)*w6
                                    + CharToFloat(PatientsData[patient-1][7], strlen(PatientsData[patient-1][7]), 7)*w7
                                    + b);   // pourrait etre optimise avec une boucle for

            // Comparaison du risque predit avec le risque attendu
            if (risque != (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8)){    // s'il s'agit d'une erreur

                // Modification des poids et du biais b
                w1 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][1], strlen(PatientsData[patient-1][1]), 1);
                w2 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][2], strlen(PatientsData[patient-1][2]), 2);
                w3 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][3], strlen(PatientsData[patient-1][3]), 3);
                w4 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][4], strlen(PatientsData[patient-1][4]), 4);
                w5 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][5], strlen(PatientsData[patient-1][5]), 5);
                w6 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][6], strlen(PatientsData[patient-1][6]), 6);
                w7 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][7], strlen(PatientsData[patient-1][7]), 7);
                b += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque);

                flag_modif=1;
            }
        }
        iteration++;
        accuracy = AccuracyBonus(w1, w2, w3, w4, w5, w6, w7, b, data_train, length_data_train, PatientsData);    // calcul de l'accuracy pour ces nouvelles valeurs de poids
    }

    // Resultat en sortie
    ResTrainingBonus res;
    res.w1=w1;
    res.w2=w2;
    res.w3=w3;
    res.w4=w4;
    res.w5=w5;
    res.w6=w6;
    res.w7=w7;
    res.b=b;
    res.accuracy=accuracy;

    return res;
}


float TestBonus(char* PatientsData[5001][13], ResTrainingBonus resPerceptronBonus, int* data_test, int length_data_test){

    /* Renvoie l'accuracy calculee sur les donnees de test */

                
    // Calcul de l'accuracy
    float accuracy = AccuracyBonus(resPerceptronBonus.w1, resPerceptronBonus.w2, resPerceptronBonus.w3, resPerceptronBonus.w4, resPerceptronBonus.w5, resPerceptronBonus.w6, resPerceptronBonus.w7, resPerceptronBonus.b, data_test, length_data_test, PatientsData);

    return accuracy;
}