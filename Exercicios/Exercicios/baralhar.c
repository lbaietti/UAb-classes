#include<stdio.h>

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

//Algoritmo de Fisher-Yates para baralhar um array
/*Para cada posição i, escolhe um índice aleatório de i até o final e troca com a posição atual*/

void baralhar(int vec[], int size )
{
    int i, j, temp;

    //Percorre cada posição do array
    for(i=0; i<size; i++)
    /*Gera índice aleatório entre i e size-1*/
    {
        j = i + (randaux() % (size - 1));

        /*Troca vec[i] com vec[j]*/
        temp = vec[i];
        vec[i] = vec[j];
        vec[j] = temp;
    }
}

int main()
{
    int v[1000], i, n;
    scanf("%d", &n);

    // Inicializar vetor identidade: v[i]=1
    for(i=0; i<n; i++)
        v[i]=i+1;

    baralhar(v, n);

    // Imprime os 10 primeiros elementos
    for(i=0; i<10; i++)
        printf("%d ", v[i]);

    printf("\n");
    return 0;
}