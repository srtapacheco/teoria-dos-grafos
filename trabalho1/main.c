
#include <stdio.h>
#include "biblioteca_grafos.h"

int main() {
    char nome_arquivo[100];
    int vertice_inicial;
    GrafoLista* grafo_lista;
    GrafoMatriz* grafo_matriz;
    FILE* arquivo_saida_lista = fopen("saida_grafo_lista.txt", "w");
    FILE* arquivo_saida_matriz = fopen("saida_grafo_matriz.txt", "w");

    printf("Digite o nome do arquivo de entrada (com extensão): ");
    scanf("%s", nome_arquivo);

    printf("Digite o vértice inicial (começando em 1): ");
    scanf("%d", &vertice_inicial);
    vertice_inicial--; 

    int num_vertices_lista = processa_lista_adjacencia(nome_arquivo, &grafo_lista);
    if (num_vertices_lista == -1) {
        printf("Erro ao processar o arquivo (Lista de Adjacência).\n");
        return 1;
    }

    int num_vertices_matriz = processa_matriz_adjacencia(nome_arquivo, &grafo_matriz);
    if (num_vertices_matriz == -1) {
        printf("Erro ao processar o arquivo (Matriz de Adjacência).\n");
        return 1;
    }

    int grau_min_lista, grau_max_lista, num_arestas_lista;
    double grau_medio_lista, mediana_grau_lista;
    calcula_graus(grafo_lista, &grau_min_lista, &grau_max_lista, &grau_medio_lista, &mediana_grau_lista, &num_arestas_lista);

    fprintf(arquivo_saida_lista, "Número de vértices: %d\n", grafo_lista->num_vertices);
    fprintf(arquivo_saida_lista, "Número de arestas: %d\n", num_arestas_lista);
    fprintf(arquivo_saida_lista, "Grau mínimo: %d\n", grau_min_lista);
    fprintf(arquivo_saida_lista, "Grau máximo: %d\n", grau_max_lista);
    fprintf(arquivo_saida_lista, "Grau médio: %.2f\n", grau_medio_lista);
    fprintf(arquivo_saida_lista, "Mediana de grau: %.2f\n", mediana_grau_lista);

    // Realiza BFS - Lista de Adjacência
    bfs_lista_adjacencia(grafo_lista, vertice_inicial, arquivo_saida_lista);

    // Realiza DFS - Lista de Adjacência
    processa_dfs_lista(grafo_lista, vertice_inicial, arquivo_saida_lista);

    // Calcular graus e número de arestas - Matriz de Adjacência
    int grau_min_matriz, grau_max_matriz, num_arestas_matriz;
    double grau_medio_matriz, mediana_grau_matriz;
    calcula_graus_matriz(grafo_matriz, &grau_min_matriz, &grau_max_matriz, &grau_medio_matriz, &mediana_grau_matriz, &num_arestas_matriz);

    fprintf(arquivo_saida_matriz, "Número de vértices: %d\n", grafo_matriz->num_vertices);
    fprintf(arquivo_saida_matriz, "Número de arestas: %d\n", num_arestas_matriz);
    fprintf(arquivo_saida_matriz, "Grau mínimo: %d\n", grau_min_matriz);
    fprintf(arquivo_saida_matriz, "Grau máximo: %d\n", grau_max_matriz);
    fprintf(arquivo_saida_matriz, "Grau médio: %.2f\n", grau_medio_matriz);
    fprintf(arquivo_saida_matriz, "Mediana de grau: %.2f\n", mediana_grau_matriz);

    // Realiza BFS - Matriz de Adjacência
    bfs_matriz_adjacencia(grafo_matriz, vertice_inicial, arquivo_saida_matriz);

    // Realiza DFS - Matriz de Adjacência
    dfs_matriz_adjacencia(grafo_matriz, vertice_inicial, arquivo_saida_matriz);

    // Libera a memória utilizada
    libera_lista_adjacencia(grafo_lista);
    libera_matriz_adjacencia(grafo_matriz);

    fclose(arquivo_saida_lista);
    fclose(arquivo_saida_matriz);

    printf("Operação concluída com sucesso. Resultados gravados nos arquivos de saída.\n");

    return 0;
}
