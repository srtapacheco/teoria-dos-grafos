// main.c (Apenas Lista de Adjacência)
#include <stdio.h>
#include "biblioteca_grafos.h"

int main() {
    GrafoLista* grafo_lista;

    // Processa o grafo para lista de adjacência
    processa_lista_adjacencia("grafo_6.txt", &grafo_lista);
    printf("Lista de adjacência carregada.\n");

    // Aguarde para verificar o uso de memória
    printf("Aperte Enter para continuar e liberar a memória...\n");
    getchar();

    // Libera a memória alocada
    libera_lista_adjacencia(grafo_lista);

    return 0;
}
