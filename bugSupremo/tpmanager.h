#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#define quantum 5
#define TamanhoString 50

typedef struct {
	int valor;
	char *filename;
	char instrucao;
}Instrucao;
// Tempo é um inteiro inicializado com 0
int TEMPO = 0;
//CPU é usado para simular a execução de um processo simulado que está no estado executando
typedef struct{
    int cont_programa;
    int valor_inteiro;
    int tempo_processo;
    int tempo_atual;// verificar 
    int nintrucoes;//
    // int prioridade;
    Instrucao *programa;
}CPU;
//TabelaPcb é um array com uma entrada para cada processo que ainda não terminou sua execução.
typedef struct{
    int prioridade;
    int nintrucoes;
	int id_processo_pai;
	int id_processo;
	int cont_programa;////// contrario
    int valor_inteiro;
    int estado; // 1 : executando, 2 : bloqueado, 3 : pronto
    int tempo_inicio;
    int tempo_cpu_utilizada;
    Instrucao *programa;
}TabelaPcb;

typedef struct ListaTabelaPcb{
    int indice;//
    TabelaPcb tabelaPcb;
    struct ListaTabelaPcb *prox;
    struct ListaTabelaPcb *anterior;
} ListaTabelaPcb;

ListaTabelaPcb *tabelaPcb = NULL;

//TabelaPcb tabelaPcb[100];// arrumar
int nProcessos = 0;
//Fila para estados
typedef struct fila{
    ListaTabelaPcb *referenceTabelaPcb;
	struct fila *prox;
} Fila;

//Estado Pronto 
Fila *estado_pronto = NULL;
//Estado Bloqueado 
Fila *estado_bloqueado = NULL;
//Estado Executando 
ListaTabelaPcb *estado_executando = NULL;//N

Instrucao* lerArq(char * filename,int *nintrucoes);
ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont, int nintrucoes);

// ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont);

CPU processoSimulado(CPU cpu);

Fila* addFila(Fila *fila,ListaTabelaPcb *processo);
Fila* retirarFila(Fila *fila);

void bloquearProcessoSimulado();
void desbloquerProcesso();
CPU trocaContexto(CPU cpu);

// void processoReporter();

//Funcoes testes
void printFILA(Fila *fila);
void printTabelaPCB();
void imprimaCpu(Instrucao *intrucao);
void imprimirLinha();
//
CPU escalonar();
void processoReporter();
void ordenarFila (Fila *fila);