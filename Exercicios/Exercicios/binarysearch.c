#include<stdio.h>
#define MAXVECTOR 1000000

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

//Arrays globais
static int v[MAXVECTOR];
static int temp[MAXVECTOR]; //Array auxiliar para merge

// MERGE SORT -> Do exercício anterior
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
        if ( temp[i] <= temp[j]) 
            arr[k++] = temp[i++];
        else
            arr[k++] = temp[j++];
    }

    while (i <= mid) arr[k++] = temp[i++];
    while (j <= right) arr[k++] = temp[j++];
}

void MergeSortRecursivo(int arr[], int left, int right)
{
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Ordena a primeira e a segunda metade
        MergeSortRecursivo(arr, left, mid);
        MergeSortRecursivo(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void MergeSort(int arr[], int size)
{
    MergeSortRecursivo(arr, 0, size - 1);
}

// ==================BINARY SERACH=======================

/*Busca binária recursiva
Retorna a posição do elemento ou -1 se não encontrado*/

int BinarySearchRecursivo(int arr[], int left, int right, int element)
{
    if (left > right)
        return -1; // Elemento não encontrado

    int mid = left + (right - left) / 2;

    if (arr[mid] == element)
        return mid; // Encontrou !
    
    if (arr[mid] > element)
        return BinarySearchRecursivo(arr, left, mid - 1, element); // Vai para a metade esquerda
    else
        return BinarySearchRecursivo(arr, mid + 1, right, element); // Vai para a metade direita
}

//Função Wrapper para busca binária
int BinarySearch(int arr[], int size, int element)
{
    return BinarySearchRecursivo(arr, 0, size - 1, element);
}

// =================MAIN=======================

int main()
{
    int i, n, element, pos;

    /*Lê o número máximo e o elemento a procurar*/
    scanf("%d", &n);
    scanf("%d", &element);
    n++; 

    /*Inicializa o vetor com valores aleatórios*/
    for(i=0; i<MAXVECTOR; i++)
        v[i]= randaux() % n;
    
    /*Ordena o array*/
    MergeSort(v, MAXVECTOR);

    /*Procura o elemento*/
    pos= BinarySearch(v, MAXVECTOR, element);

    /*Imprime a posição encontrada*/
    printf("%d\n", pos);
    return 0;
}