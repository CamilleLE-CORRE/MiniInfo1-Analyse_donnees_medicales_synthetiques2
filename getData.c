#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 5001
#define MAX_FIELDS 20

void BrowseFilePatients(char file_name[], char* PatientsData[MAX_PATIENTS][MAX_FIELDS]) {

    /* Remplit le tableau PatientsData avec les donnees du fichier patients
     dont le nom ets passe en argument de la fonction */


    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int i_line = 0;
    char line[256];

    fgets(line, sizeof(line), file); // Lecture de la première ligne du fichier (nom des colonnes), qu'on ignore

    // Lire chaque ligne
    while (fgets(line, sizeof(line), file) && i_line < MAX_PATIENTS) {
        char* tokens[MAX_FIELDS];
        int field_index = 0;

        // Découper la ligne en tokens
        char *token = strtok(line, "$");
        while (token != NULL && field_index < MAX_FIELDS) {
            tokens[field_index++] = token;
            token = strtok(NULL, "$");
        }

        // Ajouter les données du patient
        for (int i = 0; i < 9; i++) {
            PatientsData[i_line][i] = strdup(tokens[i + 1]);
        }

        i_line++;
    }

    fclose(file);
}

void BrowseFileLivestyle(char file_name[], char* PatientsData[MAX_PATIENTS][MAX_FIELDS]) {

    /* Remplit le tableau PatientsData avec les donnees du fichier livestyle dont le nom
     est passe en argument de la fonction */


    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int i_line = 0;
    char line[256];

    fgets(line, sizeof(line), file); // Lecture de la première ligne du fichier (nom des colonnes), qu'on ignore

    // Lire chaque ligne
    while (fgets(line, sizeof(line), file) && i_line < MAX_PATIENTS) {
        char* tokens[MAX_FIELDS];
        int field_index = 0;

        // Découper la ligne en tokens
        char *token = strtok(line, "$");
        while (token != NULL && field_index < MAX_FIELDS) {
            tokens[field_index++] = token;
            token = strtok(NULL, "$");
        }

        // Récupérer l'ID du patient (colonne 2 dans le fichier lifestyle)
        int i_patient = atoi(tokens[1]) - 1;  // L'ID commence à 1 dans le fichier, ajustement à 0 pour le tableau

        PatientsData[i_patient][9] = strdup(tokens[2]);  // physical_activity
        PatientsData[i_patient][10] = strdup(tokens[3]); // alcohol_consumption
        PatientsData[i_patient][11] = strdup(tokens[4]); // caffeine_consumption
        PatientsData[i_patient][12] = strdup(tokens[5]); // sleep_quality

        i_line++;
    }

    fclose(file);
}