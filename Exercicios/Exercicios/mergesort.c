#include<stdio.h>
#define SIZE 10000000

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

// Arrays globais para evitar stack overflow
static int v[SIZE];
static int temp[SIZE]; //Array auxiliar para merge

void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;

    // Copia os dados para o array temporário
    for (i = left; i <= right; i++) {
        temp[i] = arr[i];
    }

    i = left;    // Índice inicial do subarray esquerdo
    j = mid + 1; // Índice inicial do subarray direito  
    k = left;    // Índice inicial do array mesclado

    //Merge: Compara topos de cada metade
    while (i <= mid && j <= right) {
        if (temp[i] <= temp[j]) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = temp[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes da primeira metade, se houver
    while (i <= mid) {
        arr[k] = temp[i];
        i++;
        k++;
    }

    // Os elementos restantes da segunda metade já estão no lugar
    while (j <= right) {
        arr[k] = temp[j];
        j++;
        k++;
    }
}

// Ordena o array recursivamente usando merge sort
void mergeSort(int arr[], int left, int right)
{
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Ordena a primeira e a segunda metade
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main()
{
    int n, i;
    scanf("%d", &n);
    n++; // Para incluir o valor n na geração aleatória

    // Inicializa o vetor com valores aleatórios
    for(i=0; i<SIZE; i++)
        v[i]=randaux()%n;

    // Ordena o vetor usando merge sort
    mergeSort(v, 0, SIZE - 1);

    // Imprime quartis: 25%, 50% e 75%
    printf("%d %d %d\n", v[SIZE/4], v[SIZE/2], v[(3*SIZE)/4]);

    return 0;
}