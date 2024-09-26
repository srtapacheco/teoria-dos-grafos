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

// Função para adicionar aresta na lista de adjacência
void adiciona_aresta_lista(GrafoLista* grafo, int v1, int v2) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = v2;
    novo_no->proximo = grafo->lista_adjacencia[v1];
    grafo->lista_adjacencia[v1] = novo_no;

    // Para grafos não direcionados
    novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = v1;
    novo_no->proximo = grafo->lista_adjacencia[v2];
    grafo->lista_adjacencia[v2] = novo_no;
}

// Função para liberar a memória da lista de adjacência
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

// Função para realizar BFS na matriz de adjacência
void bfs_matriz_adjacencia(int **matriz_adjacencia, int num_vertices, int vertice_inicial) {
    FILE *arquivo_saida = fopen("saida_bfs_matriz.txt", "w");

    bool *visitado = (bool*)calloc(num_vertices, sizeof(bool));
    int *fila = (int*)malloc(num_vertices * sizeof(int));
    int *nivel = (int*)calloc(num_vertices, sizeof(int));
    int *pai = (int*)malloc(num_vertices * sizeof(int));
    for (int i = 0; i < num_vertices; i++) pai[i] = -1;

    int inicio = 0, fim = 0;

    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;
    nivel[vertice_inicial] = 0;

    while (inicio < fim) {
        int vertice_atual = fila[inicio++];
        for (int i = 0; i < num_vertices; i++) {
            if (matriz_adjacencia[vertice_atual][i] == 1 && !visitado[i]) {
                visitado[i] = true;
                fila[fim++] = i;
                pai[i] = vertice_atual;
                nivel[i] = nivel[vertice_atual] + 1;
            }
        }
    }

    // Salva no arquivo
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i] + 1, nivel[i]);
    }

    free(visitado);
    free(fila);
    free(nivel);
    free(pai);
    fclose(arquivo_saida);
}

// Função para realizar BFS na lista de adjacência
void bfs_lista_adjacencia(GrafoLista *grafo, int vertice_inicial) {
    FILE *arquivo_saida = fopen("saida_bfs_lista.txt", "w");

    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *fila = (int*)malloc(grafo->num_vertices * sizeof(int));
    int *nivel = (int*)calloc(grafo->num_vertices, sizeof(int));
    int *pai = (int*)malloc(grafo->num_vertices * sizeof(int));
    for (int i = 0; i < grafo->num_vertices; i++) pai[i] = -1;

    int inicio = 0, fim = 0;

    visitado[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;
    nivel[vertice_inicial] = 0;

    while (inicio < fim) {
        int vertice_atual = fila[inicio++];
        No* adjacente = grafo->lista_adjacencia[vertice_atual];
        while (adjacente != NULL) {
            if (!visitado[adjacente->vertice]) {
                visitado[adjacente->vertice] = true;
                if (fim < grafo->num_vertices) { // Verifique se ainda há espaço na fila
                    fila[fim++] = adjacente->vertice;
                }
                pai[adjacente->vertice] = vertice_atual;
                nivel[adjacente->vertice] = nivel[vertice_atual] + 1;
            }
            adjacente = adjacente->proximo;
        }
    }

    // Salva no arquivo
    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i] != -1 ? pai[i] + 1 : 0, nivel[i]);
    }

    free(visitado);
    free(fila);
    free(nivel);
    free(pai);
    fclose(arquivo_saida);
}

// Função para realizar DFS na matriz de adjacência
void dfs_matriz_adjacencia(int **matriz_adjacencia, int num_vertices, int vertice_inicial, int *pai, int *nivel, bool *visitado, int nivel_atual) {
    visitado[vertice_inicial] = true;
    nivel[vertice_inicial] = nivel_atual;

    for (int i = 0; i < num_vertices; i++) {
        if (matriz_adjacencia[vertice_inicial][i] == 1 && !visitado[i]) {
            pai[i] = vertice_inicial;
            dfs_matriz_adjacencia(matriz_adjacencia, num_vertices, i, pai, nivel, visitado, nivel_atual + 1);
        }
    }
}

// Função para iniciar DFS na matriz de adjacência
void processa_dfs_matriz(int **matriz_adjacencia, int num_vertices, int vertice_inicial) {
    FILE *arquivo_saida = fopen("saida_dfs_matriz.txt", "w");

    bool *visitado = (bool*)calloc(num_vertices, sizeof(bool));
    int *pai = (int*)malloc(num_vertices * sizeof(int));
    int *nivel = (int*)calloc(num_vertices, sizeof(int));

    for (int i = 0; i < num_vertices; i++) pai[i] = -1;

    dfs_matriz_adjacencia(matriz_adjacencia, num_vertices, vertice_inicial, pai, nivel, visitado, 0);

    // Salva no arquivo
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i] + 1, nivel[i]);
    }

    free(visitado);
    free(pai);
    free(nivel);
    fclose(arquivo_saida);
}

