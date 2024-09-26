#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura para a lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

typedef struct GrafoLista {
    int num_vertices;
    No** lista_adjacencia;
} GrafoLista;

typedef struct GrafoMatriz {
    int num_vertices;
    int** matriz_adjacencia;
} GrafoMatriz;

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

// Função principal
int main() {
    char nome_arquivo[100];
    int vertice_inicial;
    GrafoLista* grafo_lista;
    GrafoMatriz* grafo_matriz;
    FILE* arquivo_saida_lista = fopen("saida_grafo_lista.txt", "w");
    FILE* arquivo_saida_matriz = fopen("saida_grafo_matriz.txt", "w");

    // Entrada de dados pelo usuário
    printf("Digite o nome do arquivo de entrada (com extensão): ");
    scanf("%s", nome_arquivo);

    printf("Digite o vértice inicial (começando em 1): ");
    scanf("%d", &vertice_inicial);
    vertice_inicial--; // Para ajustar ao índice 0

    // Processar lista de adjacência
    int num_vertices_lista = processa_lista_adjacencia(nome_arquivo, &grafo_lista);
    if (num_vertices_lista == -1) {
        printf("Erro ao processar o arquivo (Lista de Adjacência).\n");
        return 1;
    }

    // Processar matriz de adjacência
    int num_vertices_matriz = processa_matriz_adjacencia(nome_arquivo, &grafo_matriz);
    if (num_vertices_matriz == -1) {
        printf("Erro ao processar o arquivo (Matriz de Adjacência).\n");
        return 1;
    }

    // Calcular graus e número de arestas - Lista de Adjacência
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
