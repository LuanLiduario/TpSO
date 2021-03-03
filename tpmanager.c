#include "tpmanager.h"

int main() {//PM
	char string ;
	Instrucao *programa;
	CPU cpu;
	int nintrucoes = 0;
	programa = lerArq("init",&nintrucoes);
	cpu.cont_programa = 0;
    cpu.valor_inteiro = 0;
    cpu.tempo_processo = 0;
    cpu.tempo_atual = 0;
    cpu.programa = programa;
    estado_executando = addTabelaPCB(programa,0,0,0,0,nintrucoes);
    //printTabelaPCB();
	do {
		scanf("%c", &string);
		//Comandos
		switch(string){
			case 'Q': // Fim de uma unidade de tempo
					if (estado_executando != NULL)
					{
						cpu = processoSimulado(cpu);
						//printTabelaPCB();
					}else{
						cpu = trocaContexto(cpu);
						if(tabelaPcb != NULL){
							//printTabelaPCB();
						}else{
							printf("TabelaPcb vazia\n");
						}
					}
					//printf("TADS POS INSTRUCAO:\n");
					// printf("Lista Prontos\n");
					// printFILA(estado_pronto);
					// printf("Lista bloqueados\n");
					// printFILA(estado_pronto);
					// printTabelaPCB();
					// printf("\n");
					TEMPO++;
					//printf("Q\n");
				break;
			case 'U': // Desbloqueie o primeiro processo simulado que esta na fila de bloqueados
					desbloquerProcesso();
					if(estado_executando == NULL){
						cpu = trocaContexto(cpu);
					}
				break;
			case 'P': // Imprima o estado atual do sistema
					if(estado_executando != NULL){
						estado_executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    estado_executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    estado_executando->tabelaPcb.tempo_cpu_utilizada =  cpu.tempo_processo + cpu.tempo_atual;
				    	estado_executando->tabelaPcb.programa = cpu.programa;
					}
					processoReporter();
				break;
			case 'T': // Imprima o tempo de retorno medio e finaliza o simulador
					if(estado_executando != NULL){
						estado_executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    estado_executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    estado_executando->tabelaPcb.tempo_cpu_utilizada =  cpu.tempo_processo;
				    	estado_executando->tabelaPcb.programa = cpu.programa;
					}
					processoReporter();
				break;
			default:
				break;
		}
	} while(string != 't' && string != 'T' );

}

ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont, int nintrucoes){
	TabelaPcb processo;
	processo.id_processo = id;
    processo.id_processo_pai = id_pai;
	processo.cont_programa = cont;
	processo.valor_inteiro = valor;
	processo.estado = 3;// Pronto
	processo.tempo_inicio = TEMPO;
	processo.tempo_cpu_utilizada = 0;
	processo.prioridade = (int)(nintrucoes/quantum);
	processo.programa = programa;
	int indice = nProcessos;////
	nProcessos ++;
	if(tabelaPcb == NULL){// tabela está vazia
		tabelaPcb = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		tabelaPcb->indice = indice;
		tabelaPcb->tabelaPcb = processo;
		tabelaPcb->prox = NULL;
		tabelaPcb->anterior = NULL; 
		return tabelaPcb; 
	}else{// encintrar fim da lista
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
}

void terminarProcessoSimulado(){
	ListaTabelaPcb *aux;
	aux = estado_executando;
	//Retirar da tabela PCB
	if (estado_executando->anterior == NULL && estado_executando->prox == NULL){ 
		free(tabelaPcb);
		tabelaPcb = NULL;
	}else if(estado_executando->anterior == NULL){
		tabelaPcb = tabelaPcb->prox;
		tabelaPcb->anterior = NULL;
		free(aux);
	}else if(estado_executando->prox == NULL){
		aux->anterior->prox = NULL;
		free(aux);
	}else{
		aux->prox->anterior = aux->anterior;
		aux->anterior->prox = aux->prox;
		free(aux);
	}
}

CPU escalonar(){
	CPU cpu;
	if(estado_pronto != NULL){
		estado_executando = estado_pronto->referenceTabelaPcb;
		estado_pronto = retirarFila(estado_pronto);
		cpu.cont_programa = estado_executando->tabelaPcb.cont_programa;
	    cpu.valor_inteiro = estado_executando->tabelaPcb.valor_inteiro;
	    cpu.tempo_processo = estado_executando->tabelaPcb.tempo_cpu_utilizada;
	    cpu.tempo_atual = 0;
	    cpu.programa = estado_executando->tabelaPcb.programa;
	}else{//se termina um processo e não tem nenhum na fila de prontos desbloqueia algum?
		estado_executando = NULL;
		//printf("ESCALONADOR : Sem processos prontos\n");
	}
	return cpu;
}

Instrucao* lerArq(char * filename,int *nintrucoes){
	FILE *arq = fopen(filename, "r");
	//printf("%s\n",filename);
    if(arq == NULL){ 
        printf("\nERRO NA ABERTURA DO ARQUIVO\n\n\n");
        exit(1);
    };
     (*nintrucoes) = 1;
    char *string;
    string = (char*) malloc(sizeof(char)*50);
    char *valor;
    valor = (char*) malloc(sizeof(char)*50);
 	Instrucao instrucao;
    Instrucao *programa = (Instrucao*)malloc(sizeof(Instrucao)* (*nintrucoes));
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
			valor[i-3]='\0';//se der tempo eu olho
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
    	
    	 programa[(*nintrucoes)-1] = instrucao;
    	 (*nintrucoes)++;
    	 programa = realloc(programa,((*nintrucoes)+1)*sizeof(Instrucao));
    }
    //imprimaCpu(programa);
    // getchar();
    fclose(arq);
    return programa;
}

