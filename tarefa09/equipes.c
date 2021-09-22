#include <stdio.h>
#include <stdlib.h>

typedef struct No {
    int dado;
    struct No *ant;
    struct No *prox;
} No;

typedef struct No * p_no;

p_no criar_lista_circular_dupla_com_cabeca() {
    p_no cabeca;
    cabeca = malloc(sizeof(No));
    cabeca->prox = cabeca;
    cabeca->ant = cabeca;
    return cabeca;
}

p_no inserir_elemento_lista_circular_dupla(p_no lista, int x) { //lista circular dupla com cabeça
    p_no novo;
    novo = malloc(sizeof(No));
    novo->dado = x;

    novo->prox = lista->prox;
    lista->prox = novo;

    novo->ant = lista; //o anterior seria a cabeça
    novo->prox->ant = novo;

    return lista;
}

//retirar elemento 
p_no remove_a_direita(p_no lista, int n, int *escolhido) {
    p_no anterior = lista->ant; //o anterior começa sendo o primeiro colocado na lista
    
    for (int i = 0; i < n; i++) {
        if (anterior->ant == lista)
            anterior = anterior->ant->ant; //temos que pular a cabeça
        else
            anterior = anterior->ant;
    }
    anterior->ant->prox = anterior->prox;
    anterior->prox->ant = anterior->ant;
    *escolhido = anterior->dado;
    free(anterior);
     
    if (lista->prox == lista) {
        free(lista); //liberando a cabeça
        return NULL; //a lista esta vazia
    }
    return lista;    
}

p_no remove_a_esquerda(p_no lista, int n, int *escolhido) {
    p_no posterior = lista->ant; //o posterior começa sendo o primeiro colocado na lista

    if (n != 0) {
        posterior = posterior->prox->prox; //temos que pular a cabeça
        for (int i = 1; i < n; i++) {
            if (posterior->prox == lista)
                posterior = posterior->prox->prox; //temos que pular a cabeça
            else
                posterior = posterior->prox;
        }
    }

    posterior->ant->prox = posterior->prox;
    posterior->prox->ant = posterior->ant;
    *escolhido = posterior->dado;
    free(posterior);

    if (lista->prox == lista) {
        free(lista); //liberando a cabeça
        return NULL; //a lista esta vazia
    }
    return lista;    
}

//criar lista normal para cada equipe
p_no cria_lista_encadeada() {
    return NULL;
}

p_no insere_elemento_lista_encadeada(p_no lista, int dado) {
    p_no elemento = malloc(sizeof(No)); //seria melhor criar outro no?
    elemento->dado = dado;
    elemento->prox = lista;
    return elemento;
}

p_no busca_elemento(p_no lista, int x) {
    p_no atual;
    for (atual = lista; atual != NULL; atual = atual->prox) 
        if (atual->dado == x)
            return atual;
    return NULL; 
}

p_no remove_elemento(p_no lista, int x) {
    p_no aux, elemento_ptr;
    elemento_ptr = busca_elemento(lista, x);

    if (lista->prox != NULL) {
        if (lista != elemento_ptr) {
            aux = lista;
            while(aux->prox != elemento_ptr) //percorre a lista ate uma posição antes da do elemento
                aux = aux->prox; 
            aux->prox = elemento_ptr->prox;
        } else 
            lista = lista->prox; //retira do inicio da lista
        free(elemento_ptr); 
        return lista;
    } else {
        free(lista);
        return NULL;
    }
}

void imprime_ordenado(p_no lista) {
    p_no atual;
    int menor;

    while (lista != NULL) {
        menor = lista->dado;
        for (atual = lista->prox; atual != NULL; atual = atual->prox) {
            if (atual->dado < menor)
                menor = atual->dado;
        }
        lista = remove_elemento(lista, menor);
        printf("%d ", menor);
    }
}

int main() {
    int m, n, i, altura, escolhido;
    p_no lista, time1, time2;

    lista = criar_lista_circular_dupla_com_cabeca();
    time1 = cria_lista_encadeada();
    time2 = cria_lista_encadeada();

    scanf("%d", &m);
    for (i = 0; i < m; i++) {
        scanf("%d", &altura); //agrupar?
        lista = inserir_elemento_lista_circular_dupla(lista, altura);
    }
    scanf("%d", &n);
    lista = remove_a_esquerda(lista, n - 1, &escolhido);
    time1 = insere_elemento_lista_encadeada(time1, escolhido);
    i = 1;
    while (lista != NULL) {
        scanf("%d", &n);
        if (i % 2 == 0) {
            lista = remove_a_esquerda(lista, n - 1, &escolhido);
            time1 = insere_elemento_lista_encadeada(time1, escolhido);
        } else {
            lista = remove_a_direita(lista, n - 1, &escolhido);
            time2 = insere_elemento_lista_encadeada(time2, escolhido);
        }
        i++;
    }

    imprime_ordenado(time1);
    printf("\n");
    imprime_ordenado(time2);

    return 0;
}
