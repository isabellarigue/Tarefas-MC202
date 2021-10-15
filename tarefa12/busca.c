#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char loja[20]; 
    int codigo;
    double valor;
    struct No *esq, *dir; 
} No;

typedef No * p_no;

p_no criar_arvore() {
    return NULL;
}

void destruir_arvore(p_no raiz) {
    if (raiz != NULL) {
        destruir_arvore(raiz->esq);
        destruir_arvore(raiz->dir);
        free(raiz);
    }
}

p_no inserir_anuncio_por_valor(p_no raiz, char loja[], int codigo, double valor) { 
    p_no novo;
    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy(novo->loja, loja);
        novo->codigo = codigo;
        novo->valor = valor;
        return novo;
    }
    if (valor < raiz->valor)
        raiz->esq = inserir_anuncio_por_valor(raiz->esq, loja, codigo, valor);
    else
        raiz->dir = inserir_anuncio_por_valor(raiz->dir, loja, codigo, valor);
    return raiz;
}

p_no inserir_anuncio_por_loja(p_no raiz, char loja[], int codigo, double valor) { 
    p_no novo;
    if (raiz == NULL) {
        novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy(novo->loja, loja);
        novo->codigo = codigo;
        novo->valor = valor;
        return novo;
    }
    if (strcmp(loja, raiz->loja) < 0) 
        raiz->esq = inserir_anuncio_por_loja(raiz->esq, loja, codigo, valor);
    else
        raiz->dir = inserir_anuncio_por_loja(raiz->dir, loja, codigo, valor);
    return raiz;
}

p_no buscar_por_intervalo(p_no raiz, p_no lojas, double max, int codigo) {
    while (raiz != NULL) {
        if (raiz->valor <= max) { //esta no intervalo de preços
            if (raiz->codigo == codigo) //é o produto desejado
                lojas = inserir_anuncio_por_loja(lojas, raiz->loja, raiz->codigo, raiz->valor);
            lojas = buscar_por_intervalo(raiz->esq, lojas, max, codigo); //nesse caso os dois caminhos sao possiveis
            raiz = raiz->dir;
        } else if (raiz->valor > max)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }
    return lojas;
}

void remover_sucessor(p_no raiz) { 
    p_no min = raiz->dir, pai = raiz; 
    while (min->esq != NULL) { //buscando pelo minimo da subarvore esquerda e seu pai
        pai = min;
        min = min->esq;
    }
    if (pai->esq == min)
        pai->esq = min->dir;
    else
        pai->dir = min->dir;

    strcpy(raiz->loja, min->loja);
    raiz->codigo = min->codigo;
    raiz->valor = min->valor;

    free(min);
}

p_no remover_rec(p_no raiz, char loja[], p_no ptr_removido) { 
    if (raiz == NULL)
        return NULL;
    if (strcmp(loja, raiz->loja) < 0)
        raiz->esq = remover_rec(raiz->esq, loja, ptr_removido);
    else if (strcmp(loja, raiz->loja) > 0)
        raiz->dir = remover_rec(raiz->dir, loja, ptr_removido);
    else if (raiz->esq == NULL) {
        ptr_removido = raiz;
        return raiz->dir;
    } else if (raiz->dir == NULL) {
        ptr_removido = raiz;
        return raiz->esq;
    } else
        remover_sucessor(raiz);
    free(ptr_removido);
    return raiz;
}

p_no minimo(p_no raiz) { 
    if (raiz == NULL || raiz->esq == NULL)
        return raiz;
    return minimo(raiz->esq);
}

void imprime_crescente(p_no raiz, int codigo) { //essa função imprime e libera
    printf("OFERTAS para %d:\n", codigo);
    if (raiz != NULL) {
        p_no elemento = minimo(raiz);
        p_no ptr_removido = NULL;
        while (elemento != NULL) {
            printf("%s %d %.2lf\n", elemento->loja, elemento->codigo, elemento->valor);
            raiz = remover_rec(raiz, elemento->loja, ptr_removido); //colocar uma função q ja usa o ponteiro pra otimizar?
            elemento = minimo(raiz);
        }
    } else 
        printf("nenhuma boa oferta foi encontrada\n");

    printf("\n");
}

int main() {
//buscar por preços q estejam no intervalo
//para cada produto q achar, compara o codigo e ve se é o desejado
//criar uma outra arvore para os produtos desejados e ir inserindo em ordem alfabetica
    int n, m, i, codigo;
    char loja[20];
    double valor;
    p_no anuncios = criar_arvore();
    p_no lojas;

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%s %d %lf", loja, &codigo, &valor);
        anuncios = inserir_anuncio_por_valor(anuncios, loja, codigo, valor);
    }

    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        lojas = criar_arvore();
        scanf("%d %lf", &codigo, &valor);
        lojas = buscar_por_intervalo(anuncios, lojas, valor*1.1, codigo);
        imprime_crescente(lojas, codigo);
    }
    destruir_arvore(anuncios);

    return 0;
}