#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define TAMANHO 8
#define INFINITO INT_MAX

/*
Aqui, vamos usar o Djikstra, entrando o nó inicial e o nó destino, 
encontrando o menor caminho entre eles.

1. Ler a entrada no formato (e2 e4), por exemplo

2. Inicializar estrutura de grafo que represente um tabuleiro de xadrez 
(64 casas, a1-a8, b1-b8...)

3. Realizar o movimento em L do cavalo: como ele vai percorrer 
o grafo no código?

4. Contar o número de vezes em que o cavalo se moveu ou não

O cavalo pode-se movimentar na seguinte maneira:

    8  . . . . . . . .
    7  . . . . . . . .
    6  . . 1 . 2 . . .
    5  . 3 . . . 4 . .
    4  . . . N . . . .
    3  . 5 . . . 6 . .
    2  . . 7 . 8 . . .
    1  . . . . . . . .
       a b c d e f g h

    , que deve ser representado no código.
    Dada uma posição qualquer, ele pode se mover uma linha acima ou abaixo + 2 pra direita e esquerda
    OU                                  se move duas linhas acima ou abaixo, +1 pra direita ou esquerda. (8 possibilidades)
    Trabalhando diretamente com isso, podemos representar os movimentos do cavalo por dois vetores, Dx e Dy.
*/


// Estrutura para representar uma posição no tabuleiro
typedef struct {
    int x, y;
    int distancia;
} Posicao;

// Verifica se a posição está dentro dos limites do tabuleiro
int posicao_valida(int x, int y) {
    return (x >= 0 && x < TAMANHO && y >= 0 && y < TAMANHO);
}

int dijkstra(int origem_x, int origem_y, int destino_x, int destino_y) {
    // Vetores de movimento do cavalo
    int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
    int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};

    int distancia[TAMANHO][TAMANHO];
    int visitado[TAMANHO][TAMANHO] = {0};

    // Inicializa todas as distâncias como infinito
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            distancia[i][j] = INFINITO;
        }
    }

    // Distância da origem para ela mesma é 0
    distancia[origem_x][origem_y] = 0;

    while (1) {
        // Encontra a posição não visitada com menor distância
        int menor_distancia = INFINITO;
        int atual_x = -1, atual_y = -1;

        for (int i = 0; i < TAMANHO; i++) {
            for (int j = 0; j < TAMANHO; j++) {
                if (!visitado[i][j] && distancia[i][j] < menor_distancia) {
                    menor_distancia = distancia[i][j];
                    atual_x = i;
                    atual_y = j;
                }
            }
        }

        // Se não há mais posições a visitar ou já chegamos ao destino
        if (atual_x == -1) break;
        if (atual_x == destino_x && atual_y == destino_y) break;

        visitado[atual_x][atual_y] = 1;

        // Verifica todos os 8 movimentos possíveis do cavalo
        for (int i = 0; i < 8; i++) {
            int novo_x = atual_x + dx[i];
            int novo_y = atual_y + dy[i];

            if (posicao_valida(novo_x, novo_y) && !visitado[novo_x][novo_y]) {
                if (distancia[atual_x][atual_y] + 1 < distancia[novo_x][novo_y]) {
                    distancia[novo_x][novo_y] = distancia[atual_x][atual_y] + 1;
                }
            }
        }
    }

    return distancia[destino_x][destino_y];
}

int main() {
    char origem_str[3], destino_str[3];

    while (scanf("%s %s", origem_str, destino_str) != EOF) {
        // Converte as coordenadas do formato "e2" para índices de matriz
        int origem_x = origem_str[0] - 'a';
        int origem_y = origem_str[1] - '1';
        int destino_x = destino_str[0] - 'a';
        int destino_y = destino_str[1] - '1';

        // Calcula a menor quantidade de movimentos
        int movimentos = dijkstra(origem_x, origem_y, destino_x, destino_y);

        // Imprime o resultado
        printf("To get from %s to %s takes %d knight moves.\n", origem_str, destino_str, movimentos);
    }

    return 0;
}