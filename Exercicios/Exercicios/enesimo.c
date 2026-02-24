#include<stdio.h>
#include<stdlib.h>

/*Sobre o código desenvolvido no exercício insertsort.c, faça uma função para retornar o enésimo elemento numa lista de inteiros.

Notas:

Ao reutilizar todo o exercício insertsort.c, apenas tem de implementar a nova função.
Se for solicitado um índice superior ao tamanho da lista, retornar 0.*/

//Estrutura para lista ligada simples
typedef struct lista
{
    int valor;
    struct lista *seguinte;
}Lista;

unsigned int randaux()
{
    static long seed=1;
    return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

// Adiciona um elemento no topo da lista
Lista* Adiciona(Lista *lista, int valor)
{
    Lista *novo = (Lista*)malloc(sizeof(Lista));
    if(novo==NULL)
    {
        fprintf(stderr, "Erro ao alocar a memória\n");
        exit(1);
    }
    novo->valor = valor;
    novo->seguinte = lista;
    return novo;
}

//Remove o elemento do topo da lista
Lista* Remove(Lista *lista)
{
    if(lista==NULL)
        return NULL;
    
    Lista *seguinte = lista->seguinte;
    free(lista);
    return seguinte;
}

// Insere um elemento de forma ordenada na lista
Lista* InsereOrdenado(Lista *lista, int valor)
{
    Lista *novo = (Lista*)malloc(sizeof(Lista));
    if(novo==NULL)
    {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    novo->valor = valor;
    novo->seguinte = NULL;

    /*Se a lista está vazia ou o valor é menor que o primeiro elemento*/
    if(lista==NULL || valor <= lista->valor)
    {
        novo->seguinte = lista;
        return novo;
    }

    /*Procurar a posição correta para inserir*/
    Lista *atual = lista;
    while(atual->seguinte != NULL && atual->seguinte->valor < valor)
    {
        atual = atual->seguinte;
    }

    /*inserir o novo elemento na posição encontrada*/
    novo->seguinte = atual->seguinte;
    atual->seguinte = novo;
    return lista;
}

//Ordena a lista usando InsertSort
Lista* InsertSort(Lista *lista)
{
    Lista *ordenada = NULL;
    Lista *atual = lista;   
    Lista *seguinte;

    /*Percorrer a lista original*/
    while(atual != NULL)
    {
        //Guardar o seguinte antes de remover o nó atual
        seguinte = atual->seguinte;

        //Inserir ordenamente na nova lista
        ordenada = InsereOrdenado(ordenada, atual->valor);

        /*Libertar o nó da lista original*/
        free(atual);

        /*Avançar para o seguinte*/
        atual = seguinte;
    }
    return ordenada;
}

//Retorna o enésimo elemento da lista (indexado a partir de 1)
int Valor(Lista *lista, int n)
{
    int i;
    Lista *atual = lista;

    //Percorrer a lista até o n-ésimo elemento
    for(i=1; i<n && atual != NULL; i++)
    {
        atual = atual->seguinte;
    }

    //Se chegamos ao elemento desejado, retornar seu valor
    if(atual != NULL)
    {
        return atual->valor;
    }

    //Se n for maior que o tamanho da lista, retornar 0
    return 0;
}

int main()
{
    int i, n;
    Lista *lista=NULL;

    scanf("%d", &n);

    for(i=0; i<n; i++)
        lista=Adiciona(lista, randaux()%1000);
    
    lista=InsertSort(lista);

    printf("elementos 250, 500 e 750: %d %d %d\n", Valor(lista, 250), Valor(lista, 500), Valor(lista, 750));

    while(lista != NULL)
        lista=Remove(lista);
    
    return 0;
}