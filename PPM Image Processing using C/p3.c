#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

Pixel* readP3PPM(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char format[3];
    fscanf(file, "%2s", format);
    if (format[0] != 'P' || format[1] != '3') {
        fprintf(stderr, "Invalid image format (must be 'P3')\n");
        fclose(file);
        return NULL;
    }

    fgetc(file); // Read newline character after the format
    fscanf(file, "%d %d", width, height);
    fgetc(file); // Read newline character after the dimensions
    int maxVal;
    fscanf(file, "%d", &maxVal);
    fgetc(file); // Read newline character after the max color value

    int size = (*width) * (*height);
    Pixel* image = malloc(size * sizeof(Pixel));
    if (image == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        fscanf(file, "%hhu %hhu %hhu", &image[i].r, &image[i].g, &image[i].b);
    }

    fclose(file);
    return image;
}

void writeP3PPM(const char* filename, Pixel* image, int width, int height) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n"); // Assume max color value is 255

    for (int i = 0; i < width * height; i++) {
        fprintf(file, "%d %d %d ", image[i].r, image[i].g, image[i].b);
    }

    fclose(file);
}

void modifyImage(Pixel* image, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // Grayscale conversion: average of RGB components
        unsigned char gray = (image[i].r + image[i].g + image[i].b) / 3;
        image[i].r = gray;
        image[i].g = gray;
        image[i].b = gray;
    }
}

int main() {
    int width, height;
    Pixel* image = readP3PPM("C:\\Users\\viswa\\PycharmProjects\\PPM\\circle.ppm", &width, &height);
    if (image == NULL) return 1;

    printf("Original Image Pixels:\n");
    for (int i = 0; i < width * height; i++) {
        printf("(%d, %d, %d) ", image[i].r, image[i].g, image[i].b);
    }
    printf("\n");

    // Modify the image using grayscale conversion
    modifyImage(image, width, height);

    printf("Modified Image Pixels (Grayscale):\n");
    for (int i = 0; i < width * height; i++) {
        printf("(%d, %d, %d) ", image[i].r, image[i].g, image[i].b);
    }
    printf("\n");

    writeP3PPM("outputp3.ppm", image, width, height);

    free(image);
    return 0;
}
