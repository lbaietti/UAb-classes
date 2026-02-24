#include <stdio.h>
#include <stdlib.h>
#define TAMANHO 10
#define MAX_DIAS 10
#define MAX_DRONES_DIA 10

#define DESCONHECIDA 0
#define SEGURA 1
#define INSEGURA 2

/* ============================================================================
 * CÓDIGO REUTILIZADO DAS PARTES 1 E 2
 * ============================================================================ */

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

void Inicializar(int mapa[TAMANHO][TAMANHO], int *tamanho, int *numDrones) {
    int semente, prob;
    printf(">Introduza (semente, probabilidade (1 em N), tamanho ate 10, numero de drones): \n");
    scanf("%d %d %d %d", &semente, &prob, tamanho, numDrones);
    
    while (semente-- > 0)
        randaux();
    
    if (*tamanho > TAMANHO)
        *tamanho = TAMANHO;
    else if (*tamanho < 2)
        *tamanho = 2;
    
    GerarMapa(mapa, *tamanho, prob);
}

void ImprimirLinhaLetras(int tamanho) {
    int j;
    printf(">    ");
    for (j = 0; j < tamanho; j++) {
        printf("%c", 'A' + j);
        if (j < tamanho - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int Posicao(char *str, int tamanho) {
    int linha, coluna;
    coluna = str[0] - 'A';
    if (coluna < 0 || coluna >= tamanho)
        return -1;
    linha = atoi(str+1);
    if (linha < 1 || linha > tamanho)
        return -1;
    return (linha - 1) * tamanho + coluna;
}

void IndicePara2D(int indice, int tamanho, int *linha, int *coluna) {
    *linha = indice / tamanho;
    *coluna = indice % tamanho;
}

int ArtilhariaProx(int mapa[TAMANHO][TAMANHO], int linha, int coluna, int tamanho) {
    int deltaLinha, deltaColuna;
    int novaLinha, novaColuna;

    if (mapa[linha][coluna] == 1) {
        return 1;
    }

    for (deltaLinha = -1; deltaLinha <= 1; deltaLinha++) {
        for (deltaColuna = -1; deltaColuna <= 1; deltaColuna++) {
            if (deltaLinha != 0 || deltaColuna != 0) {
                novaLinha = linha + deltaLinha;
                novaColuna = coluna + deltaColuna;

                if (novaLinha >= 0 && novaLinha < tamanho && 
                    novaColuna >= 0 && novaColuna < tamanho) {
                    if (mapa[novaLinha][novaColuna] == 1) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void AtualizarMapaInfo(int mapaInfo[TAMANHO][TAMANHO], int linha, int coluna, 
                       int droneAbatido, int tamanho) {
    int deltaLinha, deltaColuna;
    int novaLinha, novaColuna;
    int novoEstado;

    novoEstado = droneAbatido ? INSEGURA : SEGURA;
    mapaInfo[linha][coluna] = novoEstado;

    for (deltaLinha = -1; deltaLinha <= 1; deltaLinha++) {
        for (deltaColuna = -1; deltaColuna <= 1; deltaColuna++) {
            if (deltaLinha != 0 || deltaColuna != 0) {
                novaLinha = linha + deltaLinha;
                novaColuna = coluna + deltaColuna;

                if (novaLinha >= 0 && novaLinha < tamanho && 
                    novaColuna >= 0 && novaColuna < tamanho) {
                    
                    if (!droneAbatido) {
                        mapaInfo[novaLinha][novaColuna] = SEGURA;
                    } else if (mapaInfo[novaLinha][novaColuna] == DESCONHECIDA) {
                        mapaInfo[novaLinha][novaColuna] = INSEGURA;
                    }
                }
            }
        }
    }
}

void InicializarMapaInfo(int mapaInfo[TAMANHO][TAMANHO], int tamanho) {
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            mapaInfo[linha][coluna] = DESCONHECIDA;
        }
    }
}

char ObterSimboloInfo(int estado) {
    if (estado == SEGURA) {
        return '.';
    } else if (estado == INSEGURA) {
        return '!';
    } else {
        return '?';
    }
}

void ImprimirLinhaInfo(int mapaInfo[TAMANHO], int linha, int tamanho) {
    int j;
    if (linha + 1 < 10) {
        printf(">  %d", linha + 1);
    } else {
        printf("> %d", linha + 1);
    }

    for (j = 0; j < tamanho; j++) {
        printf(" %c", ObterSimboloInfo(mapaInfo[j]));
    }
    printf("\n");
}

void ImprimirMapaInfo(int mapaInfo[TAMANHO][TAMANHO], int tamanho) {
    int i;
    ImprimirLinhaLetras(tamanho);
    for (i = 0; i < tamanho; i++) {
        ImprimirLinhaInfo(mapaInfo[i], i, tamanho);
    }
}

/* ============================================================================
 * CÓDIGO NOVO PARA PARTE 3
 * ============================================================================ */

void InicializarContador(int contador[TAMANHO][TAMANHO], int tamanho) {
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            contador[linha][coluna] = 0;
        }
    }
}

/* Substitua a função TemCasasDesconhecidas por esta nova função */
int DeveContinuar(int mapaInfo[TAMANHO][TAMANHO], int tamanho) {
    int linha, coluna;
    
    /* Verifica se há casas desconhecidas */
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            if (mapaInfo[linha][coluna] == DESCONHECIDA) {
                return 1;
            }
        }
    }
    
    /* Verifica se há casas inseguras */
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            if (mapaInfo[linha][coluna] == INSEGURA) {
                return 1;
            }
        }
    }
    
    return 0;
}

