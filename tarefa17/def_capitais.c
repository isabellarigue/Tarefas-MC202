#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INT_MAX 1000000
#define PAI(i) ((i-1)/2) /*Pai de i*/
#define F_ESQ(i) (2*i+1) /*Filho esquerdo de i*/
#define F_DIR(i) (2*i+2) /*Filho direito de i*/

typedef struct No {
    int populacao;
    double peso;
    char nome[50];
    int vertice;
    double f_centralidade;
    struct No *prox;
} No;
typedef No * p_no;

typedef struct Grafo {
    int n;
    p_no *adj;
} Grafo;
typedef Grafo * p_grafo;

typedef struct {
    double prioridade;
    int vertice;
} Item;
typedef Item * p_item;

typedef struct {
    p_item *v;
    int *indice; //indice[vetice_x] = ?
    int n;
} FP;
typedef FP * p_fp;

p_grafo criar_grafo(int n) {
    p_grafo g = malloc(sizeof(Grafo));
    g->n = n;
    g->adj = calloc(n, sizeof(p_no)); 
    return g;
}

void libera_lista(p_no lista) {
    if (lista != NULL) {
        libera_lista(lista->prox);
        free(lista);
    }
}

void destroi_grafo(p_grafo g) {
    int i;
    for (i = 0; i < g->n; i++)
        libera_lista(g->adj[i]);
    free(g->adj);
    free(g);
} 

p_no insere_na_lista(p_no lista, char nome[], int populacao, int vertice, double distancia) { 
    p_no novo = malloc(sizeof(No));
    novo->populacao = populacao;
    novo->vertice = vertice;
    strcpy(novo->nome, nome);
    novo->peso = distancia;
    novo->prox = lista;
    return novo;
}

void insere_aresta(p_grafo g, char cidade1[], char cidade2[], double distancia, p_no *cidades) { 
    int i, vertice1 = -1, vertice2 = -1;
    for (i = 0; (i < g->n) && (vertice1 == -1 || vertice2 == -1); i++) {
        if(strcmp(cidade1, cidades[i]->nome) == 0)
            vertice1 = i;
        if(strcmp(cidade2, cidades[i]->nome) == 0)
            vertice2 = i;
    }
        
    g->adj[vertice1] = insere_na_lista(g->adj[vertice1], cidades[vertice2]->nome, cidades[vertice2]->populacao, cidades[vertice2]->vertice, distancia);
    g->adj[vertice2] = insere_na_lista(g->adj[vertice2], cidades[vertice1]->nome, cidades[vertice1]->populacao, cidades[vertice1]->vertice, distancia);
}

p_fp criar_filaprio(int tam) {
    p_fp fprio = malloc(sizeof(FP));
    fprio->v = malloc(tam * sizeof(Item));
    fprio->n = 0;
    return fprio;
}

/* Libera a memoria alocada para a estrutura. */
void destruir_fila(p_fp fila, int n) {
    for (int i = 0; i < n; i++) 
        free(fila->v[i]);
    free(fila->v);
    free(fila);
}

/* Troca a posição de duas cidades (Item) em um vetor. */
void troca(p_item *a, p_item *b) {
    p_item aux = *a;
    *a = *b;
    *b = aux;
}

/* Sobe no heap, verificando e trocando as posições de alguns elementos para respeitar a condição de min heap, em que os filhos são maiores ou iguais ao pai. */
void sobe_no_heap(p_fp fprio, int t) {
    if (t > 0 && fprio->v[PAI(t)]->prioridade > fprio->v[t]->prioridade) {
        troca(&fprio->v[t], &fprio->v[PAI(t)]);
        sobe_no_heap(fprio, PAI(t));
    }
}

/* Desce no heap, verificando e trocando as posições de alguns elementos para respeitar a condição de min heap, em que os filhos são maiores ou iguais ao pai. */
void desce_no_heap(p_fp fprio, int t) {
    int menor_filho;
    if (F_ESQ(t) < fprio->n) {
        menor_filho = F_ESQ(t);
        if (F_DIR(t) < fprio->n && fprio->v[F_ESQ(t)]->prioridade > fprio->v[F_DIR(t)]->prioridade)
            menor_filho = F_DIR(t);
        if (fprio->v[t]->prioridade > fprio->v[menor_filho]->prioridade) {
            troca(&fprio->v[t], &fprio->v[menor_filho]);
            desce_no_heap(fprio, menor_filho);
        }
    }
}

/* Insere em um heap. */
void insere_heap(p_fp fprio, int vertice, double prioridade) { 
    p_item elemento = malloc(sizeof(Item));
    elemento->vertice = vertice;
    elemento->prioridade = prioridade;
    fprio->v[fprio->n] = elemento;
    fprio->n++;
    sobe_no_heap(fprio, fprio->n - 1);
}

/* Devolve o minimo do min heap. */
p_item extrai_minimo(p_fp fprio) {
    p_item elemento = fprio->v[0];
    if (fprio->n > 0) {    
        troca(&fprio->v[0], &fprio->v[fprio->n - 1]);
        fprio->n--;
        desce_no_heap(fprio, 0);
    } else 
        fprio->n--;
    return elemento;
}

/* Diminui a prioridade de um item em um min heap. */
void diminui_prioridade(p_fp fprio, int t, double valor) {
    fprio->v[t]->prioridade = valor;
    sobe_no_heap(fprio, t);
}

void atualiza_indice(p_fp h, int n) {
    for (int i = 0; i < n; i++) 
        h->indice[h->v[i]->vertice] = i;    
}