CPU processoSimulado(CPU cpu)
{
//Processo Simulado
	estado_executando->tabelaPcb.estado = 1;
	// printf("\n\nTO NO Q com instrucao %c \n",cpu.programa[cpu.cont_programa].instrucao);
	// printf("EXECUTANDO : %d\n\n",estado_executando->tabelaPcb.id_processo);
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
			bloquearProcessoSimulado();//trocar contexto
			cpu.cont_programa++;
			cpu = trocaContexto(cpu);
			//printTabelaPCB();
			break;
		case 'E': // Termina o processo simulado.
			// printf("TO NO E\n");
			// printTabelaPCB();
			terminarProcessoSimulado();//trocar contexto
			// printf("TERMINOU\n");
			// printTabelaPCB();
			estado_executando = NULL;
			// printf("TROCOU\n");
			// printTabelaPCB();
			cpu = escalonar();
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			//printf("TO NO F\n");
			cpu.cont_programa++;
			ListaTabelaPcb *aux;
			aux = addTabelaPCB(cpu.programa,nProcessos,estado_executando->tabelaPcb.id_processo,cpu.valor_inteiro,cpu.cont_programa);
			estado_pronto = addFila(estado_pronto,aux);
			// if(cpu.programa[cpu.cont_programa].instrucao == 'R'){
				// printf("SOMA %d ", cpu.cont_programa);
				cpu.cont_programa =  cpu.cont_programa + cpu.programa[cpu.cont_programa-1].valor;
				// printf("+ %d = %d\n",cpu.programa[cpu.cont_programa-1].valor, cpu.cont_programa);
			// }
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			// printf("TO NO R\n");
			// printTabelaPCB();
			cpu.programa = lerArq(cpu.programa[cpu.cont_programa].filename);
			cpu.cont_programa = 0;
			cpu.valor_inteiro = 0;
			//estado_executando->tabelaPcb.programa = cpu.programa;//mover para main
			// printTabelaPCB();
			break;
		default:
			printf("ERRO INSTRUCAO NAO RECONHECIDA\n");
			printf("CONTADOR %d\n",cpu.cont_programa );
			//exit(1);
			break;
	}
	return cpu;
}

Fila* addFila(Fila *fila,ListaTabelaPcb *processo){
	if(fila == NULL){
		//printf("FILA TA VAZIA ENTAO ALOCO\n");
		fila = (Fila*) malloc(sizeof(Fila));
		fila->referenceTabelaPcb = processo;
		fila->prox = NULL;
	}else{
		Fila *aux = fila;
		while(aux->prox != NULL){// sempre inserir no FIM
			aux = aux->prox;
		}
		aux->prox = (Fila*) malloc(sizeof(Fila));
		aux->prox->referenceTabelaPcb = processo;
		aux->prox->prox = NULL;
	}
	return fila;
}

Fila* retirarFila(Fila *fila){
	if(fila == NULL){
		// printf("FILA VAZIA NO RETIRA\n");
	}else if(fila->prox == NULL){
		free(fila);
		fila = NULL;
	}else{
		Fila *aux = fila;
		fila = fila->prox;
		free(aux);
	}
	return fila;
}

