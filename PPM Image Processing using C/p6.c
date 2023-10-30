#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

Pixel* readPPM(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char format[3];
    fscanf(file, "%2s", format);
    if (format[0] != 'P' || format[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
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

    fread(image, sizeof(Pixel), size, file);
    fclose(file);
    return image;
}

void printPixels(Pixel* image, int width, int height) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            printf("(%d, %d, %d) ", image[index].r, image[index].g, image[index].b);
        }
        printf("\n");
    }
}

void modifyImage(Pixel* image, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        // Example modification: invert the color
        image[i].r = 255 - image[i].r;
        image[i].g = 255 - image[i].g;
        image[i].b = 255 - image[i].b;
    }
}

void writePPM(const char* filename, Pixel* image, int width, int height) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "P6\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n"); // Assume max color value is 255

    fwrite(image, sizeof(Pixel), width * height, file);
    fclose(file);
}

int main() {
    int width, height;
    Pixel* image = readPPM("C:\\Users\\viswa\\PycharmProjects\\PPM\\sample.ppm", &width, &height);
    if (image == NULL) return 1;

    printf("Original Image Pixels:\n");
    printPixels(image, width, height);

    modifyImage(image, width, height);

    printf("\nModified Image Pixels:\n");
    printPixels(image, width, height);

    writePPM("outputp6.ppm", image, width, height);

    free(image);
    return 0;
}