int * dijkstra(p_grafo g, int s, p_no *cidades) {
    int i, *pai = malloc(g->n * sizeof(int));
    p_item v;
    p_no t;
    p_fp h = criar_filaprio(g->n);
    for (i = 0; i < g->n; i++) {
        pai[i] = -1;
        insere_heap(h, i, INT_MAX);
    }

    h->indice = malloc(g->n * sizeof(int));

    pai[s] = s;
    diminui_prioridade(h, s, 0);
    atualiza_indice(h, g->n); //arrumar que isso vai deixar lerdo
    while (h->n > 0) { //heap nao vazio
        v = extrai_minimo(h);
        atualiza_indice(h, g->n); 
        if (v->prioridade != INT_MAX)
            for (t = g->adj[v->vertice]; t != NULL; t = t->prox) {
                if (v->prioridade + t->peso < h->v[h->indice[t->vertice]]->prioridade) {
                    diminui_prioridade(h, h->indice[t->vertice], v->prioridade + t->peso);
                    atualiza_indice(h, g->n);
                    pai[t->vertice] = v->vertice; 
                }
            }
    }
    free(h->indice);
    destruir_fila(h, g->n);
    return pai;
}

/* Organiza o vetor decrescentemente de acordo com os valores de fator de centralidade. */
void merge_f(p_no *cidades, int l, int m, int r, int n) {
    p_no *aux = malloc(n * sizeof(No));
    int i = l, j = m + 1, k = 0;
    /*intercala*/
    while (i <= m && j <= r) {
        if (cidades[i]->f_centralidade < cidades[j]->f_centralidade)
            aux[k++] = cidades[i++];
        else if (cidades[i]->f_centralidade > cidades[j]->f_centralidade)
            aux[k++] = cidades[j++];
        else { //se for o mesmo f_centralidade, o desempate é por ordem alfabetica
            if(strcmp(cidades[i]->nome, cidades[j]->nome) < 0)
                aux[k++] = cidades[i++];
            else
               aux[k++] = cidades[j++];
        }
    }
    /*copia o resto do subvetor que não terminou*/
    while (i <= m)
        aux[k++] = cidades[i++];
    while (j <= r)
        aux[k++] = cidades[j++];
    /*copia de volta para v*/
    for (i = l, k = 0; i <= r; i++, k++)
        cidades[i] = aux[k];
    
    free(aux);
}

/* Algoritmo MergeSort de ordenação. */
void mergesort_f(p_no *cidades, int l, int r, int n) {
    int m = (l + r) / 2;
    if (l < r) {
        /*divisão*/
        mergesort_f(cidades, l, m, n);
        mergesort_f(cidades, m + 1, r, n);
        /*conquista*/
        merge_f(cidades, l, m, r, n);
    }
}

/* Libera a memoria alocada para um p_no vetor, sendo max a qtd de espaços alocados. */
void destruir_vetor(p_no *vetor, int max) {
    for(int i = 0; i < max; i++)
        free(vetor[i]);
    free(vetor);
}

int main() {
    int n, i, j, w, populacao_total = 0, *caminhos, pop_atingida; 
    double distancia, f_centralidade;
    char cidade1[50], cidade2[50];

    scanf("%d", &n);
    p_no *cidades = malloc(n * sizeof(No));
    for (i = 0; i < n; i++) {
        p_no cidade = malloc(sizeof(No));
        scanf("%s %d", cidade->nome, &cidade->populacao);
        cidade->vertice = i; 
        populacao_total += cidade->populacao;
        cidades[i] = cidade;
    }

    p_grafo grafo = criar_grafo(n);
    while (scanf("%s %s %lf", cidade1, cidade2, &distancia) != EOF) {
        insere_aresta(grafo, cidade1, cidade2, distancia, cidades);
    }

    for (i = 0; i < n; i++) {
        caminhos = dijkstra(grafo, i, cidades);
        f_centralidade = 0;
        pop_atingida = cidades[i]->populacao;
        for (j = 0; j < n; j++) {
            distancia = 0;
            if (j == i)
                continue;
            else {
                w = j;
                while (caminhos[w] != i) {
                    if (caminhos[w] == -1)
                        break;
                    else {
                        p_no vertice1 = grafo->adj[w];
                        while (vertice1->vertice != caminhos[w])
                            vertice1 = vertice1->prox;
                        distancia += vertice1->peso;
                        w = caminhos[w];   
                    }
                }
                if (caminhos[w] != -1) {
                    p_no vertice1 = grafo->adj[w];
                    while (vertice1->vertice != i) //fazer função
                        vertice1 = vertice1->prox;
                    distancia += vertice1->peso;
                    pop_atingida += cidades[j]->populacao;
                    f_centralidade += (distancia * cidades[j]->populacao);
                }
            }
        }
        f_centralidade = f_centralidade/pop_atingida;
        if (pop_atingida > populacao_total/2) //verificar se é mais da metade da pop
            cidades[i]->f_centralidade = f_centralidade;
        else
            cidades[i]->f_centralidade = 0;

        free(caminhos);
    }
    mergesort_f(cidades, 0, n - 1, n);
    for (i = 0; i < n; i++)
        if (cidades[i]->f_centralidade != 0)
            printf("%s %.2lf \n", cidades[i]->nome, cidades[i]->f_centralidade);
    
    destruir_vetor(cidades, n);
    destroi_grafo(grafo);

    return 0;
}