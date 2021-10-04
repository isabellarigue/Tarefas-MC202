#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    char nome[21];
    char tipo[11];
    char mercadoria[21];
    int qtd;
    struct No *prox;
} No;
typedef struct No * p_no;

typedef struct No_docas {
    char mercadoria[21];
    int qtd_atual;
    struct No_docas *prox;
} No_docas;
typedef struct No_docas * p_no1;

typedef struct {
    p_no ini, fim;
} Fila;
typedef Fila * p_fila;

typedef struct {
    p_no1 topo;
} Pilha;
typedef Pilha * p_pilha;

p_fila criar_fila() {
    p_fila f;
    f = malloc(sizeof(Fila));
    f->ini = NULL;
    f->fim = NULL;
    return f;
}

p_pilha criar_pilha() {
    p_pilha pilha;
    pilha = malloc(sizeof(Pilha));
    pilha->topo = NULL;
    return pilha;
}

void destruir_lista(p_no lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

void destruir_fila(p_fila f) {
    destruir_lista(f->ini);
    free(f);
}

void destruir_lista1(p_no1 lista) {
    if (lista != NULL) {
        destruir_lista1(lista->prox);
        free(lista);
    }
}

void destruir_pilha(p_pilha p) {
    destruir_lista1(p->topo);
    free(p);
}

void enfileira(p_fila fila, char nome[], char tipo[], char mercadoria[], int qtd) {
    p_no novo;
    novo = malloc(sizeof(No));
    strcpy(novo->nome, nome);
    strcpy(novo->tipo, tipo);
    strcpy(novo->mercadoria, mercadoria);
    novo->qtd = qtd;
    novo->prox = NULL;
    if (fila->ini == NULL)
        fila->ini = novo;
    else
        fila->fim->prox = novo;
    fila->fim = novo;
}

void desenfileira(p_fila fila) { 
    p_no primeiro = fila->ini;
    fila->ini = fila->ini->prox;
    if (fila->ini == NULL)
        fila->fim = NULL;
    free(primeiro);
}

void empilhar(p_pilha pilha, char mercadoria[]) {
    p_no1 novo = malloc(sizeof(No_docas));
    strcpy(novo->mercadoria, mercadoria);
    novo->prox = pilha->topo;
    if (novo->prox != NULL)
        novo->qtd_atual = novo->prox->qtd_atual + 1; //é somado 1 na quantiade atual de containeres da doca
    else
        novo->qtd_atual = 1; //se esse for o primeiro container da doca, a quantidade atual é 1
    pilha->topo = novo;
}

void desempilhar(p_pilha pilha) {
    p_no1 topo = pilha->topo;
    pilha->topo = pilha->topo->prox;
    free(topo);
}

void carrega_descarrega(p_pilha *docas, int capacidade, int qtd_docas, char nome[], char tipo[], char mercadoria[], int qtd, p_fila fila, int remanescente) {
    int i, total = 0;
    for (i = 0; i < qtd_docas; i++) {
        if (strcmp(tipo, "carrega") == 0) {
            if (docas[i]->topo != NULL && strcmp(docas[i]->topo->mercadoria, mercadoria) == 0) { // verificando se a doca n esta vazia e se tem a mercadoria no topo
                while (docas[i]->topo != NULL && strcmp(docas[i]->topo->mercadoria, mercadoria) == 0 && qtd > 0) { //enquanto houver mercadoria e a qtd a ser carregada for > 0
                    desempilhar(docas[i]);
                    qtd -= 1;
                    total += 1;
                }
                printf("%s carrega %s doca: %d conteineres: %d\n", nome, mercadoria, i, total);
                break;
            }
        } else { //descarrega
            if (docas[i]->topo == NULL || docas[i]->topo->qtd_atual < capacidade) { //verificando se há espaço para novos containeres na doca
                while ((docas[i]->topo == NULL || docas[i]->topo->qtd_atual < capacidade) && qtd > 0) { //enquanto houver espaço na doca e a qtd a ser descarregada for > 0
                    empilhar(docas[i], mercadoria);
                    total += 1;
                    qtd -= 1;
                }
                printf("%s descarrega %s doca: %d conteineres: %d\n", nome, mercadoria, i, total);
                break;
            }
        }
    }
    if (qtd > 0) //se ainda houver produtos para serem carregados ou descarregados
        enfileira(fila, nome, tipo, mercadoria, qtd);

    if (remanescente)
        desenfileira(fila);
}

int main() {
    int qtd_docas, capacidade, navios, i, qtd, contador = 0;
    p_pilha *docas;
    p_fila fila = criar_fila();
    char nome[21], tipo[11], mercadoria[21];
    p_no atual;

    scanf("%d", &qtd_docas);
    scanf("%d", &capacidade);
    scanf("%d", &navios);
    docas = malloc(qtd_docas * sizeof(Pilha));
    for (i = 0; i < qtd_docas; i++)
        docas[i] = criar_pilha();

    for (i = 0; i < navios; i++) {
        scanf("%s %s %s %d", nome, tipo, mercadoria, &qtd);
        carrega_descarrega(docas, capacidade, qtd_docas, nome, tipo, mercadoria, qtd, fila, 0); 
    }
    
    while (fila->ini != NULL && contador < navios*100) { 
        carrega_descarrega(docas, capacidade, qtd_docas, fila->ini->nome, fila->ini->tipo, fila->ini->mercadoria, fila->ini->qtd, fila, 1);
        contador++;
    }

    i = 0;
    for (atual = fila->ini; atual != NULL; atual = atual->prox) //contando quantos navios restam na fila
        i++;
    if (i != 0)
        printf("ALERTA: impossivel esvaziar fila, restam %d navios.", i);

    for (i = 0; i < qtd_docas; i++)
        destruir_pilha(docas[i]);
    free(docas);
    destruir_fila(fila);

    return 0;
}


