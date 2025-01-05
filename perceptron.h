#ifndef PERCEPTRON_H
#define PERCEPTRON_H

typedef struct ResTraining{
    float w1;
    float w2;
    float b;
    float nu;
    float accuracy;
} ResTraining;

float CharToFloat(const char* str, size_t length, int c);
int activation(float z);
ResTraining perceptron(int c1, int c2, char* PatientsData[5001][13], int* data_train, int length_data_train);
ResTraining* Training(char* PatientsData[5001][13], int* data_train, int length_data_train);
float* Test(char* PatientsData[5001][13], ResTraining* tabResPerceptron, int* data_test, int length_data_test);
float* meanAccuracy(int N, char* PatientsData[5001][13], ResTraining* tabResPerceptron, int* data_test, int length_data_test);
void explorationResultatsTest(float* tabResPerceptron);

#endif