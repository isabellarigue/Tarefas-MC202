#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    char chave[51];
    int frequencia;
} No;
typedef No * p_no;

/* Retorna o valor do hash. */
int hash(char *chave, int M) {  
    int i, n = 0;
    for (i = 0; i < strlen(chave); i++)
        n = (256 * n + chave[i]) % M;
    return n;
}

/* Libera a memoria alocada para um p_no vetor. */
void destruir_vetor(p_no *vetor, int tam) {
    for(int i = 0; i < tam; i++)
        free(vetor[i]);
    free(vetor);
}

/* Busca o indice de uma chave no vetor de hash. */
int retorna_indice(p_no *t, char *chave, int M) {
    int n = hash(chave, M); 
    if (t[n] != NULL && strcmp(t[n]->chave, chave) == 0)
        return n;
    else {
        for (int i = n + 1; i < M; i++)
            if (t[i]->chave != NULL && strcmp(t[i]->chave, chave) == 0)
                return i;
        for (int i = 0; i < n; i++)  //tem q voltar para o modulo M
            if (t[i]->chave != NULL && strcmp(t[i]->chave, chave) == 0)
                return i;
    }
    return -8; //palavra não esta no hashing
}

/* Insere uma chave no vetor de hash. */
void inserir(p_no *t, char *chave, int M, int eh_stop_word) { 
    int n = hash(chave, M); 
    p_no novo = malloc(sizeof(No));
    strcpy(novo->chave, chave);
    if (eh_stop_word)
        novo->frequencia = - 1;
    else
        novo->frequencia = 1;
    while (t[n] != 0) //procurando um lugar livre
        n = (n + (M/2) + 1) % M; //hashing duplo
    t[n] = novo;
}

/* Retorna a palavra no formato pedido no enunciado, caso não seja possível devolve null. */
void arruma_palavra(char *palavra, p_no *hash_palavras, int M_p) {
    int j = 0;
    if (strlen(palavra) == 1) { //apenas um caractere não é considerado palavra
        strcpy(palavra, "null");
    } else {
        char palavra_copia[51];
        for (int i = 0; palavra[i] != '\0'; i++) 
            if(isalpha(palavra[i])) //retirando - ' e outros caracteres especiais
                palavra_copia[j++] = tolower(palavra[i]); 
        palavra_copia[j] = '\0';

        if ((strlen(palavra_copia)) <= 1) {  //verificando se ainda sobrou algo para ser a palavra
            strcpy(palavra, "null");
            return;
        }

        int indice = retorna_indice(hash_palavras, palavra_copia, M_p);
        if (indice > 0 && hash_palavras[indice]->frequencia == -1) { //verificando se é stop word
            strcpy(palavra, "null");
            return;
        }

        strcpy(palavra, palavra_copia);
    }
}

/* Organiza o vetor decrescentemente de acordo com os valores de frequencia. */
void merge_frequencias(p_no *v, int l, int m, int r, int max, p_no *hash_palavras, int M) {
    p_no *aux = malloc(max *sizeof(p_no));
    int i = l, j = m + 1, k = 0;
    /*intercala*/
    while (i <= m && j <= r) {
        int indice_i = retorna_indice(hash_palavras, v[i]->chave, M);
        int indice_j = retorna_indice(hash_palavras, v[j]->chave, M);
        if (hash_palavras[indice_i]->frequencia > hash_palavras[indice_j]->frequencia)
            aux[k++] = v[i++];
        else if (hash_palavras[indice_i]->frequencia < hash_palavras[indice_j]->frequencia)
            aux[k++] = v[j++];
        else { //se for a mesma frequencia, o desempate é por ordem alfabetica
            if(strcmp(hash_palavras[indice_i]->chave, hash_palavras[indice_j]->chave) < 0)
                aux[k++] = v[i++];
            else
                aux[k++] = v[j++];
        }
    }
    /*copia o resto do subvetor que não terminou*/
    while (i <= m)
        aux[k++] = v[i++];
    while (j <= r)
        aux[k++] = v[j++];
    /*copia de volta para v*/
    for (i = l, k = 0; i <= r; i++, k++)
        v[i] = aux[k];

    free(aux);
}

/* Algoritmo MergeSort de ordenação. */
void mergesort_frequencias(p_no *v, int l, int r, int max, p_no *hash_palavras, int M) {
    int m = (l + r) / 2;
    if (l < r) {
        /*divisão*/
        mergesort_frequencias(v, l, m, max, hash_palavras, M);
        mergesort_frequencias(v, m + 1, r, max, hash_palavras, M);
        /*conquista*/
        merge_frequencias(v, l, m, r, max, hash_palavras, M);
    }
}

int main () {
    int n, m, i = 0, M_p = 10007, indice;
    char stop_word[51], palavra[51];
    scanf("%d", &n);
    scanf("%d", &m);

    p_no *hash_palavras = calloc(M_p, sizeof(No));
    for (int k = 0; k < m; k++) {
        scanf("%s", stop_word);
        inserir(hash_palavras, stop_word, M_p, 1);
    }

    p_no *musica = calloc(M_p, sizeof(No));
    while (scanf(" %s", palavra) != EOF) {
        arruma_palavra(palavra, hash_palavras, M_p);
        if (strcmp(palavra, "null") != 0) { //se de fato for uma palavra
            indice = retorna_indice(hash_palavras, palavra, M_p);
            if (indice == -8) { //se ainda não estiver no hash
                p_no novo = malloc(sizeof(No));
                strcpy(novo->chave, palavra);
                musica[i++] = novo; //mantendo um vetor com a copia das palavras
                inserir(hash_palavras, palavra, M_p, 0);
            } else 
                hash_palavras[indice]->frequencia += 1; 
        }
    }
    
    mergesort_frequencias(musica, 0, i - 1, i, hash_palavras, M_p);
    for (int j = 0; j < 50; j++) 
        printf("%s %d\n", musica[j]->chave, hash_palavras[retorna_indice(hash_palavras, musica[j]->chave, M_p)]->frequencia);
    
    destruir_vetor(hash_palavras, M_p);
    destruir_vetor(musica, M_p);

    return 0;
}