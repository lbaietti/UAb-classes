#include<stdio.h>
#include<math.h>

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

/*Função que gera valores uniformes enter 0 e 1 (excluindo 0)*/
float uniforme(){
    int valor=(randaux() % 10000)+1; /*Gera valores de 1 a 10000*/
    return valor / 10000.0; /*Divide por 10000.0 para obter valores entre 0.0001 e 1.0000*/
}

//Função que retorna o valor máximo num vetor de reais 
float maximo(float vec[], int size){
    float max=vec[0];
    int i;

    for(i=1; i<size; i++){
        if(vec[i] > max){
            max = vec[i];
        }
    }
    return max;
}
 
int main(){
    float v[1000];
    int n, i;
    float max_valor;

    /*Lê o número de elementos */
    scanf("%d", &n);

    /*Inicializa o vector com valores da distribuição exponencial negativa*/
    for(i=0; i<n; i++){
        v[i]= -log(uniforme());
    }

    /*Imprime os 10 primeiros elementos com 2 casas decimais*/
    for(i=0; i<10; i++){
        printf("%.2f", v[i]);
    }
    printf("\n");

    /*Encontra e imprime o valor máximo*/
    max_valor= maximo(v, n);
    printf("%.2f\n", max_valor);

    return 0;
 }
