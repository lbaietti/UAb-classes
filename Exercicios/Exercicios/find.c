#include<stdio.h>

unsigned int randaux(){
    static long seed=1;
    return(((seed = seed * 214013L + 2531011L)>>16)& 0x7fff);
}

/*Função que procura um elemento no vector.
Retorna a posição do elemento ou -1 se não encontrado*/
int find(int vec[], int size, int element){
    int i;
    for(i=0; i<size;i++){
        if(vec[i]==element)
            return i;
    }
    return -1;
}

int main(){
    /*declaração do vetor v com 1000 elementos (de 0 a n)*/
    int v[1000], i, n, pos;
    scanf("%d", &n);
    n++;

    /*Inicializar o vector, com valores aleatórios de 0 a n*/
    for(i=0; i<1000; i++)
        v[i]= randaux() % n;

    /*Imprime os 10 primeiros elementos*/
    for(i=0; i<10; i++){
        printf("%d ", v[i]);
    }
    printf("\n");

    /*Procurar o número 2 dentro do vetor[]*/
    pos=find(v, 1000, 2);

    /*Imprime a posição encontrada*/
    printf("%d\n", pos);
    return 0;
}