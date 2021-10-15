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

/* Cria uma árvore binária. */
p_no criar_arvore() {
    return NULL;
}

/* Libera a memória alocada para uma árvore binária. */
void destruir_arvore(p_no raiz) {
    if (raiz != NULL) {
        destruir_arvore(raiz->esq);
        destruir_arvore(raiz->dir);
        free(raiz);
    }
}

/* Insere um anuncio na arvore binaria de busca organizada por valor. */
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

/* Insere um anuncio na arvore binaria de busca organizada alfabeticamente pelo nome das lojas. */
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

/* Busca por anuncios na arvore cujo codigo do produto seja o mesmo e o valor não ultrapasse o max, 
satisfeita essas condições o anuncio é adicionado em outra arvore binaria de busca organizada alfabeticamente. */
p_no buscar_por_intervalo(p_no raiz, p_no lojas, double max, int codigo) {
    while (raiz != NULL) {
        if (raiz->valor <= max) { //esta no intervalo de preços
            if (raiz->codigo == codigo) //é o produto desejado
                lojas = inserir_anuncio_por_loja(lojas, raiz->loja, raiz->codigo, raiz->valor);
            //nesse caso os dois caminhos (esq, dir) sao possiveis para encontrar mais anuncios validos:
            lojas = buscar_por_intervalo(raiz->esq, lojas, max, codigo); 
            raiz = raiz->dir;
        } else if (raiz->valor > max)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }
    return lojas;
}

/* Substitui o elemento por seu sucessor e remove a antiga posição do sucessor, liberando memoria. */
void remover_sucessor(p_no raiz) { 
    p_no sucessor = raiz->dir, pai = raiz; 
    //buscando pelo minimo da subarvore esquerda (sucessor) e seu pai:
    while (sucessor->esq != NULL) { //o sucessor nunca tem filho esquerdo
        pai = sucessor;
        sucessor = sucessor->esq;
    }
    
    if (pai->esq == sucessor)
        pai->esq = sucessor->dir;
    else
        pai->dir = sucessor->dir;

    strcpy(raiz->loja, sucessor->loja);
    raiz->codigo = sucessor->codigo;
    raiz->valor = sucessor->valor;

    free(sucessor);
}

/* Remove um elemento de uma árvore binária de busca, liberando a memória. */
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

/* Retorna o minimo de uma arvore. */
p_no minimo(p_no raiz) { 
    if (raiz == NULL || raiz->esq == NULL)
        return raiz;
    return minimo(raiz->esq);
}

/* Imprime em ordem crescente (nesse caso alfabética) de acordo com o enunciado. 
Essa função também libera a memoria da arvore, removendo cada elemento apos imprimir. */
void imprime_crescente(p_no raiz) { 
    if (raiz != NULL) {
        p_no elemento = minimo(raiz);
        p_no ptr_removido = NULL;
        while (elemento != NULL) {
            printf("%s %d %.2lf\n", elemento->loja, elemento->codigo, elemento->valor);
            raiz = remover_rec(raiz, elemento->loja, ptr_removido); 
            elemento = minimo(raiz);
        }
    } else 
        printf("nenhuma boa oferta foi encontrada\n");

    printf("\n");
}

int main() {
    int n, m, i, codigo;
    char loja[20];
    double valor;
    p_no anuncios = criar_arvore();
    p_no lojas;

    //lendo os anuncios:
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%s %d %lf", loja, &codigo, &valor);
        anuncios = inserir_anuncio_por_valor(anuncios, loja, codigo, valor);
    }

    //lendo a consulta do usuario, buscando as ofertas e imprimindo-as:
    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        scanf("%d %lf", &codigo, &valor);
        lojas = criar_arvore();
        lojas = buscar_por_intervalo(anuncios, lojas, valor*1.1, codigo);
        printf("OFERTAS para %d:\n", codigo);
        imprime_crescente(lojas);
    }

    destruir_arvore(anuncios);

    return 0;
}