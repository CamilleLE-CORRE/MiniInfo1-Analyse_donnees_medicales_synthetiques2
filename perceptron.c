#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "getData.h"

// separer les donnees en 70% - 30%

// pour chaque couple de caracteristiques
    // pour chaque patient des 70%
    // recuperer les deux caracteristiques w1 et w2
    // calculer b
    // appliquer la fonction d'activation pourclasser dans la classe 0 ou 1


//// Fonctions pour la construction des deux jeux de donnees ////

int* creerTableauEntiers(int N){

    /* Retourne un tableau des entiers de 1 à N */

    int* tableau = (int*)malloc(N * sizeof(int));
    // if (array == NULL) {
    //     perror("Failed to allocate memory");
    //     return NULL; // Retourne NULL en cas d'échec d'allocation
    // }

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

typedef struct Res {
        int* tab1;
        int* tab2;
    } Res;

Res echantillonage(int N1, float p){

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

    Res res;
    res.tab1 = tableau_reduit1;
    res.tab2 = tableau_reduit2;

    free(tableau);

    return res;
}


float charToFloat(char* str){
    return atof(str);
}

// int charToInt(char* str){
//     return atoi(str);
// }

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

int qualitativeToInt(char* str, char* val){

    /* Prend en argument une chaine de caracteres str et renvoie 1 si str = val, 0 sinon */
    // est utilisé pour transformer le sexe (H ou F) en 0 ou 1

    if (str == val){
        return 1;
    }
    else {
        return 0;
    }
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
    int nb_erreurs = 0;

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
                nb_erreurs++;
        }
    }

    //printf("nb erreurs dans calculTauxErreur = %d\n", nb_erreurs);

    return (float)nb_erreurs/(float)taille_data;

}

float Accuracy(float w1, float w2, float b, int c1, int c2, int* data, int taille_data, char* PatientsData[MAX_PATIENTS][MAX_FIELDS]){

    /* Calcul le risque des patients a partir des valeurs de w1, w2 et b determines par le perceptron,
        et renvoie le taux d'erreur de la classification des patients en comparant avec leur risque observe */

    // Declaration des variables
    // Declaration des variables
    // float vect_x1[taille_data];
    // float vect_x2[taille_data];
    // float vect_risque[taille_data];    // vecteur contenant des 0 ou des 1
    //float error_rate;
    int cpt_erreurs;
    //int nb_erreurs = 0;

    // Stocker les valeurs des champs c1 et c2 pour tous les patients, ainsi que leur risque respectif
    // for (int i=1; i<taille_data+1; i++){
    //     vect_x1[i-1] = CharToFloat(PatientsData[i-1][c1], strlen(PatientsData[i-1][c1]), c1);
    //     vect_x2[i-1] = CharToFloat(PatientsData[i-1][c2], strlen(PatientsData[i-1][c2]), c2);
    //     vect_risque[i-1] = CharToFloat(PatientsData[i-1][8], strlen(PatientsData[i-1][8]), 8);
    // }

    int TP=0;
    int TN=0;
    int FP=0;
    int FN=0;

    for (int i=0; i<taille_data; i++){  // pour chaque patient
        int patient = data[i];  // id du patient

        // Calcul du risque a partir de w1, w2, b et la fonction d'activation
        int risque_calcule = activation(CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1)*w1 + CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2)*w2 + b);
        if (risque_calcule != (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8)){    // s'il s'agit d'une erreur
            //nb_erreurs++;
            if ((int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) == 0){  // si la valeur reelle est Faux
                FP++;
                //printf("patient %d : risque calcule=%d ; risque reel=%d ==>> FP\n", patient, risque_calcule, (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8));
            }
            else{
                FN++;
                //printf("patient %d : risque calcule=%d ; risque reel=%d ==>> FN\n", patient, risque_calcule, (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8));

            }
        }
        else{
            if ((int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) == 0){  // si la valeur reelle est Faux
                TN++;
                //printf("patient %d : risque calcule=%d ; risque reel=%d ==>> TN\n", patient, risque_calcule, (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8));

            }
            else{
                TP++;
                //printf("patient %d : risque calcule=%d ; risque reel=%d ==>> TP\n", patient, risque_calcule, (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8));
        
            }
        }
    }

    //printf("TP=%d ; TN=%d ; FP=%d ; FN=%d\n", TP, TN, FP, FN);

    float accuracy = (float)(TP+TN)/(float)(TP+TN+FP+FN);
    //printf("acc=%f\n", accuracy);

    return accuracy;

}

typedef struct ResTraining{
    float w1;
    float w2;
    float b;
    float nu;
    float accuracy;
} ResTraining;

