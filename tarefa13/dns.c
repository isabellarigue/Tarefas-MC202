#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Cor {VERMELHO , PRETO};

typedef struct No {
    int acessos; /* para o registro da quantidade de consultas do usuário */
    char dominio[101], endereco[16]; 
    enum Cor cor;
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

/* Retorna verdadeiro se a cor de x é VERMELHO. */
int ehVermelho(p_no x) { 
    if (x == NULL)
        return 0;
    return x->cor == VERMELHO;
}

/* Retorna verdadeiro se a cor de x é PRETO. */
int ehPreto(p_no x) {
    if (x == NULL)
        return 1;
    return x->cor == PRETO;
}

/* Faz uma rotação para a esquerda entre elementos, sem estragar as propriedades
de busca e altura negra de uma arvore rubro negra esquerdista. */
p_no rotaciona_para_esquerda(p_no raiz) { 
    p_no x = raiz->dir;
    raiz->dir = x->esq;
    x->esq = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}

/* Faz uma rotação para a direita entre elementos, sem estragar as propriedades
de busca e altura negra de uma arvore rubro negra esquerdista. */
p_no rotaciona_para_direita(p_no raiz) { 
    p_no x = raiz->esq; 
    raiz->esq = x->dir;
    x->dir = raiz;
    x->cor = raiz->cor;
    raiz->cor = VERMELHO;
    return x;
}

/* Troca a cor do pai para VERMELHO e as cores dos filhos para PRETO, sem estragar
a propriedade da altura negra de uma arvore rubro negra esquerdista. */
void sobe_vermelho(p_no raiz) { 
    raiz->cor = VERMELHO;
    raiz->esq->cor = PRETO;
    raiz->dir->cor = PRETO;
}

/* Insere um elemento em uma ABB rubro negra esquerdista, mantendo todas as propriedades, 
conforme código e funções auxiliares vistas durante a aula. */
p_no inserir_rec(p_no raiz, char endereco[], char dominio[], int acessos, int busca) {
    if (raiz == NULL) {
        p_no novo = malloc(sizeof(No));
        novo->esq = novo->dir = NULL;
        strcpy(novo->endereco, endereco);
        strcpy(novo->dominio, dominio);
        novo->acessos = acessos;
        novo->cor = VERMELHO;
        return novo;
    }

    /* Se busca igual a 1, a arvore é organizada de acordo com os dominios, caso contrario a arvore é organizada pelos enderecos. */
    if (busca == 1)  
        if (strcmp(dominio, raiz->dominio) < 0)
            raiz->esq = inserir_rec(raiz->esq, endereco, dominio, acessos, 1);
        else
            raiz->dir = inserir_rec(raiz->dir, endereco, dominio, acessos, 1);   
    else 
        if (strcmp(endereco, raiz->endereco) < 0)
            raiz->esq = inserir_rec(raiz->esq, endereco, dominio, acessos, 0);
        else
            raiz->dir = inserir_rec(raiz->dir, endereco, dominio, acessos, 0);

    /* corrige a árvore */
    if (ehVermelho(raiz->dir) && ehPreto(raiz->esq))
        raiz = rotaciona_para_esquerda(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->esq->esq))
        raiz = rotaciona_para_direita(raiz);
    if (ehVermelho(raiz->esq) && ehVermelho(raiz->dir))
        sobe_vermelho(raiz);

    return raiz;
}

/* Retorna o elemento desejado de uma ABB, organizada alfabeticamente pelo dominio. */
p_no buscar_por_dominio(p_no raiz, char dominio[]) {
    if (raiz == NULL || strcmp(dominio, raiz->dominio) == 0)
        return raiz;
    if (strcmp(dominio, raiz->dominio) < 0)
        return buscar_por_dominio(raiz->esq, dominio);
    else
        return buscar_por_dominio(raiz->dir, dominio);
}

/* Retorna o elemento desejado de uma ABB, organizada alfabeticamente pelo endereco. */
p_no buscar_por_endereco(p_no raiz, char endereco[]) {
    if (raiz == NULL || strcmp(endereco, raiz->endereco) == 0)
        return raiz;
    if (strcmp(endereco, raiz->endereco) < 0)
        return buscar_por_endereco(raiz->esq, endereco);
    else
        return buscar_por_endereco(raiz->dir, endereco);
}

/* A ideia do codigo é utilizar ABBs balanceadas, no caso a escolhida foi a rubro negra esquerdista. Há uma 
arvore para armazenar os servidores, que é organizada de acordo com os dominios, e uma arvore para os usuarios 
(para ter controle do numero de consultas ja feitas), que é organizada de acordo com os endereços dos mesmos. */
int main() {
    int u, n, m, i;
    char dominio[101], endereco[16], leitura[4], leitura1[5];
    p_no DNS = criar_arvore(), servidor;
    p_no usuarios = criar_arvore(), usuario;

    scanf("%d", &u);
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%s %s", dominio, endereco);
        DNS = inserir_rec(DNS, endereco, dominio, 0, 1);
    }

    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        scanf("%s %s %s %s", leitura, dominio, leitura1, endereco);
        usuario = buscar_por_endereco(usuarios, endereco);
        servidor = buscar_por_dominio(DNS, dominio);

        if (usuario == NULL) {
            usuarios = inserir_rec(usuarios, endereco, "none", 1, 0);
            if (servidor == NULL)
                printf("NOTFOUND %s FROM %s\n", dominio, endereco);
            else 
                printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, endereco, servidor->endereco);
        
        } else {
            if (usuario->acessos >= u)
                printf("FORBIDDEN %s FROM %s\n", dominio, endereco);
            else if (servidor == NULL) 
                printf("NOTFOUND %s FROM %s\n", dominio, endereco);
            else 
                printf("ACCEPTED %s FROM %s RESPOND %s\n", dominio, endereco, servidor->endereco);
            usuario->acessos += 1;
        }
    }

    destruir_arvore(DNS);
    destruir_arvore(usuarios);

    return 0;
}