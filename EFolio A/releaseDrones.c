#include<stdio.h>
#include<stdlib.h>
#define TAMANHO 10

//Estados de conhecimento do mapa de informação
#define DESCONHECIDA 0
#define SEGURA  1
#define INSEGURA 2

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

//Imprime cabeçalho com letras das colunas (A B C ...)
void ImprimirLinhaLetras(int tamanho) {
    int j;
    printf(">  ");  /* > + 2 espaços para alinhamento */
    for (j = 0; j < tamanho; j++) {
        printf("%c ", 'A' + j);
    }
    printf("\n");
}

/*====================================================================*/
//               PARTE 2: CONVERSÃO DE COORDENADAS
/*====================================================================*/

/*Converte posição em string para índice linear*/
/*RETORNA: Índice Linear ou -1 se posição inválida*/
int Posicao(char *str, int tamanho){
    int linha, coluna;
    /* ler coordenada letra/número*/
    coluna = str[0] - 'A';
    if (coluna < 0 || coluna >= tamanho)
        return -1;
    linha = atoi(str+1);
    if (linha < 1 || linha > tamanho)
        return -1;
    return (linha - 1)* tamanho +  coluna;
}

/* Converte índice linear para coordenadas 2D (linha, coluna)*/
void IndicePara2D(int indice, int tamanho, int *linha, int *coluna){
    *linha = indice / tamanho;
    *coluna = indice % tamanho;
}

/*=====================================================================*/
//                    PARTE2: LÓGICA DOS DRONES
/*=====================================================================*/

/*Verifica se há artilharia na casa especificada ou em alguna próxima*/
/*RETORNA: 1 se encontrar artilharia (drone abatido), 0 casos encontrados*/
int  ArtilhariaProx(int mapa[TAMANHO][TAMANHO], int linha, int coluna, int tamanho){
    int deltaLinha, deltaColuna;
    int novaLinha, novaColuna;

    /*Verificar a própria casa*/
    if (mapa[linha][coluna] == 1)
    {
        return 1;
    }

    /*Verifica as 8 casas vizinhas*/
    for (deltaLinha = -1; deltaLinha <= 1; deltaLinha++)
    {
        for (deltaColuna = -1; deltaColuna <=1; deltaColuna++)
        {
            /*Ignora a própria casa (centro)*/
            if (deltaLinha != 0 || deltaColuna != 0)
            {
                novaLinha = linha + deltaLinha;
                novaColuna = coluna + deltaColuna;

                /* Verifica se vizinho está dentro dos limites do mapa*/
                if (novaLinha >= 0 && novaLinha < tamanho && novaColuna >= 0 && novaColuna < tamanho)
                {
                    if(mapa[novaLinha][novaColuna] == 1)
                    {
                        return 1; // Encontrou artilharia
                    }
                }
            }
        }
    }
    return 0; //Nenhuma artilharia encontrada
}

// Atualiza estado de conhecimento do mapa após lançamento de drone
/*Se abatido: casa e vizinhas desconhecidas -> INSEGURA (!)*/
/*Se não abatido: casa e vizinhas desconhecidas -> SEGURA (.)*/
/*Casas já conhecidas não são alteradas*/
void AtualizarMapaInfo(int mapaInfo[TAMANHO][TAMANHO], int linha, int coluna, int droneAbatido, int tamanho){
    int deltaLinha, deltaColuna;
    int novaLinha, novaColuna;
    int novoEstado;

    /*Define novo estado baseado no resultado do drone*/
    novoEstado = droneAbatido ? INSEGURA : SEGURA;

    /*Atualiza a casa de onde foi lançado*/
    mapaInfo[linha][coluna] = novoEstado;

    /*Atualiza casas vizinhas (apenas as desconhecidas)*/
    for (deltaLinha = -1; deltaLinha <= 1; deltaLinha++)
    {
        for (deltaColuna = -1; deltaColuna <= 1; deltaColuna++)
        {
            /*Ignora a própria casa (centro)*/
            if (deltaLinha != 0 || deltaColuna != 0)
            {
                novaLinha = linha + deltaLinha;
                novaColuna = coluna + deltaColuna;

                /* Verifica limites do mapa*/
                if (novaLinha >= 0 && novaLinha < tamanho && novaColuna >= 0 && novaColuna < tamanho){
                    /*Só atualiza se a casa for desconhecida*/
                    if(mapaInfo[novaLinha][novaColuna] == DESCONHECIDA){
                        mapaInfo[novaLinha][novaColuna] = novoEstado;
                    }
                }
            }
        }   
    }            
}

