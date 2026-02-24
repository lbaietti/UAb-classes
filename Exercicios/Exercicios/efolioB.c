#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
   CONSTANTES
   ============================================================================ */

#define MAX_NOME 101
#define MAX_LINHA 256
#define SAUDE_MAXIMA 100
#define SAUDE_MINIMA 0
#define SAUDE_LIMITE_ALTO 50
#define SAUDE_LIMITE_MEDIO 20
#define SAUDE_LIMITE_BAIXO 10

#define TICS_POR_SOL 100
#define SOLS_POR_TRATAMENTO_LENTO 10

/* ============================================================================
   DEFINIÇÃO DE TIPOS ABSTRATOS DE DADOS (TAD)
   ============================================================================ */

/* Estrutura para representar uma Câmara Térmica */
typedef struct Camara {
    char nome[MAX_NOME];             /* Nome da câmara (formato: Nome-Sobrenome) */
    struct Camara *proxima;          /* Ponteiro para a próxima câmara na lista */
} Camara;

/* Estrutura para representar um Aqualin */
typedef struct Aqualin {
    char nome[MAX_NOME];             /* Nome do Aqualin (formato: Nome-Sobrenome) */
    int saude_inicial;               /* Saúde no momento de entrada (0-100) */
    int saude_atual;                 /* Saúde atual (pode variar durante espera) */
    long instante_entrada;           /* Instante de entrada no centro (em tics) */
    long instante_saida;             /* Instante de alta ou morte */
    long tempo_espera;               /* Tempo em espera (tics) */
    char estado;                     /* 'E'=espera, 'T'=tratamento, 'A'=alta, 'M'=morte */
    char *camara_usada;              /* Nome da câmara onde foi tratado */
    struct Aqualin *proximo;         /* Ponteiro para o próximo Aqualin */
} Aqualin;

/* Estrutura para uma atribuição de tratamento (Aqualin em câmara) */
typedef struct Tratamento {
    char aqualin[MAX_NOME];          /* Nome do Aqualin tratado */
    char camara[MAX_NOME];           /* Nome da câmara usada */
    int saude;                       /* Saúde ao entrar na câmara */
    long instante_entrada;           /* Quando entrou na câmara */
    long instante_saida;             /* Quando saiu da câmara */
    struct Tratamento *proximo;      /* Próximo tratamento */
} Tratamento;

/* Estrutura global do sistema */
typedef struct {
    Camara *camaras;                 /* Lista ligada de câmaras */
    Aqualin *aqualins;               /* Lista ligada de Aqualins */
    Tratamento *tratamentos;         /* Lista de tratamentos realizados */
    size_t num_camaras;              /* Contador de câmaras */
    size_t num_aqualins;             /* Contador de Aqualins */
    long ultimo_instante;            /* Último instante registado */
    int tratamentos_executados;      /* Flag: tratamentos já foram processados? */
} Sistema;


/* ============================================================================
   PROTÓTIPOS DE FUNÇÕES (Abstração Funcional)
   ============================================================================ */

/* Funções de Inicialização e Libertação de Memória */
void inicializar_sistema(Sistema *sys);
void libertar_sistema(Sistema *sys);

/* Funções de Validação */
int validar_nome(const char *nome);
int camara_existe(Sistema *sys, const char *nome);
int aqualin_existe(Sistema *sys, const char *nome);

/* Funções de Gestão de Câmaras (Parte 1) */
void adicionar_camara(Sistema *sys, const char *nome);
void relatorio_global(Sistema *sys);

/* Funções de Gestão de Aqualins (Parte 2) */
void adicionar_aqualin(Sistema *sys, const char *nome, int saude, long instante);

/* Funções de Cálculo de Tratamento (Parte 3) */
long calcular_tempo_tratamento(int saude);
int calcular_degradacao_espera(int saude, long tempo_espera);
void resetar_tratamentos(Sistema *sys);
void processar_tratamentos(Sistema *sys);
void relatorio_altas(Sistema *sys);

/* Funções de Relatórios (Parte 4) */
void relatorio_aqualins(Sistema *sys);
void relatorio_camaras(Sistema *sys);
void indicador_espera(Sistema *sys);
void indicador_trabalho(Sistema *sys);
void indicador_salvos(Sistema *sys);

