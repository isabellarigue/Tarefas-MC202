#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int forca, destreza, constituicao, inteligencia, sabedoria, carisma;
    char nome[31];
} Atributos;
typedef Atributos * p_at;

typedef struct {
    int **adj;
    int n;
} Grafo;
typedef Grafo * p_grafo;

p_grafo criar_grafo(int n) { 
    int i, j;
    p_grafo g = malloc(sizeof(Grafo));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (i = 0; i < n; i++)
        g->adj[i] = malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adj[i][j] = 0;
    return g;
}

void destruir_grafo(p_grafo g) { 
    for (int i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

int calcular_distancia(p_at vertice1, p_at vertice2) { //sendo vertice1 a sala e vertice2 o jogador
    int distancia = 0;
    if (vertice1->forca > vertice2->forca)
        distancia += vertice1->forca - vertice2->forca;

    if (vertice1->destreza > vertice2->destreza)
        distancia += vertice1->destreza - vertice2->destreza;

    if (vertice1->constituicao > vertice2->constituicao)
        distancia += vertice1->constituicao - vertice2->constituicao;

    if (vertice1->inteligencia > vertice2->inteligencia)
        distancia += vertice1->inteligencia - vertice2->inteligencia;

    if (vertice1->sabedoria > vertice2->sabedoria)
        distancia += vertice1->sabedoria - vertice2->sabedoria;

    if (vertice1->carisma > vertice2->carisma)
        distancia += vertice1->carisma - vertice2->carisma;
    
    return distancia;
}

/* Libera a memoria alocada para um p_at vetor, sendo max a qtd de espaços alocados. */
void destruir_vetor(p_at *vetor, int max) {
    for(int i = 0; i < max; i++)
        free(vetor[i]);
    free(vetor);
}

void leitura();

int main() {
    int i, j, n, m, eh_possivel, imprimiu = 0;
    scanf("%d", &n);
    p_at *celas = malloc(n * sizeof(Atributos));
    for (i = 0; i < n; i++) {
        p_at cela = malloc(sizeof(Atributos));
        scanf("%s %d %d %d %d %d %d", cela->nome, &cela->forca, &cela->destreza, &cela->constituicao,
        &cela->inteligencia, &cela->sabedoria, &cela->carisma);
        celas[i] = cela;
    }
    scanf("%d", &m);
    p_grafo grafo = criar_grafo(n + m);

    p_at *personagens = malloc(m * sizeof(Atributos));
    for (i = 0; i < m; i++) { 
        p_at personagem = malloc(sizeof(Atributos));
        scanf("%s %d %d %d %d %d %d", personagem->nome, &personagem->forca, &personagem->destreza, &personagem->constituicao,
            &personagem->inteligencia, &personagem->sabedoria, &personagem->carisma); 
        personagens[i] = personagem;
    }

    for (i = 0; i < n; i++)
        for(j = n; j < (n + m); j++)
            grafo->adj[i][j] = calcular_distancia(celas[i], personagens[j - n]);


    for (i = 0; i < n; i++) {
        eh_possivel = 1;
        j = n;
        while(eh_possivel && j < (n + m))
            for(j = n; j < (n + m); j++) {
                if (grafo->adj[i][j] > 2)
                    eh_possivel = 0;
            }
        if (eh_possivel) {
            printf("%s\n", celas[i]->nome);
            imprimiu = 1;
        }
    }
    if (! imprimiu)
        printf("Impossivel terminar em duas rodadas.");

    destruir_grafo(grafo);
    destruir_vetor(celas, n);
    destruir_vetor(personagens, m);

    return 0;
}