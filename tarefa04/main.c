#include <stdio.h>
#include <string.h>
#include "planejar.h"
#define MAX_VOOS 100

int main() {
    char tipo[30], codigo_origem[4];
    Voo voos[MAX_VOOS];
    int qtd_voos=0, numero_voo;
    double novo_valor;
    Data data_inicio, data_fim;

    scanf("%s", tipo);
    while (strcmp(tipo, "planejar") != 0) {
        if (strcmp(tipo, "registrar") == 0) {
            voos[qtd_voos] = ler_voo();
            qtd_voos++;

        } else if (strcmp(tipo, "alterar") == 0) {
            scanf("%d %lf", &numero_voo, &novo_valor);
            alterar(voos, qtd_voos, numero_voo, novo_valor);

        } else { //cancelar
            scanf("%d", &numero_voo);
            cancelar(voos, qtd_voos, numero_voo);
            qtd_voos--;
        }
        scanf("%s", tipo);
    }

    //lendo o planejamento
    scanf("%s", codigo_origem);
    data_inicio = ler_data(); 
    data_fim = ler_data();

    planejar(voos, qtd_voos, data_inicio, data_fim, codigo_origem);

    return 0;
}