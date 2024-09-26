#ifndef BIBLIOTECA_GRAFOS_H
#define BIBLIOTECA_GRAFOS_H

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

GrafoLista* cria_lista_adjacencia(int num_vertices);
GrafoMatriz* cria_matriz_adjacencia(int num_vertices);
void adiciona_aresta_lista(GrafoLista* grafo, int v1, int v2);
void adiciona_aresta_matriz(GrafoMatriz* grafo, int v1, int v2);
void libera_lista_adjacencia(GrafoLista* grafo);
void libera_matriz_adjacencia(GrafoMatriz* grafo);
void bfs_matriz_adjacencia(GrafoMatriz* grafo, int vertice_inicial, FILE* arquivo_saida);
void dfs_matriz_adjacencia(GrafoMatriz* grafo, int vertice_inicial, FILE* arquivo_saida);
void calcula_graus_matriz(GrafoMatriz* grafo, int* grau_min, int* grau_max, double* grau_medio, double* mediana_grau, int* num_arestas);
void calcula_graus(GrafoLista* grafo, int* grau_min, int* grau_max, double* grau_medio, double* mediana_grau, int* num_arestas);
void bfs_lista_adjacencia(GrafoLista* grafo, int vertice_inicial, FILE* arquivo_saida);
void processa_dfs_lista(GrafoLista* grafo, int vertice_inicial, FILE* arquivo_saida);
int processa_lista_adjacencia(const char* nome_arquivo, GrafoLista** grafo);
int processa_matriz_adjacencia(const char* nome_arquivo, GrafoMatriz** grafo);
int bfs_calcula_distancia_entre_vertices(GrafoLista* grafo, int vertice_inicial, int vertice_final);

#endif // BIBLIOTECA_GRAFOS_H
