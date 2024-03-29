typedef struct Bloco{
    int M;
}Bloco;

/**
 * Cria uma matriz de altitudes representando
 * o relevo de um mundo.
 **/
int **calcularAltitudes(int m, int n, int seed);

/**
 * Cria um novo mundo representado por uma matriz
 * de blocos alocada dinamicamente.
 **/
Bloco ***criarMundo(int m, int n, int **altitudes, int seed);

/**
 * Explora o mapa de um mundo e devolve o tempo
 * necessário para minerar todo ele. Além disso,
 * devolve a quantidade de alguns minérios de
 * interesse.
 **/
double explorarMundo(
    Bloco ***mundo, int m, int n, int **altitudes, double tempoPorBloco,
    int *qtdDiamante, int *qtdOuro, int *qtdFerro, int *qtdBlocos);

/**
 * Libera uma matriz de duas
 * dimensoes alocada dinamicamente.
 **/
void libera_matriz2d(int **matriz, int m);

/**
 * Libera matriz de tres dimensoes
 * de blocos alocada dinamicamente.
 **/
void libera_matriz3d(Bloco ***matriz, int m, int n);

