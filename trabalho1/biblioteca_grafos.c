// Teoria do Grafos - 2024.2 - Universidade Federal do Rio de Janeiro
// Trabalho 1 
// Alunos: Karen Pacheco e Guilherme

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100

typedef struct {
    int tamanho;
    int vertices[MAX_VERTICES];
} Componente;

int compara(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int compara_componentes(const void *a, const void *b) {
    Componente *compA = (Componente *)a;
    Componente *compB = (Componente *)b;
    return compB->tamanho - compA->tamanho;
}

void cria_matriz_adjacencia(FILE *arquivo, int num_vertices, int **matriz) {
    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        matriz[v1 - 1][v2 - 1] = 1;
        matriz[v2 - 1][v1 - 1] = 1;
    }
}

void busca_largura(int **matriz, int num_vertices, int vertice_inicial, FILE *arquivo_saida) {
    int fila[MAX_VERTICES], nivel[MAX_VERTICES], pai[MAX_VERTICES], visitado[MAX_VERTICES];
    int inicio = 0, fim = 0;

    for (int i = 0; i < num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
        pai[i] = -1;
    }

    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;
    pai[vertice_inicial] = 0;

    fprintf(arquivo_saida, "\nBusca em Largura (BFS):\n");
    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < num_vertices; i++) {
            if (matriz[atual][i] && !visitado[i]) {
                fila[fim++] = i;
                visitado[i] = 1;
                nivel[i] = nivel[atual] + 1;
                pai[i] = atual + 1;
            }
        }
    }

    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i], nivel[i]);
    }
}

void busca_profundidade_recursiva(int **matriz, int vertice, int *visitado, int *nivel, int *pai, int profundidade, FILE *arquivo_saida, int num_vertices) {
    visitado[vertice] = 1;
    nivel[vertice] = profundidade;

    for (int i = 0; i < num_vertices; i++) {
        if (matriz[vertice][i] && !visitado[i]) {
            pai[i] = vertice + 1;
            busca_profundidade_recursiva(matriz, i, visitado, nivel, pai, profundidade + 1, arquivo_saida, num_vertices);
        }
    }
}

void busca_profundidade(int **matriz, int num_vertices, int vertice_inicial, FILE *arquivo_saida) {
    int visitado[MAX_VERTICES], nivel[MAX_VERTICES], pai[MAX_VERTICES];

    for (int i = 0; i < num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
        pai[i] = -1;
    }

    pai[vertice_inicial] = 0;
    busca_profundidade_recursiva(matriz, vertice_inicial, visitado, nivel, pai, 0, arquivo_saida, num_vertices);

    fprintf(arquivo_saida, "\nBusca em Profundidade (DFS):\n");
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i], nivel[i]);
    }
}

int bfs_distancia(int **matriz, int num_vertices, int vertice_inicial, int *distancias) {
    int fila[MAX_VERTICES], nivel[MAX_VERTICES], visitado[MAX_VERTICES];
    int inicio = 0, fim = 0, maior_distancia = 0;

    for (int i = 0; i < num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
    }

    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;

    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < num_vertices; i++) {
            if (matriz[atual][i] && !visitado[i]) {
                fila[fim++] = i;
                visitado[i] = 1;
                nivel[i] = nivel[atual] + 1;
                distancias[i] = nivel[i];
                if (nivel[i] > maior_distancia) {
                    maior_distancia = nivel[i];
                }
            }
        }
    }

    return maior_distancia;
}

int calcula_diametro(int **matriz, int num_vertices) {
    int maior_distancia = 0;
    int distancias[MAX_VERTICES];

    for (int i = 0; i < num_vertices; i++) {
        int distancia_atual = bfs_distancia(matriz, num_vertices, i, distancias);
        if (distancia_atual > maior_distancia) {
            maior_distancia = distancia_atual;
        }
    }

    return maior_distancia;
}

int distancia_entre_vertices(int **matriz, int num_vertices, int vertice_inicial, int vertice_final) {
    int distancias[MAX_VERTICES];
    bfs_distancia(matriz, num_vertices, vertice_inicial - 1, distancias);
    return distancias[vertice_final - 1];
}

int encontrar_componentes_conexas(int **matriz, int num_vertices, Componente *componentes) {
    int id_componente[MAX_VERTICES];
    for (int i = 0; i < num_vertices; i++) {
        id_componente[i] = -1;
    }

    int componente_atual = 0;

    for (int v = 0; v < num_vertices; v++) {
        if (id_componente[v] == -1) {
            int fila[MAX_VERTICES];
            int inicio = 0, fim = 0;

            fila[fim++] = v;
            id_componente[v] = componente_atual;
            componentes[componente_atual].tamanho = 0;

            while (inicio < fim) {
                int atual = fila[inicio++];

                componentes[componente_atual].vertices[componentes[componente_atual].tamanho++] = atual + 1;

                for (int i = 0; i < num_vertices; i++) {
                    if (matriz[atual][i] && id_componente[i] == -1) {
                        fila[fim++] = i;
                        id_componente[i] = componente_atual;
                    }
                }
            }

            componente_atual++;
        }
    }

    return componente_atual;
}

