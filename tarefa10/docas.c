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

/* Função auxiliar para liberar os ponteiros de uma fila. */
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

/* Função auxiliar para liberar os ponteiros de uma pilha. */
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

/* Adiciona um navio e seus parâmetros (nome, tipo...) no final de uma fila. */
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

/* Retira o navio que está no início da fila, supondo que ela não está vazia. */
void desenfileira(p_fila fila) { 
    p_no primeiro = fila->ini;
    fila->ini = fila->ini->prox;
    if (fila->ini == NULL)
        fila->fim = NULL;
    free(primeiro);
}

/* Empilha um containere e sua mercadoria correspondente na pilha. */
void empilhar(p_pilha pilha, char mercadoria[]) {
    p_no1 novo = malloc(sizeof(No_docas));
    strcpy(novo->mercadoria, mercadoria);
    novo->prox = pilha->topo;
    if (novo->prox != NULL)
        novo->qtd_atual = novo->prox->qtd_atual + 1; /* é somado 1 na quantiade atual de containeres da doca */
    else
        novo->qtd_atual = 1; /* se esse for o primeiro container da doca, a quantidade atual é 1 */
    pilha->topo = novo;
}

/* Desempilha um containere e sua mercadoria correspondente da pilha */
void desempilhar(p_pilha pilha) {
    p_no1 topo = pilha->topo;
    pilha->topo = pilha->topo->prox;
    free(topo);
}

/* Realiza as ações de carregamento/descarregamento de um navio quando possível. 
Se não for possível ele é adicionado ao fim da fila. */
void carrega_descarrega(p_pilha *docas, int capacidade, int qtd_docas, char nome[], char tipo[], char mercadoria[], int qtd, p_fila fila, int remanescente, int *contador) {
    int i, total = 0;
    for (i = 0; i < qtd_docas; i++) {
        if (strcmp(tipo, "carrega") == 0) {
            if (docas[i]->topo != NULL && strcmp(docas[i]->topo->mercadoria, mercadoria) == 0) { /* verificando se a doca n esta vazia e se tem a mercadoria no topo */
                while (docas[i]->topo != NULL && strcmp(docas[i]->topo->mercadoria, mercadoria) == 0 && qtd > 0) { /* enquanto houver mercadoria e a qtd a ser carregada for > 0 */
                    desempilhar(docas[i]);
                    qtd -= 1;
                    total += 1;
                }
                printf("%s carrega %s doca: %d conteineres: %d\n", nome, mercadoria, i, total);
                break;
            }
        } else { /* descarrega */
            if (docas[i]->topo == NULL || docas[i]->topo->qtd_atual < capacidade) { /* verificando se há espaço para novos containeres na doca */
                while ((docas[i]->topo == NULL || docas[i]->topo->qtd_atual < capacidade) && qtd > 0) { /* enquanto houver espaço na doca e a qtd a ser descarregada for > 0 */
                    empilhar(docas[i], mercadoria);
                    total += 1;
                    qtd -= 1;
                }
                printf("%s descarrega %s doca: %d conteineres: %d\n", nome, mercadoria, i, total);
                break;
            }
        }
    }
    if (qtd > 0) /* se ainda houver produtos para serem carregados ou descarregados */
        enfileira(fila, nome, tipo, mercadoria, qtd);

    /* Se não é a primeira vez que um navio tenta fazer o carregamento/descarregamento então ele já estava na fila, e deve ser 
    retirado do inicío dela, ou porque conseguiu realizar a ação ou porque não conseguiu e foi recolocado no final da fila. */
    if (remanescente) { 
        desenfileira(fila);
        if (total == 0)
            *contador += 1;
        else
            *contador = 0;
    } 
}

int tamanho_fila(p_fila fila) {
    p_no atual;
    int i = 0;
    for (atual = fila->ini; atual != NULL; atual = atual->prox) 
        i++;
    return i;
}

int main() {
    int qtd_docas, capacidade, navios, i, qtd, contador = 0;
    p_pilha *docas;
    p_fila fila = criar_fila();
    char nome[21], tipo[11], mercadoria[21];

    scanf("%d", &qtd_docas);
    scanf("%d", &capacidade);
    scanf("%d", &navios);
    docas = malloc(qtd_docas * sizeof(Pilha));
    for (i = 0; i < qtd_docas; i++)
        docas[i] = criar_pilha();

    /* Lendo a entrada de cada navio. A príncipio, sempre tenta-se realizar o 
    carregamento/descarregamento, se isso não for possível os navios entram no final da fila.*/
    for (i = 0; i < navios; i++) {
        scanf("%s %s %s %d", nome, tipo, mercadoria, &qtd);
        carrega_descarrega(docas, capacidade, qtd_docas, nome, tipo, mercadoria, qtd, fila, 0, &contador); 
    }
    
    /* A ideia abaixo é tentar esvaziar a fila (chegar em fila->ini == NULL), porém também é verificado 
    se isso é possível. É somado um no contador para cada navio da fila que não é possível carregar/descarregar,
    quando chegar em um navio que é possível realizar a ação, o contador zera. Se o contador for igual ao tamanho 
    da fila, sabemos que a fila está se repetindo e portanto nunca irá esvaziar. */
    while (fila->ini != NULL) { 
        carrega_descarrega(docas, capacidade, qtd_docas, fila->ini->nome, fila->ini->tipo, fila->ini->mercadoria, fila->ini->qtd, fila, 1, &contador);
        i = tamanho_fila(fila);
        if (contador == i && i != 0) {
            printf("ALERTA: impossivel esvaziar fila, restam %d navios.", i);
            break;
        }
    }

    /* Liberando as alocações: */
    for (i = 0; i < qtd_docas; i++)
        destruir_pilha(docas[i]);
    free(docas);
    destruir_fila(fila);

    return 0;
}


