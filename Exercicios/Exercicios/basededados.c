#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 256
#define FICHEIRO_DB "contactos.dat"

/* Estrutura para um contacto */
typedef struct {
    char nome[MAX_STR];
    char telefone[MAX_STR];
    char cidade[MAX_STR];
    char descricao[MAX_STR];
} Contacto;

/* Função para limpar o buffer de input */
void LimparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Função para ler uma linha de input */
void LerLinha(char *str, int max)
{
    if (fgets(str, max, stdin) != NULL) {
        /* Remover o newline se existir */
        int len = strlen(str);
        if (len > 0 && str[len-1] == '\n') {
            str[len-1] = '\0';
        }
    }
}

/* Função para mostrar o menu */
void MostrarMenu()
{
    printf("\n#########################\n");
    printf("# Menu:                 #\n");
    printf("#########################\n");
    printf("# 1 | LISTAR registos   #\n");
    printf("# 2 | ADICIONAR registo #\n");
    printf("# 3 | VER registo       #\n");
    printf("# 4 | EDITAR registo    #\n");
    printf("#########################\n");
    printf("Opção: ");
}

/* Função para contar o número de registos */
int ContarRegistos()
{
    FILE *f;
    int count = 0;
    Contacto c;
    
    f = fopen(FICHEIRO_DB, "rb");
    if (f == NULL) {
        return 0;
    }
    
    while (fread(&c, sizeof(Contacto), 1, f) == 1) {
        count++;
    }
    
    fclose(f);
    return count;
}

/* Função para listar todos os registos */
void ListarRegistos()
{
    FILE *f;
    Contacto c;
    int id = 0;
    
    printf("\n########\n");
    printf("# Lista:\n");
    printf("########\n");
    
    f = fopen(FICHEIRO_DB, "rb");
    if (f == NULL) {
        printf("# (vazio)\n");
        printf("########\n");
        return;
    }
    
    while (fread(&c, sizeof(Contacto), 1, f) == 1) {
        printf("# %d | %s\n", id, c.nome);
        id++;
    }
    
    printf("########\n");
    fclose(f);
}

/* Função para adicionar um registo */
void AdicionarRegisto()
{
    FILE *f;
    Contacto c;
    
    printf("Nome: ");
    LerLinha(c.nome, MAX_STR);
    
    printf("Telefone: ");
    LerLinha(c.telefone, MAX_STR);
    
    printf("Cidade: ");
    LerLinha(c.cidade, MAX_STR);
    
    printf("Descrição: ");
    LerLinha(c.descricao, MAX_STR);
    
    /* Abrir ficheiro em modo append binário */
    f = fopen(FICHEIRO_DB, "ab");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }
    
    fwrite(&c, sizeof(Contacto), 1, f);
    fclose(f);
}

/* Função para ler um registo específico */
int LerRegisto(int id, Contacto *c)
{
    FILE *f;
    int i = 0;
    
    f = fopen(FICHEIRO_DB, "rb");
    if (f == NULL) {
        return 0;
    }
    
    while (fread(c, sizeof(Contacto), 1, f) == 1) {
        if (i == id) {
            fclose(f);
            return 1;
        }
        i++;
    }
    
    fclose(f);
    return 0;
}

/* Função para ver um registo */
void VerRegisto()
{
    int id;
    Contacto c;
    
    printf("ID: ");
    scanf("%d", &id);
    LimparBuffer();
    
    if (LerRegisto(id, &c)) {
        printf("\n#############\n");
        printf("# Nome | %s\n", c.nome);
        printf("# Telefone | %s\n", c.telefone);
        printf("# Cidade | %s\n", c.cidade);
        printf("# Descrição | %s\n", c.descricao);
        printf("#############\n");
    } else {
        printf("Registo não encontrado!\n");
    }
}

/* Função para editar um registo */
void EditarRegisto()
{
    int id, i = 0;
    Contacto c;
    FILE *f;
    
    printf("ID: ");
    scanf("%d", &id);
    LimparBuffer();
    
    if (!LerRegisto(id, &c)) {
        printf("Registo não encontrado!\n");
        return;
    }
    
    printf("Nome: ");
    LerLinha(c.nome, MAX_STR);
    
    printf("Telefone: ");
    LerLinha(c.telefone, MAX_STR);
    
    printf("Cidade: ");
    LerLinha(c.cidade, MAX_STR);
    
    printf("Descrição: ");
    LerLinha(c.descricao, MAX_STR);
    
    /* Abrir ficheiro em modo read/write binário */
    f = fopen(FICHEIRO_DB, "r+b");
    if (f == NULL) {
        printf("Erro ao abrir o ficheiro!\n");
        return;
    }
    
    /* Posicionar no registo correto */
    fseek(f, id * sizeof(Contacto), SEEK_SET);
    
    /* Escrever o registo atualizado */
    fwrite(&c, sizeof(Contacto), 1, f);
    fclose(f);
}

int main()
{
    int opcao;
    char input[10];
    
    while (1) {
        MostrarMenu();
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        /* Verificar se é apenas newline (opção vazia) */
        if (input[0] == '\n') {
            break;
        }
        
        opcao = atoi(input);
        
        switch (opcao) {
            case 1:
                ListarRegistos();
                break;
            case 2:
                AdicionarRegisto();
                break;
            case 3:
                VerRegisto();
                break;
            case 4:
                EditarRegisto();
                break;
            case 0:
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
    
    return 0;
}