#include<stdio.h>

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

/* Função que imprime n elementos de um vector de inteiros*/
void PrintInts(int vec[], int n, char *end)
{
    int i;
    for(i=0; i<n; i++)
    {
        printf("%d ", vec[i]);
    }
    printf("%s ", end);
}

/* Função que ordena o vector comparando todos os pares sequencialmente*/
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

/* Função que remove elementos duplicados de um vector ordenado
Retorna o novo tamanho do vector*/
int RemoveDups(int vec[], int size)
{
    int i, j, new_size;

    if(size == 0)
        return 0;

    new_size = 1; // Índice para o próximo elemento único

    for(i = 1; i < size; i++)
    {
        if(vec[i] != vec[new_size-1])
        {
            vec[new_size] = vec[i];
            new_size++;
        }
    }
    return new_size; // +1 para contar o primeiro elemento
}


int main()
{
    int v[1000], i, n, new_size;
    scanf("%d", &n);
    n++;

    /*Inicializa o vector com valores aleatórios*/
    for(i=0; i<1000; i++)
        v[i]=randaux()%n;

    /* Imprime os 10 primeiros elementos antes de ordenar*/
    PrintInts(v, 10, "\n");
    // Ordena o vector
    Sort(v, 1000);
    
    /*Remove os duplicados*/
    new_size = RemoveDups(v, 1000);

    /* Imprime os 10 primeiros elementos após ordenar*/
    PrintInts(v, 10, "\n");
    /*Imprime o novo tamanho do vector
    printf("%d\n", new_size);
    return 0; -> No entanto, é desnecessário para o exercício proposto. Entretanto poderá ficar como exemplo*/
}