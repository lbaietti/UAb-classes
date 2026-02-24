#include <stdio.h>
#define TAMANHO 10

unsigned int randaux() {
    static long seed = 1;
    return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

void GerarMapa(int mapa[TAMANHO][TAMANHO], int tamanho, int prob) {
    int i, j;
    for (i = 0; i < tamanho; i++)
        for (j = 0; j < tamanho; j++) {
            mapa[i][j] = 0;
            if (prob > 1 && randaux() % prob == 0)
                mapa[i][j] = 1;
        }
}

void Inicializar(int mapa[TAMANHO][TAMANHO], int *tamanho) {
    int semente, prob;
    // inicializar gerador de números aleatórios e ler probabilidade
    printf(">Introduza (semente, probabilidade (1 em N), tamanho ate 10):\n");
    scanf("%d %d %d", &semente, &prob, tamanho);
    
    /* Consome 'semente' chamadas ao gerador para avançar seu estado */
    while (semente-- > 0)
        randaux();
    
    // limitar tamanho permitido
    if (*tamanho > TAMANHO)
        *tamanho = TAMANHO;
    else if (*tamanho < 2)
        *tamanho = 2;
    
    GerarMapa(mapa, *tamanho, prob);
}

/* Converte valor interno (0/1) em símbolo visual ('.', '#') */
char ObterSimbolo(int valor) {
    if (valor == 1) {
        return '#';
    } else {
        return '.';
    }
}

/* Imprime cabeçalho com letras das colunas (A B C ...) */
void ImprimirLinhaLetras(int tamanho) {
    int j;
    printf(">  ");  /* > + 2 espaços para alinhamento */
    for (j = 0; j < tamanho; j++) {
        printf("%c ", 'A' + j);
    }
    printf("\n");
}

/* Imprime uma linha específica do mapa com sua numeração */
void ImprimirLinhaMapa(int mapa[TAMANHO], int linha, int tamanho) {
    int j;
    /* Garante alinhamento correto para números de 1 ou 2 dígitos */
    if (linha + 1 < 10) {
        printf("> %d ", linha + 1);  /* "> 1 " */
    } else {
        printf(">%d ", linha + 1);   /* ">10 " */
    }
    
    for (j = 0; j < tamanho; j++) {
        printf("%c ", ObterSimbolo(mapa[j]));
    }
    printf("\n");
}

/* Coordena a impressão completa do mapa */
void ImprimirMapa(int mapa[TAMANHO][TAMANHO], int tamanho) {
    int i;
    
    ImprimirLinhaLetras(tamanho);
    
    for (i = 0; i < tamanho; i++) {
        ImprimirLinhaMapa(mapa[i], i, tamanho);
    }
}

int main() {
    int tamanho;
    int mapaPecas[TAMANHO][TAMANHO];  // posição das peças de artilharia
    
    Inicializar(mapaPecas, &tamanho);
    
    printf(">Mapa das pecas de artilharia:\n");
    ImprimirMapa(mapaPecas, tamanho);
    
    return 0;
}