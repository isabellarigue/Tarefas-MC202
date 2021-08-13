#include <stdio.h>

void cria_vetores(int vetor[], int n) {
    int i;
    for(i=0; i < n; i++)
        vetor[i] = i + 1;
}

void inverso(int vetor[], int n, int a, int b) {
    int i, pos_a, pos_b, max, contador=0, aux;
    for(i=0; i < n; i++){
        if(vetor[i] == a)
            pos_a = i;
        if(vetor[i] == b)
            pos_b = i;
    }
    max = ((pos_b - pos_a) / 2) + 1;
    max += pos_a;
    for(i=pos_a; i < max; i++) {
        aux = vetor[i];
        vetor[i] = vetor[pos_b - contador];
        vetor[pos_b - contador] = aux;
        contador += 1;
    }
}

void espera(int vetor[], int n, int a, int b) {
    int i=0;
    while(vetor[i] != a) 
        i++;
    int x = i;
    while(vetor[x] != b) {
        vetor[x] = vetor[x + 1];
        x++;
    }
    vetor[x] = a;
}

int main() {
    int n, m, i, tipo, a, b;
    scanf("%d", &n);
    int participantes[10000];
    cria_vetores(participantes, n);

    scanf("%d", &m);
    for(i=0; i < m; i++){
        scanf("%d %d %d", &tipo, &a, &b);
        if(tipo == 1)
            inverso(participantes, n, a, b);
        else
            espera(participantes, n, a, b);
    }

    for(i=0; i < n; i++)
        printf("%d ", participantes[i]);

    return 0;
}