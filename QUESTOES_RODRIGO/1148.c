
/* 
 O que vai definir essa questão é entender quais cidades são parte do mesmo país, ou seja, entender
 quais cidades um país compreende. Há cidades que não possuem conexão entre elas - isso é garantia de que 
 não pertencem ao mesmo país. Podemos entender o problema como a definição de componentes fortemente conectados,
 estes sendo países, e a verificação de conexão entre cidades quaisquer, dado um número de conexões.

 Na entrada, é definido o número de cidades, número de permissões de enviar carta, as cartas a serem enviadas e o número de horas
 para entrega. Após entender a entrada, vamos montar uma matriz de conexão entre as cidades, e utilizar de Djikstra para encontrar
 o menor caminho.

 */

// Estrutura para armazenar uma aresta do grafo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CIDADES 505
#define INFINITO 1000000000

// Arestas do grafo
typedef struct Aresta {
    int destino;
    int peso;
    struct Aresta *proxima;
} Aresta;

Aresta *listaAdj[MAX_CIDADES];
Aresta *listaAdjTransposta[MAX_CIDADES];

int numCidades, numArestas;
int visitado[MAX_CIDADES];
int ordem[MAX_CIDADES], tamanhoOrdem;
int componente[MAX_CIDADES];

// Definições de operações da Heap 
typedef struct {
    int cidade;
    int distancia;
} NodoHeap;

NodoHeap heap[MAX_CIDADES * MAX_CIDADES];
int tamanhoHeap;

void trocar(int i, int j) {
    NodoHeap temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void shiftup(int i) {
    while (i > 1 && heap[i].distancia < heap[i / 2].distancia) {
        trocar(i, i / 2);
        i /= 2;
    }
}

void heapify(int i) {
    int menor = i;
    int esq = 2 * i, dir = 2 * i + 1;
    if (esq <= tamanhoHeap && heap[esq].distancia < heap[menor].distancia)
        menor = esq;
    if (dir <= tamanhoHeap && heap[dir].distancia < heap[menor].distancia)
        menor = dir;
    if (menor != i) {
        trocar(i, menor);
        heapify(menor);
    }
}

void inserirHeap(int cidade, int distancia) {
    heap[++tamanhoHeap].cidade = cidade;
    heap[tamanhoHeap].distancia = distancia;
    shiftup(tamanhoHeap);
}

NodoHeap extrairMinimo() {
    NodoHeap min = heap[1];
    heap[1] = heap[tamanhoHeap--];
    heapify(1);
    return min;
}

Aresta* criarAresta(int destino, int peso, Aresta* proximo) {
    Aresta* nova = (Aresta*) malloc(sizeof(Aresta));
    nova->destino = destino;
    nova->peso = peso;
    nova->proxima = proximo;
    return nova;
}

void dfsNumbering(int u) {
    visitado[u] = 1;
    for (Aresta* a = listaAdj[u]; a; a = a->proxima)
        if (!visitado[a->destino])
            dfsEmpilharOrdem(a->destino);
    ordem[tamanhoOrdem++] = u;
}

void dfsMarcarComponente(int u, int id) {
    visitado[u] = 1;
    componente[u] = id;
    for (Aresta* a = listaAdjTransposta[u]; a; a = a->proxima)
        if (!visitado[a->destino])
            dfsMarcarComponente(a->destino, id);
}

// Dijkstra com heap para componentes
int dijkstra(int origem, int destino, int totalComp, int matrizAdj[][MAX_CIDADES]) {
    int dist[MAX_CIDADES];
    for (int i = 1; i <= totalComp; i++) dist[i] = INFINITO;
    dist[origem] = 0;
    tamanhoHeap = 0;
    inserirHeap(origem, 0);

    while (tamanhoHeap > 0) {
        NodoHeap atual = extrairMinimo();
        int u = atual.cidade;
        if (u == destino) return dist[u];
        if (atual.distancia > dist[u]) continue;

        for (int v = 1; v <= totalComp; v++) {
            if (matrizAdj[u][v] < INFINITO && dist[u] + matrizAdj[u][v] < dist[v]) {
                dist[v] = dist[u] + matrizAdj[u][v];
                inserirHeap(v, dist[v]);
            }
        }
    }

    return INFINITO;
}

int main() {
    while (1) {
        if (scanf("%d %d", &numCidades, &numArestas) != 2) break;
        if (numCidades == 0 && numArestas == 0) break;

        for (int i = 1; i <= numCidades; i++) {
            Aresta *aux;
            while (listaAdj[i]) {
                aux = listaAdj[i];
                listaAdj[i] = aux->proxima;
                free(aux);
            }
            while (listaAdjTransposta[i]) {
                aux = listaAdjTransposta[i];
                listaAdjTransposta[i] = aux->proxima;
                free(aux);
            }
            listaAdj[i] = NULL;
            listaAdjTransposta[i] = NULL;
        }

        for (int i = 0; i < numArestas; i++) {
            int origem, destino, peso;
            scanf("%d %d %d", &origem, &destino, &peso);
            listaAdj[origem] = criarAresta(destino, peso, listaAdj[origem]);
            listaAdjTransposta[destino] = criarAresta(origem, peso, listaAdjTransposta[destino]);
        }

        memset(visitado, 0, sizeof(visitado));
        tamanhoOrdem = 0;
        for (int i = 1; i <= numCidades; i++)
            if (!visitado[i])
                dfsNumbering(i);

        memset(visitado, 0, sizeof(visitado));
        int totalComponentes = 0;
        for (int i = tamanhoOrdem - 1; i >= 0; i--) {
            int v = ordem[i];
            if (!visitado[v])
                dfsMarcarComponente(v, ++totalComponentes);
        }

        int matrizAdj[MAX_CIDADES][MAX_CIDADES];
        for (int i = 1; i <= totalComponentes; i++)
            for (int j = 1; j <= totalComponentes; j++)
                matrizAdj[i][j] = (i == j ? 0 : INFINITO);

        for (int u = 1; u <= numCidades; u++) {
            for (Aresta* a = listaAdj[u]; a; a = a->proxima) {
                int v = a->destino;
                int compU = componente[u];
                int compV = componente[v];
                if (compU != compV && a->peso < matrizAdj[compU][compV])
                    matrizAdj[compU][compV] = a->peso;
            }
        }

        int consultas;
        scanf("%d", &consultas);
        for (int i = 0; i < consultas; i++) {
            int origem, destino;
            scanf("%d %d", &origem, &destino);
            int compOrigem = componente[origem];
            int compDestino = componente[destino];

            if (compOrigem == compDestino) {
                printf("0\n");
            } else {
                int resposta = dijkstra(compOrigem, compDestino, totalComponentes, matrizAdj);
                if (resposta == INFINITO)
                    printf("Nao e possivel entregar a carta\n");
                else
                    printf("%d\n", resposta);
            }
        }
        printf("\n");
    }

    return 0;
}