// Função para realizar DFS na lista de adjacência
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
void processa_dfs_lista(GrafoLista *grafo, int vertice_inicial) {
    FILE *arquivo_saida = fopen("saida_dfs_lista.txt", "w");

    bool *visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int *pai = (int*)malloc(grafo->num_vertices * sizeof(int));
    int *nivel = (int*)calloc(grafo->num_vertices, sizeof(int));

    for (int i = 0; i < grafo->num_vertices; i++) pai[i] = -1;

    dfs_lista_adjacencia(grafo, vertice_inicial, pai, nivel, visitado, 0);

    // Salva no arquivo
    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i] + 1, nivel[i]);
    }

    free(visitado);
    free(pai);
    free(nivel);
    fclose(arquivo_saida);
}

// Função para processar o arquivo e representar o grafo em matriz de adjacência
void processa_matriz_adjacencia(const char *nome_arquivo, int ***matriz_adjacencia, int *num_vertices) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    fscanf(arquivo, "%d", num_vertices);

    // Verifique se o número de vértices é válido
    if (*num_vertices > 100) {
        printf("Número de vértices excede o limite de 100.\n");
        fclose(arquivo);
        return;
    }

    *matriz_adjacencia = (int**)malloc(*num_vertices * sizeof(int*));
    for (int i = 0; i < *num_vertices; i++) {
        (*matriz_adjacencia)[i] = (int*)calloc(*num_vertices, sizeof(int));
    }

    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        if (v1 <= *num_vertices && v2 <= *num_vertices) {
            (*matriz_adjacencia)[v1 - 1][v2 - 1] = 1;
            (*matriz_adjacencia)[v2 - 1][v1 - 1] = 1;
        }
    }

    fclose(arquivo);
}


// Função para processar o arquivo e representar o grafo em lista de adjacência
void processa_lista_adjacencia(const char *nome_arquivo, GrafoLista *grafo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    int v1, v2;
    fscanf(arquivo, "%d", &grafo->num_vertices);
    for (int i = 0; i < grafo->num_vertices; i++) {
        grafo->lista_adjacencia[i] = NULL;
    }

    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        adiciona_aresta_lista(grafo, v1 - 1, v2 - 1);
    }

    fclose(arquivo);
}

// Função principal

int main() {
    char nome_arquivo[100];
    int tipo_representacao, tipo_busca, vertice_inicial;
    const char *arquivo_saida = NULL;

    // Entrada de dados pelo usuário
    printf("Digite o nome do arquivo de entrada (com extensão): ");
    scanf("%s", nome_arquivo);

    printf("Escolha o tipo de representação do grafo:\n");
    printf("1 - Matriz de adjacência\n");
    printf("2 - Lista de adjacência\n");
    printf("Opção: ");
    scanf("%d", &tipo_representacao);

    printf("Escolha o tipo de busca:\n");
    printf("1 - BFS (Busca em Largura)\n");
    printf("2 - DFS (Busca em Profundidade)\n");
    printf("Opção: ");
    scanf("%d", &tipo_busca);

    printf("Digite o vértice inicial (começando em 1): ");
    scanf("%d", &vertice_inicial);
    vertice_inicial--;  // Para ajustar ao índice 0

    if (tipo_representacao == 1) {
        int num_vertices;
        int **matriz_adjacencia;
        processa_matriz_adjacencia(nome_arquivo, &matriz_adjacencia, &num_vertices);

        if (tipo_busca == 1) {
            bfs_matriz_adjacencia(matriz_adjacencia, num_vertices, vertice_inicial);
            arquivo_saida = "saida_bfs_matriz.txt";
        } else if (tipo_busca == 2) {
            processa_dfs_matriz(matriz_adjacencia, num_vertices, vertice_inicial);
            arquivo_saida = "saida_dfs_matriz.txt";
        }

        for (int i = 0; i < num_vertices; i++) {
            free(matriz_adjacencia[i]);
        }
        free(matriz_adjacencia);

    } else if (tipo_representacao == 2) {
        GrafoLista *grafo = cria_lista_adjacencia(100);  // Ajuste de tamanho
        processa_lista_adjacencia(nome_arquivo, grafo);

        if (tipo_busca == 1) {
            bfs_lista_adjacencia(grafo, vertice_inicial);  // Busca em Largura
            arquivo_saida = "saida_bfs_lista.txt";
        } else if (tipo_busca == 2) {
            processa_dfs_lista(grafo, vertice_inicial);    // Busca em Profundidade
            arquivo_saida = "saida_dfs_lista.txt";
        }

        libera_lista_adjacencia(grafo);

    } else {
        printf("Tipo de representação inválido. Use 1 para matriz de adjacência ou 2 para lista de adjacência.\n");
        return 1;
    }

    // Verifica se o arquivo de saída foi definido corretamente e retorna a mensagem de sucesso
    if (arquivo_saida != NULL) {
        printf("Operação concluída com sucesso. O resultado foi registrado no arquivo: %s\n", arquivo_saida);
    } else {
        printf("Ocorreu um erro durante o processamento.\n");
    }

    return 0;
}


