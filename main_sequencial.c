#include <stdio.h>
#include <stdlib.h>
#include <time.h> // usado para medir o tempo de execução

#define SIZE 10 // tamanho do array original

int originalList[SIZE] = {55,661,3,29,10,23,4,1,32,190}; //array original
int sortedList[SIZE]; //array final com mesmo tamanho da original

//Função de ordenação bubble sort (reutilizada no lugar das threads)
void bubbleSort(int start, int end) {
    for(int i = start; i < end - 1; i++) {
        for(int j = start; j < end - 1 - (i - start); j++) {
            if(originalList[j] > originalList[j+1]) {
                int temp = originalList[j];
                originalList[j] = originalList[j+1];
                originalList[j+1] = temp;
            }
        }
    }
}

//Função de merge igual à original, mas chamada diretamente sem thread
void mergeSublists() {
    int mid = SIZE / 2;
    int i = 0; //apontador para a primeira metade
    int j = mid; //apontador para a segunda metade
    int k = 0; //apontador para o array final

    while(i < mid && j < SIZE) { //combina os elementos das duas metades ordenadas 
        if(originalList[i] < originalList[j]) {
            sortedList[k++] = originalList[i++];
        } else {
            sortedList[k++] = originalList[j++];
        }
    }

    while(i < mid) { //Copia o restante da primeira metade, se houver
        sortedList[k++] = originalList[i++];
    }

    while(j < SIZE) { //Copia o restante da segunda metade, se houver
        sortedList[k++] = originalList[j++];
    }
}

int main() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); // Início da medição

    //Ordena diretamente as duas metades do array original, sem threads
    bubbleSort(0, SIZE/2);
    bubbleSort(SIZE/2, SIZE);

    //Faz o merge das duas metades ordenadas diretamente
    mergeSublists();

    clock_gettime(CLOCK_MONOTONIC, &end_time); // Fim da medição

    printf("Lista ordenada: ");
    for(int i = 0; i < SIZE; i++) { //mostra a lista ordenada
        printf("%d ", sortedList[i]);
    }
    printf("\n");

    double elapsed_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
    printf("Tempo de execução (sequencial): %.3f ms\n", elapsed_ms);

    return 0;
}
