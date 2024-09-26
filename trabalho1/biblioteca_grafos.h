// biblioteca_grafos.h
#ifndef BIBLIOTECA_GRAFOS_H
#define BIBLIOTECA_GRAFOS_H

#include <stdio.h>
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

typedef struct GrafoMatriz {
    int num_vertices;
    int** matriz_adjacencia;
} GrafoMatriz;

// Funções de criação e manipulação do grafo
GrafoLista* cria_lista_adjacencia(int num_vertices);
GrafoMatriz* cria_matriz_adjacencia(int num_vertices);
void adiciona_aresta_lista(GrafoLista* grafo, int v1, int v2);
void adiciona_aresta_matriz(GrafoMatriz* grafo, int v1, int v2);
void libera_lista_adjacencia(GrafoLista* grafo);
void libera_matriz_adjacencia(GrafoMatriz* grafo);

// Funções de busca
void bfs_lista_adjacencia(GrafoLista* grafo, int vertice_inicial, FILE* arquivo_saida);
void bfs_matriz_adjacencia(GrafoMatriz* grafo, int vertice_inicial, FILE* arquivo_saida);
void dfs_lista_adjacencia(GrafoLista* grafo, int vertice_inicial, int* pai, int* nivel, bool* visitado, int nivel_atual);
void dfs_matriz_adjacencia_aux(GrafoMatriz* grafo, int vertice, bool* visitado);
void dfs_matriz_adjacencia(GrafoMatriz* grafo, int vertice_inicial, FILE* arquivo_saida);

// Funções para processar arquivos
int processa_lista_adjacencia(const char* nome_arquivo, GrafoLista** grafo);
int processa_matriz_adjacencia(const char* nome_arquivo, GrafoMatriz** grafo);

#endif
