#include <stdio.h>
#include <stdlib.h>
#include "biblioteca_grafos.h"

int main() {
    // Carregar grafo a partir de um arquivo
    GrafoLista* grafo;
    const char* nome_arquivo = "grafo_6.txt"; // Arquivo contendo o grafo

    int num_vertices = processa_lista_adjacencia(nome_arquivo, &grafo);
    if (num_vertices == -1) {
        printf("Erro ao processar o grafo.\n");
        return -1;
    }

    // Calcular o diâmetro do grafo
    int diametro = calcula_diametro(grafo);
    printf("O diâmetro do grafo é: %d\n", diametro);

    // Liberar memória do grafo
    libera_lista_adjacencia(grafo);

    return 0;
}
