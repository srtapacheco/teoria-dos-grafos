// Teoria dos Grafos - COS 242
// 2024/2
// Trabalho de Disciplina – Parte 1
// Alunos: Karen Pacheco e Guilherme

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 100

// Estrutura para armazenar informações de uma componente conexa
typedef struct {
    int tamanho;
    int vertices[MAX_VERTICES];
} Componente;

// Estrutura para Grafo utilizando Matriz de Adjacência
typedef struct {
    int num_vertices;
    int **matriz;
} GrafoMatriz;

// Estrutura para Grafo utilizando Lista de Adjacência
typedef struct {
    int num_vertices;
    int **listas;      // Array de ponteiros para listas de adjacência
    int *tamanhos;     // Tamanho de cada lista
} GrafoLista;

// Função de comparação para qsort (inteiros)
int compara(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Função de comparação para qsort (Componentes Conexas) - ordena por tamanho decrescente
int compara_componentes(const void *a, const void *b) {
    Componente *compA = (Componente *)a;
    Componente *compB = (Componente *)b;
    return compB->tamanho - compA->tamanho;
}

// Função para criar e preencher a matriz de adjacência
void cria_matriz_adjacencia(FILE *arquivo, int num_vertices, GrafoMatriz *grafo) {
    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        grafo->matriz[v1 - 1][v2 - 1] = 1;
        grafo->matriz[v2 - 1][v1 - 1] = 1;
    }
}

// Função para criar e preencher a lista de adjacência
void cria_lista_adjacencia(FILE *arquivo, int num_vertices, GrafoLista *grafo) {
    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        grafo->listas[v1 - 1][grafo->tamanhos[v1 - 1]++] = v2;
        grafo->listas[v2 - 1][grafo->tamanhos[v2 - 1]++] = v1;
    }
}

// Implementação da BFS para Matriz de Adjacência
void busca_largura_matriz(GrafoMatriz *grafo, int vertice_inicial, FILE *arquivo_saida) {
    int fila[MAX_VERTICES], nivel[MAX_VERTICES], pai[MAX_VERTICES], visitado[MAX_VERTICES];
    int inicio = 0, fim = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
        pai[i] = -1;
    }

    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;
    pai[vertice_inicial] = 0;

    fprintf(arquivo_saida, "\nBusca em Largura (BFS) - Matriz de Adjacência:\n");
    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < grafo->num_vertices; i++) {
            if (grafo->matriz[atual][i] && !visitado[i]) {
                fila[fim++] = i;
                visitado[i] = 1;
                nivel[i] = nivel[atual] + 1;
                pai[i] = atual + 1;
            }
        }
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i], nivel[i]);
    }
}

// Implementação da BFS para Lista de Adjacência
void busca_largura_lista(GrafoLista *grafo, int vertice_inicial, FILE *arquivo_saida) {
    int fila[MAX_VERTICES], nivel[MAX_VERTICES], pai[MAX_VERTICES], visitado[MAX_VERTICES];
    int inicio = 0, fim = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
        pai[i] = -1;
    }

    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;
    pai[vertice_inicial] = 0;

    fprintf(arquivo_saida, "\nBusca em Largura (BFS) - Lista de Adjacência:\n");
    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < grafo->tamanhos[atual]; i++) {
            int adj = grafo->listas[atual][i] - 1; // Ajusta para índice 0
            if (!visitado[adj]) {
                fila[fim++] = adj;
                visitado[adj] = 1;
                nivel[adj] = nivel[atual] + 1;
                pai[adj] = atual + 1;
            }
        }
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i], nivel[i]);
    }
}

// Implementação da DFS Recursiva para Matriz de Adjacência
void busca_profundidade_recursiva_matriz(GrafoMatriz *grafo, int vertice, int *visitado, int *nivel, int *pai, int profundidade, FILE *arquivo_saida) {
    visitado[vertice] = 1;
    nivel[vertice] = profundidade;

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (grafo->matriz[vertice][i] && !visitado[i]) {
            pai[i] = vertice + 1;
            busca_profundidade_recursiva_matriz(grafo, i, visitado, nivel, pai, profundidade + 1, arquivo_saida);
        }
    }
}

// Implementação da DFS para Matriz de Adjacência
void busca_profundidade_matriz(GrafoMatriz *grafo, int vertice_inicial, FILE *arquivo_saida) {
    int visitado[MAX_VERTICES], nivel[MAX_VERTICES], pai[MAX_VERTICES];

    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
        pai[i] = -1;
    }

    pai[vertice_inicial] = 0;
    busca_profundidade_recursiva_matriz(grafo, vertice_inicial, visitado, nivel, pai, 0, arquivo_saida);

    fprintf(arquivo_saida, "\nBusca em Profundidade (DFS) - Matriz de Adjacência:\n");
    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i], nivel[i]);
    }
}

