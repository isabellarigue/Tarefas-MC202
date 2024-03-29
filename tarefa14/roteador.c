#include <stdio.h>
#include <stdlib.h>
#define PAI(i) ((i-1)/2) /*Pai de i*/
#define F_ESQ(i) (2*i+1) /*Filho esquerdo de i*/
#define F_DIR(i) (2*i+2) /*Filho direito de i*/

typedef struct {
    int id, prioridade, incremento;
} Pacote;

typedef struct {
    Pacote *v;
    int i;
} FP;

typedef FP * p_fp;

/* Troca a posição de dois pacotes em um vetor. */
void troca(Pacote *a, Pacote *b) {
    Pacote aux = *a;
    *a = *b;
    *b = aux;
}

/* Cria uma fila de prioridade, através de um max heap. */
p_fp criar_filaprioridade(int tam) {
    p_fp fprio = malloc(sizeof(FP));
    fprio->v = malloc(tam * sizeof(Pacote)); 
    fprio->i = 0;
    return fprio;
}

/* Libera a memoria alocada para a estrutura. */
void destruir_fila(p_fp fila) {
    free(fila->v);
    free(fila);
}

/* Sobe no heap, verificando e trocando as posições de alguns elementos para respeitar a condição de 
max heap, em que os filhos são menores ou iguais ao pai. Conforme função vista na aula. */
void sobe_no_heap(p_fp fprio, int t) {
    if (t > 0 && fprio->v[PAI(t)].prioridade < fprio->v[t].prioridade) {
        troca(&fprio->v[t], &fprio->v[PAI(t)]);
        sobe_no_heap(fprio, PAI(t));
    }
}

/* Desce no heap, verificando e trocando as posições de alguns elementos para respeitar a condição de 
max heap, em que os filhos são menores ou iguais ao pai. Conforme função vista na aula. */
void desce_no_heap(p_fp fprio, int t) {
    int maior_filho;
    if (F_ESQ(t) < fprio->i) {
        maior_filho = F_ESQ(t);
        if (F_DIR(t) < fprio->i && fprio->v[F_ESQ(t)].prioridade < fprio->v[F_DIR(t)].prioridade)
            maior_filho = F_DIR(t);
        if (fprio->v[t].prioridade < fprio->v[maior_filho].prioridade) {
            troca(&fprio->v[t], &fprio->v[maior_filho]);
            desce_no_heap(fprio, maior_filho);
        }
    }
}

/* Insere um pacote em um max heap. */
void insere(p_fp fprio, int id, int incremento, int prioridade) {
    Pacote pacote;
    pacote.id = id;
    pacote.incremento = incremento;
    pacote.prioridade = prioridade;
    fprio->v[fprio->i] = pacote;
    fprio->i++;
    sobe_no_heap(fprio, fprio->i - 1);
}

/* Devolve o maximo (pacote com maior prioridade) do max heap. */
Pacote extrai_maximo(p_fp fprio) {
    Pacote pacote = fprio->v[0];
    troca(&fprio->v[0], &fprio->v[fprio->i - 1]);
    fprio->i--;
    desce_no_heap(fprio, 0);
    return pacote;
}

/* Muda a prioridade de um pacote em um max heap. */
void muda_prioridade(p_fp fprio, int t, int valor) {
    if (fprio->v[t].prioridade < valor) {
        fprio->v[t].prioridade = valor;
        sobe_no_heap(fprio, t);
    } else {
        fprio->v[t].prioridade = valor;
        desce_no_heap(fprio, t);
    }
}

/* Realiza as operações após o roteador emitir um tick, ou seja 
encaminha e muda a prioridade dos pacotes, conforme enunciado.*/
void tick(p_fp fila, int k, int *num) { 
    int i, nova_prioridade; 
    Pacote pacote;
    printf("TICK %d\n", *num);
    *num += 1;

    /* encaminhando os k pacotes com maior prioridade: */
    for (i = 0; i < k && fila->i > 0; i++) {
        pacote = extrai_maximo(fila);
        printf("%d %d %d\n", pacote.id, pacote.prioridade, pacote.incremento);
    }
    printf("\n");

    /* aumentando a prioridade de cada pacote de acordo com o incremento: */
    for (i = 0; i < fila->i; i++) { 
        nova_prioridade = fila->v[i].prioridade + fila->v[i].incremento;
        muda_prioridade(fila, i, nova_prioridade);
    }
}

int main() {
    int k, m, id, prioridade, incremento, num=1;
    scanf("%d %d", &k, &m);
    p_fp fila = criar_filaprioridade(m);

    while (scanf("%d %d %d", &id, &prioridade, &incremento) != EOF){ 
        if (id == 0 && prioridade == 0 && incremento == 0) 
            tick(fila, k, &num);
        else 
            if (fila->i < m) /* se ainda não atingiu a capacidade maxima m do roteador */
                insere(fila, id, incremento, prioridade);
    }

    destruir_fila(fila);

    return 0;
}