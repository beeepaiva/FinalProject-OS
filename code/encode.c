#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 54
#define SIZE_OFFSET 2
#define BITS_PER_PIXEL_OFFSET 28

void encode(FILE *in) {

 FILE *file_base, *file_in;
    unsigned int storage, data_size;

    file_base = fopen("base.bmp", "rb+"); // rb, read binary mode
    if (file_base == NULL) {
        printf("File not found!\n");
        return -1;
    }

    file_in = fopen(in, "rb")
    if (file_in == NULL) {
        printf("File not found!\n");
        return -1;
    }


    typedef struct {
        unsigned int img_size;
        unsigned int bits_per_pixel;
        unsigned char* data;
    } BMP;

   
    BMP bmp;

    fseek(file_base, SIZE_OFFSET, SEEK_SET);
    fread(&bmp.img_size, 4, 1, file_base);
    bmp.img_size = bmp.img_size - HEADER_SIZE;

    storage = bmp.img_size / 8; // This is the available storage space in bytes

    printf("Storage capacity (bytes): %d [bytes]\n", storage); // 3.839.635 bytes

    fseek(file_in, 0, SEEK_END);
    data_size = ftell(file_in);
    printf("Input file size: %d [bytes]\n", data_size);

    if (data_size > storage) {
        printf("There is no available space");
        return -1;
    }

      
    fseek(file_base, HEADER_SIZE, SEEK_SET);

    unsigned char byte_in;
    unsigned char byte_base;
    unsigned char bit_in;
    unsigned char bit_base;

    rewind(in);
    while (!feof(in)) {
        byte_in = fgetc(in);

        // For each bit in the current byte from the input file
        for (int i = 7; i >= 0; i--) {
            bit_in = (byte_in >> i) & 0x1;

            // Get the lsb of the current base byte
            byte_base = fgetc(base);
            bit_base = byte_base & 0x1;

            // Invert the lsb if necessary
            if (bit_in != bit_base) {
                byte_base = byte_base ^ 0x1; // XOR
            }

            // Write the encoded byte to the output file
            fputc(byte_base, base);
        }
    }

    fclose(file_base);
    fclose(file_in);
}