/* Funções Auxiliares */
void adicionar_tratamento(Sistema *sys, const char *aqualin, const char *camara, 
                          int saude, long entrada, long saida);


/* ============================================================================
   IMPLEMENTAÇÃO DAS FUNÇÕES
   ============================================================================ */

/* Inicializar o sistema */
void inicializar_sistema(Sistema *sys) {
    sys->camaras = NULL;
    sys->aqualins = NULL;
    sys->tratamentos = NULL;
    sys->num_camaras = 0;
    sys->num_aqualins = 0;
    sys->ultimo_instante = -1;
    sys->tratamentos_executados = 0;
}

/* Libertar toda a memória alocada */
void libertar_sistema(Sistema *sys) {
    /* Libertar câmaras */
    Camara *cam = sys->camaras;
    while (cam != NULL) {
        Camara *temp = cam;
        cam = cam->proxima;
        free(temp);
    }
    
    /* Libertar Aqualins */
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        Aqualin *temp = aq;
        aq = aq->proximo;
        if (temp->camara_usada != NULL) {
            free(temp->camara_usada);
        }
        free(temp);
    }
    
    /* Libertar tratamentos */
    Tratamento *tr = sys->tratamentos;
    while (tr != NULL) {
        Tratamento *temp = tr;
        tr = tr->proximo;
        free(temp);
    }
}

/* Validar formato do nome (deve conter exatamente um traço) */
int validar_nome(const char *nome) {
    size_t tracos = 0;
    size_t i;
    
    if (nome == NULL || strlen(nome) == 0) {
        return 0;
    }
    
    /* Nome não pode começar ou terminar com traço */
    if (nome[0] == '-' || nome[strlen(nome) - 1] == '-') {
        return 0;
    }
    
    for (i = 0; nome[i] != '\0'; i++) {
        if (nome[i] == '-') {
            tracos++;
        }
    }
    
    return tracos == 1;
}

/* Verificar se câmara já existe */
int camara_existe(Sistema *sys, const char *nome) {
    Camara *cam = sys->camaras;
    while (cam != NULL) {
        if (strcmp(cam->nome, nome) == 0) {
            return 1;
        }
        cam = cam->proxima;
    }
    return 0;
}

/* Verificar se Aqualin já existe */
int aqualin_existe(Sistema *sys, const char *nome) {
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        if (strcmp(aq->nome, nome) == 0) {
            return 1;
        }
        aq = aq->proximo;
    }
    return 0;
}

/* ============================================================================
   PARTE 1 - CÂMARAS TÉRMICAS
   ============================================================================ */

void adicionar_camara(Sistema *sys, const char *nome) {
    /* Validações */
    if (!validar_nome(nome)) {
        return;
    }
    
    if (camara_existe(sys, nome)) {
        return;
    }
    
    /* Alocar nova câmara */
    Camara *nova = malloc(sizeof(Camara));
    if (nova == NULL) {
        return; /* Falha na alocação */
    }
    
    strcpy(nova->nome, nome);
    nova->proxima = NULL;
    
    /* Adicionar ao fim da lista (mantém ordem de inserção) */
    if (sys->camaras == NULL) {
        sys->camaras = nova;
    } else {
        Camara *cam = sys->camaras;
        while (cam->proxima != NULL) {
            cam = cam->proxima;
        }
        cam->proxima = nova;
    }
    
    sys->num_camaras++;
}

void relatorio_global(Sistema *sys) {
    printf("Aqualins: %zu\n", sys->num_aqualins);
    printf("Camaras: %zu\n", sys->num_camaras);
}

/* ============================================================================
   PARTE 2 - REGISTO DE AQUALINS
   ============================================================================ */

