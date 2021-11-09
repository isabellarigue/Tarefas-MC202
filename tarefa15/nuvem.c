#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    char chave[50];
    int frequencia;
} No;
typedef No * p_no;

typedef struct {
    char palavra[50];
} No_palavras;
typedef No_palavras * p_no1;

int hash(char *chave, int M) {  //MUDAR PARA HASHING DUPLO
    int i, n = 0;
    for (i = 0; i < strlen(chave); i++)
        n = (256 * n + chave[i]) % M;
    return n;
}

int encontrei(p_no *t, char *chave, int M) {
    int n = hash(chave, M); 
    if (t[n] != NULL && strcmp(t[n]->chave, chave) == 0)
        return 1;
    else
        for (int i = n + 1; i < M; i++) {
            if (t[i]->chave != NULL && strcmp(t[i]->chave, chave) == 0)
                return 1;
        }
    return 0;
}

int retorna_indice(p_no *t, char *chave, int M) {
    int n = hash(chave, M); 
    if (t[n] != NULL && strcmp(t[n]->chave, chave) == 0)
        return n;
    else
        for (int i = n + 1; i < M; i++) {
            if (t[i]->chave != NULL && strcmp(t[i]->chave, chave) == 0)
                return i;
        }
    return 0; //para evitar warning
}

void inserir(p_no *t, char *chave, int M) { //testar depois com a tecnica do sizeof para ver o tamanho
    int indice = -1;
    if (encontrei(t, chave, M))
        indice = retorna_indice(t, chave, M);
    if (indice != -1) 
        t[indice]->frequencia += 1;
    else {
        int n = hash(chave, M); 
        p_no novo = malloc(sizeof(No));
        strcpy(novo->chave, chave);
        novo->frequencia = 1;
        while (t[n] != 0)
            n = (n + 1) % M;
        t[n] = novo;
    }
}


void destruir_hash(p_no t); 

int eh_primo(int num) {
    for (int i = 2; i <= num / 2; i++) 
        if (num % i == 0) 
            return 0;
    return 1;
}

int determinar_M(int n) {
    while (! eh_primo(n) || n == 2)
        n++;
    return n;
}

void merge_frequencias(p_no1 *v, int l, int m, int r, int max, p_no *hash_palavras, int M) {
    p_no1 aux[1000]; //vai ter q mudar e alocar dinamico
    int i = l, j = m + 1, k = 0;
    /*intercala*/
    while (i <= m && j <= r) {
        int indice_i = retorna_indice(hash_palavras, v[i]->palavra, M);
        int indice_j = retorna_indice(hash_palavras, v[j]->palavra, M);
        if (hash_palavras[indice_i]->frequencia <= hash_palavras[indice_j]->frequencia)
            aux[k++] = v[i++];
        else
            aux[k++] = v[j++];
    }
    /*copia o resto do subvetor que não terminou*/
    while (i <= m)
        aux[k++] = v[i++];
    while (j <= r)
        aux[k++] = v[j++];
    /*copia de volta para v*/
    for (i = l, k = 0; i <= r; i++, k++)
        v[i] = aux[k];
}

void mergesort_frequencias(p_no1 *v, int l, int r, int max, p_no *hash_palavras, int M) {
    int m = (l + r) / 2;
    if (l < r) {
        /*divisão*/
        mergesort_frequencias(v, l, m, max, hash_palavras, M);
        mergesort_frequencias(v, m + 1, r, max, hash_palavras, M);
        /*conquista*/
        merge_frequencias(v, l, m, r, max, hash_palavras, M);
    }
}

char * arruma_palavra(char *palavra, p_no *hash_sw, int M_sw) {
    int tamanho = strlen(palavra), j = 0;
    if (tamanho == 1)
        return "null";
    else {
        char *palavra_copia = malloc((tamanho + 1) * sizeof(char));
        for (int i = 0; i < strlen(palavra); i++) 
            if(isalpha(palavra[i])) 
                palavra_copia[j++] = tolower(palavra[i]); 
        palavra_copia[j] = '\0';
        if ((strlen(palavra_copia)) <= 1 || encontrei(hash_sw, palavra_copia, M_sw)) //é stop word
            return "null";
        return palavra_copia;
    }
}