ResTraining perceptron(int c1, int c2, char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data_train, int length_data_train){

    /* Effectue l'entrainement du perceptron pour les champs c1 et c2 et retourne son taux d'erreur*/

    // Declaration des variables
    // float* vect_x1 = malloc(length_data_train * sizeof(float));
    // float* vect_x2 = malloc(length_data_train * sizeof(float));
    // float* vect_risque = malloc(length_data_train * sizeof(float));    // vecteur contenant des 0 ou des 1
    float w1=0;
    float w2=0;
    float b=0;
    float nu=0.00001;   //prendre ensuite une valeur aléatoire ??
    int iteration=0;
    float accuracy = Accuracy(w1, w2, b, c1, c2, data_train, length_data_train, PatientsData);
    int cpt_erreurs;
    int flag_modif = 1;

    //printf("accuracy avant toute modif = %f (w1=%f, w2=%f)\n", accuracy, w1, w2);

    // Stocker les valeurs des champs c1 et c2 pour tous les patients, ainsi que leur risque respectif
    // for (int i=1; i<length_data_train+1; i++){
    //     vect_x1[i-1] = CharToFloat(PatientsData[i-1][c1], strlen(PatientsData[i-1][c1]), c1);
    //     vect_x2[i-1] = CharToFloat(PatientsData[i-1][c2], strlen(PatientsData[i-1][c2]), c2);
    //     vect_risque[i-1] = CharToFloat(PatientsData[i-1][8], strlen(PatientsData[i-1][8]), 8);
    // }

    // printf("age du 3677 dans perceptron = %s\n", PatientsData[3676][1]);
    // printf("charToFloat : %f\n", CharToFloat(PatientsData[3676][1], strlen(PatientsData[3676][1]), 1));

    // printf("x1=%f\n", vect_x1[3676]);

    // for (int i=0; i<5; i++){
    //     printf("i=%d -> x1=%f\n", i, vect_x1[i]);
    // }
    // for (int i=0; i<5; i++){
    //     printf("i=%d -> x2=%f\n", i, vect_x2[i]);
    // }
    // for (int i=0; i<5; i++){
    //     printf("i=%d -> risque=%f\n", i, vect_risque[i]);
    // }

    // Initialisation de w1 et w2 par un flottant aleatoire dans [-1;1]
    srand(time(NULL));
    // w1 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
    // w2 = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

    float previous_accuracy = 0;

    while (iteration < 100 && fabs(accuracy - previous_accuracy) > 0.001){ //&& (previous_accuracy - accuracy) < 0.01){    // au maximum 100 itérations

        previous_accuracy = accuracy;
        cpt_erreurs = 0;
        flag_modif = 0;

        //printf("iteration %d :\n", iteration);

        for (int i=0; i<length_data_train; i++){   // pour chaque patient

            // if (i<=0 && i<100 && iteration==0){
            //     printf("w1=%f\n", w1);
            // }

            int patient = data_train[i];
            // printf("patient etudie : %d\n", patient);
            //printf("patient : %d\n", patient);
            //int patient_id = patient;
            // printf("id : %s | age : %s | sexe : %s | weight : %s | blood_pressure : %s | average_heart_rate : %s | blood_sugar : %s | cholesterol : %s | risk : %s | physical_activity : %s | alcohol_consumption : %s | caffeine_consumption : %s | sleep_quality : %s\n",
            // PatientsData[patient_id-1][0], PatientsData[patient_id-1][1], PatientsData[patient_id-1][2], PatientsData[patient_id-1][3], PatientsData[patient_id-1][4],
            // PatientsData[patient_id-1][5], PatientsData[patient_id-1][6], PatientsData[patient_id-1][7], PatientsData[patient_id-1][8],
            // PatientsData[patient_id-1][9], PatientsData[patient_id-1][10], PatientsData[patient_id-1][11], PatientsData[patient_id-1][12]);

            // Utilisation de la fonction d'activation pour determiner le risque du patient
            //int risque = activation(vect_x1[patient-1]*w1 + vect_x2[patient-1]*w2 + b);
            int risque = activation(CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1)*w1 + CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2)*w2 + b);
            // printf("calcul du risque : w1=%f ; w2=%f ; b=%f ; x1=%f ; x2=%f\n", w1, w2, b, vect_x1[patient-1], vect_x2[patient-1]);
            // printf("entree de la fonction d'activation : %f\n", vect_x1[patient-1]*w1 + vect_x2[patient-1]*w2 + b);
            // printf("risque estime (sortie de la fonction d'activation): %d\n", risque);
            // printf("risque avere : %f\n", vect_risque[patient-1]);

            // Comparaison du risque predit avec le risque attendu
            if (risque != (int)CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8)){    // s'il s'agit d'une erreur
                //if (iteration == 99){
                cpt_erreurs++;//}

                //printf("calcul du risque : patient=%d ; w1=%f ; w2=%f ; b=%f ; x1=%f ; x2=%f\n", patient, w1, w2, b, vect_x1[patient-1], vect_x2[patient-1]);
                // printf("modif des poids\n");

                // Modification des poids w1 et w2, ainsi que du biais b
                w1 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][c1], strlen(PatientsData[patient-1][c1]), c1);
                w2 += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque) * CharToFloat(PatientsData[patient-1][c2], strlen(PatientsData[patient-1][c2]), c2);
                b += nu * (CharToFloat(PatientsData[patient-1][8], strlen(PatientsData[patient-1][8]), 8) - risque);

                //printf("nouvelles valeurs : w1=%f ; w2=%f ; b=%f\n", w1, w2, b);

                flag_modif=1;

                //if (patient <100){
                    //printf("w1=%f ; w2=%f\n", w1, w2);
                //}
            }
        }
        iteration++;
        //error_rate = calculTauxErreur(w1, w2, b, c1, c2, data_train, 3500, PatientsData);
        accuracy = Accuracy(w1, w2, b, c1, c2, data_train, length_data_train, PatientsData);

        //printf("taux erreur dans calculTauxErreur = %f\n", error_rate);
        //printf("accuracy = %f\n", accuracy);
        //printf("nb erreurs dans perceptron : %f\n", (float)cpt_erreurs/(float)3500);
        //printf("w1 apres une iteration : %f\n", w1);
    }

    //printf("test risque : %d\n", (int)vect_risque[2500-1]);
    //printf("nb erreurs dans perceptron : %d\n", cpt_erreurs);
    // printf("nb iterations : %d\n", iteration);
    //printf("accuracy finale = %f\n", accuracy);

    // Calcul du taux d'erreur
    // error_rate = calculTauxErreur(w1, w2, b, c1, c2, data_train, 3500, PatientsData);
    // printf("error rate = %f\n", error_rate);

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

    // Initialisation des variables permettant de stocker le meilleur score et les caracteristiques associees
    //int best_c1;
    // int best_c2;
    // float best_accuracy = perceptron(1, 2, PatientsData, data_train, length_data_train).accuracy;
    //int accuracy_par_couples[21];
    ResTraining* tabResPerceptron = (ResTraining*)malloc(21 * sizeof(ResTraining));
    // if (tabResPerceptron == NULL) {
    // printf("Erreur d'allocation mémoire\n");
    // exit(EXIT_FAILURE);
    // }
    // else{
    //     printf("ok pour l'allocation memoire\n");
    // }
    //printf("Taille de tabResPerceptron : %ld éléments\n", sizeof(tabResPerceptron) / sizeof(tabResPerceptron[0]));

   // stockage de w1, w2, b et accuracy pour chaque couple de champs

    int i_couple = 0;

    // Pour chaque couple (c1, c2) des champs de patients.pengu
    for (int c1 = 1; c1 < 8; c1++){
        for (int c2 = c1+1; c2 < 8; c2++){
            if (c1 != c2){
                
                //printf("champs testes : %d et %d\n", c1, c2);

                ResTraining res = perceptron(c1, c2, PatientsData, data_train, 3500);
                tabResPerceptron[i_couple] = res;
                
                i_couple++;

                // printf("resultats : w1=%f, w2=%f, b=%f, nu=%f\n", res.w1, res.w2, res.b, res.nu);
                //printf("accuracy = %f\n", res.accuracy);

                // if (res.accuracy > best_accuracy){ // Memoriser les meilleurs champs
                //     best_c1 = c1;
                //     best_c2 = c2;
                //     best_accuracy = res.accuracy;
                // }
            }
        }
    }
    //printf("nb couples = %d", nb_couples);
    //printf("meilleur accuracy = %f; obtenue avec les champs %d et %d\n", best_accuracy, best_c1, best_c2);
    //perceptron(3,4, PatientsData, data_train);

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


