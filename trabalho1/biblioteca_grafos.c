// biblioteca_grafos.c
#include <stdio.h>
#include <stdlib.h>
#include "biblioteca_grafos.h"

// Implementação das funções

// Função para criar uma lista de adjacência
GrafoLista* cria_lista_adjacencia(int num_vertices) {
    GrafoLista* grafo = (GrafoLista*)malloc(sizeof(GrafoLista));
    grafo->num_vertices = num_vertices;
    grafo->lista_adjacencia = (No**)malloc(num_vertices * sizeof(No*));

    for (int i = 0; i < num_vertices; i++) {
        grafo->lista_adjacencia[i] = NULL;
    }
    return grafo;
}

// Função para criar uma matriz de adjacência
GrafoMatriz* cria_matriz_adjacencia(int num_vertices) {
    GrafoMatriz* grafo = (GrafoMatriz*)malloc(sizeof(GrafoMatriz));
    grafo->num_vertices = num_vertices;
    grafo->matriz_adjacencia = (int**)malloc(num_vertices * sizeof(int*));

    for (int i = 0; i < num_vertices; i++) {
        grafo->matriz_adjacencia[i] = (int*)calloc(num_vertices, sizeof(int));
    }
    return grafo;
}

// Função para adicionar aresta na lista de adjacência
void adiciona_aresta_lista(GrafoLista* grafo, int v1, int v2) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = v2;
    novo_no->proximo = grafo->lista_adjacencia[v1];
    grafo->lista_adjacencia[v1] = novo_no;

    novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = v1;
    novo_no->proximo = grafo->lista_adjacencia[v2];
    grafo->lista_adjacencia[v2] = novo_no;
}

// Função para adicionar aresta na matriz de adjacência
void adiciona_aresta_matriz(GrafoMatriz* grafo, int v1, int v2) {
    grafo->matriz_adjacencia[v1][v2] = 1;
    grafo->matriz_adjacencia[v2][v1] = 1; // Grafo não direcionado
}

// Função para liberar memória da lista de adjacência
void libera_lista_adjacencia(GrafoLista* grafo) {
    for (int i = 0; i < grafo->num_vertices; i++) {
        No* no_atual = grafo->lista_adjacencia[i];
        while (no_atual != NULL) {
            No* temp = no_atual;
            no_atual = no_atual->proximo;
            free(temp);
        }
    }
    free(grafo->lista_adjacencia);
    free(grafo);
}

// Função para liberar memória da matriz de adjacência
void libera_matriz_adjacencia(GrafoMatriz* grafo) {
    for (int i = 0; i < grafo->num_vertices; i++) {
        free(grafo->matriz_adjacencia[i]);
    }
    free(grafo->matriz_adjacencia);
    free(grafo);
}

// Função para realizar BFS em matriz de adjacência
void bfs_matriz_adjacencia(GrafoMatriz* grafo, int vertice_inicial, FILE* arquivo_saida) {
    bool* visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int* fila = (int*)malloc(grafo->num_vertices * sizeof(int));
    int inicio = 0, fim = 0;

    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;

    fprintf(arquivo_saida, "\nBusca em Largura (BFS) - Matriz de Adjacência:\n");
    while (inicio < fim) {
        int vertice_atual = fila[inicio++];
        for (int i = 0; i < grafo->num_vertices; i++) {
            if (grafo->matriz_adjacencia[vertice_atual][i] == 1 && !visitado[i]) {
                visitado[i] = true;
                fila[fim++] = i;
            }
        }
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (visitado[i]) {
            fprintf(arquivo_saida, "Vértice: %d foi visitado\n", i + 1);
        }
    }

    free(visitado);
    free(fila);
}

// Função auxiliar recursiva para DFS em matriz de adjacência
void dfs_matriz_adjacencia_aux(GrafoMatriz* grafo, int vertice, bool* visitado) {
    visitado[vertice] = true;

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (grafo->matriz_adjacencia[vertice][i] == 1 && !visitado[i]) {
            dfs_matriz_adjacencia_aux(grafo, i, visitado);
        }
    }
}

// Função para realizar DFS em matriz de adjacência
void dfs_matriz_adjacencia(GrafoMatriz* grafo, int vertice_inicial, FILE* arquivo_saida) {
    bool* visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));

    dfs_matriz_adjacencia_aux(grafo, vertice_inicial, visitado);

    fprintf(arquivo_saida, "\nBusca em Profundidade (DFS) - Matriz de Adjacência:\n");
    for (int i = 0; i < grafo->num_vertices; i++) {
        if (visitado[i]) {
            fprintf(arquivo_saida, "Vértice: %d foi visitado\n", i + 1);
        }
    }

    free(visitado);
}

// Função para processar o arquivo e representar o grafo em matriz de adjacência
int processa_matriz_adjacencia(const char* nome_arquivo, GrafoMatriz** grafo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return -1;
    }

    int num_vertices;
    fscanf(arquivo, "%d", &num_vertices); // Ler o número de vértices do arquivo

    *grafo = cria_matriz_adjacencia(num_vertices); // Aloca dinamicamente a matriz de adjacência

    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        adiciona_aresta_matriz(*grafo, v1 - 1, v2 - 1);
    }

    fclose(arquivo);
    return num_vertices; // Retorna o número de vértices
}

// Função para realizar BFS em lista de adjacência
void bfs_lista_adjacencia(GrafoLista *grafo, int vertice_inicial, FILE *arquivo_saida) {
    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *fila = (int*)malloc(grafo->num_vertices * sizeof(int));
    int inicio = 0, fim = 0;

    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;

    fprintf(arquivo_saida, "\nBusca em Largura (BFS) - Lista de Adjacência:\n");
    while (inicio < fim) {
        int vertice_atual = fila[inicio++];
        No* adjacente = grafo->lista_adjacencia[vertice_atual];
        while (adjacente != NULL) {
            if (!visitado[adjacente->vertice]) {
                visitado[adjacente->vertice] = true;
                fila[fim++] = adjacente->vertice;
            }
            adjacente = adjacente->proximo;
        }
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (visitado[i]) {
            fprintf(arquivo_saida, "Vértice: %d foi visitado\n", i + 1);
        }
    }

    free(visitado);
    free(fila);
}

// Função auxiliar recursiva para DFS em lista de adjacência
void dfs_lista_adjacencia(GrafoLista *grafo, int vertice_inicial, int *pai, int *nivel, bool *visitado, int nivel_atual) {
    visitado[vertice_inicial] = true;
    nivel[vertice_inicial] = nivel_atual;

    No* adjacente = grafo->lista_adjacencia[vertice_inicial];
    while (adjacente != NULL) {
        if (!visitado[adjacente->vertice]) {
            pai[adjacente->vertice] = vertice_inicial;
            dfs_lista_adjacencia(grafo, adjacente->vertice, pai, nivel, visitado, nivel_atual + 1);
        }
        adjacente = adjacente->proximo;
    }
}

// Função para processar o arquivo e representar o grafo em lista de adjacência
int processa_lista_adjacencia(const char *nome_arquivo, GrafoLista **grafo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return -1;
    }

    int num_vertices;
    fscanf(arquivo, "%d", &num_vertices);  // Ler o número de vértices do arquivo

    *grafo = cria_lista_adjacencia(num_vertices);  // Aloca dinamicamente a lista de adjacência

    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        adiciona_aresta_lista(*grafo, v1 - 1, v2 - 1);
    }

    fclose(arquivo);
    return num_vertices;  // Retorna o número de vértices
}
