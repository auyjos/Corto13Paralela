#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define LIST_SIZE 10000

// Estructura para pasar los parámetros a los hilos
typedef struct {
    int *sublist;
    int length;
    int result;
} ThreadData;

// Función que ejecutará cada hilo: suma de los elementos de la sublista
void* sum_sublist(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int sum = 0;
    for (int i = 0; i < data->length; i++) {
        sum += data->sublist[i];
    }
    data->result = sum; // Guardar el resultado en la estructura
    pthread_exit(NULL);
}

// Función principal que usa Fork-Join para dividir el trabajo
int sum_fork_join(int* list, int list_size) {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    int sublist_size = list_size / NUM_THREADS;
    int total_sum = 0;

    // Crear los hilos para procesar cada sublista
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].sublist = &list[i * sublist_size];
        thread_data[i].length = sublist_size;
        pthread_create(&threads[i], NULL, sum_sublist, (void*)&thread_data[i]);
    }

    // Esperar a que los hilos terminen y sumar los resultados parciales
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].result;
    }

    return total_sum;
}

int main() {
    int list[LIST_SIZE];
    
    // Inicializar la lista con valores del 1 al LIST_SIZE
    for (int i = 0; i < LIST_SIZE; i++) {
        list[i] = i + 1;
    }

    // Calcular la suma usando Fork-Join
    int result = sum_fork_join(list, LIST_SIZE);
    printf("La suma de la lista es: %d\n", result);

    return 0;
}
