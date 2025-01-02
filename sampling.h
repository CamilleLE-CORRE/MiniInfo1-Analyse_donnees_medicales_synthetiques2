#ifndef SAMPLING_H
#define SAMPLING_H

typedef struct ResSampling {
        int* tab1;
        int* tab2;
    } ResSampling;

ResSampling sampling(int N1, float p);

#endif