// Implementação da DFS Recursiva para Lista de Adjacência
void busca_profundidade_recursiva_lista(GrafoLista *grafo, int vertice, int *visitado, int *nivel, int *pai, int profundidade, FILE *arquivo_saida) {
    visitado[vertice] = 1;
    nivel[vertice] = profundidade;

    for (int i = 0; i < grafo->tamanhos[vertice]; i++) {
        int adj = grafo->listas[vertice][i] - 1; // Ajusta para índice 0
        if (!visitado[adj]) {
            pai[adj] = vertice + 1;
            busca_profundidade_recursiva_lista(grafo, adj, visitado, nivel, pai, profundidade + 1, arquivo_saida);
        }
    }
}

// Implementação da DFS para Lista de Adjacência
void busca_profundidade_lista(GrafoLista *grafo, int vertice_inicial, FILE *arquivo_saida) {
    int visitado[MAX_VERTICES], nivel[MAX_VERTICES], pai[MAX_VERTICES];

    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
        pai[i] = -1;
    }

    pai[vertice_inicial] = 0;
    busca_profundidade_recursiva_lista(grafo, vertice_inicial, visitado, nivel, pai, 0, arquivo_saida);

    fprintf(arquivo_saida, "\nBusca em Profundidade (DFS) - Lista de Adjacência:\n");
    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice: %d, Pai: %d, Nível: %d\n", i + 1, pai[i], nivel[i]);
    }
}

