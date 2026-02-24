#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAXSTR 1024

// Estrutura para a lista ligada de strings 
typedef struct listastr{
    char *str;
    struct listastr *seg;
} ListaSTR;

// Adiciona um elemento no topo da lista
ListaSTR* Adiciona(ListaSTR *lista, char *str)
{
    ListaSTR *novo = (ListaSTR*)malloc(sizeof(ListaSTR));
    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }

    // Alocar espaço para a string e copiar a string
    novo->str = (char*)malloc(strlen(str)+1);
    if(novo->str == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a string\n");
        exit(1);
    }
    strcpy(novo->str, str);

    novo->seg = lista;
    return novo;    
}

// Remove o elemento no topo da lista
ListaSTR* Remove(ListaSTR *lista)
{
    if (lista == NULL)
        return NULL;

    ListaSTR *seg = lista->seg;
    free(lista->str); // Liberar a string alocada
    free(lista);      // Liberar o nó da lista
    return seg;
}

// Função para contar o número de elementos na lista
int Tamanho(ListaSTR *lista)
{
    int count = 0;
    while (lista != NULL)
    {
        count++;
        lista = lista->seg;
    }
    return count;
}

// Função para dividir a lista em duas metades
void DividirLista(ListaSTR *lista, ListaSTR **metade1, ListaSTR **metade2)
{
    int meio, i;
    ListaSTR *atual;

    int tamanho = Tamanho(lista);
    meio = tamanho / 2;

    *metade1 = lista;

    /*Ir até o elemento antes da metade*/
    atual = lista;
    for (i = 0; i < meio - 1 && atual != NULL; i++)
    {
        atual = atual->seg;
    }


    
}