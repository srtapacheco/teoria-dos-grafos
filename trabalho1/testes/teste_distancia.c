#include <stdio.h>
#include "biblioteca_grafos.h"

int main() {
    GrafoLista* grafo_lista;
    
    // Carrega o grafo
    processa_lista_adjacencia("grafo_1.txt", &grafo_lista);

    // Calcula as distâncias entre os pares de vértices
    int distancia_10_20 = bfs_calcula_distancia_entre_vertices(grafo_lista, 9, 19); // Vértices 10 e 20 (índices 9 e 19)
    int distancia_10_30 = bfs_calcula_distancia_entre_vertices(grafo_lista, 9, 29); // Vértices 10 e 30 (índices 9 e 29)
    int distancia_20_30 = bfs_calcula_distancia_entre_vertices(grafo_lista, 19, 29); // Vértices 20 e 30 (índices 19 e 29)

    // Exibe os resultados
    printf("Distância entre 10 e 20: %d\n", distancia_10_20);
    printf("Distância entre 10 e 30: %d\n", distancia_10_30);
    printf("Distância entre 20 e 30: %d\n", distancia_20_30);

    // Libera memória alocada para o grafo
    libera_lista_adjacencia(grafo_lista);

    return 0;
}