// Implementação da BFS para calcular distâncias (válido para ambas as representações)
int bfs_distancia_matriz(GrafoMatriz *grafo, int vertice_inicial, int *distancias) {
    int fila[MAX_VERTICES], nivel[MAX_VERTICES], visitado[MAX_VERTICES];
    int inicio = 0, fim = 0, maior_distancia = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
    }

    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;

    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < grafo->num_vertices; i++) {
            if (grafo->matriz[atual][i] && !visitado[i]) {
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

int bfs_distancia_lista(GrafoLista *grafo, int vertice_inicial, int *distancias) {
    int fila[MAX_VERTICES], nivel[MAX_VERTICES], visitado[MAX_VERTICES];
    int inicio = 0, fim = 0, maior_distancia = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        visitado[i] = 0;
        nivel[i] = -1;
    }

    fila[fim++] = vertice_inicial;
    visitado[vertice_inicial] = 1;
    nivel[vertice_inicial] = 0;

    while (inicio < fim) {
        int atual = fila[inicio++];

        for (int i = 0; i < grafo->tamanhos[atual]; i++) {
            int adj = grafo->listas[atual][i] - 1; // Ajusta para índice 0
            if (!visitado[adj]) {
                fila[fim++] = adj;
                visitado[adj] = 1;
                nivel[adj] = nivel[atual] + 1;
                distancias[adj] = nivel[adj];
                if (nivel[adj] > maior_distancia) {
                    maior_distancia = nivel[adj];
                }
            }
        }
    }

    return maior_distancia;
}

// Função para calcular o diâmetro do grafo
int calcula_diametro_matriz(GrafoMatriz *grafo) {
    int maior_distancia = 0;
    int distancias[MAX_VERTICES];

    for (int i = 0; i < grafo->num_vertices; i++) {
        int distancia_atual = bfs_distancia_matriz(grafo, i, distancias);
        if (distancia_atual > maior_distancia) {
            maior_distancia = distancia_atual;
        }
    }

    return maior_distancia;
}

int calcula_diametro_lista(GrafoLista *grafo) {
    int maior_distancia = 0;
    int distancias[MAX_VERTICES];

    for (int i = 0; i < grafo->num_vertices; i++) {
        int distancia_atual = bfs_distancia_lista(grafo, i, distancias);
        if (distancia_atual > maior_distancia) {
            maior_distancia = distancia_atual;
        }
    }

    return maior_distancia;
}

// Função para calcular a distância entre dois vértices específicos
int distancia_entre_vertices_matriz(GrafoMatriz *grafo, int vertice_inicial, int vertice_final) {
    int distancias[MAX_VERTICES];
    bfs_distancia_matriz(grafo, vertice_inicial - 1, distancias);
    return distancias[vertice_final - 1];
}

int distancia_entre_vertices_lista(GrafoLista *grafo, int vertice_inicial, int vertice_final) {
    int distancias[MAX_VERTICES];
    bfs_distancia_lista(grafo, vertice_inicial - 1, distancias);
    return distancias[vertice_final - 1];
}

// Função para encontrar componentes conexas na Matriz de Adjacência
int encontrar_componentes_conexas_matriz(GrafoMatriz *grafo, Componente *componentes) {
    int id_componente[MAX_VERTICES];
    for (int i = 0; i < grafo->num_vertices; i++) {
        id_componente[i] = -1;
    }

    int componente_atual = 0;

    for (int v = 0; v < grafo->num_vertices; v++) {
        if (id_componente[v] == -1) {
            int fila[MAX_VERTICES];
            int inicio = 0, fim = 0;

            fila[fim++] = v;
            id_componente[v] = componente_atual;
            componentes[componente_atual].tamanho = 0;

            while (inicio < fim) {
                int atual = fila[inicio++];

                componentes[componente_atual].vertices[componentes[componente_atual].tamanho++] = atual + 1;

                for (int i = 0; i < grafo->num_vertices; i++) {
                    if (grafo->matriz[atual][i] && id_componente[i] == -1) {
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

// Função para encontrar componentes conexas na Lista de Adjacência
int encontrar_componentes_conexas_lista(GrafoLista *grafo, Componente *componentes) {
    int id_componente[MAX_VERTICES];
    for (int i = 0; i < grafo->num_vertices; i++) {
        id_componente[i] = -1;
    }

    int componente_atual = 0;

    for (int v = 0; v < grafo->num_vertices; v++) {
        if (id_componente[v] == -1) {
            int fila[MAX_VERTICES];
            int inicio = 0, fim = 0;

            fila[fim++] = v;
            id_componente[v] = componente_atual;
            componentes[componente_atual].tamanho = 0;

            while (inicio < fim) {
                int atual = fila[inicio++];

                componentes[componente_atual].vertices[componentes[componente_atual].tamanho++] = atual + 1;

                for (int i = 0; i < grafo->tamanhos[atual]; i++) {
                    int adj = grafo->listas[atual][i] - 1; // Ajusta para índice 0
                    if (id_componente[adj] == -1) {
                        fila[fim++] = adj;
                        id_componente[adj] = componente_atual;
                    }
                }
            }

            componente_atual++;
        }
    }

    return componente_atual;
}

// Função para processar o arquivo de grafo
void processa_arquivo_grafo(const char *nome_arquivo, const char *representacao, int vertice_inicial, int vertice_final) {
    FILE *arquivo = fopen(nome_arquivo, "r"); // Abre o arquivo que contém o grafo
    if (arquivo == NULL) { // Testa se conseguiu abrir
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo); 
        return;
    }

    FILE *arquivo_saida = fopen("saida_grafo.txt", "w"); // Abre o arquivo de saída
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n"); // Testa se conseguiu abrir
        fclose(arquivo);
        return;
    }

    int num_vertices, v1, v2; // v1 e v2 são os pares de vértices
    fscanf(arquivo, "%d", &num_vertices); // Lê o número de vértices do grafo na primeira linha do arquivo

    int graus[num_vertices]; // Cria um array de graus de tamanho do número de vértices
    for (int i = 0; i < num_vertices; i++) { // Percorre n vertices
        graus[i] = 0; // Inicializa o valor de cada elemento do array com 0
    }

    int num_arestas = 0;

    // Declara estruturas para as duas representações
    GrafoMatriz grafo_matriz;
    GrafoLista grafo_lista;

    // Inicializa as estruturas
    grafo_matriz.num_vertices = num_vertices;
    grafo_matriz.matriz = (int **)malloc(num_vertices * sizeof(int *));
    for (int i = 0; i < num_vertices; i++) {
        grafo_matriz.matriz[i] = (int *)calloc(num_vertices, sizeof(int));
    }

    grafo_lista.num_vertices = num_vertices;
    grafo_lista.listas = (int **)malloc(num_vertices * sizeof(int *));
    grafo_lista.tamanhos = (int *)malloc(num_vertices * sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        grafo_lista.listas[i] = (int *)malloc(num_vertices * sizeof(int)); // Assumindo no máximo num_vertices adjacentes
        grafo_lista.tamanhos[i] = 0;
    }

    // Cria a representação escolhida
    if (strcmp(representacao, "matriz") == 0) {
        cria_matriz_adjacencia(arquivo, num_vertices, &grafo_matriz);
    } else if (strcmp(representacao, "lista") == 0) {
        cria_lista_adjacencia(arquivo, num_vertices, &grafo_lista);
    } else {
        printf("Representação inválida. Use 'matriz' ou 'lista'.\n");
        fclose(arquivo);
        fclose(arquivo_saida);
        return;
    }

    // Reposiciona o ponteiro do arquivo para o início para recontar arestas e graus
    rewind(arquivo);
    fscanf(arquivo, "%d", &num_vertices);
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        graus[v1 - 1]++;
        graus[v2 - 1]++;
        num_arestas++;
    }

    // Cálculo de grau mínimo, máximo e médio
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

    // Cálculo da mediana de grau
    qsort(graus, num_vertices, sizeof(int), compara);
    double mediana_grau;
    if (num_vertices % 2 == 0) {
        mediana_grau = (graus[num_vertices / 2 - 1] + graus[num_vertices / 2]) / 2.0;
    } else {
        mediana_grau = graus[num_vertices / 2];
    }

    // Impressão das informações básicas do grafo
    fprintf(arquivo_saida, "Número de vértices: %d\n", num_vertices);
    fprintf(arquivo_saida, "Número de arestas: %d\n", num_arestas);
    fprintf(arquivo_saida, "Grau mínimo: %d\n", grau_min);
    fprintf(arquivo_saida, "Grau máximo: %d\n", grau_max);
    fprintf(arquivo_saida, "Grau médio: %.2f\n", grau_medio);
    fprintf(arquivo_saida, "Mediana de grau: %.2f\n", mediana_grau);

    // Executa BFS e DFS dependendo da representação
    if (strcmp(representacao, "matriz") == 0) {
        busca_largura_matriz(&grafo_matriz, vertice_inicial - 1, arquivo_saida);
        busca_profundidade_matriz(&grafo_matriz, vertice_inicial - 1, arquivo_saida);
    } else if (strcmp(representacao, "lista") == 0) {
        busca_largura_lista(&grafo_lista, vertice_inicial - 1, arquivo_saida);
        busca_profundidade_lista(&grafo_lista, vertice_inicial - 1, arquivo_saida);
    }

    // Calcula e imprime distância e diâmetro se vertice_final foi fornecido
    if (vertice_final != -1) {
        if (strcmp(representacao, "matriz") == 0) {
            int distancia = distancia_entre_vertices_matriz(&grafo_matriz, vertice_inicial, vertice_final);
            fprintf(arquivo_saida, "\nDistância entre vértice %d e vértice %d: %d\n", vertice_inicial, vertice_final, distancia);

            int diametro = calcula_diametro_matriz(&grafo_matriz);
            fprintf(arquivo_saida, "Diâmetro do grafo: %d\n", diametro);
        } else if (strcmp(representacao, "lista") == 0) {
            int distancia = distancia_entre_vertices_lista(&grafo_lista, vertice_inicial, vertice_final);
            fprintf(arquivo_saida, "\nDistância entre vértice %d e vértice %d: %d\n", vertice_inicial, vertice_final, distancia);

            int diametro = calcula_diametro_lista(&grafo_lista);
            fprintf(arquivo_saida, "Diâmetro do grafo: %d\n", diametro);
        }
    }

    // Encontrar e imprimir componentes conexas dependendo da representação
    Componente componentes[MAX_VERTICES];
    int num_componentes;
    if (strcmp(representacao, "matriz") == 0) {
        num_componentes = encontrar_componentes_conexas_matriz(&grafo_matriz, componentes);
    } else { // lista
        num_componentes = encontrar_componentes_conexas_lista(&grafo_lista, componentes);
    }

    // Ordenar as componentes por tamanho decrescente
    qsort(componentes, num_componentes, sizeof(Componente), compara_componentes);

    // Impressão das componentes conexas
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

    // Libera a memória alocada para as estruturas
    if (strcmp(representacao, "matriz") == 0) {
        for (int i = 0; i < grafo_matriz.num_vertices; i++) {
            free(grafo_matriz.matriz[i]);
        }
        free(grafo_matriz.matriz);
    } else { // lista
        for (int i = 0; i < grafo_lista.num_vertices; i++) {
            free(grafo_lista.listas[i]);
        }
        free(grafo_lista.listas);
        free(grafo_lista.tamanhos);
    }

    fclose(arquivo);
    fclose(arquivo_saida);

    printf("Dados do grafo processados e gravados no arquivo saida_grafo.txt\n");
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) { // Condicional para quantidade de argumentos
        printf("Uso: %s <arquivo_grafo> <representacao (matriz|lista)> <vertice_inicial> [<vertice_final>]\n", argv[0]); // argv[0] é o nome do programa
        return 1;
    }

    const char *nome_arquivo = argv[1]; // argv[1] é o nome do arquivo do grafo
    const char *representacao = argv[2]; // argv[2] é a representação escolhida ('matriz' ou 'lista')
    int vertice_inicial = atoi(argv[3]); // argv[3] é o vértice inicial
    int vertice_final;

    if (argc == 5) {
        vertice_final = atoi(argv[4]); // Se houver quinto argumento, define o vértice final
    } else {
        vertice_final = -1; // Caso contrário, não calcula distância e diâmetro
    }

    // Validação da representação
    if (strcmp(representacao, "matriz") != 0 && strcmp(representacao, "lista") != 0) {
        printf("Representação inválida. Use 'matriz' ou 'lista'.\n");
        return 1;
    }

    processa_arquivo_grafo(nome_arquivo, representacao, vertice_inicial, vertice_final); 
    return 0;
}
