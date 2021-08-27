#include <stdio.h>
#include <string.h>
#include "planejar.h"

Data ler_data() {
    Data data;
    scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    return data;
}

Voo ler_voo() {
    Voo voo;
    scanf("%d", &voo.numero);
    scanf("%s %s", voo.aeroporto1, voo.aeroporto2);
    voo.data = ler_data();
    scanf("%lf", &voo.valor);
    return voo;
}

int compara_data(Data data1, Data data2) { 
    if (data1.ano == data2.ano && data1.mes == data2.mes) {
        if (data1.dia > data2.dia)
            return 0;
        else // (data2.dia >= data1.dia)
            return 1;
    } else if (data1.ano == data2.ano) {
        if (data1.mes > data2.mes)
            return 0;
        else
            return 1;
    } else {
        if (data1.ano > data2.ano)
            return 0;
        else
            return 1;
    }
}

int ultimo_dia_do_mes(int mes) {
    if (mes == 4 || mes == 6 || mes == 9)
        return 30;
    else if (mes == 2)
        return 28;
    else
        return 31;
}

int diferenca_data(Data data1, Data data2) {
    int i, dias;
    if (data1.mes == data2.mes && data1.ano == data2.ano)
        return (data2.dia - data1.dia + 1);
    else if (data1.ano == data2.ano) //meses diferentes tb
        return (data2.dia + (ultimo_dia_do_mes(data1.mes) - data1.dia) + 1);
    else { //anos diferentes
        dias = (ultimo_dia_do_mes(data1.mes) - data1.dia) + 1;
        for (i = data1.mes + 1; i <= 12; i++)
            dias += ultimo_dia_do_mes(i);
        if (data2.mes != 1){
            for (i=1; i < data2.mes; i++)
                dias += ultimo_dia_do_mes(i);
        } 
        dias += data2.dia;
        return dias;
    }
}

void cancelar(Voo voos[], int qtd_voos, int numero_voo) {
    int i, j;
    for(i=0; i < qtd_voos; i++) {
        if (voos[i].numero == numero_voo) {
            qtd_voos--;
            for(j = i; j < qtd_voos; j++) {
                voos[j] = voos[j + 1];
            }
            break;
        }
    }
}

void alterar(Voo voos[], int qtd_voos, int numero_voo, double novo_valor) {
    int i;
    for(i=0; i < qtd_voos; i++){ 
        if (voos[i].numero == numero_voo){
            voos[i].valor = novo_valor;
            break;
        }
    }
}

int confere_voos(Voo voo1, Voo voo2, int periodo_ferias) { 
    int dias = diferenca_data(voo1.data, voo2.data);
    if (dias >= 4 && dias <= periodo_ferias)
        return 1;
    else
        return 0;
}

void planejar(Voo voos[], int qtd_voos, Data data_inicio, Data data_fim, char codigo_origem[]) {
    int i, j, contador1=0, contador2=0, periodo_ferias, primeiro=1;
    double menor=1000000, valor_total;
    Voo voos_ida[100];
    Voo voos_volta[100];
    Voo melhor_opcao[2];

    //conferindo o aeroporto de origem/destino e separando os possiveis voos de ida e volta
    for (i = 0; i < qtd_voos; i++) {
        if (strcmp(voos[i].aeroporto1, codigo_origem) == 0) {
            voos_ida[contador1] = voos[i]; 
            contador1++;
        }
        if (strcmp(voos[i].aeroporto2, codigo_origem) == 0) {
            voos_volta[contador2] = voos[i]; 
            contador2++;          
        }
    }

    periodo_ferias = diferenca_data(data_inicio, data_fim);

    //verificando quais voos estao dentro do periodo estipulado e dentre esses separa-se o de menor valor
    for (i = 0; i < contador1; i++ ) {
        for (j = 0; j < contador2; j++) {
            if (confere_voos(voos_ida[i], voos_volta[j], periodo_ferias) && compara_data(data_inicio, voos_ida[i].data) && compara_data(voos_volta[j].data, data_fim)) {
                valor_total = voos_ida[i].valor + voos_volta[j].valor;
                if (primeiro) {
                    menor = valor_total;
                    melhor_opcao[0] = voos_ida[i];
                    melhor_opcao[1] = voos_volta[j];
                    primeiro = 0;
                } else {
                    if (valor_total < menor) {
                        menor = valor_total;
                        melhor_opcao[0] = voos_ida[i];
                        melhor_opcao[1] = voos_volta[j];
                    }      
                }
            }
        }
    }
    
    printf("%d\n", melhor_opcao[0].numero);
    printf("%d", melhor_opcao[1].numero);
}