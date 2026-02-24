#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
    char input[256];
    char *pt;
    double soma=0.0;

    /*Lê a string de entrada*/
    fgets(input, 256, stdin);

    // Remove o \n do final se existir
    input[strcspn(input, "\n")] = 0;

    /*Separa a string em tokens usando espaço como delimitador*/
    pt= strtok(input, " ");
    while(pt != NULL)
    {
        /*Converte o token para double e adiciona à soma*/
        soma += atof(pt);
        /*Obtém o próximo token*/
        pt= strtok(NULL, " ");
    }

    /*Imprime a soma com precisão de 15 dígitos significativos*/
    printf("%.15g\n", soma);
    return 0;
}

/* VERSÃO 2 - ABORDAGEM ALTERNATIVA (sem strtok)
 * Processa caractere por caractere, construindo números manualmente
 */

/*
#include <stdio.h>
#include <stdlib.h>

int main() {
    char input[256];
    char numero[50];
    int i = 0, j = 0;
    double soma = 0.0;
    
    fgets(input, 256, stdin);
    
    while (input[i] != '\0' && input[i] != '\n') {
        // Pula espaços
        while (input[i] == ' ') {
            i++;
        }
        
        // Extrai o número
        j = 0;
        while (input[i] != ' ' && input[i] != '\0' && input[i] != '\n') {
            numero[j] = input[i];
            i++;
            j++;
        }
        numero[j] = '\0';
        
        // Converte e soma se extraímos algo
        if (j > 0) {
            soma += atof(numero);
        }
    }
    
    printf("%.15g\n", soma);
    
    return 0;
}*/