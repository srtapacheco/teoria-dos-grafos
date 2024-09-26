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

// Declaração da função bfs_calcula_distancia
int bfs_calcula_distancia(GrafoLista *grafo, int vertice_inicial);

// Função auxiliar para calcular o grau de cada vértice
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

// Função para realizar BFS e gerar a saída
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

// Função para realizar DFS
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

// Função para iniciar DFS
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
void encontra_componentes_conexos(GrafoLista* grafo, FILE* arquivo_saida) {
    bool* visitado = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int* componentes = (int*)malloc(grafo->num_vertices * sizeof(int));
    int num_componentes = 0;

    int* tamanhos = (int*)malloc(grafo->num_vertices * sizeof(int));
    int** listas_vertices = (int**)malloc(grafo->num_vertices * sizeof(int*));
    for (int i = 0; i < grafo->num_vertices; i++) {
        listas_vertices[i] = (int*)malloc(grafo->num_vertices * sizeof(int));
        tamanhos[i] = 0;
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (!visitado[i]) {
            num_componentes++;
            int tamanho = 0;
            int inicio = 0, fim = 0;
            componentes[i] = num_componentes;

            int* fila = (int*)malloc(grafo->num_vertices * sizeof(int));
            fila[fim++] = i;
            visitado[i] = true;

            while (inicio < fim) {
                int vertice_atual = fila[inicio++];
                listas_vertices[num_componentes - 1][tamanho++] = vertice_atual + 1;

                No* adjacente = grafo->lista_adjacencia[vertice_atual];
                while (adjacente != NULL) {
                    if (!visitado[adjacente->vertice]) {
                        visitado[adjacente->vertice] = true;
                        fila[fim++] = adjacente->vertice;
                    }
                    adjacente = adjacente->proximo;
                }
            }

            tamanhos[num_componentes - 1] = tamanho;
            free(fila);
        }
    }

    // Ordenar os componentes por tamanho (decrescente)
    for (int i = 0; i < num_componentes - 1; i++) {
        for (int j = i + 1; j < num_componentes; j++) {
            if (tamanhos[i] < tamanhos[j]) {
                int temp_tamanho = tamanhos[i];
                tamanhos[i] = tamanhos[j];
                tamanhos[j] = temp_tamanho;

                int* temp_lista = listas_vertices[i];
                listas_vertices[i] = listas_vertices[j];
                listas_vertices[j] = temp_lista;
            }
        }
    }

    fprintf(arquivo_saida, "\nComponentes Conexas:\nNúmero de componentes conexas: %d\n", num_componentes);
    for (int i = 0; i < num_componentes; i++) {
        fprintf(arquivo_saida, "Componente %d:\n", i + 1);
        fprintf(arquivo_saida, "  Tamanho: %d\n  Vértices: ", tamanhos[i]);
        for (int j = 0; j < tamanhos[i]; j++) {
            if (j != 0) fprintf(arquivo_saida, ", ");
            fprintf(arquivo_saida, "%d", listas_vertices[i][j]);
        }
        fprintf(arquivo_saida, "\n");
    }

    free(visitado);
    free(componentes);
    free(tamanhos);
    for (int i = 0; i < grafo->num_vertices; i++) {
        free(listas_vertices[i]);
    }
    free(listas_vertices);
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
    int vertice_inicial;
    FILE* arquivo_saida = fopen("saida_grafo.txt", "w");

    // Entrada de dados pelo usuário
    printf("Digite o nome do arquivo de entrada (com extensão): ");
    scanf("%s", nome_arquivo);

    printf("Digite o vértice inicial (começando em 1): ");
    scanf("%d", &vertice_inicial);
    vertice_inicial--;  // Para ajustar ao índice 0

    GrafoLista *grafo = cria_lista_adjacencia(1000000);  // Ajuste de tamanho
    processa_lista_adjacencia(nome_arquivo, grafo);

    // Calcular graus e número de arestas
    int grau_min, grau_max, num_arestas;
    double grau_medio, mediana_grau;
    calcula_graus(grafo, &grau_min, &grau_max, &grau_medio, &mediana_grau, &num_arestas);

    fprintf(arquivo_saida, "Número de vértices: %d\n", grafo->num_vertices);
    fprintf(arquivo_saida, "Número de arestas: %d\n", num_arestas);
    fprintf(arquivo_saida, "Grau mínimo: %d\n", grau_min);
    fprintf(arquivo_saida, "Grau máximo: %d\n", grau_max);
    fprintf(arquivo_saida, "Grau médio: %.2f\n", grau_medio);
    fprintf(arquivo_saida, "Mediana de grau: %.2f\n", mediana_grau);

    // Realiza BFS
    bfs_lista_adjacencia(grafo, vertice_inicial, arquivo_saida);

    // Realiza DFS
    processa_dfs_lista(grafo, vertice_inicial, arquivo_saida);

    // Calcula o diâmetro
    int diametro = calcula_diametro(grafo);
    fprintf(arquivo_saida, "\nDiâmetro do grafo: %d\n", diametro);

    // Calcula componentes conexas
    encontra_componentes_conexos(grafo, arquivo_saida);

    libera_lista_adjacencia(grafo);
    fclose(arquivo_saida);

    printf("Operação concluída com sucesso. O resultado foi registrado no arquivo: saida_grafo.txt\n");

    return 0;
}
