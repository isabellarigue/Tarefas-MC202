typedef struct No {
    int documento;
    char profissao[26];
    char status_vacinacao[13];
    struct No *prox;
} No;

typedef struct No * p_no;

/* cria uma lista */
p_no criar_lista();

/* retorna 1 se a lista esta vazia, caso contrário retorna 0 */
int lista_vazia(p_no lista);

/* cria e aloca um nó para uma pessoa, adicionando suas caracteristicas: documento, profissão e status de vacinação */
p_no cria_pessoa(int documento, char profissao[26], char status_vacinacao[13]);

/* retorna 1 se a profissão da pessoa a permite passar a frente na fila, caso contrário retorna 0 */
int tem_privilegios(char profissao[26]);

/* adiciona um elemento/pessoa no inicio da lista, tornando essa pessoa a lanterna */
p_no adicionar_inicio(p_no lista, p_no pessoa, p_no *lanterna);

/* adiciona um elemento/pessoa no final da lista, tornando essa pessoa a lanterna */
p_no adicionar_final(p_no lista, p_no pessoa, p_no *lanterna);

/* adiciona um elemento/pessoa uma posição após a lanterna, em seguida
 essa pessoa torna-se a lanterna, para manter a continuidade da fila */
p_no segue_a_fila(p_no lista, p_no pessoa, p_no *lanterna);

/* remove um elemento de uma lista, dado como parâmetro seu ponteiro */
p_no remove_elemento(p_no lista, p_no elemento);

/* verifica se a pessoa atras do fiscal esta vacinada, se não estiver ela é removida 
da lista/fila, repete-se esse processo ate que a pessoa atras do fiscal esteja vacinada */
p_no fiscal(p_no lista, p_no fiscal);

/* imprime cada elemento da lista, no formato "documento profissão status_de_vacinação" */
void imprime(p_no lista);

/* libera a memoria alocada para a lista, destruindo-a */
void destruir_lista(p_no lista);