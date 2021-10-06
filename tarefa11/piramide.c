#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    float saldo;
    char nome[21];
    int nivel;
    struct No *esq, *dir, *ant; 
    struct No *prox; //para fila
} No;
typedef No * p_no;

typedef struct {
    p_no ini, fim;
} Fila;
typedef Fila * p_fila;

p_fila criar_fila() {
    p_fila fila;
    fila = malloc(sizeof(Fila));
    fila->ini = NULL;
    fila->fim = NULL;
    return fila;
}

/* Adiciona um elemento e seus parâmetros (nome, saldo...) da pirâmide no final de uma fila. */
void enfileirar(p_fila fila, p_no elemento) {
    p_no novo;
    novo = malloc(sizeof(No));
    strcpy(novo->nome, elemento->nome);
    novo->saldo = elemento->saldo;
    novo->nivel = elemento->nivel;
    novo->esq = elemento->esq;
    novo->dir = elemento->dir;
    novo->prox = NULL;
    if (fila->ini == NULL)
        fila->ini = novo;
    else
        fila->fim->prox = novo;
    fila->fim = novo;
}

/* Retira o elemento que está no início da fila, supondo que ela não seja vazia, sem liberar a memoria. */
p_no desenfileirar(p_fila fila) { 
    p_no primeiro = fila->ini;
    fila->ini = fila->ini->prox;
    if (fila->ini == NULL)
        fila->fim = NULL;
    return primeiro;
}

p_no criar_arvore(float saldo, char nome[]) {
    p_no raiz = malloc(sizeof(No));
    raiz->saldo = saldo;
    strcpy(raiz->nome, nome);
    raiz->nivel = 1;
    raiz->esq = raiz->dir = raiz->ant = NULL;
    return raiz;
}

p_no procurar_no(p_no raiz, char nome[]) {
    p_no esq;
    if (raiz == NULL || strcmp(raiz->nome, nome) == 0)
        return raiz;
    esq = procurar_no(raiz->esq, nome);
    if (esq != NULL)
        return esq;
    return procurar_no(raiz->dir, nome);
}

void adicionar_filho(p_no pai, char nome[], float saldo, char tipo[]) {
    p_no filho = malloc(sizeof(No));
    float bonificacao = saldo * 0.1;
    pai->saldo -= bonificacao;
    filho->saldo = saldo + bonificacao;
    strcpy(filho->nome, nome); 
    filho->nivel = pai->nivel + 1;
    filho->esq = filho->dir = NULL;
    filho->ant = pai;
    if (strcmp(tipo, "esq") == 0)
        pai->esq = filho;
    else
        pai->dir = filho;
}

void bonus_recrutamentos(p_no ultimo_recrutado) {
    float i = 0.06;
    int primeiro = 1;
    p_no recrutador, atual = ultimo_recrutado;
    while (i != 0.00 && atual->ant != NULL) {
        recrutador = atual->ant;
        if (primeiro) {
            recrutador->saldo += (recrutador->esq->saldo * i) + (recrutador->dir->saldo * i); 
            recrutador->esq->saldo -= (recrutador->esq->saldo * i);
            recrutador->dir->saldo -= (recrutador->dir->saldo * i);
            primeiro = 0;
        } else {
            recrutador->saldo += (atual->saldo * i);
            atual->saldo -= atual->saldo * i;
        }
        i -= 0.01;
        atual = recrutador;
    }
}

void imprime_piramide(p_no topo) {
    p_fila fila;
    int ultimo_nivel = 0;
    fila = criar_fila();
    enfileirar(fila, topo); 
    while(fila->ini != NULL) { //enquanto fila nao vazia
        topo = desenfileirar(fila);
        if (topo->esq != NULL)
            enfileirar(fila, topo->esq);
        if (topo->dir != NULL)
            enfileirar(fila, topo->dir);
        if (topo->nivel == ultimo_nivel)
            printf("[%s %.2f] ", topo->nome, topo->saldo);
        else {
            if (ultimo_nivel != 0)
                printf("\n");
            printf("Nivel %d: [%s %.2f] ", topo->nivel, topo->nome, topo->saldo);
            ultimo_nivel++;
        }
        free(topo); 
    }
    free(fila);
}

void destruir_arvore(p_no raiz) {
    if (raiz != NULL) {
        destruir_arvore(raiz->esq);
        destruir_arvore(raiz->dir);
        free(raiz);
    }
}

int main() {
    char leitura[8], recrutador[21], recrutado[21];
    float saldo;
    p_no piramide, pai;

    scanf("%s %s %f", leitura, recrutador, &saldo);
    piramide = criar_arvore(saldo, recrutador);
    printf("Nivel 1: [%s %.2f] \n\n", recrutador, saldo); 
    while(scanf("%s %s %s %f", recrutador, leitura, recrutado, &saldo) != EOF) {
        pai = procurar_no(piramide, recrutador);
        if (pai->esq == NULL) 
            adicionar_filho(pai, recrutado, saldo, "esq");
        else {
            adicionar_filho(pai, recrutado, saldo, "dir");
            bonus_recrutamentos(pai->dir);
        }
        imprime_piramide(piramide);
        printf("\n\n");
    }

    destruir_arvore(piramide);

    return 0;
}