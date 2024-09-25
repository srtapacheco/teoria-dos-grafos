// Teoria do Grafos - 2024.2 - Universidade Federal do Rio de Janeiro
// Trabalho 1 : 
// Entrada e Saida OK
// Alunos: Karen Pacheco e Guilherme

#include <stdio.h>
#include <stdlib.h>

// Função auxiliar para comparar dois inteiros, usada na ordenação dos graus
int compara(const void *a, const void *b) {
    return (*(int*)a - *(int*)b); // Função de comparação para a função qsort
}

void processa_arquivo_grafo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r"); //abre o arquivo com os grafos
    if (arquivo == NULL) {  //verifica se consegue abrir o arquivo
        printf("Erro ao abrir o arquivo: %s\n", nome_arquivo);
        return;
    }

    FILE *arquivo_saida = fopen("saida_grafo.txt", "w"); //abre o arquivo onde vai ser guardado a resposta
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de saída.\n");
        fclose(arquivo);
        return;
    }

    int num_vertices, v1, v2; //num_vertices = numero de veritices do grafo, v1 e v2 eh cada par de arestas
    fscanf(arquivo, "%d", &num_vertices); // lê o número de vértices do grafo na primeira linha do arquivo
    int graus[num_vertices]; //inicializa um array de graus para armazenar o grau de cada vértice, inicialmente com tamanho de num_vertices
    for (int i = 0; i < num_vertices; i++) { //percorre cada aresta
        graus[i] = 0; //inicializa o valor de cada elemento do array com 0, entao todos tem 0 no inicio
    }

    int num_arestas = 0; 

    while (fscanf(arquivo, "%d %d", &v1, &v2) != EOF) { //le o arquivo com os pares de vertices
        graus[v1 - 1]++; // incrementa o grau do vértice v1
        graus[v2 - 1]++; // Os vértices são numerados de 1 a num_vertices, mas o array graus[] começa no índice 0, entao subtraío 1 de v1 para ajustar o índice do array
        num_arestas++;  // incrementa o número total de arestas
    }

    // Inicializa as variáveis de grau mínimo e máximo, e soma de graus
    int grau_min = graus[0], grau_max = graus[0]; // Inicializa com o grau do primeiro vértice
    double soma_graus = 0.0; 


    // Calcula o grau mínimo, grau máximo e a soma dos graus
    for (int i = 0; i < num_vertices; i++) {
        if (graus[i] < grau_min) grau_min = graus[i]; // Atualiza o grau mínimo se encontrar um menor
        if (graus[i] > grau_max) grau_max = graus[i]; // Atualiza o grau máximo se encontrar um maior
        soma_graus += graus[i]; // Acumula a soma dos graus
    }
    double grau_medio = soma_graus / num_vertices; // O grau médio é a soma dos graus dividida pelo número de vértices


    qsort(graus, num_vertices, sizeof(int), compara); // usa a função qsort para ordenar os graus

    // Calcula a mediana dos graus
    double mediana_grau;
    if (num_vertices % 2 == 0) {
        mediana_grau = (graus[num_vertices / 2 - 1] + graus[num_vertices / 2]) / 2.0;  // Se o número de vértices for par, a mediana é a média dos dois valores centrais
    } else {
        mediana_grau = graus[num_vertices / 2];  // Se for ímpar, a mediana é o valor central
    }

    // Escreve os resultados no arquivo de saída e fecha dps
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
        printf("Uso: Inserir como parametro %s <arquivo_grafo>\n", argv[0]); // requere os parametros do grafo pra rodar o programa
        return 1;
    }

    const char *nome_arquivo = argv[1];  // argv[0] eh o indice com o nome do programa e o arg[1] eh o indice com do primeiro argumento passado pelo user no console (do grafo a ser lido)
    processa_arquivo_grafo(nome_arquivo);  
    return 0;
}
