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

p_no inserir_elemento_lista_circular_dupla(p_no lista, int x) { //lista com cabeça
    p_no novo;
    novo = malloc(sizeof(No));
    novo->dado = x;

    novo->prox = lista->prox;
    lista->prox = novo;

    novo->ant = lista; //o anterior é a cabeça
    novo->prox->ant = novo;

    return lista;
}

//remove o elemento n posições a direita de quem jogou o dado, considerando a formação do circulo 
p_no remove_a_direita(p_no lista, int n, int *escolhido) {
    p_no elemento = lista->ant; //o elemento começa sendo o ponteiro mais antigo colocado na lista, ou seja, quem jogou o dado
    
    for (int i = 0; i < n; i++) {
        if (elemento->ant == lista)
            elemento = elemento->ant->ant; //ignorando a cabeça
        else
            elemento = elemento->ant;
    }
    elemento->ant->prox = elemento->prox;
    elemento->prox->ant = elemento->ant;
    *escolhido = elemento->dado;
    free(elemento);
     
    if (lista->prox == lista) {
        free(lista); //liberando a cabeça
        return NULL; //a lista esta vazia
    }
    return lista;    
}

//remove o elemento n posições a esquerda de quem jogou o dado, considerando a formação do circulo
p_no remove_a_esquerda(p_no lista, int n, int *escolhido) {
    p_no elemento = lista->ant; //o elemento começa sendo o ponteiro mais antigo colocado na lista, ou seja, quem jogou o dado

    if (n != 0) {
        elemento = elemento->prox->prox; //ignorando a cabeça
        for (int i = 1; i < n; i++) {
            if (elemento->prox == lista)
                elemento = elemento->prox->prox; //ignorando a cabeça
            else
                elemento = elemento->prox;
        }
    }
    elemento->ant->prox = elemento->prox;
    elemento->prox->ant = elemento->ant;
    *escolhido = elemento->dado;
    free(elemento);

    if (lista->prox == lista) {
        free(lista); //liberando a cabeça
        return NULL; //a lista esta vazia
    }
    return lista;    
}

p_no cria_lista_encadeada() {
    return NULL;
}

p_no insere_elemento_lista_encadeada(p_no lista, int dado) {
    p_no elemento = malloc(sizeof(No)); 
    elemento->dado = dado;
    elemento->prox = lista;
    return elemento;
}

p_no remove_elemento(p_no lista, p_no elemento_ptr) { //função para lista encadeada normal
    p_no aux;

    if (lista->prox != NULL) {
        if (lista != elemento_ptr) {
            aux = lista;
            while(aux->prox != elemento_ptr) //percorre a lista ate uma posição antes da do elemento
                aux = aux->prox; 
            aux->prox = elemento_ptr->prox;
        } else 
            lista = lista->prox; //remove do inicio da lista
        free(elemento_ptr); 
        return lista;
    } else { //so resta um unico elemento na lista para ser liberado/removido
        free(lista); 
        return NULL;
    }
}

void imprime_ordenado(p_no lista) { 
    p_no atual, menor_ptr;
    int menor;

    while (lista != NULL) {
        menor = lista->dado;
        menor_ptr = lista;
        for (atual = lista->prox; atual != NULL; atual = atual->prox) {
            if (atual->dado < menor) {
                menor = atual->dado;
                menor_ptr = atual;
            }
        }
        lista = remove_elemento(lista, menor_ptr);
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
        scanf("%d", &altura); 
        lista = inserir_elemento_lista_circular_dupla(lista, altura);
    }

    for (i = 0; lista != NULL; i++) {
        scanf("%d", &n);
        if (i % 2 == 0) {
            lista = remove_a_esquerda(lista, n - 1, &escolhido);
            time1 = insere_elemento_lista_encadeada(time1, escolhido);
        } else {
            lista = remove_a_direita(lista, n - 1, &escolhido);
            time2 = insere_elemento_lista_encadeada(time2, escolhido);
        }
    }

    imprime_ordenado(time1);
    printf("\n");
    imprime_ordenado(time2);

    return 0;
}
