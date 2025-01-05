#ifndef PERCEPTRON_H
#define PERCEPTRON_H

int max_patients = 5001;
int max_fields = 13;

typedef struct ResTraining{
    float w1;
    float w2;
    float b;
    float nu;
    float accuracy;
} ResTraining;

float CharToFloat(const char* str, size_t length, int c);
int activation(float z);
ResTraining perceptron(int c1, int c2, char* PatientsData[max_patients][max_fields], int* data_train, int length_data_train);
ResTraining* Training(char* PatientsData[max_patients][max_fields], int* data_train, int length_data_train);
float* Test(char* PatientsData[max_patients][max_fields], ResTraining* tabResPerceptron, int* data_test, int length_data_test);
float* meanAccuracy(int N, char* PatientsData[max_patients][max_fields], ResTraining* tabResPerceptron, int* data_test, int length_data_test);
void explorationResultatsTest(float* tabResPerceptron);

#endif