void processa_arquivo_grafo(const char *nome_arquivo, const char *representacao, int vertice_inicial, int vertice_final) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    FILE *arquivo_saida = fopen("saida_grafo.txt", "w");
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        fclose(arquivo);
        return;
    }

    int num_vertices, v1, v2;
    fscanf(arquivo, "%d", &num_vertices);
    int graus[num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        graus[i] = 0;
    }

    int num_arestas = 0;

    int **matriz = (int **)malloc(num_vertices * sizeof(int *));
    for (int i = 0; i < num_vertices; i++) {
        matriz[i] = (int *)calloc(num_vertices, sizeof(int));
    }

    cria_matriz_adjacencia(arquivo, num_vertices, matriz);

    rewind(arquivo);
    fscanf(arquivo, "%d", &num_vertices);
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        graus[v1 - 1]++;
        graus[v2 - 1]++;
        num_arestas++;
    }

    int grau_min = graus[0], grau_max = graus[0];
    double soma_graus = 0.0;
    for (int i = 0; i < num_vertices; i++) {
        if (graus[i] < grau_min)
            grau_min = graus[i];
        if (graus[i] > grau_max)
            grau_max = graus[i];
        soma_graus += graus[i];
    }
    double grau_medio = soma_graus / num_vertices;

    qsort(graus, num_vertices, sizeof(int), compara);
    double mediana_grau;
    if (num_vertices % 2 == 0) {
        mediana_grau = (graus[num_vertices / 2 - 1] + graus[num_vertices / 2]) / 2.0;
    } else {
        mediana_grau = graus[num_vertices / 2];
    }

    fprintf(arquivo_saida, "Número de vértices: %d\n", num_vertices);
    fprintf(arquivo_saida, "Número de arestas: %d\n", num_arestas);
    fprintf(arquivo_saida, "Grau mínimo: %d\n", grau_min);
    fprintf(arquivo_saida, "Grau máximo: %d\n", grau_max);
    fprintf(arquivo_saida, "Grau médio: %.2f\n", grau_medio);
    fprintf(arquivo_saida, "Mediana de grau: %.2f\n", mediana_grau);

    busca_largura(matriz, num_vertices, vertice_inicial - 1, arquivo_saida);
    busca_profundidade(matriz, num_vertices, vertice_inicial - 1, arquivo_saida);

    if (vertice_final != -1) {
        int distancia = distancia_entre_vertices(matriz, num_vertices, vertice_inicial, vertice_final);
        fprintf(arquivo_saida, "\nDistância entre vértice %d e vértice %d: %d\n", vertice_inicial, vertice_final, distancia);

        int diametro = calcula_diametro(matriz, num_vertices);
        fprintf(arquivo_saida, "Diâmetro do grafo: %d\n", diametro);
    }

    Componente componentes[MAX_VERTICES];
    int num_componentes = encontrar_componentes_conexas(matriz, num_vertices, componentes);

    qsort(componentes, num_componentes, sizeof(Componente), compara_componentes);

    fprintf(arquivo_saida, "\nComponentes Conexas:\n");
    fprintf(arquivo_saida, "Número de componentes conexas: %d\n", num_componentes);
    for (int i = 0; i < num_componentes; i++) {
        fprintf(arquivo_saida, "Componente %d:\n", i + 1);
        fprintf(arquivo_saida, "  Tamanho: %d\n", componentes[i].tamanho);
        fprintf(arquivo_saida, "  Vértices: ");
        for (int j = 0; j < componentes[i].tamanho; j++) {
            fprintf(arquivo_saida, "%d%s", componentes[i].vertices[j], (j < componentes[i].tamanho - 1) ? ", " : "\n");
        }
    }

    for (int i = 0; i < num_vertices; i++) {
        free(matriz[i]);
    }
    free(matriz);

    fclose(arquivo);
    fclose(arquivo_saida);

    printf("Dados do grafo processados e gravados no arquivo saida_grafo.txt\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Uso: %s <arquivo_grafo> <representacao (matriz|lista)> <vertice_inicial> [<vertice_final>]\n", argv[0]);
        return 1;
    }

    const char *nome_arquivo = argv[1];
    const char *representacao = argv[2];
    int vertice_inicial = atoi(argv[3]);
    int vertice_final = (argc == 5) ? atoi(argv[4]) : -1;

    processa_arquivo_grafo(nome_arquivo, representacao, vertice_inicial, vertice_final);
    return 0;
}
