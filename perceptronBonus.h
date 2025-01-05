#ifndef PERCEPTRONBONUS_H
#define PERCEPTRONBONUS_H

#define MAX_PATIENTS 5001
#define MAX_FIELDS 20

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

ResTrainingBonus perceptronBonus(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data_train, int length_data_train);
float TestBonus(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], ResTrainingBonus tabResPerceptron, int* data_test, int length_data_test);

#endif