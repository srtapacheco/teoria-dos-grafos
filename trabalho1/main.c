#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "biblioteca_grafos.h"

// Função para criar a pasta de resultados se ela não existir
void criar_pasta_resultados() {
    struct stat st = {0};
    if (stat("resultados", &st) == -1) {
        mkdir("resultados", 0700); 
    }
}

// Função para salvar o tempo de execução de uma busca em um arquivo específico
void salvar_tempo_busca(Grafo *grafo, int vertice_inicial, void (*funcao_busca)(Grafo*, int, int*), const char *nome_arquivo) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);
    
    FILE *arquivo_saida = fopen(caminho_arquivo, "a");
    if (!arquivo_saida) {
        printf("Erro ao abrir o arquivo de saída: %s\n", caminho_arquivo);
        return;
    }

    int *pais = (int*)calloc(grafo->num_vertices, sizeof(int));
    clock_t inicio = clock();
    funcao_busca(grafo, vertice_inicial, pais);
    clock_t fim = clock();
    double tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    fprintf(arquivo_saida, "Tempo de execução a partir do vértice %d: %.6f segundos\n", vertice_inicial, tempo_gasto);

    free(pais);
    fclose(arquivo_saida);
}

// Função para salvar os pais de uma busca em um arquivo específico
void salvar_pais(Grafo *grafo, int *pais, const char *nome_arquivo) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);

    FILE *arquivo_saida = fopen(caminho_arquivo, "w");
    if (!arquivo_saida) {
        printf("Erro ao abrir o arquivo de saída: %s\n", caminho_arquivo);
        return;
    }

    for (int i = 0; i < grafo->num_vertices; i++) {
        fprintf(arquivo_saida, "Vértice %d -> Pai: %d\n", i, pais[i]);
    }

    fclose(arquivo_saida);
}

// Função para salvar a quantidade de memória utilizada por diferentes representações de grafos em um arquivo
void salvar_memoria_grafo(Grafo *grafo, const char *nome_arquivo) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);
    
    FILE *arquivo_saida = fopen(caminho_arquivo, "w");
    if (!arquivo_saida) {
        printf("Erro ao abrir o arquivo de saída: %s\n", caminho_arquivo);
        return;
    }

    int memoria_lista_adj = sizeof(NoListaAdj*) * grafo->num_vertices;
    int memoria_matriz_adj = sizeof(int*) * grafo->num_vertices + sizeof(int) * grafo->num_vertices * grafo->num_vertices;

    fprintf(arquivo_saida, "Memória utilizada (Lista de Adjacência): %d bytes\n", memoria_lista_adj);
    fprintf(arquivo_saida, "Memória utilizada (Matriz de Adjacência): %d bytes\n", memoria_matriz_adj);

    fclose(arquivo_saida);
}

// Função para salvar o resultado das distâncias entre pares de vértices específicos em um arquivo
void salvar_distancias(Grafo *grafo, const char *nome_arquivo) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);
    
    FILE *arquivo_saida = fopen(caminho_arquivo, "w");
    if (!arquivo_saida) {
        printf("Erro ao abrir o arquivo de saída: %s\n", caminho_arquivo);
        return;
    }

    fprintf(arquivo_saida, "Distâncias entre Pares de Vértices\n");
    int dist_10_20 = calcular_distancia(grafo, 10, 20);
    int dist_10_30 = calcular_distancia(grafo, 10, 30);
    int dist_20_30 = calcular_distancia(grafo, 20, 30);

    fprintf(arquivo_saida, "Distância entre 10 e 20: %d\n", dist_10_20);
    fprintf(arquivo_saida, "Distância entre 10 e 30: %d\n", dist_10_30);
    fprintf(arquivo_saida, "Distância entre 20 e 30: %d\n", dist_20_30);

    fclose(arquivo_saida);
}

// Função para salvar as componentes conexas em um arquivo
void salvar_componentes_conexas(Grafo *grafo, const char *nome_arquivo) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);
    
    FILE *arquivo_saida = fopen(caminho_arquivo, "w");
    if (!arquivo_saida) {
        printf("Erro ao abrir o arquivo de saída: %s\n", caminho_arquivo);
        return;
    }

    fprintf(arquivo_saida, "Componentes Conexas:\n");
    bool *visitados = (bool*)calloc(grafo->num_vertices, sizeof(bool));
    int num_componentes = 0;

    for (int i = 0; i < grafo->num_vertices; i++) {
        if (!visitados[i]) {
            num_componentes++;
            fprintf(arquivo_saida, "Componente %d: ", num_componentes);

            int *pilha = (int*)malloc(grafo->num_vertices * sizeof(int));
            int topo = -1;
            int tamanho_componente = 0;

            pilha[++topo] = i;
            visitados[i] = true;

            while (topo >= 0) {
                int vertice = pilha[topo--];
                fprintf(arquivo_saida, "%d ", vertice);
                tamanho_componente++;

                NoListaAdj *adj = grafo->lista_adj[vertice];
                while (adj != NULL) {
                    if (!visitados[adj->vertice]) {
                        visitados[adj->vertice] = true;
                        pilha[++topo] = adj->vertice;
                    }
                    adj = adj->proximo;
                }
            }
            fprintf(arquivo_saida, "(Tamanho: %d)\n", tamanho_componente);
            free(pilha);
        }
    }
    fprintf(arquivo_saida, "Número total de componentes conexas: %d\n", num_componentes);
    free(visitados);

    fclose(arquivo_saida);
}

