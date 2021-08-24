typedef struct data {
} Data;

typedef struct voo {
} Voo;

Data ler_data();

int compara_data(Data data1, Data data2);

int diferenca_data(Data data1, Data data2);

void cancelar(Voo voos[], int qtd_voos, int numero_voo);

void alterar(Voo voos[], int qtd_voos, int numero_voo, float novo_valor);

void planejar(Voo voos[], int qtd_voos, Data data_inicio, Data data_fim, char codigo_origem[]);