void adicionar_aqualin(Sistema *sys, const char *nome, int saude, long instante) {
    /* Validações */
    if (!validar_nome(nome)) {
        return;
    }
    
    if (aqualin_existe(sys, nome)) {
        return;
    }
    
    /* Instante não pode decrescer */
    if (instante < sys->ultimo_instante) {
        return;
    }
    
    /* Apenas aceitar Aqualins com saúde entre 1 e 99 */
    if (saude <= SAUDE_MINIMA || saude >= SAUDE_MAXIMA) {
        return;
    }
    
    /* Alocar novo Aqualin */
    Aqualin *novo = malloc(sizeof(Aqualin));
    if (novo == NULL) {
        return;
    }
    
    strcpy(novo->nome, nome);
    novo->saude_inicial = saude;
    novo->saude_atual = saude;
    novo->instante_entrada = instante;
    novo->instante_saida = -1;
    novo->tempo_espera = 0;
    novo->estado = 'E'; /* Em espera */
    novo->camara_usada = NULL;
    novo->proximo = NULL;
    
    /* Adicionar ao fim da lista */
    if (sys->aqualins == NULL) {
        sys->aqualins = novo;
    } else {
        Aqualin *aq = sys->aqualins;
        while (aq->proximo != NULL) {
            aq = aq->proximo;
        }
        aq->proximo = novo;
    }
    
    sys->num_aqualins++;
    sys->ultimo_instante = instante;
}

/* ============================================================================
   PARTE 3 - TRATAMENTOS
   ============================================================================ */

/* Calcular tempo necessário para recuperar 1 ponto de saúde */
long calcular_tempo_tratamento(int saude) {
    if (saude > SAUDE_LIMITE_ALTO) {
        return 1;                                              /* +1 por tic */
    } else if (saude > SAUDE_LIMITE_MEDIO) {
        return 10;                                             /* +1 por 10 tics */
    } else if (saude > SAUDE_LIMITE_BAIXO) {
        return TICS_POR_SOL;                                   /* +1 por sol */
    } else if (saude > SAUDE_MINIMA) {
        return TICS_POR_SOL * SOLS_POR_TRATAMENTO_LENTO;       /* +1 por 10 sols */
    }
    return 0;
}

/* Calcular degradação de saúde durante a espera */
int calcular_degradacao_espera(int saude, long tempo_espera) {
    int saude_final = saude;
    long tempo_restante = tempo_espera;
    
    while (tempo_restante > 0 && saude_final > SAUDE_MINIMA) {
        long intervalo;
        
        if (saude_final > SAUDE_LIMITE_ALTO) {
            intervalo = TICS_POR_SOL * SOLS_POR_TRATAMENTO_LENTO;  /* -1 por 10 sols */
        } else if (saude_final > SAUDE_LIMITE_MEDIO) {
            intervalo = TICS_POR_SOL;                              /* -1 por sol */
        } else if (saude_final > SAUDE_LIMITE_BAIXO) {
            intervalo = 10;                                        /* -1 por 10 tics */
        } else {
            intervalo = 1;                                         /* -1 por tic */
        }
        
        if (tempo_restante >= intervalo) {
            saude_final--;
            tempo_restante -= intervalo;
        } else {
            break;
        }
    }
    
    return saude_final;
}

/* Adicionar registo de tratamento */
void adicionar_tratamento(Sistema *sys, const char *aqualin, const char *camara, 
                          int saude, long entrada, long saida) {
    Tratamento *novo = malloc(sizeof(Tratamento));
    if (novo == NULL) {
        return;
    }
    
    strcpy(novo->aqualin, aqualin);
    strcpy(novo->camara, camara);
    novo->saude = saude;
    novo->instante_entrada = entrada;
    novo->instante_saida = saida;
    novo->proximo = NULL;
    
    /* Adicionar ao fim */
    if (sys->tratamentos == NULL) {
        sys->tratamentos = novo;
    } else {
        Tratamento *tr = sys->tratamentos;
        while (tr->proximo != NULL) {
            tr = tr->proximo;
        }
        tr->proximo = novo;
    }
}

