#include <stdio.h>
#include <stdlib.h>
#include "biblioteca_grafos.h"

int main() {
    GrafoLista* grafo;
    const char* nome_arquivo = "grafo_6.txt";

    int num_vertices = processa_lista_adjacencia(nome_arquivo, &grafo);
    if (num_vertices == -1) {
        printf("Erro ao processar o grafo.\n");
        return -1;
    }

    int diametro = calcula_diametro(grafo);
    printf("O diâmetro do grafo é: %d\n", diametro);

    libera_lista_adjacencia(grafo);

    return 0;
}
