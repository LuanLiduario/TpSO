#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <string.h>
//Constantes
#define prioridadeMAX 10 
#define prioridadeMIN 1
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
	int cont_programa;
    int valor_inteiro;
    int estado; // 1 : executando, 2 : bloqueado, 3 : pronto 
    int tempo_inicio;
    int tempo_cpu_utilizada;
    Instrucao *programa;
}TabelaPcb;

// Lista duplamente encadeada para guardar as structs tabelaPcb
typedef struct ListaTabelaPcb{
    TabelaPcb tabelaPcb;
    struct ListaTabelaPcb *prox;
    struct ListaTabelaPcb *anterior;
} ListaTabelaPcb;

ListaTabelaPcb *tabelaPcb = NULL;

int nProcessos = 0;// Numero de processos ja criados, é utilizado para escolher o id dos processos criados

//Fila para estados
typedef struct fila{
    ListaTabelaPcb *referenceTabelaPcb;// aponta para uma posição da lista de tabelaPcb
	struct fila *prox;// aponta para o proximo da lista
    struct fila *anterior;// aponta para anterior (utilizado para percorrer a lista para trás)
} Fila;

//Estado Pronto 
Fila *fila_prontos = NULL;// irá referenciar os processos que estão no estado de prontos
//Estado Bloqueado 
Fila *fila_bloqueados = NULL;// irá referenciar os processos que estão no estado de bloqueados
//Estado Executando 
ListaTabelaPcb *executando = NULL;//// irá referenciar o processo que esta no estado de executando

//Funções de simular processo:
CPU processoSimulado(CPU cpu);
void bloquearProcessoSimulado();
void desbloquerProcesso();
Instrucao* lerArq(char * filename,int *nInstrucoes);
CPU escalonar(CPU cpu);
CPU trocaContexto(CPU cpu);
int definirPrioridade(int prioridade);

//Funções responsaveis por gerenciar a Lista TabelaPcb
ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont, int nInstrucoes);
void terminarProcessoSimulado();// retira da tabela 

//Funções responsaveis por gerenciar as filas
Fila* addFila(Fila *fila,ListaTabelaPcb *processo);
Fila* retirarFila(Fila *fila);
void ordenarFila (Fila *fila);

//cria e enviar informações para o processo reporter
void reporter();