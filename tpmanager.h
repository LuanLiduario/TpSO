#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
#define prioridadeMAX 10
#define prioridadeMIN 1
#define quantum 5
#define TamanhoString 50
#define prioridadeMAX 10

typedef struct {
	int valor;// trocar nome
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
    int tempo_atual;
    int nInstrucoes;
    int prioridade;
    Instrucao *programa;
}CPU;
//TabelaPcb é um array com uma entrada para cada processo que ainda não terminou sua execução.
typedef struct{
    int prioridade;
    int nInstrucoes;
	int id_processo_pai;
	int id_processo;
	int cont_programa;////// contrario
    int valor_inteiro;
    int estado; // 1 : executando, 2 : bloqueado, 3 : pronto // verificar 
    int tempo_inicio;
    int tempo_cpu_utilizada;
    Instrucao *programa;
}TabelaPcb;

typedef struct ListaTabelaPcb{
    TabelaPcb tabelaPcb;
    struct ListaTabelaPcb *prox;
    struct ListaTabelaPcb *anterior;
} ListaTabelaPcb;

ListaTabelaPcb *tabelaPcb = NULL;

int nProcessos = 0;// trocar nome
//Fila para estados
typedef struct fila{
    ListaTabelaPcb *referenceTabelaPcb;
	struct fila *prox;
    struct fila *anterior;
} Fila;

//Estado Pronto 
Fila *fila_prontos = NULL;//trocar nome
//Estado Bloqueado 
Fila *fila_bloqueados = NULL;//trocar nome
//Estado Executando 
ListaTabelaPcb *executando = NULL;//N

Instrucao* lerArq(char * filename,int *nInstrucoes);
ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont, int nInstrucoes);

// ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont);

CPU processoSimulado(CPU cpu);

Fila* addFila(Fila *fila,ListaTabelaPcb *processo);
Fila* retirarFila(Fila *fila);

void bloquearProcessoSimulado();
void desbloquerProcesso();
CPU trocaContexto(CPU cpu);

// void processoReporter();

//Funcoes testes
void imprimaCpu(Instrucao *intrucao);
//
CPU escalonar();

void ordenarFila (Fila *fila);

void reporter();

int definirPrioridade(int prioridade);