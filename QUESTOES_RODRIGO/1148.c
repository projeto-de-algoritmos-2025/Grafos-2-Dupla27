#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CIDADES 505
#define INFINITO 1000000000

/* 
 O que vai definir essa questão é entender quais cidades são parte do mesmo país, ou seja, entender
 quais cidades um país compreende. Há cidades que não possuem conexão entre elas - isso é garantia de que 
 não pertencem ao mesmo país. Podemos entender o problema como a definição de componentes fortemente conectados,
 estes sendo países, e a verificação de conexão entre cidades quaisquer, dado um número de conexões.

 Na entrada, é definido o número de cidades, número de permissões de enviar carta, as cartas a serem enviadas e o número de horas
 para entrega. Por fim, precisamos também pegar o tempo mínimo para entrega, então após entender a entrada, vamos montar uma matriz
 de conexão entre as cidades.

 */

// Estrutura para armazenar uma aresta do grafo
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *proxima;
} Aresta;

// Listas de adjacências
Aresta *listaAdj[MAX_CIDADES];    
Aresta *listaAdjTransposta[MAX_CIDADES];  

int numCidades, numArestas;

int visitado[MAX_CIDADES];
int ordem[MAX_CIDADES], tamanhoOrdem;
int componente[MAX_CIDADES];

// Função para criar uma nova aresta
Aresta* criarAresta(int destino, int peso, Aresta* proximo) {
    Aresta* novaAresta = (Aresta*) malloc(sizeof(Aresta));
    novaAresta->destino = destino;
    novaAresta->peso = peso;
    novaAresta->proxima = proximo;
    return novaAresta;
}

// Primeiro DFS: empilha vértices na ordem de saída
void dfsEmpilharOrdem(int u) {
    visitado[u] = 1;
    for (Aresta* a = listaAdj[u]; a != NULL; a = a->proxima) {
        if (!visitado[a->destino]) {
            dfsEmpilharOrdem(a->destino);
        }
    }
    ordem[tamanhoOrdem++] = u;
}

// Segundo DFS: marca os vértices de uma mesma componente fortemente conexa
void dfsMarcarComponente(int u, int idComponente) {
    visitado[u] = 1;
    componente[u] = idComponente;
    for (Aresta* a = listaAdjTransposta[u]; a != NULL; a = a->proxima) {
        if (!visitado[a->destino]) {
            dfsMarcarComponente(a->destino, idComponente);
        }
    }
}

int main() {
    while (1) {
        if (scanf("%d %d", &numCidades, &numArestas) != 2) break;
        if (numCidades == 0 && numArestas == 0) break;

        // Liberar memória e resetar listas
        for (int i = 1; i <= numCidades; i++) {
            Aresta *aux;
            while (listaAdj[i] != NULL) {
                aux = listaAdj[i];
                listaAdj[i] = aux->proxima;
                free(aux);
            }
            while (listaAdjTransposta[i] != NULL) {
                aux = listaAdjTransposta[i];
                listaAdjTransposta[i] = aux->proxima;
                free(aux);
            }
            listaAdj[i] = NULL;
            listaAdjTransposta[i] = NULL;
        }

        // Ler as arestas e construir grafo e grafo transposto
        for (int i = 0; i < numArestas; i++) {
            int origem, destino, peso;
            scanf("%d %d %d", &origem, &destino, &peso);
            listaAdj[origem] = criarAresta(destino, peso, listaAdj[origem]);
            listaAdjTransposta[destino] = criarAresta(origem, peso, listaAdjTransposta[destino]);
        }

        // 1) DFS no grafo original para empilhar vértices por ordem de saída
        memset(visitado, 0, sizeof(visitado));
        tamanhoOrdem = 0;
        for (int i = 1; i <= numCidades; i++) {
            if (!visitado[i]) {
                dfsEmpilharOrdem(i);
            }
        }

        // 2) DFS no grafo transposto para marcar componentes
        memset(visitado, 0, sizeof(visitado));
        int totalComponentes = 0;
        for (int i = tamanhoOrdem - 1; i >= 0; i--) {
            int v = ordem[i];
            if (!visitado[v]) {
                totalComponentes++;
                dfsMarcarComponente(v, totalComponentes);
            }
        }

        // Construir matriz de distâncias entre as componentes
        int dist[MAX_CIDADES][MAX_CIDADES];
        for (int i = 1; i <= totalComponentes; i++) {
            for (int j = 1; j <= totalComponentes; j++) {
                dist[i][j] = (i == j) ? 0 : INFINITO;
            }
        }

        // Atualizar distâncias diretas entre componentes (arestas entre componentes fortemente conectados)
        for (int u = 1; u <= numCidades; u++) {
            for (Aresta* a = listaAdj[u]; a != NULL; a = a->proxima) {
                int v = a->destino;
                int compU = componente[u];
                int compV = componente[v];
                if (compU != compV) {
                    if (a->peso < dist[compU][compV]) {
                        dist[compU][compV] = a->peso;
                    }
                }
            }
        }

        // Floyd-Warshall para calcular menores distâncias entre componentes
        for (int k = 1; k <= totalComponentes; k++) {
            for (int i = 1; i <= totalComponentes; i++) {
                if (dist[i][k] == INFINITO) continue;
                for (int j = 1; j <= totalComponentes; j++) {
                    if (dist[k][j] == INFINITO) continue;
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }

        int consultas;
        scanf("%d", &consultas);

        for (int i = 0; i < consultas; i++) {
            int origem, destino;
            scanf("%d %d", &origem, &destino);
            int compOrigem = componente[origem];
            int compDestino = componente[destino];
            int resposta = dist[compOrigem][compDestino];
            if (resposta == INFINITO) {
                printf("Nao e possivel entregar a carta\n");
            } else {
                printf("%d\n", resposta);
            }
        }
        printf("\n"); // Linha em branco após cada caso
    }

    return 0;
}
