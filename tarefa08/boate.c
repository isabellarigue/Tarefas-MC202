#include <stdio.h>
#include <string.h>
#include "fila.h"

int main() {
    p_no lista; //agrupar
    p_no pessoa;
    p_no lanterna;
    int documento;
    char profissao[26];
    char status_vacinacao[13];
    int primeira=1;

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
                    continue;
            else {
                if (strcmp(pessoa->profissao, "funcionario") == 0)
                    lista = adicionar_final(lista, pessoa, &lanterna); 
                else { //pessoas comuns ou fiscal
                    lista = adicionar_meio(lista, pessoa, &lanterna); //mudar nome dessa função?
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