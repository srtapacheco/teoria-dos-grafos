#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compara(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void cria_matriz_adjacencia(FILE *arquivo, int num_vertices, int **matriz, int *graus) {
    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        matriz[v1 - 1][v2 - 1] = 1;
        matriz[v2 - 1][v1 - 1] = 1;
        graus[v1 - 1]++;  
        graus[v2 - 1]++;  
    }
}

void cria_lista_adjacencia(FILE *arquivo, int num_vertices, int *graus, int **lista) {
    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        lista[v1 - 1][graus[v1 - 1]++] = v2 - 1;
        lista[v2 - 1][graus[v2 - 1]++] = v1 - 1;
    }
}

void processa_arquivo_grafo(const char *nome_arquivo, const char *representacao) {
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

    int num_vertices;
    fscanf(arquivo, "%d", &num_vertices); 
    int graus[num_vertices]; 
    for (int i = 0; i < num_vertices; i++) {
        graus[i] = 0;  
    }

    int num_arestas = 0;
    rewind(arquivo); // Volta ao início para reler o número de vértices
    fscanf(arquivo, "%d", &num_vertices); // Rele o número de vértices
    
    // Escolhe a representação
    if (strcmp(representacao, "matriz") == 0) {
        // Aloca e inicializa a matriz de adjacência
        int **matriz = (int **)malloc(num_vertices * sizeof(int *));
        for (int i = 0; i < num_vertices; i++) {
            matriz[i] = (int *)calloc(num_vertices, sizeof(int)); // Inicializa com 0
        }

        // Preenche a matriz de adjacência e calcula graus
        cria_matriz_adjacencia(arquivo, num_vertices, matriz, graus);

        // Libera a memória da matriz
        for (int i = 0; i < num_vertices; i++) {
            free(matriz[i]);
        }
        free(matriz);

    } else if (strcmp(representacao, "lista") == 0) {
        // Inicializa a lista de adjacência
        int **lista = (int **)malloc(num_vertices * sizeof(int *));
        for (int i = 0; i < num_vertices; i++) {
            lista[i] = (int *)malloc(num_vertices * sizeof(int)); // Aloca espaço suficiente
        }

        // Preenche a lista de adjacência e calcula graus
        cria_lista_adjacencia(arquivo, num_vertices, graus, lista);

        // Libera a memória da lista
        for (int i = 0; i < num_vertices; i++) {
            free(lista[i]);
        }
        free(lista);
    }

    // Calcula as estatísticas dos graus
    int grau_min = graus[0], grau_max = graus[0];
    double soma_graus = 0.0;
    for (int i = 0; i < num_vertices; i++) {
        if (graus[i] < grau_min) grau_min = graus[i];
        if (graus[i] > grau_max) grau_max = graus[i];
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

    // Imprime as estatísticas
    fprintf(arquivo_saida, "Número de vértices: %d\n", num_vertices);
    fprintf(arquivo_saida, "Número de arestas: %d\n", num_arestas);
    fprintf(arquivo_saida, "Grau mínimo: %d\n", grau_min);
    fprintf(arquivo_saida, "Grau máximo: %d\n", grau_max);
    fprintf(arquivo_saida, "Grau médio: %.2f\n", grau_medio);
    fprintf(arquivo_saida, "Mediana de grau: %.2f\n", mediana_grau);

    fclose(arquivo);
    fclose(arquivo_saida);

    printf("Dados do grafo foram processados e gravados no arquivo saida_grafo.txt\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo_grafo> <representacao (matriz|lista)>\n", argv[0]);
        return 1;
    }

    const char *nome_arquivo = argv[1];
    const char *representacao = argv[2]; // Escolha da representação: matriz ou lista
    processa_arquivo_grafo(nome_arquivo, representacao);  
    return 0;
}
