#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Desenvolva um “Sistema de Arquivos em Espaço de Usuário” que use um arquivo de mídia (imagem, vídeo, som) 
como meio de armazenamento através da técnica de esteganografia.

Requisitos:

Uso de arquivo de mídia (o mais fácil será uma imagem BMP)
Uso da biblioteca LIBFUSE
Código em C
Montagem em Linux
*/

/*
BITMAP file format

Header: 54 bytes (below)
Image Data: file size - 54

typedef struct {              // Total: 54 bytes
  uint16_t  type;             // Magic identifier: 0x4d42
  uint32_t  size;             // File size in bytes
  uint16_t  reserved1;        // Not used
  uint16_t  reserved2;        // Not used
  uint32_t  offset;           // Offset to image data in bytes from beginning of file (54 bytes)
  uint32_t  dib_header_size;  // DIB Header size in bytes (40 bytes)
  int32_t   width_px;         // Width of the image
  int32_t   height_px;        // Height of image
  uint16_t  num_planes;       // Number of color planes
  uint16_t  bits_per_pixel;   // Bits per pixel
  uint32_t  compression;      // Compression type
  uint32_t  image_size_bytes; // Image size in bytes
  int32_t   x_resolution_ppm; // Pixels per meter
  int32_t   y_resolution_ppm; // Pixels per meter
  uint32_t  num_colors;       // Number of colors  
  uint32_t  important_colors; // Important colors 
} BMPHeader;

Source: https://engineering.purdue.edu/ece264/17au/hw/HW15
*/

#define HEADER_SIZE 54
#define SIZE_OFFSET 2
#define BITS_PER_PIXEL_OFFSET 28

int main() {

    FILE *file_base, *file_in, *file_out;
    unsigned int storage, data_size;

    file_base = fopen("base.bmp", "rb+"); // rb, read binary mode
    if (file_base == NULL) {
        printf("File not found!\n");
        return -1;
    }

    file_in = fopen("input.txt", "r"); // Open file for update (both input and output)
    if (file_in == NULL) {
        printf("File not found!\n");
        return -1;
    }

    file_out = fopen("output.bmp", "wb+"); // Create binary and open for update (both input and output)
    if (file_out == NULL) {
        printf("File could not be created!\n");
        return -1;
    }

    typedef struct {
        unsigned int img_size;
        unsigned int bits_per_pixel;
        unsigned char* data;
    } BMP;

    /******************************************************
     * 
     *  1. Check the available storage space in the bitmap 
     * 
     ******************************************************/
    
    // fread:
    //     - args: ptr destination, size in bytes, nr of elements of said size, input)
    // fseek: sets file position to a given offset in bytes
    //     - args: input, offset, whence (the position from which the offset is applied, SEEK_SET, SEEK_CUR or SEEK_END) 

    BMP bmp;

    fseek(file_base, SIZE_OFFSET, SEEK_SET);
    fread(&bmp.img_size, 4, 1, file_base);
    bmp.img_size = bmp.img_size - HEADER_SIZE;

    storage = bmp.img_size / 8; // This is the available storage space in bytes

    /*  
        Validation

        Doing a hex dump of the base image through 'xxd img.bmp | vim -' we get:

        00000000: 424d d2b4 d401 0000 0000 8a00 0000 7c00  BM............|.
    
        This is the first line in the bitmap. The hex sequence 'd2b4 d401' corresponds to the file size (uint32_t).
        The bitmap is formatted as little endian. Thus the sequence is initially converted to big endian and after that to decimal.
        The following size in bytes should be read: 30.717.138.

        The image size should be equal to the value above minus the header, thus: 30.717.084.

        This should result in a storage size of 3.839.635 bytes.

    */

    printf("Storage capacity (bytes): %d\n", storage); // 3.839.635 bytes


     /********************************************************************
     * 
     *  2. Check if there is enough space available for storing the input 
     * 
     *********************************************************************/ 

    // ftell: returns the current file position of the given stream

    fseek(file_in, 0, SEEK_END);
    data_size = ftell(file_in);
    printf("Input file size (bytes): %d\n", data_size);

    if (data_size > storage) {
        printf("There is no available space");
        return -1;
    }

    /****************************************************************
     * 
     *  3. Insert data in the least significant bit (LSB) of each byte 
     * 
     ****************************************************************/ 

    // The out file is a control copy of the base file used for the encoding
    // Write the input file onto the out file, bit by bit 
    void encode(FILE *base, FILE *in, FILE *out) {

        // Copy the HEADER from the base file to the ouput file
        // fgetc: gets the next unsigned char and advances the position indicator for the stream
        // fputc: writes an unsigned char to the specified stream and advances the position indicator for the stream
        rewind(base);
        unsigned char copy;
        for (int i = 0; i < HEADER_SIZE; i++) {
            copy = fgetc(base);
            fputc(copy, out);
        }

        unsigned char byte_in;
        unsigned char byte_base;
        unsigned char bit_in;
        unsigned char bit_base;

        rewind(in);
        while (!feof(in)) {
            byte_in = fgetc(in);

            // For each bit in the current byte from the input file
            for (int i = 0; i < 8; i++) {
                bit_in = (byte_in >> i) & 0x1;

                // Get a byte from the base file
                byte_base = fgetc(base);
                bit_base = (byte_base >> 7) & 0x1;

                // Invert the lsb if necessary
                if (bit_in != bit_base) {
                    byte_base = byte_base ^ 1; // XOR
                }

                // Write the encoded byte to the output file
                fputc(byte_base, out);
            }
        }

        // Write the leftover unencoded data from the base file to the output file
        while (!feof(base)) {
            copy = fgetc(base);
            fputc(copy, out);
        }
    }

    encode(file_base, file_in, file_out);
}