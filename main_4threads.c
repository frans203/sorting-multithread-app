#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 12 // tamanho do array alterado para distribuir igualmente o trabalho entre as threads

int originalList[SIZE] = {55,661,3,29,10,23,4,1,32,190,7,5}; //array original
int tempList[SIZE]; //array intermediário para os merges parciais
int sortedList[SIZE]; //array final com mesmo tamanho da original

//Estrutura usada para passar os indices final e inicial pra as threads de ordenação
typedef struct {
    int start;
    int end;
} ThreadArgs; 

void *sortSublist(void *args) { //precisa do * por exigencia da API de threads do POSIX (pthread_create)
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

//Função de merge adaptada para threads intermediárias (usa tempList como destino)
void *mergeSubsection(void *args) {
    ThreadArgs *range = (ThreadArgs *)args;
    int start = range->start;
    int mid = (start + range->end) / 2;
    int end = range->end;

    int i = start; //apontador para a primeira subparte
    int j = mid;   //apontador para a segunda subparte
    int k = start; //posição no array temporário

    while(i < mid && j < end) {
        if(originalList[i] < originalList[j]) {
            tempList[k++] = originalList[i++];
        } else {
            tempList[k++] = originalList[j++];
        }
    }

    while(i < mid) {
        tempList[k++] = originalList[i++];
    }

    while(j < end) {
        tempList[k++] = originalList[j++];
    }

    pthread_exit(NULL);
}

//Merge final entre as duas metades do tempList para sortedList
void *mergeFinal(void *args){
    int mid = SIZE / 2;
    int i = 0; //apontador para a primeira metade
    int j = mid; //apontador para a segunda metade
    int k = 0; //apontador para o array final

    while(i<mid && j<SIZE) { //combina os elements das duas metades ordenadas 
        if(tempList[i] < tempList[j]) {
            sortedList[k++] = tempList[i++];
        }else {
            sortedList[k++] = tempList[j++];
        }
    }

    while(i<mid) { //Copia o restante da primeira metade, se houver
        sortedList[k++] = tempList[i++];
    }

    while(j<SIZE){//Copia o restante da segunda metade, se houver
        sortedList[k++] = tempList[j++];
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t tid_sort1, tid_sort2, tid_sort3, tid_sort4;
    pthread_t tid_merge1, tid_merge2, tid_merge_final;

    //Marcadores de tempo
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time); // Início da medição

    //Define os indices das sublistas a serem ordenadas (divisão em 4 partes)
    ThreadArgs args_sort1 = {0, SIZE/4};
    ThreadArgs args_sort2 = {SIZE/4, SIZE/2};
    ThreadArgs args_sort3 = {SIZE/2, 3*SIZE/4};
    ThreadArgs args_sort4 = {3*SIZE/4, SIZE};

    //Cria quatro threads para ordenar, cada uma com 1/4 do array original
    pthread_create(&tid_sort1, NULL, sortSublist, &args_sort1);
    pthread_create(&tid_sort2, NULL, sortSublist, &args_sort2);
    pthread_create(&tid_sort3, NULL, sortSublist, &args_sort3);
    pthread_create(&tid_sort4, NULL, sortSublist, &args_sort4);

    //Espera as quatro threads de ordenação terminarem 
    pthread_join(tid_sort1, NULL);
    pthread_join(tid_sort2, NULL);
    pthread_join(tid_sort3, NULL);
    pthread_join(tid_sort4, NULL);

    //Threads de merge parcial (1ª metade e 2ª metade)
    ThreadArgs args_merge1 = {0, SIZE/2};
    ThreadArgs args_merge2 = {SIZE/2, SIZE};

    pthread_create(&tid_merge1, NULL, mergeSubsection, &args_merge1);
    pthread_create(&tid_merge2, NULL, mergeSubsection, &args_merge2);
    pthread_join(tid_merge1, NULL);
    pthread_join(tid_merge2, NULL);

    //Thread de merge final (resultado final no sortedList)
    pthread_create(&tid_merge_final, NULL, mergeFinal, NULL);
    pthread_join(tid_merge_final, NULL);

    clock_gettime(CLOCK_MONOTONIC, &end_time); // Fim da medição

    printf("Lista ordenada: ");
    for(int i=0;i<SIZE;i++){ //mostra a lista ordenada
        printf("%d ", sortedList[i]);
    }
    printf("\n");

    //Cálculo do tempo decorrido em milissegundos
    double elapsed_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                        (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;
    printf("Tempo de execução (4 threads): %.3f ms\n", elapsed_ms);

    return 0 ;
}
