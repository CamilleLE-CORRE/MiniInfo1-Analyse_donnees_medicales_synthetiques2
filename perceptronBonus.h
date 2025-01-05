#ifndef PERCEPTRONBONUS_H
#define PERCEPTRONBONUS_H

int max_patients = 5001;
int max_fields = 13;

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

ResTrainingBonus perceptronBonus(char* PatientsData[max_patients][max_fields], int* data_train, int length_data_train);
float TestBonus(char* PatientsData[max_patients][max_fields], ResTrainingBonus tabResPerceptron, int* data_test, int length_data_test);

#endif