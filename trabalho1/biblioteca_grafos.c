// Teoria do Grafos - 2024.2 - Universidade Federal do Rio de Janeiro
// Trabalho 1 : 
// Entrada e Saida OK
// Alunos:

#include <stdio.h>
#include <stdlib.h>

int compara(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void processa_arquivo_grafo(const char *nome_arquivo) {
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

    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) {
        graus[v1 - 1]++; 
        graus[v2 - 1]++; 
        num_arestas++;   
    }

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

    fprintf(arquivo_saida, "Número de vértices: %d\n", num_vertices);
    fprintf(arquivo_saida, "Número de arestas: %d\n", num_arestas);
    fprintf(arquivo_saida, "Grau mínimo: %d\n", grau_min);
    fprintf(arquivo_saida, "Grau máximo: %d\n", grau_max);
    fprintf(arquivo_saida, "Grau médio: %.2f\n", grau_medio);
    fprintf(arquivo_saida, "Mediana de grau: %.2f\n", mediana_grau);
    fclose(arquivo);
    fclose(arquivo_saida);

    printf("Dados do grafo foram gravados no arquivo saida_grafo.txt\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_grafo>\n", argv[0]);
        return 1;
    }

    const char *nome_arquivo = argv[1];  
    processa_arquivo_grafo(nome_arquivo);  
    return 0;
}
