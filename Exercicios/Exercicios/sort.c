#include<stdio.h>

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

/*Função que impreme n elementos de um vetor de inteiros*/
int PrintInts(int vec[], int n, char *end)
{
    int i;
    for(i=0; i<n; i++)
    {
        printf("%d ", vec[i]);
    }
    printf("%s ", end);
}


/*Função que ordena o vector comparando todos os pares sequenialmente
Se um par estiver na ordem inversa, troca os elementos*/

void Sort(int vec[], int size)
{
    int i, j, temp; 

    /*Percorre todos os pares (i, j) onde i < j*/
    for(i=0; i<size -1; i++)
    {
        for(j= i + 1; j < size; j++)
        {
            /*Se estiverem na ordem inversa, troca*/
            if(vec[i] > vec[j])
            {
                temp= vec[i];
                vec[i]=vec[j];
                vec[j]=temp;
            }
        }
    }
}

int main()
{
    int v[1000], i, n;
    scanf("%d", &n);
    n++;
    for(i=0; i<1000; i++)
    v[i]=randaux()%n;
    PrintInts(v, 10, "\n");
    Sort(v, 1000);
    PrintInts(v, 10, "\n");
}