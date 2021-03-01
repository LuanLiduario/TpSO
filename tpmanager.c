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
    addTabelaPCB(programa,0,0,0);
    programa = lerArq("file_a");
    cpu.programa = programa;
    addTabelaPCB(programa,1,0,0);
    programa = lerArq("file_b");
    cpu.programa = programa;
    addTabelaPCB(programa,2,0,0);
    printTabelaPCB();
    printf("----------------------\n");
    estado_executando = tabelaPcb;
    estado_executando = estado_executando->prox->prox;
    terminarProcessoSimulado();
    printTabelaPCB();
	do {
		scanf("%c", &string);
		//Comandos
		switch(string){
			case 'Q': // Fim de uma unidade de tempo
					// cpu = processoSimulado(cpu);
					// tempo++;
					printf("Q\n");
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

void addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor){
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
	ListaTabelaPcb *aux = NULL;
	aux = tabelaPcb;
	while(aux != NULL){
		printf("PROCESSO :\n");
		printf("indice %d:\n",aux->indice);
		printf("ID : %d\n",aux->tabelaPcb.id_processo);
		printf("ID PAI: %d\n",aux->tabelaPcb.id_processo_pai);
		printf("ESTADO : %d\n",aux->tabelaPcb.estado);
		imprimaCpu(aux->tabelaPcb.programa);
		aux = aux->prox;
	}
}

Instrucao* lerArq(char * filename){
	FILE *arq = fopen(filename, "r");
    if(arq == NULL){ 
        printf("\nERRO NA ABERTURA DO ARQUIVO\n\n\n");
        exit(1);
        return;
    };

    int nintrucoes = 1;
    char comando;
    char *string;
    string = (char*) malloc(sizeof(char)*50);
 	Instrucao instrucao;
    Instrucao *programa = (Instrucao*)malloc(sizeof(Instrucao)*100);

    while(!feof(arq)){//percorre o arquivo
    	 fscanf(arq ,"%c %s\n",&comando,string);
    	// printf(" Intrucao %d = %c %s\n",nintrucoes,comando,string );
    	 instrucao.instrucao = comando;
    	 if(comando == 'R'){
    	 	instrucao.filename = (char*) malloc(sizeof(char)*50);
    	 	instrucao.filename = string;
    	 }else if(comando != 'E' && comando != 'B'){
    	 	instrucao.valor = atoi(string);
    	 }
    
    	 programa[nintrucoes-1] = instrucao;
    	 nintrucoes++;
    	 //cpu.instrucoes = realloc(cpu.instrucoes,sizeof(Programa)*nintrucoes);
    }

    fclose(arq);
   
    return programa;
   //imprimaCpu(cpu,nintrucoes);
}

void imprimaCpu(Instrucao *programa){

	// printf("instrucoes: (%d)\n",n);
	int i = 0;
	while (1)
	{
		printf("Intrucao: %c  ",programa[i].instrucao);
		switch(programa[i].instrucao){
		case 'S': // n: Atualiza o valor da variavel inteira para n.
			printf("Valor : %d \n",programa[i].valor);
			break;
		case 'A': // n: Soma n na variavel inteira.
			printf("Valor : %d \n",programa[i].valor);
			break;
		case 'D':// D n: Subtrai n na variavel inteira.
			printf("Valor : %d \n",programa[i].valor);
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
			printf("Valor : %d \n",programa[i].valor);
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			printf("Arquivo : %s \n",programa[i].filename);
			break;
		default:
			break;
		}
		i++;
	}
}

// CPU processoSimulado(CPU cpu)
// {
// //Processo Simulado

// 	cpu.tempo_atual++;
// 	switch(cpu.programa[cpu.cont_programa].instrucao){
// 		case 'S': // n: Atualiza o valor da variavel inteira para n.
// 			cpu.programa.valor_inteiro = cpu.programa[cpu.cont_programa].valor;
// 			cpu.cont_programa++;
// 			break;
// 		case 'A': // n: Soma n na variavel inteira.
// 			cpu.programa.valor_inteiro = cpu.programa.valor_inteiro + cpu.programa[cpu.cont_programa].valor ;
// 			cpu.cont_programa++;
// 			break;
// 		case 'D':// D n: Subtrai n na variavel inteira.
// 			cpu.programa.valor_inteiro = cpu.programa.valor_inteiro - cpu.programa[cpu.cont_programa].valor ;
// 			cpu.cont_programa++;
// 			break;
// 		case 'B': // B: Bloqueia o processo simulado.
// 			// bloquearProcessoSimulado(cpu);
// 			cpu.cont_programa++;
// 			break;
// 		case 'E': // Termina o processo simulado.
// 			// terminarProcessoSimulado();
// 			break;
// 		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
// 		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
// 			Instrucao *programa;
// 			addTabelaPCB(cpu.programa,nProcessos,cpu.id_processo,cpu.valor_inteiro);
// 			cpu.cont_programa++;
// 			if(cpu.programa[cpu.cont_programa].instrucao == 'R'){
// 				cpu.cont_programa++;
// 			}
// 			break;
// 		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
// 		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
// 			programa = lerArq(cpu.programa[cpu.cont_programa].filename);
// 			cpu.programa = programa;
// 			cpu.cont_programa = 0;
// 			break;
// 		default:
// 			printf("ERRO INSTRUCAO NAO RECONHECIDA\n");
// 			//exit(1);
// 			break;
// 	}
	
// 	return cpu;
// }

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