/* Resetar tratamentos para permitir reprocessamento */
void resetar_tratamentos(Sistema *sys) {
    /* Limpar lista de tratamentos */
    Tratamento *tr = sys->tratamentos;
    while (tr != NULL) {
        Tratamento *temp = tr;
        tr = tr->proximo;
        free(temp);
    }
    sys->tratamentos = NULL;
    
    /* Resetar estado de todos os Aqualins */
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        aq->estado = 'E'; /* Voltar a espera */
        aq->saude_atual = aq->saude_inicial;
        aq->tempo_espera = 0;
        aq->instante_saida = -1;
        if (aq->camara_usada != NULL) {
            free(aq->camara_usada);
            aq->camara_usada = NULL;
        }
        aq = aq->proximo;
    }
    
    sys->tratamentos_executados = 0;
}

/* Processar todos os tratamentos (algoritmo de escalonamento) */
void processar_tratamentos(Sistema *sys) {
    size_t i;
    
    if (sys->num_camaras == 0 || sys->num_aqualins == 0) {
        return;
    }
    
    /* Criar array de disponibilidade de câmaras */
    long *camara_disponivel = calloc(sys->num_camaras, sizeof(long));
    if (camara_disponivel == NULL) {
        return;
    }
    
    /* Criar array de ponteiros para câmaras (para acesso por índice) */
    Camara **array_camaras = malloc(sys->num_camaras * sizeof(Camara *));
    if (array_camaras == NULL) {
        free(camara_disponivel);
        return;
    }
    
    i = 0;
    Camara *cam = sys->camaras;
    while (cam != NULL) {
        array_camaras[i++] = cam;
        cam = cam->proxima;
    }
    
    /* Processar Aqualins em ordem FIFO */
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        if (aq->estado == 'E') { /* Em espera */
            /* Encontrar câmara disponível mais cedo */
            size_t idx_camara = 0;
            long menor_tempo = camara_disponivel[0];
            
            for (i = 1; i < sys->num_camaras; i++) {
                if (camara_disponivel[i] < menor_tempo) {
                    menor_tempo = camara_disponivel[i];
                    idx_camara = i;
                }
            }
            
            /* Determinar quando o tratamento começa */
            long inicio_tratamento = (aq->instante_entrada > menor_tempo) ? 
                                     aq->instante_entrada : menor_tempo;
            
            /* Calcular tempo de espera */
            long tempo_espera = inicio_tratamento - aq->instante_entrada;
            aq->tempo_espera = tempo_espera;
            
            /* Calcular saúde após espera */
            int saude_apos_espera = calcular_degradacao_espera(aq->saude_inicial, tempo_espera);
            aq->saude_atual = saude_apos_espera;
            
            /* Verificar se morreu durante a espera */
            if (saude_apos_espera <= SAUDE_MINIMA) {
                aq->estado = 'M';
                aq->instante_saida = inicio_tratamento;
                
                /* Registar "tratamento" da morte (entrada = saída na câmara) */
                aq->camara_usada = malloc(MAX_NOME * sizeof(char));
                if (aq->camara_usada != NULL) {
                    strcpy(aq->camara_usada, array_camaras[idx_camara]->nome);
                }
                adicionar_tratamento(sys, aq->nome, array_camaras[idx_camara]->nome,
                                    aq->saude_inicial, inicio_tratamento, inicio_tratamento);
                
                /* Atualizar disponibilidade da câmara (sem tempo de tratamento) */
                camara_disponivel[idx_camara] = inicio_tratamento;
                
                aq = aq->proximo;
                continue;
            }
            
            /* Calcular tempo de tratamento até 100 pontos */
            long tempo_total_tratamento = 0;
            int saude_temp = saude_apos_espera;
            
            while (saude_temp < SAUDE_MAXIMA) {
                long tempo_por_ponto = calcular_tempo_tratamento(saude_temp);
                tempo_total_tratamento += tempo_por_ponto;
                saude_temp++;
            }
            
            /* Atualizar estado do Aqualin */
            aq->estado = 'A'; /* Alta */
            aq->instante_saida = inicio_tratamento + tempo_total_tratamento;
            aq->saude_atual = SAUDE_MAXIMA;
            
            /* Guardar nome da câmara */
            aq->camara_usada = malloc(MAX_NOME * sizeof(char));
            if (aq->camara_usada != NULL) {
                strcpy(aq->camara_usada, array_camaras[idx_camara]->nome);
            }
            
            /* Registar tratamento com saúde INICIAL do Aqualin */
            adicionar_tratamento(sys, aq->nome, array_camaras[idx_camara]->nome,
                                aq->saude_inicial, inicio_tratamento, aq->instante_saida);
            
            /* Atualizar disponibilidade da câmara */
            camara_disponivel[idx_camara] = aq->instante_saida;
        }
        
        aq = aq->proximo;
    }
    
    free(camara_disponivel);
    free(array_camaras);
    sys->tratamentos_executados = 1;
}