void bloquearProcessoSimulado(){
	estado_executando->tabelaPcb.estado = 2;
	estado_bloqueado = addFila(estado_bloqueado,estado_executando);//add executando na fila de bloqueados
}

void desbloquerProcesso(){
	if(estado_bloqueado == NULL){
		printf("NAO HA PROCESSOS BLOQUEADOS\n");
		return;
	}
	// if(estado_executando == NULL){
	// 	estado_executando = estado_bloqueado->referenceTabelaPcb;
	// 	estado_bloqueado->referenceTabelaPcb->tabelaPcb.estado = 1;
	// 	estado_bloqueado = retirarFila(estado_bloqueado);
	// }else{
		estado_pronto = addFila(estado_pronto,estado_bloqueado->referenceTabelaPcb);
		estado_bloqueado->referenceTabelaPcb->tabelaPcb.estado = 3;
		estado_bloqueado = retirarFila(estado_bloqueado);
//	}
}

CPU trocaContexto(CPU cpu)
{
	if(estado_executando != NULL)
	{// existe processo sendo executado
		//Atualizar tabela PCB
		estado_executando->tabelaPcb.cont_programa = cpu.cont_programa;
	    estado_executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
	    estado_executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
    	estado_executando->tabelaPcb.programa = cpu.programa;
	}
    if(estado_pronto != NULL)
    {//existe processo prontos
    	estado_executando = estado_pronto->referenceTabelaPcb;//o primeiro da fila de prontos sera o novo executando
    	estado_pronto = retirarFila(estado_pronto);//retiro o primeiro da fila de executando
	    //Troca processo na CPU
		cpu.cont_programa = estado_executando->tabelaPcb.cont_programa;
	    cpu.valor_inteiro = estado_executando->tabelaPcb.valor_inteiro;
	    cpu.tempo_processo = estado_executando->tabelaPcb.tempo_cpu_utilizada;
	    cpu.tempo_atual = 0;
	    cpu.programa = estado_executando->tabelaPcb.programa;
    }else
    {
    	estado_executando = NULL;
    	//printf("NAO HA PROCESSOS PRONTOS\n");
    }
    return cpu;
}

void processoReporter()
{
	printf("\n");
	imprimirLinha();
	printf("\nESTADO DO SISTEMA\n");
	imprimirLinha();
	printf("\nTEMPO ATUAL: %d\n",TEMPO);
	printf("\nEXECUTANDO:\n");
	if(estado_executando != NULL){
		printf("pid : %d, ppid : %d, prioridade: %d, valor: %d, tempo inicio: %d, CPU USADA ATE O MOMENTO %d \n",
		estado_executando->tabelaPcb.id_processo,
		estado_executando->tabelaPcb.id_processo_pai,
		estado_executando->tabelaPcb.prioridade,
		estado_executando->tabelaPcb.valor_inteiro,
		estado_executando->tabelaPcb.tempo_inicio,
		estado_executando->tabelaPcb.tempo_cpu_utilizada
		);
	}
	printf("BLOQUEADOS\n");
	printFILA(estado_bloqueado);
	printf("PRONTOS\n");
	printFILA(estado_pronto);
	imprimirLinha();
	printf("\n");
}

void imprimirLinha()
{
	for (int i = 0; i <= 100; i++)
	{
		printf("*");
	}
}
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
void printFILA(Fila *fila){
	if(fila == NULL){
		printf("fila vazia\n");
		return;
	}
	Fila *aux = fila;
	TabelaPcb processo;
	while(aux != NULL){// sempre inserir no FIM
		processo = aux->referenceTabelaPcb->tabelaPcb;
		printf("pid : %d, ppid : %d, prioridade: %d, valor: %d, tempo inicio: %d, CPU USADA ATE O MOMENTO %d\n",
			processo.id_processo,
			processo.id_processo_pai,
			processo.prioridade,
			processo.valor_inteiro,
			processo.tempo_inicio,
			processo.tempo_cpu_utilizada
		);
		aux = aux->prox;
	}
	printf("\n");
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
		printf("CONTADOR : %d\n",aux->tabelaPcb.cont_programa);
		imprimaCpu(aux->tabelaPcb.programa);
		aux = aux->prox;
	}
	printf("___________________________\n");
}
