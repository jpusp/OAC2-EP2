#include "file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float *readYtrainFile(const char *filename, int *size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Não foi possível abrir ytrain.txt");
        exit(1);
    }

    int capacity = 10;
    *size = 0;
    float *array = malloc(capacity * sizeof(float));
    if (array == NULL) {
        perror("Falha na alocação de memória");
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        char *endptr;
        array[*size] = strtof(line, &endptr);
        if (line == endptr) {
            fprintf(stderr, "Erro na conversão de número na linha %d\n", *size + 1);
            exit(1);
        }

        (*size)++;
        if (*size >= capacity) {
            capacity *= 2;
            array = realloc(array, capacity * sizeof(float));
            if (array == NULL) {
                perror("Falha na realocação de memória");
                exit(1);
            }
        }
    }

    fclose(file);
    return array;
}


float **readFeaturesFiles(const char *filename, int *size, int *lineSize, int fileLimit) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Não foi possível abrir o arquivo");
        exit(1);
    }

    char line[1024];
    if (fgets(line, sizeof(line), file) == NULL) {
        perror("Erro ao ler a primeira linha do arquivo");
        exit(1);
    }

    *lineSize = 1;
    for (int i = 0; line[i]; i++) {
        if (line[i] == ',') (*lineSize)++;
    }

    fseek(file, 0, SEEK_SET);

    int capacity = 10;
    *size = 0;
    float **array = malloc(capacity * sizeof(float*));
    if (array == NULL) {
        perror("Falha na alocação de memória");
        exit(1);
    }

    while (fgets(line, sizeof(line), file)) {
        array[*size] = malloc(*lineSize * sizeof(float));
        if (array[*size] == NULL) {
            perror("Falha na alocação de memória para características");
            exit(1);
        }

        char *token = strtok(line, ",");
        for (int featureIndex = 0; token != NULL && featureIndex < *lineSize; featureIndex++) {
            array[*size][featureIndex] = atof(token);
            token = strtok(NULL, ",");
        }

        if (fileLimit != -1 && *size >= fileLimit) {
            break;
        }
        (*size)++;
        if (*size >= capacity) {
            capacity *= 2;
            array = realloc(array, capacity * sizeof(float*));
            if (array == NULL) {
                perror("Falha na realocação de memória para linhas");
                exit(1);
            }
        }
    }

    fclose(file);
    return array;
}

