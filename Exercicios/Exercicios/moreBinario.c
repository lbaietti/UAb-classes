#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define MAXSTR 1024

int main(int argc, char **argv)
{
    FILE *f;
    unsigned char buffer[16];
    int bytesLidos;
    int offset = 0;
    int i;

    //Para VPL: Usar stdin em modo binário
    f = stdin;

    /*Ler e processar blocos de 16 bytes*/
    while ((bytesLidos = fread(buffer, 1, 16, f)) > 0)
    {
        /*Imprimir o offset (número do primeiro byte)*/
        printf("%6d", offset);

        /*Imprimir os valores hexadecimais*/
        for ( i = 0; i < 16; i++)
        {
            if (i < bytesLidos)
            {
                printf("%02x ", buffer[i]);
            } else {
                printf("    "); /*Espaço vazio se não hpuver byte*/
            }            
        }

        /*Separador entre hex e caracteres*/
        printf("|");

        // Imprimir os caracteres imprimíveis
        for ( i = 0; i < bytesLidos; i++)
        {
            /*Verificar se é imprimível E não é um caracter de controlo*/
            if (isprint(buffer[i]) && buffer[i] >= 32 && buffer[i] < 127)
            {
                printf("%c", buffer[i]);
            } else{
                printf(" "); /*Espaço para caracteres não imprimíveis*/
            }            
        }

        // Nova linha
        printf("\n");

        /*Incrementar o offset*/
        offset += bytesLidos;
    }
    
    return 0;
}