void relatorio_altas(Sistema *sys) {
    /* Sempre resetar e reprocessar tratamentos */
    resetar_tratamentos(sys);
    processar_tratamentos(sys);
    
    /* Encontrar primeira e última alta */
    Aqualin *primeira_alta = NULL;
    Aqualin *ultima_alta = NULL;
    long menor_instante_alta = -1;
    long maior_instante_alta = -1;
    
    int num_mortes = 0;
    int menor_saude_morte = SAUDE_MAXIMA + 1;
    int maior_saude_morte = -1;
    
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        if (aq->estado == 'A') {
            if (menor_instante_alta == -1 || aq->instante_saida < menor_instante_alta) {
                menor_instante_alta = aq->instante_saida;
                primeira_alta = aq;
            }
            if (maior_instante_alta == -1 || aq->instante_saida > maior_instante_alta) {
                maior_instante_alta = aq->instante_saida;
                ultima_alta = aq;
            }
        } else if (aq->estado == 'M') {
            num_mortes++;
            if (aq->saude_inicial < menor_saude_morte) {
                menor_saude_morte = aq->saude_inicial;
            }
            if (aq->saude_inicial > maior_saude_morte) {
                maior_saude_morte = aq->saude_inicial;
            }
        }
        aq = aq->proximo;
    }
    
    printf("Altas:\n");
    if (primeira_alta != NULL) {
        printf("- primeira: %s %d %ld %ld %ld\n", 
               primeira_alta->nome, primeira_alta->saude_inicial, 
               primeira_alta->instante_entrada, primeira_alta->tempo_espera,
               primeira_alta->instante_saida);
    }
    if (ultima_alta != NULL) {
        printf("- ultima: %s %d %ld %ld %ld\n", 
               ultima_alta->nome, ultima_alta->saude_inicial, 
               ultima_alta->instante_entrada, ultima_alta->tempo_espera,
               ultima_alta->instante_saida);
    }
    
    if (num_mortes > 0) {
        printf("Mortes: %d %d %d\n", num_mortes, menor_saude_morte, maior_saude_morte);
    }
}

/* ============================================================================
   PARTE 4 - RELATÓRIOS E INDICADORES
   ============================================================================ */

void relatorio_aqualins(Sistema *sys) {
    /* Relatório só funciona após tratamentos ter sido executado */
    if (!sys->tratamentos_executados) {
        return;
    }
    
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        printf("%s %d %ld %ld ", aq->nome, aq->saude_inicial, 
               aq->instante_entrada, aq->tempo_espera);
        
        if (aq->estado == 'A') {
            printf("alta:%ld\n", aq->instante_saida);
        } else if (aq->estado == 'M') {
            printf("morte:%ld\n", aq->instante_saida);
        } else {
            printf("espera:0\n");
        }
        
        aq = aq->proximo;
    }
}

void relatorio_camaras(Sistema *sys) {
    Camara *cam = sys->camaras;
    while (cam != NULL) {
        printf("%s:\n", cam->nome);
        
        /* Só mostrar tratamentos se já foram processados */
        if (sys->tratamentos_executados) {
            Tratamento *tr = sys->tratamentos;
            while (tr != NULL) {
                if (strcmp(tr->camara, cam->nome) == 0) {
                    printf("- %s %d %ld %ld\n", tr->aqualin, tr->saude,
                           tr->instante_entrada, tr->instante_saida);
                }
                tr = tr->proximo;
            }
        }
        
        cam = cam->proxima;
    }
}

