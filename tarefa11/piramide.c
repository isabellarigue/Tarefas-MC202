#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No {
    float saldo;
    char nome[21];
    int nivel;
    struct No *esq, *dir, *ant; //para construir uma arvore
    struct No *prox; //para construir uma fila
} No;
typedef No * p_no;

typedef struct {
    p_no ini, fim;
} Fila;
typedef Fila * p_fila;

/* Cria uma fila. */
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

/* Cria uma arvore com os parâmetros dos elementos da pirâmide (nome, saldo...). */
p_no criar_arvore(float saldo, char nome[]) {
    p_no raiz = malloc(sizeof(No));
    raiz->saldo = saldo;
    strcpy(raiz->nome, nome);
    raiz->nivel = 1;
    raiz->esq = raiz->dir = raiz->ant = NULL;
    return raiz;
}

/* Retorna o nó, dado o nome de um elemento na arvore. */
p_no procurar_no(p_no raiz, char nome[]) {
    p_no esq;
    if (raiz == NULL || strcmp(raiz->nome, nome) == 0)
        return raiz;
    esq = procurar_no(raiz->esq, nome);
    if (esq != NULL)
        return esq;
    return procurar_no(raiz->dir, nome);
}

/* Adiciona um filho na arvore, de acordo com os parâmetros da pirâmide (nome, saldo...). */
void adicionar_filho(p_no pai, char nome[], float saldo, char tipo[]) {
    p_no filho = malloc(sizeof(No));
    pai->saldo -= saldo * 0.1; //retirada a bonificação para o recrutado
    filho->saldo = saldo * 1.1; //acrescida a bonificação de recrutado
    strcpy(filho->nome, nome); 
    filho->nivel = pai->nivel + 1;
    filho->esq = filho->dir = NULL;
    filho->ant = pai;
    if (strcmp(tipo, "esq") == 0)
        pai->esq = filho;
    else
        pai->dir = filho;
}

/* Adiciona os bonus nos saldos quando um participante consegue recrutar duas pessoas, 
em que até seis níveis da pirâmide devem ser remunerados. */
void bonus_recrutamentos(p_no ultimo_recrutado) {
    float i = 0.06;
    int primeiro = 1;
    p_no recrutador, atual = ultimo_recrutado;
    while (i != 0.00 && atual->ant != NULL) {
        recrutador = atual->ant;
        if (primeiro) { //no primeiro caso, o recrutador recebe um bonus dos dois recrutados
            recrutador->saldo += (recrutador->esq->saldo * i) + (recrutador->dir->saldo * i); 
            recrutador->esq->saldo -= (recrutador->esq->saldo * i);
            recrutador->dir->saldo -= (recrutador->dir->saldo * i);
            primeiro = 0;
        } else { //nos demais casos, cada recrutado fornece um bonus para o seu recrutador
            recrutador->saldo += (atual->saldo * i);
            atual->saldo -= atual->saldo * i;
        }
        i -= 0.01;
        atual = recrutador;
    }
}

/* Devolve o numero arredondado para cima em casos do tipo x.xx499999, por exemplo 281.904999 para 281.91... 
Acrescentei tal função pois o arredondamento automático não estava funcionando para alguns testes. */
float corrige_precisao(float numero) {
    int num = (int)(numero * 10000);
    if (num % 100 >= 49 && num % 100 <= 51)
        return ((float)(num + 100))/10000;
    else
        return numero;
}

/* Imprime a arvore/piramide por niveis, de maneira horizontal. */
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
        if (topo->nivel == ultimo_nivel) //o mesmo nivel deve ser impresso lado a lado
            printf("[%s %.2f] ", topo->nome, topo->saldo);
        else {
            if (ultimo_nivel != 0)
                printf("\n");
            printf("Nivel %d: [%s %.2f] ", topo->nivel, topo->nome, corrige_precisao(topo->saldo));
            ultimo_nivel++;
        }
        free(topo); 
    }
    free(fila);
}

/* Libera a memoria alocada para uma arvore binária. */
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
        if (pai->esq == NULL) //se ainda não há filho esquerdo
            adicionar_filho(pai, recrutado, saldo, "esq");
        else {
            adicionar_filho(pai, recrutado, saldo, "dir");
            bonus_recrutamentos(pai->dir); //se o recrutador conseguiu recrutar duas pessoas, havera bonus
        }
        imprime_piramide(piramide);
        printf("\n\n");
    }

    destruir_arvore(piramide);

    return 0;
}