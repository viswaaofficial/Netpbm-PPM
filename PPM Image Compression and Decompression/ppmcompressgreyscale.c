#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLOR 255
#define ASCII_CHARS "@#8&o:*. "

void ppmToAscii(const char *ppmFileName, const char *asciiFileName) {
    FILE *file = fopen(ppmFileName, "rb");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char magic[2];
    fscanf(file, "%2s", magic);
    if (magic[0] != 'P' || magic[1] != '6') {
        fprintf(stderr, "Not a valid P6 PPM file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int width, height, maxColor;
    fscanf(file, "%d %d %d", &width, &height, &maxColor);
    fgetc(file);

    unsigned char *pixels = (unsigned char *)malloc(3 * width * height * sizeof(unsigned char));
    if (!pixels) {
        perror("Memory allocation error");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(pixels, sizeof(unsigned char), 3 * width * height, file);
    fclose(file);

    FILE *asciiFile = fopen(asciiFileName, "w");
    if (!asciiFile) {
        perror("Error creating ASCII file");
        free(pixels);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = 3 * (i * width + j);
            int intensity = (pixels[index] + pixels[index + 1] + pixels[index + 2]) / 3;
            int asciiIndex = intensity * (sizeof(ASCII_CHARS) - 1) / (maxColor + 1);
            fprintf(asciiFile, "%c", ASCII_CHARS[asciiIndex]);
        }
        fprintf(asciiFile, "\n");
    }

    free(pixels);
    fclose(asciiFile);
}

void compressAscii(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "wb");

    if (!inputFile || !outputFile) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char currentChar, nextChar;
    int count = 1;

    currentChar = fgetc(inputFile);

    while ((nextChar = fgetc(inputFile)) != EOF) {
        if (currentChar == nextChar) {
            count++;
        } else {
            fwrite(&count, sizeof(int), 1, outputFile);
            fwrite(&currentChar, sizeof(char), 1, outputFile);
            currentChar = nextChar;
            count = 1;
        }
    }

    fwrite(&count, sizeof(int), 1, outputFile);
    fwrite(&currentChar, sizeof(char), 1, outputFile);

    fclose(inputFile);
    fclose(outputFile);
}

void decompressAscii(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "rb");
    FILE *outputFile = fopen(outputFileName, "w");

    if (!inputFile || !outputFile) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count;
    char currentChar;

    while (fread(&count, sizeof(int), 1, inputFile) == 1) {
        fread(&currentChar, sizeof(char), 1, inputFile);

        for (int i = 0; i < count; i++) {
            fprintf(outputFile, "%c", currentChar);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

void asciiToPpm(const char *asciiFileName, const char *ppmFileName, int width, int height) {
    FILE *asciiFile = fopen(asciiFileName, "r");
    FILE *ppmFile = fopen(ppmFileName, "wb");

    if (!asciiFile || !ppmFile) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(ppmFile, "P6\n%d %d\n255\n", width, height);

    char currentChar;
    unsigned char pixelValue;

    for (int i = 0; i < height; i++) {
        for (int j = width - 1; j >= 0; j--) {
            // Read ASCII character
            currentChar = fgetc(asciiFile);

            // Map ASCII character to pixel value
            int asciiIndex = strchr(ASCII_CHARS, currentChar) - ASCII_CHARS;
            pixelValue = (asciiIndex * MAX_COLOR) / (sizeof(ASCII_CHARS) - 1);

            // Write pixel value for each color channel (R, G, B)
            for (int k = 0; k < 3; k++) {
                fwrite(&pixelValue, sizeof(unsigned char), 1, ppmFile);
            }
        }

        // Skip newline character
        fgetc(asciiFile);
    }

    fclose(asciiFile);
    fclose(ppmFile);
}



int main() {
    const char *originalPpmFileName = "C:\\Sony Project\\Netpbm-PPM-main\\PPM Image Processing using C\\P6 Images\\tomato.ppm";

    char asciiFileName[256], compressedAsciiFileName[256], decompressedAsciiFileName[256], compressedPpmFileName[256], decompressedPpmFileName[256];

    strcpy(asciiFileName, originalPpmFileName);
    strcat(asciiFileName, ".ascii");

    strcpy(compressedAsciiFileName, originalPpmFileName);
    strcat(compressedAsciiFileName, ".compressed.ascii");

    strcpy(decompressedAsciiFileName, originalPpmFileName);
    strcat(decompressedAsciiFileName, ".decompressed.ascii");

    strcpy(compressedPpmFileName, originalPpmFileName);
    strcat(compressedPpmFileName, ".compressed.ppm");

    strcpy(decompressedPpmFileName, originalPpmFileName);
    strcat(decompressedPpmFileName, ".decompressed.ppm");

    ppmToAscii(originalPpmFileName, asciiFileName);
    compressAscii(asciiFileName, compressedAsciiFileName);
    decompressAscii(compressedAsciiFileName, decompressedAsciiFileName);

    FILE *ppmFile = fopen(originalPpmFileName, "rb");
    int width, height;
    fscanf(ppmFile, "P6\n%d %d\n255\n", &width, &height);
    fclose(ppmFile);

    asciiToPpm(compressedAsciiFileName, compressedPpmFileName, width, height);
    asciiToPpm(decompressedAsciiFileName, decompressedPpmFileName, width, height);

    return 0;
}
