#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

#define MAXSTR 1024

/*Faça um programa que receba um ficheiro de texto em argumento, e o mostre no ecrã, utilizando os primeiros 4 caracteres para numerar a linha. Se uma linha tiver mais de 75 caracteres, a mesma é mostrada em várias linhas, mas o número de linha apresentado é sempre a do ficheiro. Considere que o comprimento máximo de uma linha é de 1024 caracteres.

Notas:

Abra um ficheiro em modo de texto, e não se preocupe agora com a alocação de memória
Verifique se o ficheiro foi bem aberto, e se foi dado um argumento com o nome do ficheiro
Para o VPL assuma que o ficheiro é o "stdin", que já está aberto, e não ligue aos argumentos do programa*/

//Função que aplica a cifra de César a um caracter
char CifraChar(char c, int chave)
{
    int pos, novo; 

    // Letras maiúsculas
    if (c >= 'A' && c<= 'Z')
    {
        pos = c - 'A';
        novo = (pos + chave) % 26;
        if(novo < 0) novo += 26; /*Corrigir de for negativo*/
        return 'A' + novo;
    }

    // Letras minúsculas
    else if(c >= 'a' && c <= 'z')
    {
        pos = c - 'a';
        novo = (pos + chave) % 26;
        if(novo < 0) novo += 26; /*Corrigir se for negativo*/
        return 'a' + novo;
    }

    // Dígitos
    else if(c >= '0' && c <= '9')
    {
        pos = c - '0';
        novo = (pos + chave) % 10;
        if(novo < 0) novo += 10; /*Corrigir se for negativo*/
        return '0' + novo;
    }

    //Outros caracteres permanecem inalterados
    else{
        return c;
    }    
}

// Função que aplica a cifra de César a uma string
void CifraDeCesar(char *str, int chave)
{
    int i;

    for ( i = 0; str[i] != '\0'; i++)
    {
        str[i] = CifraChar(str[i], chave);
    }    
}


int main()
{
    FILE *f;
    char str[MAXSTR];
    int chave;

    /*Para VPL: ler a chave da primeira linha do stdin*/
    f = stdin;
    scanf("%d\n", &chave);

    /*Ler e processar cada linha do ficheiro*/
    while (fgets(str, MAXSTR, f) != NULL)
    {
        /*Aplicar a cifra de César*/
        CifraDeCesar(str, chave);

        /*Imprimir a linha cifrada*/
        printf("%s", str);
    }

    return 0;    
}
