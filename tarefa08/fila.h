typedef struct No {
    int documento;
    char profissao[26];
    char status_vacinacao[13];
    struct No *prox;
} No;

typedef struct No * p_no;

//cria lista
p_no criar_lista();

int lista_vazia(p_no lista);
//verifica se a lista esta vazia (boas praticas)

p_no cria_pessoa(int documento, char profissao[26], char status_vacinacao[13]);

int tem_privilegios(char profissao[26]);

//remove elemento (dado seu ponteiro "elemento")
p_no remove_elemento(p_no lista, p_no elemento);

//entra no começo
p_no adicionar_inicio(p_no lista, p_no pessoa, p_no *lanterna);

//continua a fila (tendo gnt atras e na frente!)
p_no adicionar_meio(p_no lista, p_no pessoa, p_no *lanterna);

//entra no final
p_no adicionar_final(p_no lista, p_no pessoa, p_no *lanterna);

//verifica a condição do fiscal e vacinação
p_no fiscal(p_no lista, p_no fiscal);

//imprime lista
void imprime(p_no lista);

//destroi lista
void destruir_lista(p_no lista);