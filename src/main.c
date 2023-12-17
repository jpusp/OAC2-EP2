#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

typedef struct {
    float distance;
    float value;
    int originalPosition;
} FloatPair;

float calculateDistance(float *a, float *b, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(a[i] - b[i], 2);
    }
    return sqrt(sum);
}

int comparator(const void *p, const void *q) {
    FloatPair pairL = *(FloatPair *) p;
    FloatPair pairR = *(FloatPair *) q;

    float l = pairL.distance;
    float r = pairR.distance;

    float result = l - r;

    if (result > 0) return 1;
    else if (result < 0) return -1;
    else return pairL.originalPosition - pairR.originalPosition;
}

float knn(int k, float **xtrain, float *ytrain, float *xtest, int trainSize, int lineSize) {
    FloatPair distances[trainSize];
    float sum = 0;

    #pragma omp parallel for
    for (int i = 0; i < trainSize; i++) {
        distances[i].distance = calculateDistance(xtrain[i], xtest, lineSize);
        distances[i].value = ytrain[i];
        distances[i].originalPosition = i;
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
            xtrainFilePath = argv[2];
        case 2:
            numThreads = atoi(argv[1]);
            omp_set_num_threads(numThreads);
            break;
        default:
            break;
    }

    int xtrainSize, xtrainLineSize, ytrainSize, xtestSize, xtestLineSize;

    float *ytrain = readYtrainFile(ytrainFilePath, &ytrainSize);
    float **xtrain = readFeaturesFiles(xtrainFilePath, &xtrainSize, &xtrainLineSize, fileLimit);
    float **xtest = readFeaturesFiles(xtestFilePath, &xtestSize, &xtestLineSize, -1);

    FILE *outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        perror("Não foi possível abrir output_test.txt");
        exit(1);
    }

    float results[xtestSize];
    double startTime = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < xtestSize; i++) {
        results[i] = knn(k, xtrain, ytrain, xtest[i], xtrainSize, xtrainLineSize);
    }

    double totalTime = omp_get_wtime() - startTime;

    printf("%d;%d;%f", numThreads, xtrainSize, totalTime);

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

