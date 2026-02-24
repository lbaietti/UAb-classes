#include <stdio.h>

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

int simularJogo(int num_lancamentos) {
    int i, dado_atual, dado_anterior = 0;
    int pontos = 0;
    int jogo_ativo = 1;  // Flag para controlar se o jogo continua
    
    for (i = 0; i < num_lancamentos && jogo_ativo; i++) {
        dado_atual = (randaux() % 6) + 1;
        printf("%d ", dado_atual);
        pontos += dado_atual;
        
        // Verifica se deve parar
        if (i > 0 && dado_atual == dado_anterior) {
            pontos = -pontos;
            jogo_ativo = 0;  // Marca para parar
        }
        
        dado_anterior = dado_atual;
    }
    
    printf("Pontos: %d\n", pontos);
    
    return pontos;
}

int main() {
    int num_jogos, num_lancamentos, i, total = 0;
    
    scanf("%d", &num_jogos);
    scanf("%d", &num_lancamentos);
    
    for (i = 0; i < num_jogos; i++) {
        total += simularJogo(num_lancamentos);
    }
    
    printf("%d\n", total);
    
    return 0;
}
