typedef struct data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct voo {
    Data data;
    int numero;
    char aeroporto1[4];
    char aeroporto2[4];
    double valor;
} Voo;

Data ler_data();

Voo ler_voo();

int compara_data(Data data1, Data data2);
// retorna verdadeiro se a data2 esta a frente da data1 ou se elas são iguais

int ultimo_dia_do_mes(int mes);
// retorna qual é o ultimo dia de um mes (essa função não funciona para anos bissextos)

int diferenca_data(Data data1, Data data2);
// retorna a diferença em dias entre duas datas

void cancelar(Voo voos[], int qtd_voos, int numero_voo);
// "cancela" um voo, retirando-o da lista de voos possiveis

void alterar(Voo voos[], int qtd_voos, int numero_voo, double novo_valor);
// altera o valor de um voo

int confere_voos(Voo voo1, Voo voo2, int periodo_ferias);
/* retorna verdadeiro se dois pares de voos são validos, ou seja, se as datas compreendem
 pelo menos quatro dias dentro do período de férias */

void planejar(Voo voos[], int qtd_voos, Data data_inicio, Data data_fim, char codigo_origem[]);
/* imprime os codigos das passagens mais baratas de ida e de volta cujas datas compreendam 
pelo menos quatro dias dentro de seu período de férias */
