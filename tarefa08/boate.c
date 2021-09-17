#include <stdio.h>
#include <string.h>
#include "fila.h"

int main() {
    p_no lista, pessoa, lanterna;
    int documento, primeira=1;
    char profissao[26], status_vacinacao[13];

    lista = criar_lista();
    while(scanf("%d %s %s", &documento, profissao, status_vacinacao) != EOF) {
        pessoa = cria_pessoa(documento, profissao, status_vacinacao);
        if (primeira) {
            lanterna = pessoa;
            lista = adicionar_inicio(lista, pessoa, &lanterna);
            primeira = 0;
        } else {
            if (tem_privilegios(pessoa->profissao)) 
                lista = adicionar_inicio(lista, pessoa, &lanterna);
            if (strcmp(lanterna->profissao, "fiscal") == 0 && strcmp(pessoa->status_vacinacao, "nao-vacinado") == 0)
                    continue; //Se alguém não vacinado chegar na fila atrás de um fiscal, ele nem entra na fila
            else {
                if (strcmp(pessoa->profissao, "funcionario") == 0)
                    lista = adicionar_final(lista, pessoa, &lanterna); 
                else { //pessoas comuns ou fiscal
                    lista = segue_a_fila(lista, pessoa, &lanterna); 
                    if (strcmp(pessoa->profissao, "fiscal") == 0)
                        lista = fiscal(lista, pessoa);
                }
            }
        }
    }

    imprime(lista);
    destruir_lista(lista);

    return 0;
}