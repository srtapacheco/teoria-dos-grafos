// biblioteca_grafos.c
#include <stdio.h>
#include <stdlib.h>
#include "biblioteca_grafos.h"
#include <stdbool.h>

// Implementação das funções

// Função para criar a lista de adjacência
GrafoLista* cria_lista_adjacencia(int num_vertices) {
    GrafoLista* grafo = (GrafoLista*)malloc(sizeof(GrafoLista));
    grafo->num_vertices = num_vertices;
    grafo->lista_adjacencia = (No**)malloc(num_vertices * sizeof(No*));

    for (int i = 0; i < num_vertices; i++) {
        grafo->lista_adjacencia[i] = NULL;
    }
    return grafo;
}

// Função para criar a matriz de adjacência
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

// Função para calcular o grau de um grafo em matriz de adjacência
void calcula_graus_matriz(GrafoMatriz* grafo, int* grau_min, int* grau_max, double* grau_medio, double* mediana_grau, int* num_arestas) {
    int* graus = (int*)calloc(grafo->num_vertices, sizeof(int));
    int total_graus = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        for (int j = 0; j < grafo->num_vertices; j++) {
            if (grafo->matriz_adjacencia[i][j] == 1) {
                graus[i]++;
            }
        }
        total_graus += graus[i];
    }

    *num_arestas = total_graus / 2;
    *grau_min = graus[0];
    *grau_max = graus[0];
    for (int i = 0; i < grafo->num_vertices; i++) {
        if (graus[i] < *grau_min) *grau_min = graus[i];
        if (graus[i] > *grau_max) *grau_max = graus[i];
    }

    *grau_medio = (double)total_graus / grafo->num_vertices;

    // Calcular a mediana
    for (int i = 0; i < grafo->num_vertices - 1; i++) {
        for (int j = i + 1; j < grafo->num_vertices; j++) {
            if (graus[i] > graus[j]) {
                int temp = graus[i];
                graus[i] = graus[j];
                graus[j] = temp;
            }
        }
    }

    if (grafo->num_vertices % 2 == 0) {
        *mediana_grau = (graus[grafo->num_vertices / 2 - 1] + graus[grafo->num_vertices / 2]) / 2.0;
    } else {
        *mediana_grau = graus[grafo->num_vertices / 2];
    }

    free(graus);
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

// Função para realizar DFS em matriz de adjacência
void dfs_matriz_adjacencia_aux(GrafoMatriz* grafo, int vertice, bool* visitado) {
    visitado[vertice] = true;

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (grafo->matriz_adjacencia[vertice][i] == 1 && !visitado[i]) {
            dfs_matriz_adjacencia_aux(grafo, i, visitado);
        }
    }
}

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

// Função para calcular o grau do grafo em lista de adjacência
void calcula_graus(GrafoLista* grafo, int* grau_min, int* grau_max, double* grau_medio, double* mediana_grau, int* num_arestas) {
    int* graus = (int*)calloc(grafo->num_vertices, sizeof(int));
    int total_graus = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        No* adjacente = grafo->lista_adjacencia[i];
        while (adjacente != NULL) {
            graus[i]++;
            adjacente = adjacente->proximo;
        }
        total_graus += graus[i];
    }

    *num_arestas = total_graus / 2;
    *grau_min = graus[0];
    *grau_max = graus[0];
    for (int i = 0; i < grafo->num_vertices; i++) {
        if (graus[i] < *grau_min) *grau_min = graus[i];
        if (graus[i] > *grau_max) *grau_max = graus[i];
    }

    *grau_medio = (double)total_graus / grafo->num_vertices;

    // Calcular a mediana
    for (int i = 0; i < grafo->num_vertices - 1; i++) {
        for (int j = i + 1; j < grafo->num_vertices; j++) {
            if (graus[i] > graus[j]) {
                int temp = graus[i];
                graus[i] = graus[j];
                graus[j] = temp;
            }
        }
    }

    if (grafo->num_vertices % 2 == 0) {
        *mediana_grau = (graus[grafo->num_vertices / 2 - 1] + graus[grafo->num_vertices / 2]) / 2.0;
    } else {
        *mediana_grau = graus[grafo->num_vertices / 2];
    }

    free(graus);
}

