#include <stdio.h>
#include <stdlib.h>

/*Faça uma função para ordenar uma lista de inteiros, com o algoritmo do MergeSort: ordenar cada metade da lista (não obrigatoriamente a primeira metade, apenas metade dos elementos), recursivamente, e no final juntar ambas as metades ordenadas, numa só lista ordenada.

Notas:

Basta reutilizar as funções de adição e remoção de elementos numa lista, dos exercícios anteriores sobre listas de inteiros.
Atenção ao enunciado, esta função MergeSort em listas é mais simples que a correspondente versão em vectores
Na versão do HR deve ser lido o número de elementos no stdin, que será sempre maior que 10000.*/

/* Estrutura para a lista ligada simples */
typedef struct lista {
    int valor;
    struct lista *proximo;
} Lista;

unsigned int randaux()
{
    static long seed=1;
    return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

/* Adiciona um elemento no topo da lista */
Lista* Adiciona(Lista *lista, int valor)
{
    Lista *novo = (Lista*)malloc(sizeof(Lista));
    if (novo == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    novo->valor = valor;
    novo->proximo = lista;
    return novo;
}

/* Remove o elemento no topo da lista */
Lista* Remove(Lista *lista)
{
    if (lista == NULL)
        return NULL;
    
    Lista *proximo = lista->proximo;
    free(lista);
    return proximo;
}

/* Função para dividir a lista ao meio usando técnica de dois ponteiros */
void DividirLista(Lista *origem, Lista **primeira, Lista **segunda)
{
    Lista *lento;
    Lista *rapido;
    
    if (origem == NULL || origem->proximo == NULL) {
        *primeira = origem;
        *segunda = NULL;
        return;
    }
    
    lento = origem;
    rapido = origem->proximo;
    
    /* Avançar rapido 2x mais rápido que lento */
    while (rapido != NULL) {
        rapido = rapido->proximo;
        if (rapido != NULL) {
            lento = lento->proximo;
            rapido = rapido->proximo;
        }
    }
    
    /* lento está no meio, dividir aqui */
    *primeira = origem;
    *segunda = lento->proximo;
    lento->proximo = NULL;
}

/* Função para juntar duas listas ordenadas */
Lista* Juntar(Lista *a, Lista *b)
{
    Lista dummy;
    Lista *cauda = &dummy;
    dummy.proximo = NULL;
    
    while (a != NULL && b != NULL) {
        if (a->valor <= b->valor) {
            cauda->proximo = a;
            a = a->proximo;
        } else {
            cauda->proximo = b;
            b = b->proximo;
        }
        cauda = cauda->proximo;
    }
    
    /* Adicionar o resto */
    if (a != NULL) {
        cauda->proximo = a;
    } else {
        cauda->proximo = b;
    }
    
    return dummy.proximo;
}

/* Função MergeSort para listas ligadas */
Lista* MergeSort(Lista *lista)
{
    Lista *primeira, *segunda;
    
    /* Caso base: lista vazia ou com um elemento */
    if (lista == NULL || lista->proximo == NULL) {
        return lista;
    }
    
    /* Dividir a lista em duas metades */
    DividirLista(lista, &primeira, &segunda);
    
    /* Ordenar recursivamente cada metade */
    primeira = MergeSort(primeira);
    segunda = MergeSort(segunda);
    
    /* Juntar as duas metades ordenadas */
    return Juntar(primeira, segunda);
}

int main()
{
    int i, n;
    Lista *lista = NULL;
    
    if (scanf("%d", &n) != 1) {
        return 1;
    }
    
    for(i = 0; i < n; i++)
        lista = Adiciona(lista, randaux() % 1000);
    
    lista = MergeSort(lista);
    
    /* remover os primeiros 999 elementos, para aceder à posição 1000 */
    for(i = 0; i < 999; i++)
        lista = Remove(lista);
    
    if (lista != NULL) {
        printf("Elemento na posição 1000: %d\n", lista->valor);
    }
    
    /* remover os primeiros 9000 elementos, para aceder à posição 10000 */
    for(i = 0; i < 9000; i++)
        lista = Remove(lista);
    
    if (lista != NULL) {
        printf("Elemento na posição 10000: %d", lista->valor);
    }
    
    while(lista != NULL)
        lista = Remove(lista);
    
    return 0;
}