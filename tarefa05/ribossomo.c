#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct transcricao {
    int tamanho, *id;
    char dna_padrao[14], rna_padrao[14];
    char *segmento_dna, *segmento_rna;
} Transcricao;

Transcricao le_dados() {
    Transcricao transcricao;

    scanf("%s", transcricao.dna_padrao);
    scanf("%s", transcricao.rna_padrao);
    scanf("%d", &transcricao.tamanho);
    transcricao.segmento_dna = malloc((transcricao.tamanho * sizeof(char)) + 1); //somando um para caber '\0'
    transcricao.segmento_rna = malloc((transcricao.tamanho * sizeof(char)) + 1);
    transcricao.id = malloc(transcricao.tamanho * sizeof(int));
    scanf("%s", transcricao.segmento_dna);

    return transcricao;
}

int acha_indice(char vetor[], char elemento, int n) {
    int i, indice;
    for (i = 0; i < n; i++) {
        if (vetor[i] == elemento) {
            indice = i;
            break;
        }
    }
    return indice;
}

void transcricao_dna(Transcricao transcricao) {
    int i, indice;
    for (i = 0; i < transcricao.tamanho; i++) {
        indice = acha_indice(transcricao.dna_padrao, transcricao.segmento_dna[i], 13);
        transcricao.segmento_rna[i] = transcricao.rna_padrao[indice];
    }
}

int quantidade(char *vetor, int max, char elemento) {
    // retorna a quantidade de vezes que um elemento aparece ate uma posicao maxima de um vetor
    int i, qtd=0;
    for (i = 0; i < max; i++) {
        if (vetor[i] == elemento)
            qtd++;
    }
    return qtd;
}

void id(Transcricao transcricao) {
    int i;
    for (i = 0; i < transcricao.tamanho; i++)
        transcricao.id[i] = quantidade(transcricao.segmento_dna, i, transcricao.segmento_dna[i]);
}

void imprime_vetores_char(char *vetor, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) 
        printf("%c\t", vetor[i]);
    printf("\n");
}

void imprime_vetores_int(int *vetor, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) 
        printf("%d\t", vetor[i]);
    printf("\n");
}

int main() {
    int i;
    Transcricao transcricao;

    transcricao = le_dados();
    transcricao_dna(transcricao);
    id(transcricao);

    printf("ID:\t");
    imprime_vetores_int(transcricao.id, transcricao.tamanho);
    printf("DNA:\t");
    imprime_vetores_char(transcricao.segmento_dna, transcricao.tamanho);
    printf("\t\t");
    for (i = 0; i < transcricao.tamanho; i++)
        printf("|\t");
    printf("\nRNA:\t");
    imprime_vetores_char(transcricao.segmento_rna, transcricao.tamanho);

    free(transcricao.segmento_dna);
    free(transcricao.segmento_rna);
    free(transcricao.id);
    return 0;
}