// Função para salvar o diâmetro do grafo em um arquivo
void salvar_diametro(Grafo *grafo, const char *nome_arquivo) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "resultados/%s", nome_arquivo);
    
    FILE *arquivo_saida = fopen(caminho_arquivo, "w");
    if (!arquivo_saida) {
        printf("Erro ao abrir o arquivo de saída: %s\n", caminho_arquivo);
        return;
    }

    int diametro = calcular_diametro(grafo);
    fprintf(arquivo_saida, "Diâmetro do Grafo: %d\n", diametro);

    fclose(arquivo_saida);
}

int main() {
    criar_pasta_resultados();
    Grafo *grafo = ler_grafo_do_arquivo("grafo_1.txt");
    if (!grafo) {
        return 1;
    }

    int escolha;
    printf("\nSelecione o teste a ser executado:\n");
    printf("1. Teste de Memória\n");
    printf("2. Teste de Tempo de Execução BFS\n");
    printf("3. Teste de Tempo de Execução DFS\n");
    printf("4. Teste de Pais da Busca\n");
    printf("5. Teste de Distâncias entre Pares de Vértices\n");
    printf("6. Teste de Componentes Conexas\n");
    printf("7. Teste de Diâmetro do Grafo\n");
    printf("8. Sair\n");
    printf("Escolha: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            salvar_memoria_grafo(grafo, "1.memoria.txt");
            break;
        case 2: {
            double tempo_total = 0.0;
            int *pais = (int*)calloc(grafo->num_vertices, sizeof(int));
            for (int i = 0; i < 100; i++) {
                int vertice_inicial = i % grafo->num_vertices;
                clock_t inicio = clock();
                busca_em_largura(grafo, vertice_inicial, pais);
                clock_t fim = clock();
                tempo_total += (double)(fim - inicio) / CLOCKS_PER_SEC;
            }
            double tempo_medio = tempo_total / 100.0;
            FILE *arquivo_tempo = fopen("resultados/2.tempo_medio_bfs.txt", "w");
            if (arquivo_tempo) {
                fprintf(arquivo_tempo, "Tempo médio de execução da BFS: %.6f segundos\n", tempo_medio);
                fclose(arquivo_tempo);
            }
            free(pais);
            break;
        }
        case 3: {
            double tempo_total = 0.0;
            int *pais = (int*)calloc(grafo->num_vertices, sizeof(int));
            for (int i = 0; i < 100; i++) {
                int vertice_inicial = i % grafo->num_vertices;
                clock_t inicio = clock();
                busca_em_profundidade(grafo, vertice_inicial, pais);
                clock_t fim = clock();
                tempo_total += (double)(fim - inicio) / CLOCKS_PER_SEC;
            }
            double tempo_medio = tempo_total / 100.0;
            FILE *arquivo_tempo = fopen("resultados/3.tempo_medio_dfs.txt", "w");
            if (arquivo_tempo) {
                fprintf(arquivo_tempo, "Tempo médio de execução da DFS: %.6f segundos\n", tempo_medio);
                fclose(arquivo_tempo);
            }
            free(pais);
            break;
        }
        case 4: {
            int vertices_interesse[] = {10, 20, 30};  
            int *pais = (int*)calloc(grafo->num_vertices, sizeof(int));


            FILE *arquivo_bfs = fopen("resultados/4.pais_bfs.txt", "w");
            if (!arquivo_bfs) {
                printf("Erro ao abrir o arquivo de saída para BFS.\n");
                free(pais);
                break;
            }

            for (int inicio = 1; inicio <= 3; inicio++) {
                busca_em_largura(grafo, inicio, pais);
                fprintf(arquivo_bfs, "Pais dos vértices (10, 20, 30) a partir da BFS com início no vértice %d:\n", inicio);
                for (int i = 0; i < 3; i++) {
                    fprintf(arquivo_bfs, "Pai do vértice %d: %d\n", vertices_interesse[i], pais[vertices_interesse[i]]);
                }
                fprintf(arquivo_bfs, "\n");
            }
            fclose(arquivo_bfs);

            FILE *arquivo_dfs = fopen("resultados/4.pais_dfs.txt", "w");
            if (!arquivo_dfs) {
                printf("Erro ao abrir o arquivo de saída para DFS.\n");
                free(pais);
                break;
            }

            for (int inicio = 1; inicio <= 3; inicio++) {
                busca_em_profundidade(grafo, inicio, pais);
                fprintf(arquivo_dfs, "Pais dos vértices (10, 20, 30) a partir da DFS com início no vértice %d:\n", inicio);
                for (int i = 0; i < 3; i++) {
                    fprintf(arquivo_dfs, "Pai do vértice %d: %d\n", vertices_interesse[i], pais[vertices_interesse[i]]);
                }
                fprintf(arquivo_dfs, "\n");
            }
            fclose(arquivo_dfs);

            free(pais);
            break;
        }
        case 5:
            salvar_distancias(grafo, "5.distancias.txt");
            break;
        case 6:
            salvar_componentes_conexas(grafo, "6.componentes_conexas.txt");
            break;
        case 7:
            salvar_diametro(grafo, "7.diametro.txt");
            break;
        case 8:
            printf("Saindo...\n");
            liberar_grafo(grafo);
            return 0;
        default:
            printf("Opção inválida! Tente novamente.\n");
            liberar_grafo(grafo);
            return 1;
    }
    
    liberar_grafo(grafo);
    return 0;
}