void indicador_espera(Sistema *sys) {
    if (!sys->tratamentos_executados) {
        printf("0\n");
        return;
    }
    
    long soma_espera = 0;
    size_t contador = 0;
    
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        if (aq->estado == 'A' || aq->estado == 'M') {
            soma_espera += aq->tempo_espera;
            contador++;
        }
        aq = aq->proximo;
    }
    
    if (contador == 0) {
        printf("0\n");
    } else {
        printf("%ld\n", soma_espera / (long)contador);
    }
}

void indicador_trabalho(Sistema *sys) {
    if (!sys->tratamentos_executados) {
        printf("0\n");
        return;
    }
    
    long total_trabalho = 0;
    
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        if (aq->estado == 'A' || aq->estado == 'M') {
            total_trabalho += (aq->instante_saida - aq->instante_entrada);
        }
        aq = aq->proximo;
    }
    
    printf("%ld\n", total_trabalho);
}

void indicador_salvos(Sistema *sys) {
    if (!sys->tratamentos_executados) {
        printf("0\n");
        return;
    }
    
    int salvos = 0;
    
    Aqualin *aq = sys->aqualins;
    while (aq != NULL) {
        if (aq->estado == 'A') {
            salvos++;
}   
        aq = aq->proximo;
    }
    
    printf("%d\n", salvos);
}

/* ============================================================================
   FUNÇÃO PRINCIPAL
   ============================================================================ */

int main(void) {
    Sistema sistema;
    inicializar_sistema(&sistema);
    
    char linha[MAX_LINHA];
    
    while (fgets(linha, sizeof(linha), stdin) != NULL) {
        /* Remover newline */
        linha[strcspn(linha, "\n")] = '\0';
        
        /* Validar tamanho da linha (máximo 100 caracteres conforme enunciado) */
        if (strlen(linha) > SAUDE_MAXIMA) {
            /* Linha muito longa - encerrar ou ignorar conforme especificação */
            /* Neste caso, como não está claro, continuamos a processar */
        }
        
        /* Verificar se começa com # */
        if (linha[0] != '#') {
            break;
        }
        
        /* Verificar se tem pelo menos um espaço após # */
        if (linha[1] == '\0') {
            break;
        }
        
        /* Parse do comando */
        char cmd1[MAX_NOME] = "", cmd2[MAX_NOME] = "", cmd3[MAX_NOME] = "";
        int tokens = sscanf(linha, "# %100s %100s %100s", cmd1, cmd2, cmd3);
        
        if (tokens == 0) {
            break;
        }
        
        /* Processar comandos */
        if (tokens == 1) {
            /* Apenas uma palavra após # - adicionar câmara */
            if (validar_nome(cmd1)) {
                adicionar_camara(&sistema, cmd1);
            }
        } else if (tokens == 2) {
            if (strcmp(cmd1, "relatorio") == 0 && strcmp(cmd2, "global") == 0) {
                relatorio_global(&sistema);
            } else if (strcmp(cmd1, "relatorio") == 0 && strcmp(cmd2, "aqualins") == 0) {
                relatorio_aqualins(&sistema);
            } else if (strcmp(cmd1, "relatorio") == 0 && strcmp(cmd2, "camaras") == 0) {
                relatorio_camaras(&sistema);
            } else if (strcmp(cmd1, "indicador") == 0 && strcmp(cmd2, "espera") == 0) {
                indicador_espera(&sistema);
            } else if (strcmp(cmd1, "indicador") == 0 && strcmp(cmd2, "trabalho") == 0) {
                indicador_trabalho(&sistema);
            } else if (strcmp(cmd1, "indicador") == 0 && strcmp(cmd2, "salvos") == 0) {
                indicador_salvos(&sistema);
            }
            /* Comando inválido - ignorar */
        } else if (tokens == 3) {
            if (strcmp(cmd1, "tratamentos") == 0) {
                /* Comando tratamentos pode ter palavras extras - ignorar */
            } else {
                /* Adicionar Aqualin: # nome saude instante */
                int saude = atoi(cmd2);
                long instante = atol(cmd3);
                adicionar_aqualin(&sistema, cmd1, saude, instante);
            }
        }
        
        /* Comando especial "tratamentos" */
        if (strcmp(cmd1, "tratamentos") == 0) {
            relatorio_altas(&sistema);
        }
    }
    
    libertar_sistema(&sistema);
    return 0;
}