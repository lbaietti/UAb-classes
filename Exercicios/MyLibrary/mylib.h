#ifndef MYLIB_H
#define MYLIB_H

unsigned int randaux()
{
  static long seed=1;
  return(((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff);
}

//Letra S

/* Função que ordena o vector comparando todos os pares sequencialmente */
void Sort(int vec[], int size) {
    int i, j, temp;
    
    for (i = 0; i < size - 1; i++) {
        for (j = i + 1; j < size; j++) {
            if (vec[i] > vec[j]) {
                temp = vec[i];
                vec[i] = vec[j];
                vec[j] = temp;
            }
        }
    }
}

// Letra P

/* Função que imprime n elementos de um vector de inteiros */
void PrintInts(int vec[], int n, char* end) {
    int i;
    for (i = 0; i < n; i++) {
        printf("%d ", vec[i]);
    }
    printf("%s", end);
}

#endif // MYLIB_H

