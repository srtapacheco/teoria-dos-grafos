// main.c (Apenas Matriz de Adjacência)
#include <stdio.h>
#include "biblioteca_grafos.h"

int main() {
    GrafoMatriz* grafo_matriz;
    processa_matriz_adjacencia("grafo_6.txt", &grafo_matriz);
    printf("Matriz de adjacência carregada.\n");
    printf("Aperte Enter para continuar e liberar a memória...\n");
    getchar();
    libera_matriz_adjacencia(grafo_matriz);

    return 0;
}
