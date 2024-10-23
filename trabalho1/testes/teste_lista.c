// main.c (Apenas Lista de Adjacência)
#include <stdio.h>
#include "biblioteca_grafos.h"

int main() {
    GrafoLista* grafo_lista;

    processa_lista_adjacencia("grafo_6.txt", &grafo_lista);
    printf("Lista de adjacência carregada.\n");
    printf("Aperte Enter para continuar e liberar a memória...\n");
    getchar();
    libera_lista_adjacencia(grafo_lista);

    return 0;
}
