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

/* Cria um grafo, alocando a memoria. */
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

/* Libera a memoria alocada para um p_grafo g. */
void destruir_grafo(p_grafo g) { 
    for (int i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

/* Calcula e retorna quantas rodadas são necessarias para o personagem conseguir entrar na sala. Colocando
em termos do grafo, calcula qual a distancia entre dois vertices, ou seja, o tamanho da aresta que liga os dois. 
Para o funcionamento adequado da função, vertice1 deve ser uma cela e vertice2 um personagem. */
int calcular_distancia(p_at vertice1, p_at vertice2) { 
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

/* Le as celas/personagens e seus atributos, armazenando esses dados em um vetor auxiliar. */
void leitura(p_at *vetor, int max) {
    for (int i = 0; i < max; i++) {
        p_at elemento = malloc(sizeof(Atributos));
        scanf("%s %d %d %d %d %d %d", elemento->nome, &elemento->forca, &elemento->destreza, 
        &elemento->constituicao, &elemento->inteligencia, &elemento->sabedoria, &elemento->carisma);
        vetor[i] = elemento;
    }
}

/* Verifica e imprime as celas nas quais o grupo todo poderá se encontrar após duas rodadas, se for possivel. 
Em termos do grafo, quando as arestas existem e são maiores que 2, não é possivel que o vertice1 fique 
coincidente com o vertice2 em apenas duas jogadas, ou seja, o personagem não conseguirá chegar nessa cela.*/
void verificar_possibilidades(p_grafo grafo, int n, int m, p_at *celas) {
    int i, j, eh_possivel, imprimiu = 0;
    for (i = 0; i < n; i++) {
        eh_possivel = 1, j = n;
        while(eh_possivel && j < (n + m))
            for(j = n; j < (n + m); j++) 
                if (grafo->adj[i][j] > 2) 
                    eh_possivel = 0;
        if (eh_possivel) {
            printf("%s\n", celas[i]->nome);
            imprimiu = 1;
        }
    }
    if (! imprimiu)
        printf("Impossivel terminar em duas rodadas.");   
}

/* Libera a memoria alocada para um p_at vetor, sendo max a qtd de espaços alocados. */
void destruir_vetor(p_at *vetor, int max) {
    for(int i = 0; i < max; i++)
        free(vetor[i]);
    free(vetor);
}

int main() {
    int i, j, n, m;

    scanf("%d", &n);
    p_at *celas = malloc(n * sizeof(Atributos));
    leitura(celas, n);

    scanf("%d", &m);
    p_at *personagens = malloc(m * sizeof(Atributos));
    leitura(personagens, m);

    p_grafo grafo = criar_grafo(n + m);
    for (i = 0; i < n; i++)
        for(j = n; j < (n + m); j++)
            grafo->adj[i][j] = calcular_distancia(celas[i], personagens[j - n]);

    verificar_possibilidades(grafo, n, m, celas);

    destruir_grafo(grafo);
    destruir_vetor(celas, n);
    destruir_vetor(personagens, m);

    return 0;
}