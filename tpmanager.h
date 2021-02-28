#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#define TAMANHOTABELAPCB = 100;

typedef struct {
	int valor;
	char *filename;
	char instrucao;
}Instrucao;
// Tempo é um inteiro inicializado com 0
int tempo = 0;
//CPU é usado para simular a execução de um processo simulado que está no estado executando
typedef struct{
    int cont_programa;
    int valor_inteiro;
    int tempo_processo;
    int tempo_atual;
    Instrucao *programa;
}CPU;
//TabelaPcb é um array com uma entrada para cada processo que ainda não terminou sua execução.
typedef struct{
	int id_processo_pai;
	int id_processo;
	int cont_programa;////// contrario
    int valor_inteiro;
    int estado; // 1 : executando, 2 : bloqueado, 3 : pronto
    int tempo_inicio;
    int tempo_cpu_utilizada;
    Instrucao *programa;
}TabelaPcb;

TabelaPcb tabelaPcb[TAMANHOTABELAPCB];// arrumar
int nProcessos = 0;
//Fila para estados
typedef struct fila{
	int indice;
	struct fila *prox;
} fila;

//Estado Pronto 
fila *estado_pronto = NULL;
//Estado Bloqueado 
fila *estado_bloqueado = NULL;
//Estado Executando 
int estado_executando = 0;//N

void imprimaCpu(CPU cpu,int n);

Instrucao* lerArq(char * filename);


