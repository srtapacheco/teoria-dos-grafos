#include <stdio.h>
#include <stdlib.h>
#include "biblioteca_grafos.h"
#include <stdbool.h>

Grafo *criar_grafo_lista_adj(int num_vertices)
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    if (!grafo)
    {
        printf("Erro ao alocar memória para o grafo.\n");
        return NULL;
    }

    grafo->num_vertices = num_vertices;
    grafo->num_arestas = 0;
    grafo->lista_adj = (NoListaAdj **)malloc(num_vertices * sizeof(NoListaAdj *));
    grafo->graus = (int *)calloc(num_vertices, sizeof(int));
    grafo->matriz_adj = NULL;
    grafo->tipo_representacao = LISTA_ADJACENCIA;

    if (!grafo->lista_adj || !grafo->graus)
    {
        printf("Erro ao alocar memória para lista de adjacência ou graus.\n");
        free(grafo);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++)
    {
        grafo->lista_adj[i] = NULL;
    }

    return grafo;
}

Grafo *criar_grafo_matriz_adj(int num_vertices)
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    if (!grafo)
    {
        printf("Erro ao alocar memória para o grafo.\n");
        return NULL;
    }

    grafo->num_vertices = num_vertices;
    grafo->num_arestas = 0;
    grafo->lista_adj = NULL;
    grafo->graus = (int *)calloc(num_vertices, sizeof(int));
    grafo->matriz_adj = (int **)malloc(num_vertices * sizeof(int *));
    grafo->tipo_representacao = MATRIZ_ADJACENCIA;

    if (!grafo->matriz_adj || !grafo->graus)
    {
        printf("Erro ao alocar memória para matriz de adjacência ou graus.\n");
        free(grafo);
        return NULL;
    }

    for (int i = 0; i < num_vertices; i++)
    {
        grafo->matriz_adj[i] = (int *)calloc(num_vertices, sizeof(int));
        if (!grafo->matriz_adj[i])
        {
            printf("Erro ao alocar memória para linha da matriz de adjacência.\n");
            // Liberar memória já alocada
            for (int j = 0; j < i; j++)
            {
                free(grafo->matriz_adj[j]);
            }
            free(grafo->matriz_adj);
            free(grafo->graus);
            free(grafo);
            return NULL;
        }
    }

    return grafo;
}

void adicionar_aresta(Grafo *grafo, int v1, int v2)
{
    if (grafo->tipo_representacao == LISTA_ADJACENCIA)
    {
        NoListaAdj *novoNo = (NoListaAdj *)malloc(sizeof(NoListaAdj));
        if (!novoNo)
        {
            printf("Erro ao alocar memória para nova aresta.\n");
            return;
        }

        novoNo->vertice = v2;
        novoNo->proximo = grafo->lista_adj[v1];
        grafo->lista_adj[v1] = novoNo;
        grafo->graus[v1]++;

        novoNo = (NoListaAdj *)malloc(sizeof(NoListaAdj));
        if (!novoNo)
        {
            printf("Erro ao alocar memória para nova aresta.\n");
            return;
        }

        novoNo->vertice = v1;
        novoNo->proximo = grafo->lista_adj[v2];
        grafo->lista_adj[v2] = novoNo;
        grafo->graus[v2]++;
    }
    else if (grafo->tipo_representacao == MATRIZ_ADJACENCIA)
    {
        grafo->matriz_adj[v1][v2] = 1;
        grafo->matriz_adj[v2][v1] = 1;
        grafo->graus[v1]++;
        grafo->graus[v2]++;
    }

    grafo->num_arestas++;
}

void exibir_lista_adj(Grafo *grafo)
{
    for (int i = 0; i < grafo->num_vertices; i++)
    {
        NoListaAdj *temp = grafo->lista_adj[i];
        printf("Vértice %d: ", i);
        while (temp)
        {
            printf("-> %d ", temp->vertice);
            temp = temp->proximo;
        }
        printf("\n");
    }
}

void exibir_matriz_adj(Grafo *grafo)
{
    for (int i = 0; i < grafo->num_vertices; i++)
    {
        for (int j = 0; j < grafo->num_vertices; j++)
        {
            printf("%d ", grafo->matriz_adj[i][j]);
        }
        printf("\n");
    }
}

Grafo *ler_grafo_do_arquivo(const char *nome_arquivo, RepresentacaoGrafo tipo_representacao)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    int num_vertices;
    if (fscanf(arquivo, "%d", &num_vertices) != 1)
    {
        printf("Erro ao ler o número de vértices.\n");
        fclose(arquivo);
        return NULL;
    }

    Grafo *grafo;
    if (tipo_representacao == LISTA_ADJACENCIA)
    {
        grafo = criar_grafo_lista_adj(num_vertices);
    }
    else if (tipo_representacao == MATRIZ_ADJACENCIA)
    {
        grafo = criar_grafo_matriz_adj(num_vertices);
    }
    else
    {
        printf("Tipo de representação desconhecido.\n");
        fclose(arquivo);
        return NULL;
    }

    if (!grafo)
    {
        fclose(arquivo);
        return NULL;
    }

    int v1, v2;
    while (fscanf(arquivo, "%d %d", &v1, &v2) == 2)
    {
        v1--;
        v2--;

        if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices)
        {
            printf("Erro: vértices fora dos limites.\n");
            liberar_grafo(grafo);
            fclose(arquivo);
            return NULL;
        }

        adicionar_aresta(grafo, v1, v2);
    }

    fclose(arquivo);
    return grafo;
}

