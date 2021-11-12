#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 10007

typedef struct No {
    char chave[10];
    int frequencia;
    struct No *prox;
} No;
typedef No * p_no;

typedef struct Hash {
    p_no vetor[MAX];
} Hash;
typedef Hash * p_hash;

/* Retorna o valor do hash. */
int hash(char *chave) {  
    int i, n = 0;
    for (i = 0; i < strlen(chave); i++)
        n = (256 * n + chave[i]) % MAX;
    return n;
}

/* Cria e inicializa uma tabela de hash. */
p_hash criar_hash() {
    p_hash t = malloc(sizeof(Hash));
    for(int i = 0; i < MAX; i++)
        t->vetor[i] = NULL; 
    return t;
}

/* Insere um elemento em uma lista ligada. */
p_no inserir_lista(p_no lista, char *chave, int frequencia) {
    p_no novo = malloc(sizeof(No));
    strcpy(novo->chave, chave);
    novo->frequencia = frequencia;
    novo->prox = lista;
    return novo;
}

/* Busca um elemento em uma lista ligada. */
p_no buscar_lista(p_no lista, char *chave) {
    for (p_no atual = lista; atual != NULL; atual = atual->prox) 
        if (strcmp(atual->chave, chave) == 0)
            return atual;
    return NULL; 
}

/* Libera a memória alocada para uma lista ligada. */
void destruir_lista(p_no lista) {
    if (lista != NULL) {
        destruir_lista(lista->prox);
        free(lista);
    }
}

/* Insere um elemento no hash. */
void inserir(p_hash t, char *chave, int frequencia) {
    int n = hash(chave);
    t->vetor[n] = inserir_lista(t->vetor[n], chave, frequencia);
}

/* Busca a frequencia de uma chave no vetor de hash. */
int retorna_frequencia(p_hash t, char *chave) {
    int n = hash(chave);
    p_no busca = (buscar_lista(t->vetor[n], chave));
    if (busca != NULL)
        return busca->frequencia;
    return -8; //palavra não esta no hash
}

/* Busca um elemento no vetor de hash. */
p_no buscar_elemento(p_hash t, char *chave) {
    int n = hash(chave);
    p_no elemento = (buscar_lista(t->vetor[n], chave));
    return elemento;  
}

/* Retorna a palavra no formato pedido no enunciado, caso não seja possível devolve null. */
void arruma_palavra(char *palavra, p_hash hash_palavras) {
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
    
    int frequencia = retorna_frequencia(hash_palavras, palavra_copia);
    if (frequencia == -1) { //verificando se é stop word
        strcpy(palavra, "null");
        return;
    }

    strcpy(palavra, palavra_copia);
}

/* Organiza o vetor decrescentemente de acordo com os valores de frequencia. */
void merge_frequencias(char v[][51], int l, int m, int r, p_hash hash_palavras) {
    char aux[MAX][51];
    int i = l, j = m + 1, k = 0;
    /*intercala*/
    while (i <= m && j <= r) {
        int frequencia_i = retorna_frequencia(hash_palavras, v[i]);
        int frequencia_j = retorna_frequencia(hash_palavras, v[j]);
        if (frequencia_i > frequencia_j)
            strcpy(aux[k++], v[i++]);
        else if (frequencia_i < frequencia_j)
            strcpy(aux[k++], v[j++]);
        else { //se for a mesma frequencia, o desempate é por ordem alfabetica
            if(strcmp(v[i], v[j]) < 0)
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
void mergesort_frequencias(char v[][51], int l, int r, p_hash hash_palavras) {
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
int leitura(char musica[][51], p_hash hash_palavras) {
    int i = 0;
    char palavra[51];
    while (scanf(" %s", palavra) != EOF) {
        arruma_palavra(palavra, hash_palavras);
        if (strcmp(palavra, "null") != 0) { //se de fato for uma palavra
            p_no elemento = buscar_elemento(hash_palavras, palavra);
            if (elemento == NULL) { //se ainda não estiver no hash
                strcpy(musica[i++], palavra); //mantendo um vetor com a copia das palavras
                inserir(hash_palavras, palavra, 1);
            } else 
                elemento->frequencia += 1; 
        }
    }
    return i; //quantidade de palavras diferentes
}

/* Libera a memoria alocada para uma tabela de hash. */
void destruir_hash(p_hash t) {
    for(int i = 0; i < MAX; i++)
        destruir_lista(t->vetor[i]);
    free(t);    
}

int main() {
    int n, m, i = 0;
    char stop_word[51], musica[MAX][51];
    scanf("%d", &n);
    scanf("%d", &m);

    p_hash hash_palavras = criar_hash();
    for (int k = 0; k < m; k++) {
        scanf("%s", stop_word);
        inserir(hash_palavras, stop_word, -1);
    }

    i = leitura(musica, hash_palavras);
    
    mergesort_frequencias(musica, 0, i - 1, hash_palavras);
    for (int j = 0; j < 50; j++) 
        printf("%s %d\n", musica[j], retorna_frequencia(hash_palavras, musica[j]));

    destruir_hash(hash_palavras);

    return 0;
}