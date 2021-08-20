#include <stdio.h>
#include <string.h>

#define MAX_ESTUDANTES 100
#define MAX_CHAR 31 //somei 1 para incluir o '\0'

void separa_nomes(char estudantes[][MAX_CHAR], int n, char nomes[][MAX_CHAR], char sobrenomes[][MAX_CHAR]){
    //função para separar o nome completo dos estudantes em outras duas matrizes (nomes e sobrenomes)

    int i, k, eh_nome, contador;
    char estudante[MAX_CHAR];
    for (i = 0; i < n; i++){
        strcpy(estudante, estudantes[i]);
        eh_nome = 1;
        contador = 0;
        for(k = 0; k < strlen(estudante); k++){
            if(estudante[k] == '_'){
                nomes[i][k] = '\0';
                eh_nome = 0;
            } else if(eh_nome){ 
                nomes[i][k] = estudante[k];
            } else {
                sobrenomes[i][contador] = estudante[k];
                contador++;
            }
        }
        sobrenomes[i][contador] = '\0';
    }
}


void ordem_alfabetica(char alfabetico[][MAX_CHAR], char padrao[][MAX_CHAR], int n){
    /* Algoritmo de ordenação BubbleSort adaptado para esse programa.
    Na matriz alfabetico as palavras são ordenadas de maneira alfabetica.
    Já a matriz padrão serve para o caso dos parentes, em que é necessário colocar os sobrenomes em ordem 
    alfabetica para possibilitar identificar os iguais, porém simultaneamente é alterada a posição do mesmo 
    nome na matriz estudantes (padrao) para que seja possivel imprimir depois.
    */

    int i, trocou, j;
    char aux1[MAX_CHAR], aux2[MAX_CHAR];

    for(i = 0; i < n; i++){
        trocou = 0;
        for(j = n - 1; j > i; j--){
            if(strcmp(alfabetico[j - 1], alfabetico[j]) > 0){
                strcpy(aux1, alfabetico[j - 1]);
                strcpy(alfabetico[j - 1], alfabetico[j]);
                strcpy(alfabetico[j], aux1);
                trocou = 1;
                strcpy(aux2, padrao[j - 1]);
                strcpy(padrao[j - 1], padrao[j]);
                strcpy(padrao[j], aux2);
            }
        }
        if (! trocou)
            break;
    }
}

void imprime_iguais(char nomes[][MAX_CHAR], int n, char estudantes[][MAX_CHAR]) {
    //identifica os homonimos ou parentes e os imprime

    int i;
    char nome[MAX_CHAR], ultimo_impresso[MAX_CHAR];

    for(i = 0; i < n; i++) {
        strcpy(nome, nomes[i - 1]);
        if(strcmp(nome, nomes[i]) == 0){
            if (strcmp(ultimo_impresso, nome) != 0)
               printf("%s\n", estudantes[i - 1]);

            printf("%s\n", estudantes[i]); //o indice do nome do estudante na matriz nomes ou sobrenomes é o mesmo na matriz estudantes
            strcpy(ultimo_impresso, nome);
        }
    }
}

int main() {
    int n, i;
    char tipo[10];
    char estudantes[MAX_ESTUDANTES][MAX_CHAR], nomes[MAX_ESTUDANTES][MAX_CHAR], sobrenomes[MAX_ESTUDANTES][MAX_CHAR]; 

    //lendo a entrada
    scanf("%d ", &n);
    scanf("%s ", tipo);
    for(i=0; i < n; i++)
        scanf("%s", estudantes[i]);

    ordem_alfabetica(estudantes, sobrenomes, n);
    separa_nomes(estudantes, n, nomes, sobrenomes);
    if (strcmp(tipo, "homonimos") == 0) 
        imprime_iguais(nomes, n, estudantes);
        
    else { //parentes
        ordem_alfabetica(sobrenomes, estudantes, n);
        imprime_iguais(sobrenomes, n, estudantes);
    }

    return 0;
}