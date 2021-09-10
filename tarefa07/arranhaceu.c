#include <stdio.h>
#include <stdlib.h>

int **aloca_matriz_quadrada(int n) {
    int **matriz, i;
    matriz = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++) 
        matriz[i] = malloc(n * sizeof(int));
    return matriz;
}

void libera_matriz_quadrada(int **matriz, int n) {
    for (int i = 0; i < n; i++)
        free(matriz[i]);
    free(matriz);
}

int maior_elemento(int **matriz, int n, int linha, int coluna, char config[]) {
    int i, maior, indice;
    if (config[0] == 'l') {
        maior = matriz[linha][coluna];
        indice = coluna;
        for (i = coluna; i < n; i++) {
            if (matriz[linha][i] > maior) {
                maior = matriz[linha][i];
                indice = i;
            }
        }     
    }
    if (config[0] == 'c') {
        maior = matriz[linha][coluna];
        indice = linha;
        for (i = linha; i < n; i++) {      //fazer função dessa parte?
            if (matriz[i][coluna] > maior) {
                maior = matriz[i][coluna];
                indice = i;
            }
        }
    }

    return indice;
}

int arranha_ceus_visiveis(int **quebra_cabeca, int n, int linha, int coluna, char config[], int qtd) {
    int indice_maior;

    if (config[0] == 'l') {
        if (config[1] == 'e') {
            indice_maior = maior_elemento(quebra_cabeca, n, linha, 1, config); //nesse caso a linha é smp a msm
            qtd += 1; //passar por referencia?
            if (indice_maior == 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, indice_maior, linha, 1, config, qtd);
        }
        else { // (config[1] == 'd') {
            indice_maior = maior_elemento(quebra_cabeca, n, linha, coluna, config); //nesse caso a linha é smp a msm
            qtd += 1; //passar por referencia?
            if (indice_maior == n - 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, n, linha, indice_maior + 1, config, qtd);
        }
    }

    else { //(config[0] == 'c') {
        if (config[1] == 'c') {
            indice_maior = maior_elemento(quebra_cabeca, n, linha, coluna, config); //nesse caso a coluna é smp a msm
            qtd += 1; //passar por referencia?
            if (indice_maior == 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, indice_maior, 1, coluna, config, qtd);
        }
        else { // (config[1] == 'b') {
            indice_maior = maior_elemento(quebra_cabeca, n, linha, coluna, config); //nesse caso a coluna é smp a msm
            qtd += 1; //passar por referencia?
            if (indice_maior == n - 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, n, indice_maior + 1, coluna, config, qtd);
        }
    }
}

int pode_inserir(int **matriz, int l, int c, int n, int elemento) {
    int i, qtd;
    char config[2];

    for (i = 1; i <= n; i++) {
        if (matriz[l][i] == elemento)
            return 0; //falso
        if (matriz[i][c] == elemento)
            return 0;
    }

    if (c == n) { //ultimo elemento da linha, ou seja, esta preenchida
        config[0] = 'l';
        config[1] = 'e';
        matriz[l][c] = elemento; //colocando o elemento pra testar se é possivel na função
        qtd = arranha_ceus_visiveis(matriz, n + 1, l, 1, config, 0);
        if (matriz[l][0] != qtd) { //vendo se condiz com o valor da pista
            matriz[l][c] = 0;
            return 0;
        }
        config[1] = 'd';
        qtd = 0;
        qtd = arranha_ceus_visiveis(matriz, n + 1, l, 1, config, 0);
        if (matriz[l][n + 1] != qtd) {
            matriz[l][c] = 0;
            return 0;
        }
    }

    if (l == n) { //ultimo elemento da coluna, ou seja, esta preenchida
        config[0] = 'c';
        config[1] = 'c';
        matriz[l][c] = elemento;
        qtd = arranha_ceus_visiveis(matriz, n + 1, 1, c, config, 0);
        if (matriz[0][c] != qtd) {
            matriz[l][c] = 0;
            return 0;
        }
        config[1] = 'b';
        qtd = 0;
        qtd = arranha_ceus_visiveis(matriz, n + 1, 1, c, config, 0);
        if (matriz[n + 1][c] != qtd) {
            matriz[l][c] = 0;
            return 0;
        }
    }

    return 1;
}


void proxima_posicao(int l, int c, int *nl, int *nc, int n) {
    if (c < n) {
        *nl = l;
        *nc = c + 1;
    } else {
        *nl = l + 1;
        *nc = 1;
    }
}

int arranha_ceuR(int **quebra_cabeca, int l, int c, int n) {
    int nl, nc, elemento;

    if (l == n + 1) //fim do quebra cabeça
        return 1;
    
    proxima_posicao(l, c, &nl, &nc, n); 
    for (elemento = 1; elemento <= n; elemento++) {
        if (pode_inserir(quebra_cabeca, l, c, n, elemento)) {
            quebra_cabeca[l][c] = elemento;
            if (arranha_ceuR(quebra_cabeca, nl, nc, n))
                return 1;
        }
    }

    quebra_cabeca[l][c] = 0;
    return 0;
}

int main () {
    int n, **quebra_cabeca, dimensao, i, j;

    scanf("%d", &n);
    dimensao = n + 2; //considerando o espaço das pistas
    quebra_cabeca = aloca_matriz_quadrada(dimensao);
    for(i = 0; i < dimensao; i++)  //fazer a função aloca e le??
        for(j = 0; j < dimensao; j++) 
            scanf("%d", &quebra_cabeca[i][j]);

    if(arranha_ceuR(quebra_cabeca, 1, 1, n)) {
        for(i = 1; i <= n; i++) { //fazer função?
            for(j = 1; j <= n; j++) {
                printf("%d ", quebra_cabeca[i][j]);
            }
            printf("\n");
        }
    }

    libera_matriz_quadrada(quebra_cabeca, dimensao);

    return 0;
}

