#ifndef FILE_READER_H
#define FILE_READER_H

float *readYtrainFile(const char *filename, int *size);
float **readFeaturesFiles(const char *filename, int *size, int *lineSize, int fileLimit);

#endif