// Inicializa todas as casas do mapa como desconhecidas
void InicializarMapaInfo(int mapaInfo[TAMANHO][TAMANHO], int tamanho){
    int linha, coluna;

    for(linha = 0; linha < tamanho; linha++){
        for(coluna = 0; coluna < tamanho; coluna++){
            mapaInfo[linha][coluna] = DESCONHECIDA;
        }
    }
}

/*===================================================================*/
//              PARTE2: IMPRESSÃO DO MAPA DE INFORMAÇÃO 
/*===================================================================*/

//Retona símbolo visual para estado CONHECIDO
/*É como o ObterSimbolo() da Parte 1, mas para estados diferentes*/
char ObterSimboloInfo(int estado){
    if(estado == SEGURA){
        return '.';
    } else if(estado == INSEGURA){
        return '!';
    } else{
        return '?';
    }
}

// Imprime uma  linha do mapa de informação
/*ADAPTADO de ImprimirLinhaMapa() Parte1*/
void ImprimirLinhaInfo(int mapaInfo[TAMANHO], int linha, int tamanho){
    int j;

    /*Garante alinhamento correcto para números de 1 ou 2 Dígitos*/
    if(linha + 1 < 10){
        printf("> %d", linha + 1);
    } else {
        printf(">%d", linha + 1);
    }

    /*Usa ObterSimboloInfo() ao invés de ObterSimbolo*/
    for(j = 0; j < tamanho; j++){
        printf("%c", ObterSimboloInfo(mapaInfo[j]));
    }
    printf("\n");
}

/*Imprime mapa completo de informação*/
/*ADAPTADO de ImprimirMapa() da Parte 1*/
void ImprimirMapaInfo(int mapaInfo[TAMANHO][TAMANHO], int tamanho){
    int i;
    
    // REUTILIZA ImprimirLinhasLetras() Parte 1
    ImprimirLinhaLetras(tamanho);

    for(i=0; i < tamanho; i++){
        ImprimirLinhaInfo(mapaInfo[i], i, tamanho);
    }
}

/*===================================================================*/
//                       PARTE 2: MAIN PROGRAM
/*===================================================================*/

int main(){
    int tamanho;
    int mapaPecas[TAMANHO][TAMANHO]; /*Posição das peças de artilharia */
    int mapaInfo[TAMANHO][TAMANHO]; /* Informação existente */
    int drones[TAMANHO]; /* Posições dos drones */
    int i;
    char str[100];

    Inicializar(mapaPecas, &tamanho);

    InicializarMapaInfo(mapaInfo, tamanho);

    printf(">Mapa inicial de informacao:\n");
    ImprimirMapaInfo(mapaInfo, tamanho);

    printf(">Lancar drones: ");
    i = 0;
    do{
        scanf("%s", str);
        if(str[0] != '.'){
            drones[i] = Posicao(str, tamanho);
            i++;
        }
    } while(str[0] != '.' && i < TAMANHO);

    //Processa cada drone lançado
    int numDrones = i;
    int j; 
    for(j=0; j < numDrones; j++){
        if(drones[j] >= 0 && drones[j] < TAMANHO * TAMANHO){
            int linha, coluna;
            int droneAbatido;

            //Converte índice para coordenadas 2D
            IndicePara2D(drones[j], tamanho, &linha, &coluna);

            /*Verifica se drone foi abatido*/
            droneAbatido = ArtilhariaProx(mapaPecas, linha, coluna, tamanho);

            // Atualiza Mapa de Informação
            AtualizarMapaInfo(mapaInfo, linha, coluna, droneAbatido, tamanho);

            // Informa se drone foi abatido
            if(droneAbatido){
                printf(">Drone %d abatido.\n", j + 1);
            }
        }
    }

    /*Mostra mapa final*/
ImprimirMapaInfo(mapaInfo, tamanho);
return 0;
}
