#include<stdio.h>
#include<stdlib.h>

/*Utilizando as funções desenvolvidas dos exercícios anteriores, nomeadamente a inserção de um elemento por ordem, faça uma função para ordenar uma lista através do algoritmo InsertSort: inserir cada elemento ordenadamente numa lista nova.

Notas:

Convém ter em atenção para não deixar blocos alocados não libertados. Veja com atenção se todos os blocos alocados no seu código são libertados no final.*/

// Estrutura para lista ligada simples
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

//Insere um elemento de forma ordenada na lista
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

    /*Inserir o novo elemento na posição encontrada*/
    novo->seguinte = atual->seguinte;  
    atual->seguinte = novo;
    return lista;
}

//Imprime os primeiros n elementos da lista
void PrintListaInts(Lista *lista, int n, const char *mensagem)
{
    int i; 
    Lista *atual = lista;

    printf("%s", mensagem);

    for(i=0; i<n && atual != NULL; i++)
    {
        printf("%d", atual->valor);
        atual = atual->seguinte;
    }
    printf("\n");
}

//Ordena a lista usando InsertSort
Lista* InsertSort(Lista *lista)
{
    Lista *ordenada = NULL;
    Lista *atual = lista;   
    Lista *seguinte;
    int count=0;

    /*Percorrer a lista original*/
    while(atual != NULL)
    {
        //Guardar o seguinte antes de remover o nó atual
        seguinte = atual->seguinte;

        //Inserir ordenamente na nova lista
        ordenada = InsereOrdenado(ordenada, atual->seguinte);

        //Imprimir apenas os primeiros 10 passos
        count++;
        if(count <= 10)
        {
            printf("  Lista: ");
            PrintListaInts(ordenada, 10, "");
        }

        /*Libertar o nó da lista original*/
        free(atual);

        /*Avançar para o seguinte*/
        atual = seguinte;
    }
    return ordenada;
}

int main()
{
    int i, n;
    Lista *lista = NULL;

    scanf("%d", &n);

    for(i=0; i<n; i++)
        lista = Adiciona(lista, randaux() % 1000);
    PrintListaInts(lista, 10, "Lista nao ordenada: ");

    lista = InsertSort(lista);

    PrintListaInts(lista, 10, "Lista ordenada: ");

    while(lista != NULL)
        lista = Remove(lista);
    
    return 0;
}