// int main(){

//     ////////////////////////////////////////////////////////
//     // Initialisation de la structure de donnees
//     char* PatientsData[MAX_PATIENTS][MAX_FIELDS];

//     memset(PatientsData, 0, sizeof(PatientsData));

//     // Remplir le tableau avec les donnees des fichiers patients et livestyle
//     BrowseFilePatients("patients.pengu", PatientsData);
//     BrowseFileLivestyle("lifestyle.pengu", PatientsData);

//     ///////////////////////////////////////////////////////
//     // Formation des deux jeux de donnees : entrainement et test
//     Res res = echantillonage(5000, 0.7);
//     int* data_train = res.tab1;
//     int* data_test = res.tab2;

//     //printf("nb de 1 dans train = %d\n", countRisk(PatientsData, data_train, 3500));
//     //printf("nb de 1 dans test = %d\n", countRisk(PatientsData, data_test, 1500));
//     //printf("nb de 1 au total = %d\n", countRisk(PatientsData, echantillonage(5000, 1.0).tab1, 3500));
//     ///////////////////////////////////////////////////////

//     // printf("!!!! %s !!!!\n", PatientsData[2][2]);
//     // printf("!!!! %f !!!!\n", CharToFloat(PatientsData[2][2], strlen(PatientsData[2][2]), 2));
//     // char* t = PatientsData[1][3];
//     // printf("char=%s ; float=%f\n", t, CharToFloat(t, strlen(t)));

//     Training(PatientsData, data_train, 3500);

//     return 0;
// }