void ReavaliarCasasInseguras(int mapaPecas[TAMANHO][TAMANHO], 
                             int mapaInfo[TAMANHO][TAMANHO], int tamanho) {
    int linha, coluna;
    
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            if (mapaInfo[linha][coluna] == INSEGURA) {
                if (ArtilhariaProx(mapaPecas, linha, coluna, tamanho) == 0) {
                    mapaInfo[linha][coluna] = SEGURA;
                }
            }
        }
    }
}

int main() {
    int tamanho;
    int mapaPecas[TAMANHO][TAMANHO];
    int mapaInfo[TAMANHO][TAMANHO];
    int contador[TAMANHO][TAMANHO];
    int posicoesDia[MAX_DRONES_DIA];
    int dronesOperacionais;
    int dia;
    int numPosicoes, dronesAbatidosDia;
    char str[100];
    int i, j;
    
    Inicializar(mapaPecas, &tamanho, &dronesOperacionais);
    InicializarMapaInfo(mapaInfo, tamanho);
    InicializarContador(contador, tamanho);
    
    printf(">Mapa inicial de informacao:\n");
    ImprimirMapaInfo(mapaInfo, tamanho);
    
    for (dia = 1; dia <= MAX_DIAS; dia++) {
        dronesAbatidosDia = 0;
        
        /* CORREÇÃO: Adiciona 1 drone ANTES de mostrar */
        if (dia > 1) {
            dronesOperacionais++;
        }
        
        printf(">Dia %d, lancar drones (#%d): \n", dia, dronesOperacionais);
        
        /* Ler posições */
        i = 0;
        scanf("%s", str);
        while (str[0] != '.' && i < MAX_DRONES_DIA && i < dronesOperacionais) {
            posicoesDia[i] = Posicao(str, tamanho);
            i++;
            scanf("%s", str);
        }
        numPosicoes = i;
        
        /* Processar drones */
        for (j = 0; j < numPosicoes; j++) {
            if (posicoesDia[j] >= 0 && posicoesDia[j] < TAMANHO * TAMANHO) {
                int linha, coluna;
                int droneAbatido;
                
                IndicePara2D(posicoesDia[j], tamanho, &linha, &coluna);
                
                droneAbatido = ArtilhariaProx(mapaPecas, linha, coluna, tamanho);
                
                contador[linha][coluna]++;
                
                if (contador[linha][coluna] == 3) {
                    int tinhaArtilharia = (mapaPecas[linha][coluna] == 1);
                    
                    if (tinhaArtilharia) {
                        mapaPecas[linha][coluna] = 0;
                    }
                    
                    int temArtilharia = ArtilhariaProx(mapaPecas, linha, coluna, tamanho);
                    
                    if (tinhaArtilharia || !temArtilharia) {
                        mapaInfo[linha][coluna] = SEGURA;
                    } else {
                        mapaInfo[linha][coluna] = INSEGURA;
                    }
                    
                    int dL, dC;
                    for (dL = -1; dL <= 1; dL++) {
                        for (dC = -1; dC <= 1; dC++) {
                            int nL = linha + dL;
                            int nC = coluna + dC;
                            if (dL != 0 || dC != 0) {
                                if (nL >= 0 && nL < tamanho && nC >= 0 && nC < tamanho) {
                                    if (mapaInfo[nL][nC] == DESCONHECIDA) {
                                        mapaInfo[nL][nC] = temArtilharia ? INSEGURA : SEGURA;
                                    }
                                }
                            }
                        }
                    }
                } else {
                    AtualizarMapaInfo(mapaInfo, linha, coluna, droneAbatido, tamanho);
                }
                
                if (droneAbatido) {
                    dronesAbatidosDia++;
                    printf(">Drone %d abatido.\n", j + 1);
                }
            }
        }
        
        ReavaliarCasasInseguras(mapaPecas, mapaInfo, tamanho);
        
        /* CORREÇÃO: Perde drones abatidos */
        dronesOperacionais -= dronesAbatidosDia;
        
        ImprimirMapaInfo(mapaInfo, tamanho);
        
        /* CORREÇÃO: Para se não há mais desconhecidas */
        if (!DeveContinuar(mapaInfo, tamanho)) {
            printf(">Duracao da campanha: %d dias\n", dia);
            return 0;
        }
    }
    
    printf(">Duracao da campanha: %d dias\n", MAX_DIAS);
    
    return 0;
}