// Função para comparar dois inteiros (usada para ordenação)
int comparar_inteiros(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void salvar_informacoes_grafo(Grafo *grafo, const char *nome_arquivo_saida)
{
    FILE *arquivo_saida = fopen(nome_arquivo_saida, "w");
    if (!arquivo_saida)
    {
        printf("Erro ao abrir o arquivo de saída.\n");
        return;
    }

    fprintf(arquivo_saida, "Número de vértices: %d\n", grafo->num_vertices);
    fprintf(arquivo_saida, "Número de arestas: %d\n", grafo->num_arestas);

    int grau_min = grafo->graus[0], grau_max = grafo->graus[0];
    int soma_graus = 0;
    for (int i = 0; i < grafo->num_vertices; i++)
    {
        if (grafo->graus[i] < grau_min)
            grau_min = grafo->graus[i];
        if (grafo->graus[i] > grau_max)
            grau_max = grafo->graus[i];
        soma_graus += grafo->graus[i];
    }
    double grau_medio = (double)soma_graus / grafo->num_vertices;

    int *graus_ordenados = (int *)malloc(grafo->num_vertices * sizeof(int));
    for (int i = 0; i < grafo->num_vertices; i++)
    {
        graus_ordenados[i] = grafo->graus[i];
    }
    qsort(graus_ordenados, grafo->num_vertices, sizeof(int), comparar_inteiros);

    double mediana_grau;
    if (grafo->num_vertices % 2 == 0)
    {
        mediana_grau = (graus_ordenados[grafo->num_vertices / 2 - 1] + graus_ordenados[grafo->num_vertices / 2]) / 2.0;
    }
    else
    {
        mediana_grau = graus_ordenados[grafo->num_vertices / 2];
    }
    free(graus_ordenados);

    fprintf(arquivo_saida, "Grau mínimo: %d\n", grau_min);
    fprintf(arquivo_saida, "Grau máximo: %d\n", grau_max);
    fprintf(arquivo_saida, "Grau médio: %.2f\n", grau_medio);
    fprintf(arquivo_saida, "Mediana do grau: %.2f\n", mediana_grau);

    // Encontrar e salvar componentes conexas
    bool *visitados = (bool *)calloc(grafo->num_vertices, sizeof(bool));
    int num_componentes = 0;
    fprintf(arquivo_saida, "Componentes Conexas:\n");

    for (int i = 0; i < grafo->num_vertices; i++)
    {
        if (!visitados[i])
        {
            num_componentes++;
            fprintf(arquivo_saida, "Componente %d: ", num_componentes);

            int *pilha = (int *)malloc(grafo->num_vertices * sizeof(int));
            int topo = -1;
            int tamanho_componente = 0;

            pilha[++topo] = i;
            visitados[i] = true;

            while (topo >= 0)
            {
                int vertice = pilha[topo--];
                fprintf(arquivo_saida, "%d ", vertice);
                tamanho_componente++;

                NoListaAdj *adj = grafo->lista_adj[vertice];
                while (adj != NULL)
                {
                    if (!visitados[adj->vertice])
                    {
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

void busca_em_largura(Grafo *grafo, int vertice_inicial, int *pais)
{
    // Ajuste para indexação correta (de 1-based para 0-based)
    vertice_inicial--;

    // Aloca memória para os arrays de visitados e fila
    bool *visitados = (bool *)calloc(grafo->num_vertices, sizeof(bool));
    int *fila = (int *)malloc(grafo->num_vertices * sizeof(int));
    int inicio = 0, fim = 0;

    // Marca o vértice inicial como visitado, adiciona à fila e define o pai como -1 (raiz)
    visitados[vertice_inicial] = true;
    fila[fim++] = vertice_inicial;
    pais[vertice_inicial] = -1;

    // Loop principal da BFS
    while (inicio < fim)
    {
        int v = fila[inicio++];
        NoListaAdj *adj = grafo->lista_adj[v];
        while (adj != NULL)
        {
            if (!visitados[adj->vertice])
            {
                visitados[adj->vertice] = true;
                pais[adj->vertice] = v + 1; // Mapeando de volta para 1-based na hora de exibir
                fila[fim++] = adj->vertice;
                printf("BFS: Visitando vértice %d a partir do vértice %d\n", adj->vertice + 1, v + 1);
            }
            adj = adj->proximo;
        }
    }

    // Libera memória alocada
    free(visitados);
    free(fila);
}
void busca_em_profundidade(Grafo *grafo, int vertice_inicial, int *pais)
{
    bool *visitados = (bool *)calloc(grafo->num_vertices, sizeof(bool));
    int *pilha = (int *)malloc(grafo->num_vertices * sizeof(int));
    int topo = -1;

    pilha[++topo] = vertice_inicial;
    visitados[vertice_inicial] = true;
    pais[vertice_inicial] = -1;

    while (topo >= 0)
    {
        int v = pilha[topo--];
        NoListaAdj *adj = grafo->lista_adj[v];
        while (adj != NULL)
        {
            if (!visitados[adj->vertice])
            {
                visitados[adj->vertice] = true;
                pais[adj->vertice] = v;
                pilha[++topo] = adj->vertice;
            }
            adj = adj->proximo;
        }
    }

    free(visitados);
    free(pilha);
}

int calcular_distancia(Grafo *grafo, int origem, int destino)
{
    // Ajuste de indexação de 1-based para 0-based
    origem--;
    destino--;

    if (origem == destino)
        return 0;

    // Verificar se os vértices origem e destino são válidos
    if (origem < 0 || origem >= grafo->num_vertices || destino < 0 || destino >= grafo->num_vertices)
    {
        return -1; // Vértice inválido
    }

    // Alocação dos arrays de visitados e distâncias
    bool *visitados = (bool *)calloc(grafo->num_vertices, sizeof(bool));
    int *distancia = (int *)calloc(grafo->num_vertices, sizeof(int));
    int *fila = (int *)malloc(grafo->num_vertices * sizeof(int));
    int inicio = 0, fim = 0;

    // Marcar o vértice de origem como visitado e inicializar sua distância
    visitados[origem] = true;
    fila[fim++] = origem;
    distancia[origem] = 0;

    // Loop principal da BFS
    while (inicio < fim)
    {
        int v = fila[inicio++];

        NoListaAdj *adj = grafo->lista_adj[v];
        while (adj != NULL)
        {
            if (!visitados[adj->vertice])
            {
                visitados[adj->vertice] = true;
                distancia[adj->vertice] = distancia[v] + 1; // Incrementa a distância para a camada seguinte
                fila[fim++] = adj->vertice;

                // Verificar se o vértice destino foi alcançado
                if (adj->vertice == destino)
                {
                    int dist = distancia[destino];
                    free(visitados);
                    free(distancia);
                    free(fila);
                    return dist;
                }
            }
            adj = adj->proximo;
        }
    }

    // Limpeza de memória e retorno em caso de caminho não encontrado
    free(visitados);
    free(distancia);
    free(fila);
    return -1; // Retorna -1 se não houver caminho entre origem e destino
}

int calcular_diametro(Grafo *grafo)
{
    int diametro = 0;
    for (int i = 0; i < grafo->num_vertices; i++)
    {
        for (int j = i + 1; j < grafo->num_vertices; j++)
        {
            int dist = calcular_distancia(grafo, i, j);
            if (dist > diametro)
            {
                diametro = dist;
            }
        }
    }
    printf("Diâmetro do grafo: %d\n", diametro);
    return diametro;
}

void encontrar_componentes_conexas(Grafo *grafo)
{
    bool *visitados = (bool *)calloc(grafo->num_vertices, sizeof(bool));
    int num_componentes = 0;

    printf("Componentes Conexas:\n");
    for (int i = 0; i < grafo->num_vertices; i++)
    {
        if (!visitados[i])
        {
            num_componentes++;
            printf("Componente %d: ", num_componentes);

            int *pilha = (int *)malloc(grafo->num_vertices * sizeof(int));
            int topo = -1;

            pilha[++topo] = i;
            visitados[i] = true;

            while (topo >= 0)
            {
                int vertice = pilha[topo--];
                printf("%d ", vertice);

                NoListaAdj *adj = grafo->lista_adj[vertice];
                while (adj != NULL)
                {
                    if (!visitados[adj->vertice])
                    {
                        visitados[adj->vertice] = true;
                        pilha[++topo] = adj->vertice;
                    }
                    adj = adj->proximo;
                }
            }
            printf("\n");
            free(pilha);
        }
    }

    printf("Número total de componentes conexas: %d\n", num_componentes);
    free(visitados);
}

void liberar_grafo(Grafo *grafo) {
    if (grafo->lista_adj) {
        for (int i = 0; i < grafo->num_vertices; i++) {
            NoListaAdj *temp = grafo->lista_adj[i];
            while (temp) {
                NoListaAdj *prox = temp->proximo;
                free(temp);
                temp = prox;
            }
        }
        free(grafo->lista_adj);
    }
    
    if (grafo->matriz_adj) {
        for (int i = 0; i < grafo->num_vertices; i++) {
            free(grafo->matriz_adj[i]);
        }
        free(grafo->matriz_adj);
    }
    
    free(grafo->graus);
    free(grafo);
}
