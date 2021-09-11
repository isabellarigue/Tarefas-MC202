#include <stdio.h>
#include <stdlib.h>

int **aloca_matriz_quadrada(int dimensao) {
    int **matriz, i;
    matriz = malloc(dimensao * sizeof(int *));
    for (i = 0; i < dimensao; i++) 
        matriz[i] = malloc(dimensao * sizeof(int));
    return matriz;
}

void libera_matriz_quadrada(int **matriz, int dimensao) {
    for (int i = 0; i < dimensao; i++)
        free(matriz[i]);
    free(matriz);
}

int maior_elemento(int **matriz, int max, int linha, int coluna, char config[]) {
    /* retorna o indice da coluna do maior elemento de uma linha ou o indice da linha do maior elemento de uma coluna;
       sendo int max o tamanho maximo da linha/coluna que se deseja analisar; int linha e int coluna os indices iniciais;
       char config[] são as configurações, no caso para definir se deseja analisar uma linha ou uma coluna.
     */
    int i, maior, indice;

    maior = matriz[linha][coluna];
    if (config[0] == 'l') {
        indice = coluna;
        for (i = coluna; i < max; i++) 
            if (matriz[linha][i] > maior) {
                maior = matriz[linha][i];
                indice = i;
            }   
    } else { // config[0] == 'c'
        indice = linha;
        for (i = linha; i < max; i++)      
            if (matriz[i][coluna] > maior) {
                maior = matriz[i][coluna];
                indice = i;
            }
    }
    return indice;
}

int arranha_ceus_visiveis(int **quebra_cabeca, int max, int linha, int coluna, char config[], int qtd) {
    /* retorna o numero de arranha ceus visiveis de acordo com a configuração pedida (vistos de cima pra baixo, da esquerda pra direita, etc); 
       nessa função primeiro encontra-se o maior elemento de uma linha/coluna, soma-se um na quantidade de edificios visiveis,
       após isso busca-se novamente o maior edificio dentre os que estavam a frente do ultimo maior, soma-se um na quantidade e assim segue recursivamente.
    */
    int indice_maior;

    indice_maior = maior_elemento(quebra_cabeca, max, linha, coluna, config); 
    qtd += 1;
    if (config[0] == 'l') { //nesse caso a linha é sempre a mesma
        if (config[1] == 'e') {
            if (indice_maior == 1) // significa que não há mais edificios a frente deste, e, portanto, chegamos na quantidade final
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, indice_maior, linha, 1, config, qtd);
        } else { //config[1] == 'd'
            if (indice_maior == max - 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, max, linha, indice_maior + 1, config, qtd);
        }
    } else { //config[0] == 'c', nesse caso a coluna é sempre a mesma
        if (config[1] == 'c') { 
            if (indice_maior == 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, indice_maior, 1, coluna, config, qtd);
        } else { //config[1] == 'b'
            if (indice_maior == max - 1)
                return qtd;
            else
               return arranha_ceus_visiveis(quebra_cabeca, max, indice_maior + 1, coluna, config, qtd);
        }
    }
}

int pode_inserir(int **matriz, int l, int c, int n, int elemento) {
    /* retorna 1 se é possivel inserir determinado elemento na posição matriz[l][c], respeitando as regras do quebra cabeça,
       caso contrário retorna 0.
    */
    int i, qtd=0;
    char config[2]; //definindo um vetor com as configurações a serem analisadas

    for (i = 1; i <= n; i++) { //verificando se há algum número igual ao elemento na mesma linha ou na mesma coluna
        if (matriz[l][i] == elemento)
            return 0; 
        if (matriz[i][c] == elemento)
            return 0;
    }

    //verificando se ao colocar determinado elemento, a linha/coluna está respeitando as condições das pistas
    //essa condição só é testada para linhas/colunas com todas as posições preenchidas
    if (c == n) { //ultimo elemento da linha, ou seja, esta preenchida
        config[0] = 'l'; //deseja-se analisar uma linha
        config[1] = 'e'; //deseja-se analisar da esquerda para direita
        matriz[l][c] = elemento; //colocando o elemento pra testar a viabilidade
        qtd = arranha_ceus_visiveis(matriz, n + 1, l, 1, config, qtd);
        if (matriz[l][0] != qtd) { //vendo se a quantidade (qtd) condiz com o valor da pista
            matriz[l][c] = 0;
            return 0;
        }
        config[1] = 'd'; //deseja-se analisar da direita para esquerda
        qtd = 0;
        qtd = arranha_ceus_visiveis(matriz, n + 1, l, 1, config, qtd);
        if (matriz[l][n + 1] != qtd) {
            matriz[l][c] = 0;
            return 0;
        }
    }

    if (l == n) { //ultimo elemento da coluna, ou seja, esta preenchida
        config[0] = 'c';  //deseja-se analisar uma coluna
        config[1] = 'c';  //deseja-se analisar de cima para baixo
        matriz[l][c] = elemento;
        qtd = arranha_ceus_visiveis(matriz, n + 1, 1, c, config, 0);
        if (matriz[0][c] != qtd) {
            matriz[l][c] = 0;
            return 0;
        }
        config[1] = 'b'; //deseja-se analisar de baixo para cima
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
    /*altera nl e nc para o indice da proxima posição após quebra_cabeca[l][c], desconsiderando a posição das pistas.*/
    if (c < n) {
        *nl = l;
        *nc = c + 1;
    } else {
        *nl = l + 1;
        *nc = 1;
    }
}

int arranha_ceuR(int **quebra_cabeca, int l, int c, int n) {
    /* resolve o quebra cabeça utilizando recursão e backtracking */
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

void imprime_matriz_quadrada(int **matriz, int n) {
    /* imprime a resolução do quebra cabeça, desconsiderando as linhas e colunas das pistas */
    int i, j;
    for(i = 1; i <= n; i++) { 
        for(j = 1; j <= n; j++)
            printf("%d ", matriz[i][j]);
        printf("\n");
    }
}

int main () {
    int n, **quebra_cabeca, dimensao, i, j;

    scanf("%d", &n);
    dimensao = n + 2; //considerando o espaço das pistas
    quebra_cabeca = aloca_matriz_quadrada(dimensao);
    for(i = 0; i < dimensao; i++)  
        for(j = 0; j < dimensao; j++) 
            scanf("%d", &quebra_cabeca[i][j]);

    if(arranha_ceuR(quebra_cabeca, 1, 1, n))
        imprime_matriz_quadrada(quebra_cabeca, n);

    libera_matriz_quadrada(quebra_cabeca, dimensao);

    return 0;
}

