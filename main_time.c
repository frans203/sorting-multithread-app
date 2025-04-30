#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> 

#define SIZE 10 // tamanho do array original

int originalList[SIZE] = {55,661,3,29,10,23,4,1,32,190}; //array original
int sortedList[SIZE]; //array final com mesmo tamanho da origina

//Estrutura usada para passar os indices final e inicial pra as threads de ordenação
typedef struct {
    int start;
    int end;
} ThreadArgs; 

void *sortSublist(void *args) { //precisa do * por exigencia da API de threads do POSIX (pthread_create)
    //basicamente é necessario pois essa função sera usada mais tarde na criação e uso de threads, e nessas funções é necessario que ela retorne um ponteiro
    ThreadArgs *range = (ThreadArgs *)args;
    int start = range->start;
    int end = range->end;

    for(int i = start; i < end-1;i++) {//Ordenação usando bubble sort
        for(int j = start;j<end - 1 - (i-start);j++){
            if(originalList[j] > originalList[j+1]) {
                int temp = originalList[j];
                originalList[j] = originalList[j+1];
                originalList[j+1] = temp;
            }
        }
    }

    pthread_exit(NULL); //Como essa função será usada por uma thread, aqui finalizamos a thread
}

void *mergeSublists(void *args){
    int mid = SIZE/2;
    int i = 0; //apontador para a primeira metade
    int j = mid; //apontador para a segunda metade
    int k = 0; //apontador para o array final

    while(i<mid && j<SIZE) { //combina os elements das duas metades ordenadas 
        if(originalList[i] < originalList[j]) {
            sortedList[k++] = originalList[i++];
        }else {
            sortedList[k++] = originalList[j++];
        }
    }

    while(i<mid) { //Copia o restante da primeira metade, se houver
        sortedList[k++] = originalList[i++];
    }

    while(j<SIZE){//Copia o restante da segunda metade, se houver
        sortedList[k++] = originalList[j++];
    }

    pthread_exit(NULL); // Como essa função vai ser usada por uma thread, damos um threead exit aqui
}

int main() {
    //declaração de 3 identificadores de thread
    pthread_t tid_sort1, tid_sort2, tid_merge;

    //Marcadores de tempo
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); // <-- Início da medição

    //Define os indices das sublistas a serem ordenadas
    ThreadArgs args1 = {0, SIZE/2};
    ThreadArgs args2 = {SIZE/2, SIZE};

    //Cria duas threads para ordenar, cada uma com metade do array original
    pthread_create(&tid_sort1, NULL, sortSublist, &args1);
    pthread_create(&tid_sort2, NULL, sortSublist, &args2);

    //Espera as duas threads de execução terminarem 
    pthread_join(tid_sort1, NULL);
    pthread_join(tid_sort2, NULL);

    //Cria uma thread para fazer o merge das duas sublistas ordenadas 
    pthread_create(&tid_merge, NULL, mergeSublists, NULL);
    pthread_join(tid_merge, NULL); //espera a thread de merge finalziar 

    clock_gettime(CLOCK_MONOTONIC, &end_time); // <-- Fim da medição

    printf("Lista ordenada: ");
    for(int i=0;i<SIZE;i++){ //mostra a lista ordenada
        printf("%d ", sortedList[i]);
    }
    printf("\n");

    //Cálculo do tempo decorrido em milissegundos
    double elapsed_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
    printf("Tempo de execução: %.3f ms\n", elapsed_ms);

    return 0 ;
}
