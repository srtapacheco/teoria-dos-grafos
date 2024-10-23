#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "biblioteca_grafos.h"

#define NUM_BUSCAS 100

double calcula_tempo_bfs_matriz(GrafoMatriz* grafo, int num_buscas) {
    clock_t inicio, fim;
    double tempo_total = 0.0;
    
    for (int i = 0; i < num_buscas; i++) {
        int vertice_inicial = rand() % grafo->num_vertices;  
        FILE* arquivo_saida = fopen("/dev/null", "w"); 
        inicio = clock();
        bfs_matriz_adjacencia(grafo, vertice_inicial, arquivo_saida);
        fim = clock();
        fclose(arquivo_saida);
        tempo_total += (double)(fim - inicio) / CLOCKS_PER_SEC;
    }
    
    return tempo_total / num_buscas; 
}

double calcula_tempo_bfs_lista(GrafoLista* grafo, int num_buscas) {
    clock_t inicio, fim;
    double tempo_total = 0.0;
    
    for (int i = 0; i < num_buscas; i++) {
        int vertice_inicial = rand() % grafo->num_vertices;  
        FILE* arquivo_saida = fopen("/dev/null", "w"); 
        inicio = clock();
        bfs_lista_adjacencia(grafo, vertice_inicial, arquivo_saida);
        fim = clock();
        fclose(arquivo_saida);
        tempo_total += (double)(fim - inicio) / CLOCKS_PER_SEC;
    }
    
    return tempo_total / num_buscas; 
}

int main() {
    // Carrega o grafo da matriz de adjacência
    GrafoMatriz* grafo_matriz = NULL;
    processa_matriz_adjacencia("grafo_3.txt", &grafo_matriz);

    //Carrega o grafo da lista de adjacência
    GrafoLista* grafo_lista = NULL;
    processa_lista_adjacencia("grafo_6.txt", &grafo_lista);

    // Calcula o tempo médio para a matriz de adjacência
    double tempo_medio_matriz = calcula_tempo_bfs_matriz(grafo_matriz, NUM_BUSCAS);
    printf("Tempo médio BFS (matriz de adjacência): %.6f segundos\n", tempo_medio_matriz);

    // Calcula o tempo médio para a lista de adjacência
    double tempo_medio_lista = calcula_tempo_bfs_lista(grafo_lista, NUM_BUSCAS);
    printf("Tempo médio BFS (lista de adjacência): %.6f segundos\n", tempo_medio_lista);

    // Libera a memória
    libera_matriz_adjacencia(grafo_matriz);
    libera_lista_adjacencia(grafo_lista);

    return 0;
}
