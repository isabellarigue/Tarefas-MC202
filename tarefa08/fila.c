#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

p_no criar_lista() {
    return NULL;
}

int lista_vazia(p_no lista) { 
    return (lista == NULL);
}

p_no cria_pessoa(int documento, char profissao[26], char status_vacinacao[13]) {
    p_no novo;
    novo = malloc(sizeof(No));
    if (novo == NULL) //verificando se há memoria suficiente
        exit(1);
    novo->documento = documento;
    strcpy(novo->profissao, profissao);
    strcpy(novo->status_vacinacao, status_vacinacao);
    return novo;
}

int tem_privilegios(char profissao[26]) {
    if (strcmp(profissao, "coach") == 0 || strcmp(profissao, "ex-bbb") == 0 || strcmp(profissao, "filho-politico") == 0 || 
    strcmp(profissao, "herdeiro") == 0 || strcmp(profissao, "youtuber-financas") == 0)
        return 1;
    return 0;
}

p_no adicionar_inicio(p_no lista, p_no pessoa, p_no *lanterna) {
    pessoa->prox = lista;
    *lanterna = pessoa; 
    return pessoa;
}

p_no adicionar_final(p_no lista, p_no pessoa, p_no *lanterna) {
    p_no atual;
    atual = lista;
    while(atual->prox != NULL) //percorre ate o fim da lista
        atual = atual->prox;
    atual->prox = pessoa;
    pessoa->prox = NULL; //define a pessoa como a ultima da lista
    *lanterna = pessoa; 
    return lista;
}

p_no segue_a_fila(p_no lista, p_no pessoa, p_no *lanterna) {
    p_no atual, aux;
    atual = lista;
    while(atual != *lanterna)
        atual = atual->prox;
    aux = atual->prox;
    atual->prox = pessoa;
    pessoa->prox = aux;
    *lanterna = pessoa;
    return lista;
}

p_no remove_elemento(p_no lista, p_no elemento) {
    p_no aux;
    aux = lista;
    while(aux->prox != elemento) //percorre a lista ate uma posição antes da do elemento
        aux = aux->prox; 
    aux->prox = elemento->prox; 
    free(elemento); 
    return lista;
}

p_no fiscal(p_no lista, p_no fiscal) {
    p_no proximo;
    proximo = fiscal->prox;
    while (proximo != NULL && strcmp(proximo->status_vacinacao, "nao-vacinado") == 0) {
        lista = remove_elemento(lista, proximo);
        proximo = fiscal->prox;
    }
    return lista;
}

void imprime(p_no lista) {
    p_no atual;
    for (atual = lista; atual != NULL; atual = atual->prox)
        printf("%d %s %s\n", atual->documento, atual->profissao, atual->status_vacinacao);
}

void destruir_lista(p_no lista) {
    if ( ! lista_vazia(lista)) {
        destruir_lista(lista->prox);
        free(lista);
    }
}
