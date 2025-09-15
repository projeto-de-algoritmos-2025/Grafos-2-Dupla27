#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Guarda uma aresta
typedef struct {
    int u, v;
    double peso;
} Aresta;

// Encontra união
typedef struct {
    int pai;
    int rank;
} Conjunto;

int encontrar(Conjunto cj[], int x) {
    if (cj[x].pai != x)
        cj[x].pai = encontrar(cj, cj[x].pai);
    return cj[x].pai;
}

// União de conjuntos
int unir(Conjunto cj[], int a, int b) {
    int ra = encontrar(cj, a);
    int rb = encontrar(cj, b);
    if (ra == rb) return 0;
    if (cj[ra].rank < cj[rb].rank) {
        cj[ra].pai = rb;
    } else if (cj[ra].rank > cj[rb].rank) {
        cj[rb].pai = ra;
    } else {
        cj[rb].pai = ra;
        cj[ra].rank++;
    }
    return 1;
}

int comparar(const void *a, const void *b) {
    double dif = ((Aresta*)a)->peso - ((Aresta*)b)->peso;
    if (dif < 0) return -1;
    else if (dif > 0) return 1;
    return 0;
}

int main() {
    int casos;
    scanf("%d", &casos);

    while (casos--) {
        int n;
        scanf("%d", &n);

        double x[n], y[n];
        for (int i = 0; i < n; i++) {
            scanf("%lf %lf", &x[i], &y[i]);
        }

        // Gera as arestas
        int m = n * (n - 1) / 2;
        Aresta *arestas = (Aresta*)malloc(m * sizeof(Aresta));

        int k = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                arestas[k].u = i;
                arestas[k].v = j;
                double dx = x[i] - x[j];
                double dy = y[i] - y[j];
                arestas[k].peso = sqrt(dx * dx + dy * dy);
                k++;
            }
        }

        qsort(arestas, m, sizeof(Aresta), comparar);

        // Inicializa encontro de união
        Conjunto cj[n];
        for (int i = 0; i < n; i++) {
            cj[i].pai = i;
            cj[i].rank = 0;
        }

        // Kruskal
        double total = 0.0;
        int usados = 0;
        for (int i = 0; i < m && usados < n - 1; i++) {
            if (unir(cj, arestas[i].u, arestas[i].v)) {
                total += arestas[i].peso;
                usados++;
            }
        }

        // Converte
        printf("%.2f\n", total / 100.0);

        free(arestas);
    }

    return 0;
}

