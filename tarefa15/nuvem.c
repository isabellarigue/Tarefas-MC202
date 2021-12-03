#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define M_p 10007

typedef struct {
    char chave[51];
    int frequencia;
} No;
typedef No * p_no;

/* Retorna o valor do hash. */
int hash(char *chave) {  
    int i, n = 0;
    for (i = 0; i < strlen(chave); i++)
        n = (256 * n + chave[i]) % M_p;
    return n;
}

/* Busca o indice de uma chave no vetor de hash. */
int retorna_indice(p_no *t, char *chave) {
    int n = hash(chave), contador = 0; 
    if (t[n] != NULL && strcmp(t[n]->chave, chave) == 0)
        return n;
    else {
        n = (n + ((M_p)/2) + 1) % M_p;
        while ((t[n]->chave == NULL || strcmp(t[n]->chave, chave) != 0) && contador < M_p) {
            n = (n + ((M_p)/2) + 1) % M_p;
            contador++;
        }
        if (contador < M_p)
            return n;
        else
            return -8; //palavra não esta no hashing
    }
}

/* Insere uma chave no vetor de hash. */
void inserir(p_no *t, char *chave, int eh_stop_word) { 
    int n = hash(chave); 
    p_no novo = malloc(sizeof(No));
    strcpy(novo->chave, chave);
    if (eh_stop_word)
        novo->frequencia = - 1;
    else
        novo->frequencia = 1;
    while (t[n] != 0) //procurando um lugar livre
        n = (n + ((M_p)/2) + 1) % M_p; //hashing duplo
    t[n] = novo;
}

/* Retorna a palavra no formato pedido no enunciado, caso não seja possível devolve null. */
void arruma_palavra(char *palavra, p_no *hash_palavras) {
    int j = 0;
    char palavra_copia[51];
    for (int i = 0; palavra[i] != '\0'; i++) 
        if(isalpha(palavra[i])) //retirando - ' e outros caracteres especiais
            palavra_copia[j++] = tolower(palavra[i]); 
    palavra_copia[j] = '\0';

    if ((strlen(palavra_copia)) <= 1) {  //apenas um caractere não é considerado palavra
        strcpy(palavra, "null");
        return;
    }

    int indice = retorna_indice(hash_palavras, palavra_copia);
    if (indice > 0 && hash_palavras[indice]->frequencia == -1) { //verificando se é stop word
        strcpy(palavra, "null");
        return;
    }

    strcpy(palavra, palavra_copia);
}

/* Organiza o vetor decrescentemente de acordo com os valores de frequencia. */
void merge_frequencias(char v[][51], int l, int m, int r, p_no *hash_palavras) {
    char aux[M_p][51];
    int i = l, j = m + 1, k = 0;
    /*intercala*/
    while (i <= m && j <= r) {
        int indice_i = retorna_indice(hash_palavras, v[i]);
        int indice_j = retorna_indice(hash_palavras, v[j]);
        if (hash_palavras[indice_i]->frequencia > hash_palavras[indice_j]->frequencia)
            strcpy(aux[k++], v[i++]);
        else if (hash_palavras[indice_i]->frequencia < hash_palavras[indice_j]->frequencia)
            strcpy(aux[k++], v[j++]);
        else { //se for a mesma frequencia, o desempate é por ordem alfabetica
            if(strcmp(hash_palavras[indice_i]->chave, hash_palavras[indice_j]->chave) < 0)
                strcpy(aux[k++], v[i++]);
            else
               strcpy(aux[k++], v[j++]);
        }
    }
    /*copia o resto do subvetor que não terminou*/
    while (i <= m)
        strcpy(aux[k++], v[i++]);
    while (j <= r)
        strcpy(aux[k++], v[j++]);
    /*copia de volta para v*/
    for (i = l, k = 0; i <= r; i++, k++)
        strcpy(v[i], aux[k]);
}

/* Algoritmo MergeSort de ordenação. */
void mergesort_frequencias(char v[][51], int l, int r, p_no *hash_palavras) {
    int m = (l + r) / 2;
    if (l < r) {
        /*divisão*/
        mergesort_frequencias(v, l, m, hash_palavras);
        mergesort_frequencias(v, m + 1, r, hash_palavras);
        /*conquista*/
        merge_frequencias(v, l, m, r, hash_palavras);
    }
}

/* Le as entradas da letra de musica, inserindo as palavras no hash e em um vetor como cópia. */
int leitura(char musica[][51], p_no *hash_palavras) {
    int indice, i = 0;
    char palavra[51];
    while (scanf(" %s", palavra) != EOF) {
        arruma_palavra(palavra, hash_palavras);
        if (strcmp(palavra, "null") != 0) { //se de fato for uma palavra
            indice = retorna_indice(hash_palavras, palavra);
            if (indice == -8) { //se ainda não estiver no hash
                strcpy(musica[i++], palavra); //mantendo um vetor com a copia das palavras
                inserir(hash_palavras, palavra, 0);
            } else 
                hash_palavras[indice]->frequencia += 1; 
        }
    }
    
    return i;
}

/* Libera a memoria alocada para um p_no vetor, sendo max a qtd de espaços alocados. */
void destruir_vetor(p_no *vetor, int max) {
    for(int i = 0; i < max; i++)
        free(vetor[i]);
    free(vetor);
}

int main () {
    int n, m, i = 0;
    char stop_word[51], musica[M_p][51];
    scanf("%d", &n);
    scanf("%d", &m);

    p_no *hash_palavras = calloc(M_p, sizeof(No));
    for (int k = 0; k < m; k++) {
        scanf("%s", stop_word);
        inserir(hash_palavras, stop_word, 1);
    }

    i = leitura(musica, hash_palavras);
    
    mergesort_frequencias(musica, 0, i - 1, hash_palavras);
    for (int j = 0; j < 50; j++) 
        printf("%s %d\n", musica[j], hash_palavras[retorna_indice(hash_palavras, musica[j])]->frequencia);
    
    destruir_vetor(hash_palavras, M_p);

    return 0;
}