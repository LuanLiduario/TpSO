#include "tpmanager.h"

int main() {//PM
	char string ;
	Instrucao *programa;
	CPU cpu;
	programa = lerArq("init");
	cpu.cont_programa = 0;
    cpu.valor_inteiro = 0;
    cpu.tempo_processo = 0;
    cpu.tempo_atual = 0;
    cpu.programa = programa;
    estado_executando = addTabelaPCB(programa,0,0,0);
    printTabelaPCB();
	do {
		scanf("%c", &string);
		//Comandos
		switch(string){
			case 'Q': // Fim de uma unidade de tempo
					if (estado_executando != NULL)
					{
						cpu = processoSimulado(cpu);
					}else
					{
						printf("SEM PROCESSOS\n");
					}
					tempo++;
					//printf("Q\n");
				break;
			case 'U': // Desbloqueie o primeiro processo simulado que esta na fila de bloqueados
					// desbloquerProcesso();
					printf("U\n");
				break;
			case 'P': // Imprima o estado atual do sistema
					// reporter();
					printf("P\n");
				break;
			case 'T': // Imprima o tempo de retorno medio e finaliza o simulador
					// reporter();
					printf("T\n");
				break;
			default:
				//printf("FILHO : %c\n", string);
				break;
		}
	} while(string != 't' && string != 'T' );

}

ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor){
	TabelaPcb processo;
	processo.id_processo = id;
    processo.id_processo_pai = id_pai;
	processo.cont_programa = 0;
	processo.valor_inteiro = valor;
	processo.estado = 3;// Pronto
	processo.tempo_inicio = tempo;
	processo.tempo_cpu_utilizada = 0;
	processo.programa = programa;
	int indice = nProcessos;////
	if(tabelaPcb == NULL){// tabela está vazia
		tabelaPcb = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		tabelaPcb->indice = indice;
		tabelaPcb->tabelaPcb = processo;
		tabelaPcb->prox = NULL;
		tabelaPcb->anterior = NULL; 
		return tabelaPcb; 
	}else{
		ListaTabelaPcb *aux = NULL;
		aux = tabelaPcb;
		while(aux->prox != NULL){
			aux = aux->prox;
		}
		aux->prox = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		aux->prox->indice = indice;
		aux->prox->tabelaPcb = processo;
		aux->prox->prox = NULL;
		aux->prox->anterior = aux;
		return aux->prox;
	}
    nProcessos ++;

}

void terminarProcessoSimulado(){
	ListaTabelaPcb *aux;
	aux = estado_executando;
	//Retirar da tabela PCB
	if (estado_executando->anterior == NULL && estado_executando->prox == NULL){ 
		// free(estado_executando);
		free(tabelaPcb);
		tabelaPcb = NULL;
		return;
	}
	if (estado_executando->anterior == NULL){
		tabelaPcb = tabelaPcb->prox;
		free(aux);
		return;
	}
	if (estado_executando->prox == NULL){
		aux->anterior->prox = NULL;
		free(aux);
		return;
	}
	aux->prox->anterior = aux->anterior;
	aux->anterior->prox = aux->prox;
	free(aux);
}

void printTabelaPCB(){
	printf("___________________________\n");
	ListaTabelaPcb *aux = NULL;
	aux = tabelaPcb;
	while(aux != NULL){
		printf("\n\nPROCESSO :\n");
		printf("indice %d:\n",aux->indice);
		printf("ID : %d\n",aux->tabelaPcb.id_processo);
		printf("ID PAI: %d\n",aux->tabelaPcb.id_processo_pai);
		printf("ESTADO : %d\n",aux->tabelaPcb.estado);
		imprimaCpu(aux->tabelaPcb.programa);
		aux = aux->prox;
	}
	printf("___________________________\n");
}