// Função para realizar BFS em lista de adjacência
void bfs_lista_adjacencia(GrafoLista *grafo, int vertice_inicial, FILE *arquivo_saida) {
    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *fila = (int*)malloc(grafo->num_vertices * sizeof(int));
    int *nivel = (int*)calloc(grafo->num_vertices, sizeof(int));
    int *pai = (int*)malloc(grafo->num_vertices * sizeof(int));
    for (int i = 0; i < grafo->num_vertices; i++) pai[i] = -1;

    int inicio = 0, fim = 0;

    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;
    nivel[vertice_inicial] = 0;

    fprintf(arquivo_saida, "\nBusca em Largura (BFS) - Lista de Adjacência:\n");
    while (inicio < fim) {
        int vertice_atual = fila[inicio++];
        No* adjacente = grafo->lista_adjacencia[vertice_atual];
        while (adjacente != NULL) {
            if (!visitado[adjacente->vertice]) {
                visitado[adjacente->vertice] = true;
                fila[fim++] = adjacente->vertice;
                pai[adjacente->vertice] = vertice_atual;
                nivel[adjacente->vertice] = nivel[vertice_atual] + 1;
            }
            adjacente = adjacente->proximo;
        }
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i] != -1 ? pai[i] + 1 : 0, nivel[i]);
    }

    free(visitado);
    free(fila);
    free(nivel);
    free(pai);
}

// Função para realizar DFS em lista de adjacência
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

// Função para iniciar DFS na lista de adjacência
void processa_dfs_lista(GrafoLista *grafo, int vertice_inicial, FILE *arquivo_saida) {
    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *pai = (int*)malloc(grafo->num_vertices * sizeof(int));
    int *nivel = (int*)calloc(grafo->num_vertices, sizeof(int));

    for (int i = 0; i < grafo->num_vertices; i++) pai[i] = -1;

    dfs_lista_adjacencia(grafo, vertice_inicial, pai, nivel, visitado, 0);

    fprintf(arquivo_saida, "\nBusca em Profundidade (DFS) - Lista de Adjacência:\n");
    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i] + 1, nivel[i]);
    }

    free(visitado);
    free(pai);
    free(nivel);
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

// Função para calcular a maior distância a partir de um vértice (BFS auxiliar)
int bfs_calcula_distancia(GrafoLista *grafo, int vertice_inicial) {
    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *fila = (int*)malloc(grafo->num_vertices * sizeof(int));
    int *nivel = (int*)calloc(grafo->num_vertices, sizeof(int));

    int inicio = 0, fim = 0;
    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;
    nivel[vertice_inicial] = 0;

    int max_nivel = 0;
    while (inicio < fim) {
        int vertice_atual = fila[inicio++];
        No* adjacente = grafo->lista_adjacencia[vertice_atual];
        while (adjacente != NULL) {
            if (!visitado[adjacente->vertice]) {
                visitado[adjacente->vertice] = true;
                fila[fim++] = adjacente->vertice;
                nivel[adjacente->vertice] = nivel[vertice_atual] + 1;
                if (nivel[adjacente->vertice] > max_nivel) {
                    max_nivel = nivel[adjacente->vertice];
                }
            }
            adjacente = adjacente->proximo;
        }
    }

    free(visitado);
    free(fila);
    free(nivel);
    return max_nivel;
}

