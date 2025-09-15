#include <stdio.h>
#include <stdlib.h>

#define MAXN 20005
#define MAXLOG 20
#define MAXM 100005

typedef struct {
    int u, v, peso;
} Aresta;

typedef struct {
    int no, peso;
} Vizinho;

Aresta arestas[MAXM];
Vizinho grafo[2*MAXM];
int inicio[MAXN], prox[2*MAXM], tot;

// Busca união
int pai[MAXN], rankUF[MAXN];

int encontrar(int x) {
    if (pai[x] != x) pai[x] = encontrar(pai[x]);
    return pai[x];
}

int unir(int a, int b) {
    int ra = encontrar(a), rb = encontrar(b);
    if (ra == rb) return 0;
    if (rankUF[ra] < rankUF[rb]) pai[ra] = rb;
    else if (rankUF[ra] > rankUF[rb]) pai[rb] = ra;
    else { pai[rb] = ra; rankUF[ra]++; }
    return 1;
}

// Adiciona aresta na l.a.
void addAresta(int u, int v, int p) {
    grafo[tot].no = v;
    grafo[tot].peso = p;
    prox[tot] = inicio[u];
    inicio[u] = tot++;
}

int cmp(const void *a, const void *b) {
    return ((Aresta*)b)->peso - ((Aresta*)a)->peso; 
}

// LCA
int nivel[MAXN];
int up[MAXN][MAXLOG];
int menor[MAXN][MAXLOG];

void dfs(int u, int p, int peso) {
    up[u][0] = p;
    menor[u][0] = peso;
    for (int k = 1; k < MAXLOG; k++) {
        if (up[u][k-1] == -1) {
            up[u][k] = -1;
            menor[u][k] = menor[u][k-1];
        } else {
            up[u][k] = up[ up[u][k-1] ][k-1];
            if (up[u][k] != -1)
                menor[u][k] = menor[u][k-1] < menor[ up[u][k-1] ][k-1] ?
                               menor[u][k-1] : menor[ up[u][k-1] ][k-1];
            else
                menor[u][k] = menor[u][k-1];
        }
    }
    for (int i = inicio[u]; i != -1; i = prox[i]) {
        int v = grafo[i].no, p2 = grafo[i].peso;
        if (v == p) continue;
        nivel[v] = nivel[u] + 1;
        dfs(v, u, p2);
    }
}

int query(int u, int v) {
    if (u == v) return 1e9; 
    int ans = 1e9;
    if (nivel[u] < nivel[v]) { int tmp = u; u = v; v = tmp; }
    // Iguala nível
    int diff = nivel[u] - nivel[v];
    for (int k = MAXLOG-1; k >= 0; k--) {
        if (diff & (1<<k)) {
            if (menor[u][k] < ans) ans = menor[u][k];
            u = up[u][k];
        }
    }
    if (u == v) return ans;
    for (int k = MAXLOG-1; k >= 0; k--) {
        if (up[u][k] != -1 && up[u][k] != up[v][k]) {
            if (menor[u][k] < ans) ans = menor[u][k];
            if (menor[v][k] < ans) ans = menor[v][k];
            u = up[u][k];
            v = up[v][k];
        }
    }
    if (menor[u][0] < ans) ans = menor[u][0];
    if (menor[v][0] < ans) ans = menor[v][0];
    return ans;
}

int main() {
    int N, M, S;
    while (scanf("%d %d %d", &N, &M, &S) == 3) {
        for (int i = 1; i <= N; i++) {
            pai[i] = i;
            rankUF[i] = 0;
            inicio[i] = -1;
        }
        tot = 0;

        for (int i = 0; i < M; i++) {
            scanf("%d %d %d", &arestas[i].u, &arestas[i].v, &arestas[i].peso);
        }

        // Kruskal 
        qsort(arestas, M, sizeof(Aresta), cmp);
        for (int i = 0; i < M; i++) {
            if (unir(arestas[i].u, arestas[i].v)) {
                addAresta(arestas[i].u, arestas[i].v, arestas[i].peso);
                addAresta(arestas[i].v, arestas[i].u, arestas[i].peso);
            }
        }

        // Prepara LCA
        for (int i = 1; i <= N; i++) {
            for (int k = 0; k < MAXLOG; k++) {
                up[i][k] = -1;
                menor[i][k] = 1e9;
            }
        }
        nivel[1] = 0;
        dfs(1, -1, 1e9);

        // Consultas
        for (int i = 0; i < S; i++) {
            int sede, deposito;
            scanf("%d %d", &sede, &deposito);
            printf("%d\n", query(sede, deposito));
        }
    }
    return 0;
}

