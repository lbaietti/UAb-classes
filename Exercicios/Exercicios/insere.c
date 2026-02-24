#include<stdio.h>
#include<stdlib.h>

/*Sobre o código desenvolvido no exercício adiciona.c, faça uma função para inserir um elemento numa lista de inteiros, após um elemento menor e antes de um elemento maior ou igual (insere de forma ordenada). Faça ainda uma outra função para apagar os elementos de uma lista de inteiros, que sejam iguais a um determinado valor.

Notas:

Pode chamar as operações atómicas desenvolvidas de adição, remoção e número de elementos, para implementar a inserção ordenada e remoção dos elementos com um determinado valor
Leia do stdin o número de elementos a adicionar, e de seguida remova todos os elementos pares, de acordo com a execução em baixo.*/

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
        fprint(stderr, "Erro ao alocar a memória\n");
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

// Calcula o número de elementos da lista
int Elementos(Lista *lista)
{
    int count=0;
    Lista *atual = lista;

    while(atual != NULL)
    {
        count++;
        atual= atual->seguinte;
    }
    return count;
}

// Insere um elemento de forma ordenada na lista
Lista* Insere(Lista *lista, int valor)
{
    Lista *novo= (Lista*)malloc(sizeof(lista));
    if(novo==NULL)
    {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    novo->valor = valor;

    /*Se a lista está vazia ou o valor é menor que o primeiro elemento*/
    if(lista==NULL || valor <= lista->valor)
    {
        novo->seguinte=lista;
        return novo;
    }

    /* Procurar a posição correcta para inserir*/
    Lista *atual=lista;
    while(atual->seguinte != NULL && atual->seguinte->valor < valor)
    {
        atual = atual->seguinte;
    }

    /*inserir o novo elemento na posição encontrada*/
    novo->seguinte = atual->seguinte;
    atual->seguinte=novo;
    return lista;
}

/*Apaga todos os elementos com um determinado valor*/
Lista* Apaga(Lista *lista, int valor)
{
    Lista *atual, *anterior, *temp;

    /*Remover elementos no início da lista*/
    while(lista != NULL && lista->valor==valor)
    {
        temp = lista;
        lista = lista->seguinte;
        free(temp);
    }
    /*Se a lista ficou vazia */
    if(lista==NULL)
        return NULL;
    
    /*Remover elementos no meio e no final da lista*/
    anterior=lista;
    atual=lista->seguinte;

    while(atual != NULL)
    {
        if(atual->valor==valor)
        {
            anterior->seguinte=atual->seguinte;
            free(atual);
            atual=anterior->seguinte;
        } else
        {
            anterior=atual;
            atual=atual->seguinte;
        }
    }

    return lista;
}

/*Imprime os n primeiros elementos da lista*/
void PrintListaInts(Lista *lista, int n, const char *mensagem)
{
    int i;
    Lista *atual=lista;

    printf("%s", mensagem);

    for(i=0; i<n && atual != NULL; i++)
    {
        printf("%d", atual->valor);
        atual= atual->seguinte;
    }

    printf("\n");
}

int main()
{
    int i, n;
    Lista *lista=NULL;

    scanf("%d", &n);

    for(i=0; i<n; i++)
        lista = Insere(lista, randaux()%1000);
    
    PrintListaInts(lista, 10, "Elementos após inserir: ");

    for(i=0; i<1000; i+=2)
        lista = Apaga(lista, i);
    
    printf("Numero de elementos na lista apos apagar: %d\n");

    while(lista)
        lista=Remove(lista);

    return 0;
}