void merge(p_no1 *v, int l, int m, int r, int max) {
    p_no1 aux[1000]; //vai ter q mudar e alocar dinamico
    int i = l, j = m + 1, k = 0;
    /*intercala*/
    while (i <= m && j <= r)
        if (strcmp(v[i]->palavra, v[j]->palavra) <= 0) //(v[i] <= v[j])
            aux[k++] = v[i++];
        else
            aux[k++] = v[j++];
    /*copia o resto do subvetor que não terminou*/
    while (i <= m)
        aux[k++] = v[i++];
    while (j <= r)
        aux[k++] = v[j++];
    /*copia de volta para v*/
    for (i = l, k = 0; i <= r; i++, k++)
        v[i] = aux[k];
}

void mergesort(p_no1 *v, int l, int r, int max) {
    int m = (l + r) / 2;
    if (l < r) {
        /*divisão*/
        mergesort(v, l, m, max);
        mergesort(v, m + 1, r, max);
        /*conquista*/
        merge(v, l, m, r, max);
    }
}

int main () {
    int n, m, i, M_sw, M_p, ultima_frequencia, teste;
    char stop_word[10], palavra[50], ultima_palavra[50];
    p_no1 *ordena;
    scanf("%d", &n);
    scanf("%d", &m);

    M_sw = determinar_M(m);
    p_no *hash_sw = calloc(M_sw, sizeof(No)); //função para criar hash?
    for (i = 0; i < m; i++) {
        scanf("%s", stop_word);
        inserir(hash_sw, stop_word, M_sw);
    }

    i = 0;
    M_p = determinar_M(n);
    p_no *hash_palavras = calloc(M_p, sizeof(No));
    p_no1 *musica = malloc(n * sizeof(No_palavras));
    //while (strcmp(palavra, "(Hahahaha)") != 0) {
    while (scanf("%s", palavra) != EOF) {
        strcpy(palavra, arruma_palavra(palavra, hash_sw, M_sw));
        if (strcmp(palavra, "null") != 0) { 
            if (! encontrei(hash_palavras, palavra, M_p)) {
                p_no1 novo = malloc(sizeof(No_palavras));
                strcpy(novo->palavra, palavra);
                musica[i++] = novo;
            }
            inserir(hash_palavras, palavra, M_p);
        }
        //scanf("%s", palavra); //teste
    }

    // merge no vetor de hash (colocar em ordem de frequencia)
    mergesort_frequencias(musica, 0, i - 1, i, hash_palavras, M_p);

    // imprimir as de maiores frequencias
    int k = 0, contador = 0;
    int indice = retorna_indice(hash_palavras, musica[i - 1]->palavra, M_p);
    ultima_frequencia = hash_palavras[indice]->frequencia;
    strcpy(ultima_palavra, musica[i - 1]->palavra);
    for (int j = i - 2; j > i - 50; j--) { //colocar alfabetico as iguais!!!!
        indice = retorna_indice(hash_palavras, musica[j]->palavra, M_p);
        if(ultima_frequencia != hash_palavras[indice]->frequencia) {
            printf("%s %d\n", ultima_palavra, ultima_frequencia);
            contador++;
            ultima_frequencia = hash_palavras[indice]->frequencia;
            strcpy(ultima_palavra, musica[j]->palavra);
        } else {
            ordena = malloc(200 * sizeof(No_palavras)); //200 é um valor chutado
            while (ultima_frequencia == hash_palavras[indice]->frequencia && k < 200 && j >= 0) {
                p_no1 novo = malloc(sizeof(No_palavras));
                strcpy(novo->palavra, ultima_palavra);
                ordena[k++] = novo;
                ultima_frequencia = hash_palavras[indice]->frequencia;
                strcpy(ultima_palavra, musica[j]->palavra);
                indice = retorna_indice(hash_palavras, musica[j--]->palavra, M_p); //vai dar conflito
            }
            mergesort(ordena, 0, k - 1, k);
            for (int c = 0; c < k && contador < 50; c++) {
                printf("%s %d\n", ordena[c]->palavra, ultima_frequencia);
                contador++;
            }
            free(ordena);
            ultima_frequencia = hash_palavras[retorna_indice(hash_palavras, musica[j + 1]->palavra, M_p)]->frequencia;
            j++;
            k = 0;
        }
    }


    //liberar memoria


    return 0;
}