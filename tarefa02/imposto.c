#include <stdio.h>

#define MAX_FUNCIONARIOS 1000
#define MESES 12

void le_matriz(double matriz[][MESES], int n) {
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 12; j++)
            scanf("%lf", &matriz[i][j]);
}

void calculo_valor_retido(double matriz[][MESES], double matriz_modificada[][15], int i, int j) {
    double rendimento, imposto=0;
    rendimento = matriz[i][j];

    //if (rendimento <=  1499,15)
    //    matriz_modificada[i][j] = 0;
    if (rendimento > 1499.15){ //fazer função desses negocios?
        if (rendimento <= 2246.75)
            imposto = (rendimento - 1499.15)*0.075;
        else 
            imposto = (2246.75 - 1499.15)*0.075;
    }
    if (rendimento > 2246.75){
        if (rendimento <= 2995.70)
            imposto += (rendimento - 2246.75)*0.15;
        else 
            imposto += (2995.70 - 2246.75)*0.15;
    }
    if (rendimento > 2995.70){
        if (rendimento <= 3743.19)
            imposto += (rendimento - 2995.70)*0.225;
        else 
            imposto += (3743.19 - 2995.70)*0.225;
    }
    if (rendimento > 3743.19){
        imposto += (rendimento - 3743.19)*0.275;
    }
    matriz_modificada[i][j] = imposto; // [i + 1] devido a linha inicial com os meses
    matriz_modificada[i][12] += imposto; //soma dos valores retidos

}

void calculo_valor_anual(double matriz_modificada[][15], int i, double base_calculo) {
    double imposto=0;

    if (base_calculo > 18000){ //fazer função desses negocios?
        if (base_calculo <= 26400)
            imposto = (base_calculo - 18000)*0.075;
        else 
            imposto = (26400 - 18000)*0.075;
    }
    if (base_calculo > 26400){
        if (base_calculo <= 36000)
            imposto += (base_calculo - 26400)*0.15;
        else 
            imposto += (36000 - 26400)*0.15;
    }
    if (base_calculo > 36000){
        if (base_calculo <= 44400)
            imposto += (base_calculo - 36000)*0.225;
        else 
            imposto += (44400 - 36000)*0.225;
    }
    if (base_calculo > 44400){
        imposto += (base_calculo - 44400)*0.275;
    }

    matriz_modificada[i][13] = imposto;
}

void modifica_matriz(double matriz[][MESES], double matriz_modificada[][15], int n, double abatimentos[]) {
    int i, j;
    double rendimento, base_calculo;
    for (i = 0; i < n; i++){
        rendimento = 0;
        for (j = 0; j < 12; j++){ // transformar essa parte em função?
            rendimento += matriz[i][j];
            calculo_valor_retido(matriz, matriz_modificada, i, j);
        }
        base_calculo = rendimento - abatimentos[i]; 
        //printf("Base de calculo %d %lf\n", i, base_calculo); //tirar depois
        calculo_valor_anual(matriz_modificada, i, base_calculo);
        matriz_modificada[i][14] = matriz_modificada[i][13] - matriz_modificada[i][12]; // calculo do ajuste
    }
}

void imprime_matriz(double matriz_modificada[][15], int n, int m) {
    int i, j;
    for (i = 0; i < n; i++){
        for (j = 0; j < m; j++)
            printf("%.2lf\t", matriz_modificada[i][j]);
        printf("\n");
    }
}

int main() {
    int n, i;
    double rendimentos[MAX_FUNCIONARIOS][MESES], abatimentos[MAX_FUNCIONARIOS], matriz_modificada[MAX_FUNCIONARIOS][15];

    // lendo a entrada
    scanf("%d", &n);
    le_matriz(rendimentos, n);
    for(i=0; i < n; i++)
        scanf("%lf", &abatimentos[i]);

    modifica_matriz(rendimentos, matriz_modificada, n, abatimentos);
    printf("Jan\tFev\tMar\tAbr\tMai\tJun\tJul\tAgo\tSet\tOut\tNov\tDez\tRetido\tDevido\tAjuste\n");
    imprime_matriz(matriz_modificada, n, 15);

    return 0;
}