Instrucao* lerArq(char * filename){
	FILE *arq = fopen(filename, "r");
	printf("%s\n",filename);
    if(arq == NULL){ 
        printf("\nERRO NA ABERTURA DO ARQUIVO\n\n\n");
        exit(1);
    };

    int nintrucoes = 1;
    char comando;
    char *string;
    string = (char*) malloc(sizeof(char)*50);
    char *valor;
    valor = (char*) malloc(sizeof(char)*50);
 	Instrucao instrucao;
    Instrucao *programa = (Instrucao*)malloc(sizeof(Instrucao)*100);
    int i;
    while(!feof(arq)){//percorre o arquivo
    	 fgets(string,50,arq);
    	 instrucao.instrucao = string[0];
    	 if(instrucao.instrucao != 'B' && instrucao.instrucao != 'E'){
			i = 2; 
			while(string[i] != '\n'){
				valor[i-2] = string[i];
				i++;
			}
			valor[i-3]='\0';
			//printf("%s",valor);
			if(instrucao.instrucao == 'R'){
				instrucao.filename = (char*) malloc(sizeof(char)*50);
				strcpy (instrucao.filename, valor);
				//printf(" Intrucao %d = %c %s\n",nintrucoes,instrucao.instrucao,instrucao.filename );
			}else{
				instrucao.valor = atoi(valor);
				//printf(" Intrucao %d = %c %d\n",nintrucoes,instrucao.instrucao,instrucao.valor );
			}
    	 }
    	
    	 programa[nintrucoes-1] = instrucao;
    	 nintrucoes++;
    	 //cpu.instrucoes = realloc(cpu.instrucoes,sizeof(Programa)*nintrucoes);
    }
    imprimaCpu(programa);
    getchar();
    fclose(arq);
   
    return programa;
   //imprimaCpu(cpu,nintrucoes);
}


void imprimaCpu(Instrucao *programa){

	// printf("instrucoes: (%d)\n",n);
	int i = 0;
	while (1)
	{
		printf(" Instrucao :%c  ",programa[i].instrucao);
		switch(programa[i].instrucao){
		case 'S': // n: Atualiza o valor da variavel inteira para n.
			printf(": %d \n",programa[i].valor);
			break;
		case 'A': // n: Soma n na variavel inteira.
			printf(": %d \n",programa[i].valor);
			break;
		case 'D':// D n: Subtrai n na variavel inteira.
			printf(": %d \n",programa[i].valor);
			break;
		case 'B': // B: Bloqueia o processo simulado.
			printf("B\n");
			break;
		case 'E': // Termina o processo simulado.
			printf("E  \n");
			return;
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			printf(": %d \n",programa[i].valor);
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			printf(": %s\n",programa[i].filename);
			break;
		default:
			printf("ERRO\n");
			break;
		}
		i++;
	}
}

CPU processoSimulado(CPU cpu)
{
//Processo Simulado
	printf("TO NO Q com instrucao %c \n",cpu.programa[cpu.cont_programa].instrucao);
	cpu.tempo_atual++;
	switch(cpu.programa[cpu.cont_programa].instrucao){
		case 'S': // n: Atualiza o valor da variavel inteira para n.
			cpu.valor_inteiro = cpu.programa[cpu.cont_programa].valor;
			cpu.cont_programa++;
			break;
		case 'A': // n: Soma n na variavel inteira.
			cpu.valor_inteiro = cpu.valor_inteiro + cpu.programa[cpu.cont_programa].valor ;
			cpu.cont_programa++;
			break;
		case 'D':// D n: Subtrai n na variavel inteira.
			cpu.valor_inteiro = cpu.valor_inteiro - cpu.programa[cpu.cont_programa].valor ;
			cpu.cont_programa++;
			break;
		case 'B': // B: Bloqueia o processo simulado.
			// bloquearProcessoSimulado(cpu);
			cpu.cont_programa++;
			break;
		case 'E': // Termina o processo simulado.
			terminarProcessoSimulado();
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			printf("TO NO F\n");
			ListaTabelaPcb *aux;
			aux = addTabelaPCB(cpu.programa,nProcessos,estado_executando->tabelaPcb.id_processo,cpu.valor_inteiro);
			addFila(estado_pronto,aux);
			cpu.cont_programa++;
			if(cpu.programa[cpu.cont_programa].instrucao == 'R'){
				cpu.cont_programa++;
			}
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			printf("TO NO R\n");
			cpu.programa = lerArq(cpu.programa[cpu.cont_programa].filename);
			cpu.cont_programa = 0;
			estado_executando->tabelaPcb.programa = cpu.programa;
			printTabelaPCB();
			break;
		default:
			printf("ERRO INSTRUCAO NAO RECONHECIDA\n");
			//exit(1);
			break;
	}
	
	return cpu;
}

