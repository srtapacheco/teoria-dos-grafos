#include <stdio.h>
#include "biblioteca_grafos.h"
#include <stdlib.h>


int main() {
    GrafoLista* grafo_lista;
    FILE* arquivo_saida = fopen("saida_componentes.txt", "w");

    processa_lista_adjacencia("grafo_6.txt", &grafo_lista);

    int num_componentes;
    int* tamanhos;
    int** listas_vertices;

    encontra_componentes_conexos(grafo_lista, &num_componentes, &tamanhos, &listas_vertices);

    int maior_componente = tamanhos[0];
    int menor_componente = tamanhos[0];
    
    for (int i = 1; i < num_componentes; i++) {
        if (tamanhos[i] > maior_componente) {
            maior_componente = tamanhos[i];
        }
        if (tamanhos[i] < menor_componente) {
            menor_componente = tamanhos[i];
        }
    }

    fprintf(arquivo_saida, "\nComponentes Conexas:\nNúmero de componentes conexas: %d\n", num_componentes);
    fprintf(arquivo_saida, "Tamanho da maior componente: %d\n", maior_componente);
    fprintf(arquivo_saida, "Tamanho da menor componente: %d\n", menor_componente);
    
    for (int i = 0; i < num_componentes; i++) {
        fprintf(arquivo_saida, "Componente %d:\n", i + 1);
        fprintf(arquivo_saida, "  Tamanho: %d\n  Vértices: ", tamanhos[i]);
        for (int j = 0; j < tamanhos[i]; j++) {
            if (j != 0) fprintf(arquivo_saida, ", ");
            fprintf(arquivo_saida, "%d", listas_vertices[i][j]);
        }
        fprintf(arquivo_saida, "\n");
    }

    libera_lista_adjacencia(grafo_lista);
    for (int i = 0; i < num_componentes; i++) {
        free(listas_vertices[i]);
    }
    free(listas_vertices);
    free(tamanhos);

    fclose(arquivo_saida);

    printf("Componentes conexas processadas. Resultados em 'saida_componentes.txt'.\n");

    return 0;
}
