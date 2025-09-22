#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VERTICES 26

int num_vertices, num_arestas;
int grafo[MAX_VERTICES][MAX_VERTICES];
int visitado[MAX_VERTICES];
char componente[MAX_VERTICES];
int tamanho_componente;

// Função para comparação para qsort - função na biblioteca C para ordenação dos componentes ao final.
int comparar_char(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}

// DFS para encontrar componente conexo em grafo não direcionado
void dfs(int vertice) {
    visitado[vertice] = 1;
    componente[tamanho_componente++] = vertice + 'a';

    for (int v = 0; v < num_vertices; v++) {
        if (grafo[vertice][v] && !visitado[v]) {
            dfs(v);
        }
    }
}

int main() {
    int casos_teste;
    scanf("%d", &casos_teste);

    for (int caso = 1; caso <= casos_teste; caso++) {
        scanf("%d %d", &num_vertices, &num_arestas);

        memset(grafo, 0, sizeof(grafo));
        memset(visitado, 0, sizeof(visitado));

        for (int i = 0; i < num_arestas; i++) {
            char u, v;
            scanf(" %c %c", &u, &v);
            int origem = u - 'a';
            int destino = v - 'a';
            grafo[origem][destino] = 1;
            grafo[destino][origem] = 1; // grafo não direcionado
        }

        printf("Case #%d:\n", caso);
        int total_componentes = 0;

        for (int i = 0; i < num_vertices; i++) {
            if (!visitado[i]) {
                tamanho_componente = 0;
                dfs(i);
                qsort(componente, tamanho_componente, sizeof(char), comparar_char);
                for (int j = 0; j < tamanho_componente; j++) {
                    printf("%c,", componente[j]);
                }
                printf("\n");
                total_componentes++;
            }
        }

        printf("%d connected components\n\n", total_componentes);
    }

    return 0;
}
