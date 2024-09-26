// main.c (Apenas Matriz de Adjacência)
#include <stdio.h>
#include "biblioteca_grafos.h"

int main() {
    GrafoMatriz* grafo_matriz;

    // Processa o grafo para matriz de adjacência
    processa_matriz_adjacencia("grafo_6.txt", &grafo_matriz);
    printf("Matriz de adjacência carregada.\n");

    // Aguarde para verificar o uso de memória
    printf("Aperte Enter para continuar e liberar a memória...\n");
    getchar();

    // Libera a memória alocada
    libera_matriz_adjacencia(grafo_matriz);

    return 0;
}
