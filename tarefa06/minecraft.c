#include <stdio.h>
#include <stdlib.h>
#include "minecraft.h"

int **calcularAltitudes(int m, int n, int seed){
    int x, z, **altitudes, altitude;

    //alocando a matriz
    altitudes = malloc(m * sizeof(int *));
    for (x = 0; x < m; x++)
        altitudes[x] = malloc(n * sizeof(int));
    
    //calculando as altitudes
    for (x = 0; x < m; x++)
        for (z = 0; z < n; z++){
            altitude = (seed * (202 + x + z) + 12345 + x + z) % 256;
            altitudes[x][z] = altitude;
        }
        
    return altitudes;
}

Bloco ***criarMundo(int m, int n, int **altitudes, int seed) {
    Bloco ***mundo;
    int x, z, y;

    //alocando a matriz 3d
    mundo = malloc(m * sizeof(Bloco **));
    for (x = 0; x < m; x++) {
        mundo[x] = malloc(n * sizeof(Bloco *));
        for (z = 0; z < n; z++)
            mundo[x][z] = malloc(256 * sizeof(Bloco));
    }

    //adicionando os tipos de bloco
    for (x = 0; x < m; x++)
        for (z = 0; z < n; z++)
            for (y = 0; y < 256; y++) {
                if (y > altitudes[x][z]) 
                    mundo[x][z][y].M = 21;
                else 
                    mundo[x][z][y].M = (seed * (202 + x + z + y) + x + z + y) % 33;
            }

    return mundo;
}

double explorarMundo(
    Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco,
    int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos) {

    int x, y, z;
    double tempo_total;

    for (x = 0; x < m; x++)
        for (z = 0; z < n; z++) 
            for (y = 0; y < 256; y++) {
                if (mundo[x][z][y].M < 21) //blocos que não sao vazios, agua ou lava
                    *qtdBlocos += 1;
                if (mundo[x][z][y].M == 0)
                    *qtdDiamante += 1;
                else if (mundo[x][z][y].M == 1 || mundo[x][z][y].M == 2)
                    *qtdOuro += 1;
                else if (mundo[x][z][y].M == 3 || mundo[x][z][y].M == 4 || mundo[x][z][y].M == 5)
                    *qtdFerro += 1;
                else
                    continue;
            }
            
    tempo_total = tempoPorBloco * *qtdBlocos;
    return tempo_total;
}

void libera_matriz2d(int **matriz, int m) {
    for (int x = 0; x < m; x++)
        free(matriz[x]);
    free(matriz);
}

void libera_matriz3d(Bloco ***matriz, int m, int n) {
    for (int x = 0; x < m; x++)
        for (int z = 0; z < n; z++)
            free(matriz[x][z]);

    for (int x = 0; x < m; x++)
        free(matriz[x]);

    free(matriz);
}

int main() {
    int m, n, seed, **altitudes;
    double tempoPorBloco, tempo_total;
    Bloco ***mundo;
    int qtdDiamante=0, qtdOuro=0, qtdFerro=0, qtdBlocos=0;

    scanf("%d %d", &m, &n);
    scanf("%d", &seed);
    scanf("%lf", &tempoPorBloco);

    altitudes = calcularAltitudes(m, n, seed);
    mundo = criarMundo(m, n, altitudes, seed);
    tempo_total = explorarMundo(mundo, m, n, altitudes, tempoPorBloco, &qtdDiamante, &qtdOuro, &qtdFerro, &qtdBlocos);

    printf("Total de Blocos: %d\n", qtdBlocos);
    printf("Tempo total: %.2lfs\n", tempo_total);
    printf("Diamantes: %d\n", qtdDiamante);
    printf("Ouros: %d\n", qtdOuro);
    printf("Ferros: %d\n", qtdFerro);

    libera_matriz2d(altitudes, m);
    libera_matriz3d(mundo, m, n);

    return 0;
}