// Função para descobrir os componentes conexos do grafo
// Função para descobrir os componentes conexos do grafo
void encontra_componentes_conexos(GrafoLista* grafo, int* num_componentes, int** tamanhos, int*** listas_vertices) {
    // Aloca memória para armazenar se um vértice já foi visitado
    bool* visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    if (visitado == NULL) {
        fprintf(stderr, "Erro ao alocar memória para 'visitado'.\n");
        exit(EXIT_FAILURE);
    }

    // Aloca memória para armazenar os componentes
    int* componentes = (int*)malloc(grafo->num_vertices * sizeof(int));
    if (componentes == NULL) {
        fprintf(stderr, "Erro ao alocar memória para 'componentes'.\n");
        free(visitado);
        exit(EXIT_FAILURE);
    }

    *num_componentes = 0;

    // Aloca memória para armazenar o tamanho de cada componente
    *tamanhos = (int*)malloc(grafo->num_vertices * sizeof(int));
    if (*tamanhos == NULL) {
        fprintf(stderr, "Erro ao alocar memória para 'tamanhos'.\n");
        free(visitado);
        free(componentes);
        exit(EXIT_FAILURE);
    }

    // Inicializa as variáveis e faz a contagem de componentes
    *listas_vertices = NULL;  // Inicialmente sem memória alocada

    // Exploração dos vértices
    for (int i = 0; i < grafo->num_vertices; i++) {
        if (!visitado[i]) {
            (*num_componentes)++;
            int tamanho = 0;
            int inicio = 0, fim = 0;

            componentes[i] = *num_componentes;

            // Realoca memória para armazenar as listas de vértices por componente
            *listas_vertices = (int**)realloc(*listas_vertices, (*num_componentes) * sizeof(int*));
            if (*listas_vertices == NULL) {
                fprintf(stderr, "Erro ao realocar memória para 'listas_vertices'.\n");
                free(visitado);
                free(componentes);
                free(*tamanhos);
                exit(EXIT_FAILURE);
            }

            (*listas_vertices)[*num_componentes - 1] = NULL;  // Inicializa como NULL

            // Fila para realizar a busca em largura (BFS)
            int* fila = (int*)malloc(grafo->num_vertices * sizeof(int));
            if (fila == NULL) {
                fprintf(stderr, "Erro ao alocar memória para 'fila'.\n");
                free(visitado);
                free(componentes);
                for (int k = 0; k < *num_componentes; k++) {
                    free((*listas_vertices)[k]);
                }
                free(*listas_vertices);
                free(*tamanhos);
                exit(EXIT_FAILURE);
            }

            fila[fim++] = i;
            visitado[i] = true;

            while (inicio < fim) {
                int vertice_atual = fila[inicio++];

                // Realoca memória para adicionar o vértice à componente atual
                (*listas_vertices)[*num_componentes - 1] = (int*)realloc((*listas_vertices)[*num_componentes - 1], (tamanho + 1) * sizeof(int));
                if ((*listas_vertices)[*num_componentes - 1] == NULL) {
                    fprintf(stderr, "Erro ao realocar memória para 'listas_vertices[%d]'.\n", *num_componentes - 1);
                    free(visitado);
                    free(componentes);
                    free(fila);
                    for (int k = 0; k < *num_componentes; k++) {
                        free((*listas_vertices)[k]);
                    }
                    free(*listas_vertices);
                    free(*tamanhos);
                    exit(EXIT_FAILURE);
                }

                (*listas_vertices)[*num_componentes - 1][tamanho++] = vertice_atual + 1;

                No* adjacente = grafo->lista_adjacencia[vertice_atual];
                while (adjacente != NULL) {
                    if (!visitado[adjacente->vertice]) {
                        visitado[adjacente->vertice] = true;
                        fila[fim++] = adjacente->vertice;
                    }
                    adjacente = adjacente->proximo;
                }
            }

            (*tamanhos)[*num_componentes - 1] = tamanho;
            free(fila);
        }
    }

    // Limpeza da memória alocada
    free(visitado);
    free(componentes);
}

// Função para calcular o diâmetro do grafo
int calcula_diametro(GrafoLista* grafo) {
    int diametro = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        int max_dist = bfs_calcula_distancia(grafo, i);
        if (max_dist > diametro) {
            diametro = max_dist;
        }
    }

    return diametro;
}

int bfs_calcula_distancia_entre_vertices(GrafoLista *grafo, int vertice_inicial, int vertice_final) {
    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *fila = (int*)malloc(grafo->num_vertices * sizeof(int));
    int *nivel = (int*)calloc(grafo->num_vertices, sizeof(int));

    int inicio = 0, fim = 0;
    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;
    nivel[vertice_inicial] = 0;

    while (inicio < fim) {
        int vertice_atual = fila[inicio++];

        // Se encontramos o vértice final, retornamos a distância
        if (vertice_atual == vertice_final) {
            int distancia = nivel[vertice_final];
            free(visitado);
            free(fila);
            free(nivel);
            return distancia;
        }

        No* adjacente = grafo->lista_adjacencia[vertice_atual];
        while (adjacente != NULL) {
            if (!visitado[adjacente->vertice]) {
                visitado[adjacente->vertice] = true;
                fila[fim++] = adjacente->vertice;
                nivel[adjacente->vertice] = nivel[vertice_atual] + 1;
            }
            adjacente = adjacente->proximo;
        }
    }

    // Se não encontrar um caminho, retorna -1
    free(visitado);
    free(fila);
    free(nivel);
    return -1;
}

