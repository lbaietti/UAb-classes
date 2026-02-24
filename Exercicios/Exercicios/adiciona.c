#include<stdio.h>
#include<string.h>

/*Estrutura para a lista ligada simples*/
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

/*Adiciona um elemento no topo da lista*/
Lista* Adiciona(Lista *lista, int valor)
{
    Lista *novo=(Lista*)malloc(sizeof(Lista));
    if(novo==NULL)
    {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }

    novo->valor=valor;
    novo->seguinte=lista;
    return novo;
}

/*Remove o elemento no tipo da lista*/
Lista* Remove(Lista *lista)
{
    if(lista==NULL)
        return NULL;
    
    Lista *seguinte = lista->seguinte;
    free(lista);
    return seguinte;
}

/*Calcula o número de elementos na lista*/
int Elementos(Lista *lista)
{
    int count=0;
    Lista *atual=lista;

    while(atual != NULL)
    {
        count++;
        atual=atual->seguinte;
    }
    return count;     
}

void PrintListaInts(Lista *lista, int n, const char *mensagem)
{
    int i;
    Lista *atual=lista;

    printf("%s", mensagem);

    for(i=0; i<n && atual!=NULL; i++)
    {
        printf("%d", atual->valor);
        atual = atual->seguinte;
    }
    printf("\n");
}

int main()
{
    int i, n;
    Lista *lista=NULL; 

    printf("Número de Elementos: ");
    scanf("%d", &n);

    /*Adicionar n elementos à lista*/
    for(i=0; i<n; i++)
        lista = Adiciona(lista, randaux()%1000);
    
    /*Retornar os valores pedidos*/
    printf("Numero de Elementos: %d\n", Elementos(lista));
    PrintListaInts(lista, 10, "Elementos: ");

    /* libertar toda a lista antes de sair*/
    while(lista!=NULL)
        lista=Remove(lista);
    return 0;
}