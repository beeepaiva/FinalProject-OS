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

    FILE *file1, *file2;
    file1 = fopen("test.bmp", "rb"); // rb, read binary mode
    file2 = fopen("test.txt", "r");
    unsigned int storage;

    typedef struct {
        unsigned int img_size;
        unsigned int bits_per_pixel;
        unsigned char* data;
    } BMP;

    /* 
    1. Check the available storage space in the bitmap 
    
    fread:
        - args: ptr destination, size in bytes, nr of elements of said size, input)
    fseek: sets file position to a given offset in bytes
        - args: input, offset, whence (the position from which the offset is applied, SEEK_SET, SEEK_CUR or SEEK_END) 
    */

    BMP bmp;

    fseek(file1, SIZE_OFFSET, SEEK_SET);
    fread(&bmp.img_size, 4, 1, file1);
    bmp.img_size = bmp.img_size - HEADER_SIZE;

    storage = bmp.img_size / 8 // This is the available storage space in bytes

    /* 
    2. Check the size of the file to be written onto the bitmap
    */

    /*
    3. Check if there is enough space. If there isn't, prompt the user that there is no space available
    */

    /* 
    4. Otherwise, insert data in the least signicant bit (LSB) of each byte
    */
}