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
O processo de esteganografia em imagens envolve a utilização de dois arquivos: uma imagem qualquer, 
que carregará a informação escondida e é chamada de imagem de cobertura, e um outro arquivo, 
que será a mensagem a ser escondida e pode ser outra imagem, um arquivo de texto ou qualquer 
outro tipo de arquivo que possa ser escondido em uma imagem. A combinação destes dois arquivos 
é chamada de estego-imagem.
*/

int main() {

    FILE *bmp, *file2;
    int width, height;

    // 1. Check the available storage space in the bitmap

    // 2. Check the size of the file to be written onto the bitmap

    // 3. Check if there is enough space. If there isn't, prompt the user that there is no space available

    // 4. Otherwise, insert data in the least signicant bit (LSB) of each byte
}