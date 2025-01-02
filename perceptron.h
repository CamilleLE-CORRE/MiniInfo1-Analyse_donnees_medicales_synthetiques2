#ifndef PERCEPTRON_H
#define PERCEPTRON_H

typedef struct ResTraining{
    float w1;
    float w2;
    float b;
    float nu;
    float accuracy;
} ResTraining;

ResTraining perceptron(int c1, int c2, char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data_train, int length_data_train);
ResTraining* Training(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], int* data_train, int length_data_train);
float* Test(char* PatientsData[MAX_PATIENTS][MAX_FIELDS], ResTraining* tabResPerceptron, int* data_test, int length_data_test);
float* meanAccuracy(int N, char* PatientsData[MAX_PATIENTS][MAX_FIELDS], ResTraining* tabResPerceptron, int* data_test, int length_data_test);
void explorationResultatsTest(float* tabResPerceptron);

#endif