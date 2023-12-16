#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <stdbool.h>

typedef struct {
    float distance;
    float value;
} FloatPair;

float calculateDistance(float *a, float *b, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(a[i] - b[i], 2);
    }
    return sqrt(sum);
}

int comparator(const void *p, const void *q) {
    float l = ((FloatPair *)p)->distance;
    float r = ((FloatPair *)q)->distance;
    return (l - r);
}

float knn(int k, float **xtrain, float *ytrain, float *xtest, int trainSize, int features) {
    FloatPair distances[trainSize];
    float sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < trainSize; i++) {
//        if ((i % 1000) == 0) {
//            int thread_id = omp_get_thread_num();
//            int num_threads = omp_get_num_threads();
//            printf("Thread %d de %d, iteração %d\n", thread_id, num_threads, i);
//        }
        distances[i].distance = calculateDistance(xtrain[i], xtest, features);
        distances[i].value = ytrain[i];
    }

    qsort(distances, trainSize, sizeof(FloatPair), comparator);

    for (int i = 0; i < k; i++) {
        sum += distances[i].value;
    }

    return sum / k >= 0.5 ? 1.0f : 0.0f;
}

int main(int argc, char *argv[]) {
    int k = 3;
    int numThreads = 8;
    int fileLimit = -1;
    char *xtrainFilePath;
    char *ytrainFilePath;
    char *xtestFilePath;
    char *outputFilePath = "output_test.txt";

    omp_set_nested(1);
    omp_set_num_threads(numThreads);

    switch (argc) {
        case 6:
            fileLimit = atoi(argv[5]);
        case 5:
            xtestFilePath = argv[4];
        case 4:
            ytrainFilePath = argv[3];
        case 3:
            fprintf(stderr, "Numero de Threads: %s\n", argv[2]);
            numThreads = atoi(argv[2]);
            omp_set_num_threads(numThreads);

        case 2:
            fprintf(stderr, "K: %s\n", argv[1]);
            k = atoi(argv[1]);
            break;
        case 1:
    }

    int xtrainSize, xtrainFeatures, ytrainSize, xtestSize, xtestFeatures;

    char ytrainFilePath[256], xtrainFilePath[256], xtestFilePath[256], outputFilePath[256];

    sprintf(ytrainFilePath, "%s/ytrain.txt", filesPath);
    sprintf(xtrainFilePath, "%s/xtrain.txt", filesPath);
    sprintf(xtestFilePath, "%s/xtest.txt", filesPath);
    sprintf(outputFilePath, "%s/output_test.txt", filesPath);

    if (isExpanded) {
        sprintf(ytrainFilePath, "%s/ytrain_expanded.txt", filesPath);
        sprintf(xtrainFilePath, "%s/xtrain_expanded.txt", filesPath);
        sprintf(xtestFilePath, "%s/xtest_expanded.txt", filesPath);
        sprintf(outputFilePath, "%s/output_test_expanded.txt", filesPath);
    }

//    char *ytrainFile = "../files/ytrain.txt";
//    char *xtrainFile = "../files/xtrain.txt";
//    char *xtestFile = "../files/xtest.txt";
//    char *outputFilePath = "../files/output_test.txt";
//
//    if (isExpanded) {
//        ytrainFile = "../files/ytrain_expanded.txt";
//        xtrainFile = "../files/xtrain_expanded.txt";
//        xtestFile = "../files/xtest_expanded.txt";
//        outputFilePath = "../files/output_test_expanded.txt";
//    }

    float *ytrain = readYtrainFile(ytrainFilePath, &ytrainSize);
    float **xtrain = readFeaturesFiles(xtrainFilePath, &xtrainSize, &xtrainFeatures);
    float **xtest = readFeaturesFiles(xtestFilePath, &xtestSize, &xtestFeatures);

    FILE *outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        perror("Não foi possível abrir output_test.txt");
        exit(1);
    }



    float results[xtestSize];
    double startTime = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < xtestSize; i++) {
//        int thread_id = omp_get_thread_num();
//        int num_threads = omp_get_num_threads();
//        printf("MAIN - Thread %d de %d, iteração %d\n", thread_id, num_threads, i);
        results[i] = knn(k, xtrain, ytrain, xtest[i], xtrainSize, xtrainFeatures);
    }

    double totalTime = omp_get_wtime() - startTime;

    //printf("Tempo gasto: %f ms\n", totalTime);
    printf("%d;%d;%f",k, numThreads, totalTime);

    for (int i = 0; i < xtestSize; i++) {
        char *formatter;
        if (i < xtestSize - 1) {
            formatter = "%.1f\n";
        } else {
            formatter = "%.1f";
        }

        fprintf(outputFile, formatter, results[i]);
    }

    for (int i = 0; i < xtrainSize; i++) {
        free(xtrain[i]);
    }
    free(xtrain);

    for (int i = 0; i < xtestSize; i++) {
        free(xtest[i]);
    }
    free(xtest);

    free(ytrain);

    return 0;
}