void addFila(Fila *fila,ListaTabelaPcb *processo){
	if(fila == NULL){
		fila = (Fila*) malloc(sizeof(Fila));
		fila->referenceTabelaPcb = processo;
		fila->prox = NULL;
	}else{
		Fila *aux = fila;
		while(aux->prox != NULL){// sempre inserir no FIM
			aux = aux->prox;
		}
		aux->prox = (Fila*) malloc(sizeof(Fila));
		aux->prox->prox = NULL;
	}
}

void retirarFila(Fila *fila){
	if(fila == NULL){
		printf("FILA VAZIA\n");
	}else if(fila->prox == NULL){
		free(fila);
		fila = NULL;
	}else{
		Fila *aux = fila;
		fila = fila->prox;
		free(aux);
	}
}

void printFILA(Fila *fila){
	printf("FUNCAO TESTE\n");
}


// void bloquearProcessoSimulado(cpu){
// 	tabelaPcb[executando].estado = 2;
// 	tabelaPcb[executando].cont_programa = cpu.cont_programa;
// 	tabelaPcb[executando].valor_inteiro = cpu.valor_inteiro;
// 	tabelaPcb[executando].tempo_cpu_utilizada = tabelaPcb[executando].tempo_cpu_utilizada + cpu.tempo_atual;
// 	if(estado_bloqueado == NULL){
// 		estado_bloqueado = (fila*) malloc(sizeof(fila));
// 		estado_bloqueado.indice = executando;
// 		estado_bloqueado.prox = NULL;
// 	}else{
// 		fila aux = estado_bloqueado;
// 		while(aux->prox != NULL){
// 			aux = aux->prox;
// 		}
// 		estado_bloqueado->prox = (fila*) malloc(sizeof(fila));
// 		estado_bloqueado->prox.indice = executando;
// 		estado_bloqueado->prox->prox = NULL;
// 	}
// 	// trocarContexto();
// }

// void desbloquerProcesso(){
// 	fila aux;
// 	int indice;
// 	if(estado_bloqueado != NULL){
// 		indice = estado_bloqueado.indice;
// 		aux = estado_bloqueado->prox;
// 		free(estado_bloqueado);
// 		estado_bloqueado = aux;
// 		if(estado_prontos == NULL){
// 			estado_prontos = (fila*) malloc(sizeof(fila));
// 			estado_prontos.indice = indice;
// 			estado_prontos.prox = NULL;
// 		}else{
// 			aux = estado_bloqueado;
// 			while(aux->prox != NULL){
// 				aux = aux->prox;
// 			}
// 			estado_bloqueado->prox = (fila*) malloc(sizeof(fila));
// 			estado_bloqueado->prox.indice = indice;
// 			estado_bloqueado->prox->prox = NULL;
// 		}
// 	}else{
// 		printf("NAO HA PROCESSOS BLOQUEADOS\n");
// 	}
// }

// void terminarProcessoSimulado(){
// 	// tabelaPcb[executando].id_processo = id;
//     // tabelaPcb[executando].id_processo_pai = id_pai;
// 	// tabelaPcb[executando].cont_programa = 0;
// 	// tabelaPcb[executando].valor_inteiro = valor;
// 	tabelaPcb[executando].estado = -1;// Pronto
// 	// tabelaPcb[executando].tempo_inicio = tempo;
// 	// tabelaPcb[executando].tempo_cpu_utilizada = 0;
// 	tabelaPcb[executando].programa = NULL;
// 	trocarContexto();
// }

// void reporter(){}

// void trocarContexto(){}
