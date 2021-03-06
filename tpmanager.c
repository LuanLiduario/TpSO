#include "tpmanager.h"

int main() {
	char comando ;
	//Iniciar cpu
	CPU cpu;
	cpu.nInstrucoes = 0;
	cpu.cont_programa = 0;
    cpu.valor_inteiro = 0;
    cpu.tempo_processo = 0;
    cpu.tempo_atual = 0;
    cpu.programa = lerArq("init",&cpu.nInstrucoes);
    executando = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
    executando->prox = NULL;
    executando->anterior = NULL;
    executando = addTabelaPCB(cpu.programa,0,0,0,0,cpu.nInstrucoes);
    cpu.prioridade = definirPrioridade((int)(cpu.nInstrucoes/quantum));
	do {
		scanf("%c", &comando);
		//Comandos
		switch(comando){
			case 'Q': // Fim de uma unidade de tempo
					if (executando != NULL)
					{
						cpu = processoSimulado(cpu);
					}
					cpu = escalonar(cpu);
					TEMPO++;
				break;
			case 'U': // Desbloqueie o primeiro processo simulado que esta na fila de bloqueados
					desbloquerProcesso();
					if(executando == NULL){// POSSO FAZER ISSO?
						cpu = trocaContexto(cpu);
					}
				break;
			case 'P': // Imprima o estado atual do sistema
					if(executando != NULL){
						executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
				    	executando->tabelaPcb.programa = cpu.programa;
				    	executando->tabelaPcb.prioridade = (int)(cpu.nInstrucoes/quantum);
					}
					reporter();
				break;
			case 'T': // Imprima o tempo de retorno medio e finaliza o simulador
					if(executando != NULL){
						executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
				    	executando->tabelaPcb.programa = cpu.programa;
				    	executando->tabelaPcb.prioridade = (int)(cpu.nInstrucoes/quantum);
					}
					reporter();
				break;
			default:
				break;
		}
	} while(comando != 'T' );
}

ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont, int nInstrucoes){
	// atribui os dados a uma variavel ques será salva na lista
	TabelaPcb processo;
	processo.id_processo = id;//trocar
    processo.id_processo_pai = id_pai;
	processo.cont_programa = cont;
	processo.valor_inteiro = valor;
	processo.estado = 3;// Pronto
	processo.tempo_inicio = TEMPO;
	processo.tempo_cpu_utilizada = 0;
	processo.nInstrucoes = nInstrucoes;
	processo.prioridade = definirPrioridade((int)(nInstrucoes/quantum));
	processo.programa = programa;
	nProcessos ++;
	if(tabelaPcb == NULL){// tabela está vazia
		tabelaPcb = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		tabelaPcb->tabelaPcb = processo;
		tabelaPcb->prox = NULL;
		tabelaPcb->anterior = NULL; 
		return tabelaPcb; 
	}else{// encontrar fim da lista
		ListaTabelaPcb *aux = NULL;
		aux = tabelaPcb;
		while(aux->prox != NULL){
			aux = aux->prox;
		}
		aux->prox = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		aux->prox->tabelaPcb = processo;
		aux->prox->prox = NULL;
		aux->prox->anterior = aux;
		return aux->prox;
	}
}

void terminarProcessoSimulado(){
	ListaTabelaPcb *aux;
	aux = executando;
	//Retirar da tabela PCB
	if (executando->anterior == NULL && executando->prox == NULL){ 
		free(tabelaPcb);
		tabelaPcb = NULL;
	}else if(executando->anterior == NULL){
		tabelaPcb = tabelaPcb->prox;
		tabelaPcb->anterior = NULL;
		free(aux);
	}else if(executando->prox == NULL){
		aux->anterior->prox = NULL;
		free(aux);
	}else{
		aux->prox->anterior = aux->anterior;
		aux->anterior->prox = aux->prox;
		free(aux);
	}
}

Instrucao* lerArq(char * filename,int *nInstrucoes){
	FILE *arq = fopen(filename, "r");
    if(arq == NULL){ 
        printf("\nERRO NA ABERTURA DO ARQUIVO\n\n\n");
        exit(1);
    };
     (*nInstrucoes) = 1;
    char *string;
    string = (char*) malloc(sizeof(char)*TamanhoString);
    char *valor;
    valor = (char*) malloc(sizeof(char)*TamanhoString);
 	Instrucao instrucao;
    Instrucao *programa = (Instrucao*)malloc(sizeof(Instrucao)* (*nInstrucoes));
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
			if(instrucao.instrucao == 'R'){
				instrucao.filename = (char*) malloc(sizeof(char)*TamanhoString);
				strcpy (instrucao.filename, valor);
			}else{
				instrucao.valor = atoi(valor);
			}
    	 }
    	 programa[(*nInstrucoes)-1] = instrucao;
    	 (*nInstrucoes)++;
    	 programa = realloc(programa,((*nInstrucoes)+1)*sizeof(Instrucao));
    }
    free(valor);
    free(string);
    fclose(arq);
    return programa;
}

CPU processoSimulado(CPU cpu)
{
//Processo Simulado
	executando->tabelaPcb.estado = 1;
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
			cpu.prioridade =  definirPrioridade(cpu.prioridade--);
			executando->tabelaPcb.prioridade = cpu.prioridade;
			bloquearProcessoSimulado();//trocar contexto
			cpu.cont_programa++;
			cpu = trocaContexto(cpu);
			break;
		case 'E': // Termina o processo simulado.
			terminarProcessoSimulado();//trocar contexto
			executando = NULL;
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			cpu.cont_programa++;
			ListaTabelaPcb *aux;
			aux = addTabelaPCB(cpu.programa,nProcessos,executando->tabelaPcb.id_processo,cpu.valor_inteiro,cpu.cont_programa,cpu.nInstrucoes);
			fila_prontos = addFila(fila_prontos,aux);//ordenar
			ordenarFila(fila_prontos);
			printf("to ak\n");
			cpu.cont_programa =  cpu.cont_programa + cpu.programa[cpu.cont_programa-1].valor;
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			cpu.nInstrucoes = 0;
			cpu.programa = lerArq(cpu.programa[cpu.cont_programa].filename,&cpu.nInstrucoes);
			cpu.cont_programa = 0;
			cpu.valor_inteiro = 0;
			cpu.prioridade =  definirPrioridade((int)(cpu.nInstrucoes/quantum));
			break;
		default:
			printf("ERRO INSTRUCAO NAO RECONHECIDA\n");
			printf("CONTADOR %d\n",cpu.cont_programa );
			exit(1);
			break;
	}
	return cpu;
}

Fila* addFila(Fila *fila,ListaTabelaPcb *processo){
	if(fila == NULL){
		fila = (Fila*) malloc(sizeof(Fila));
		fila->referenceTabelaPcb = processo;
		fila->prox = NULL;
		fila->anterior = NULL;
	}else{
		Fila *aux = fila;
		while(aux->prox != NULL){// sempre inserir no FIM
			aux = aux->prox;
		}
		aux->prox = (Fila*) malloc(sizeof(Fila));
		aux->prox->referenceTabelaPcb = processo;
		aux->prox->prox = NULL;
		aux->prox->anterior = aux;
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
		fila->prox->anterior = NULL;
		fila = fila->prox;
		free(aux);
	}
	return fila;
}

void bloquearProcessoSimulado(){
	executando->tabelaPcb.estado = 2;
	fila_bloqueados = addFila(fila_bloqueados,executando);//add executando na fila de bloqueados
}

void desbloquerProcesso(){
	if(fila_bloqueados == NULL){
		printf("NAO HA PROCESSOS BLOQUEADOS\n");
		return;
	}
	fila_prontos = addFila(fila_prontos,fila_bloqueados->referenceTabelaPcb);//ordenar fila
	ordenarFila(fila_prontos);
	fila_bloqueados->referenceTabelaPcb->tabelaPcb.estado = 3;
	fila_bloqueados = retirarFila(fila_bloqueados);
}

void ordenarFila (Fila *fila){//insert sort
	printf("ENTROU NA ORDENACAO\n");
	if(fila == NULL){
		return;
	}
	if(fila->prox == NULL){
		return;
	}
	Fila *atual = fila->prox;//Começa a percorrer a fila da segunda posição
	Fila *anterior = NULL;
	ListaTabelaPcb *aux;// a cada troca necessária, será trocado a referencia da tabeça pcb
	while(atual != NULL)
	{//percorre a lista toda
		aux = atual->referenceTabelaPcb;
		anterior = atual->anterior;
		while(anterior != NULL && anterior->referenceTabelaPcb->tabelaPcb.prioridade < aux->tabelaPcb.prioridade)
		{//verifica se há necessidade de fazer trocas
			anterior->prox->referenceTabelaPcb = anterior->referenceTabelaPcb;
			anterior = anterior->anterior;
		}
		if(anterior == NULL){// Se anterior for null, a primeira posição da fila deve receber aux
			fila->referenceTabelaPcb = aux;
		}else{
			anterior->prox->referenceTabelaPcb = aux;
		}
		atual = atual->prox;
	}
}


CPU trocaContexto(CPU cpu)
{
	if(executando != NULL)
	{// existe processo sendo executado
		//Atualizar tabela PCB
		executando->tabelaPcb.cont_programa = cpu.cont_programa;
	    executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
	    executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
    	executando->tabelaPcb.programa = cpu.programa;
    	executando->tabelaPcb.prioridade = definirPrioridade(cpu.prioridade);
    	executando->tabelaPcb.estado = 3;
	}
    if(fila_prontos != NULL)
    {//existe processo prontos
    	executando = fila_prontos->referenceTabelaPcb;//o primeiro da fila de prontos sera o novo executando
    	fila_prontos = retirarFila(fila_prontos);//retiro o primeiro da fila de executando
    	ordenarFila(fila_prontos);
	    //Troca processo na CPU
	    cpu.nInstrucoes = executando->tabelaPcb.nInstrucoes;
		cpu.cont_programa = executando->tabelaPcb.cont_programa;
	    cpu.valor_inteiro = executando->tabelaPcb.valor_inteiro;
	    cpu.tempo_processo = executando->tabelaPcb.tempo_cpu_utilizada;
	    cpu.tempo_atual = 0;
	    cpu.programa = executando->tabelaPcb.programa;
    }else
    {
    	executando = NULL;
    }
    return cpu;
}

CPU escalonar(CPU cpu){
	if(executando == NULL)
	{//processo terminou
		cpu = trocaContexto(cpu);
	}
	else if(fila_prontos != NULL)
	{//existe um processo com prioridade maior em prontos
		if(cpu.tempo_atual >= quantum)
		{//processo chegou no seu limite de cpu
			ListaTabelaPcb*aux = executando;
			cpu.prioridade = definirPrioridade(cpu.prioridade--);;
			cpu = trocaContexto(cpu);
			aux->tabelaPcb.estado = 3;
			fila_prontos = addFila(fila_prontos,aux);//add executando na fila de prontoss
		}
	}
	return cpu;
}

void reporter(){
	int writepipeReporter[2] = {-1, -1};
    pid_t id_filho;
    if(pipe(writepipeReporter) < 0) {
    	printf("ERRO NO PIPE NO REPORTER\n");
        perror("pipe");
        exit(1);
    }
    if((id_filho = fork()) == -1) {
    	printf("ERRO NO REPORTER\n");
        perror("fork");
    }
    if(id_filho == 0) {
        close(writepipeReporter[1]);
        dup2(writepipeReporter[0], STDIN_FILENO);
        close(writepipeReporter[0]);
        execlp("./reporter", "./reporter", NULL);

    } else {
        char comando;
        Fila *aux = NULL;
		TabelaPcb processo;
       	//TEMPO:
       	//comando = 'T';
        close (writepipeReporter[0]);
        write (writepipeReporter[1],&TEMPO,sizeof(int));// variavel TEMPO
        //EXECUTANDO:
        comando = 'E';
        write(writepipeReporter[1],&comando,sizeof(char));//comando PARA IMPRIMIR EXECUTANDO
        if(executando == NULL){
        	comando = 'V';
        	write(writepipeReporter[1],&comando,sizeof(char));//comando PARA IMPRIMIR SEM PROCESSO
        }else{
	        //ATRIBUTOS DO PROCESSO EXECUTANDO
	        comando = 'S';
	        write (writepipeReporter[1],&comando,sizeof(char));//comando PARA PROCESSO
	        //sleep(1);//ver dps
	        write(writepipeReporter[1],&executando->tabelaPcb.id_processo,sizeof(int));
			write(writepipeReporter[1],&executando->tabelaPcb.id_processo_pai,sizeof(int));
			write(writepipeReporter[1],&executando->tabelaPcb.prioridade,sizeof(int));
			write(writepipeReporter[1],&executando->tabelaPcb.valor_inteiro,sizeof(int));
			write(writepipeReporter[1],&executando->tabelaPcb.tempo_inicio,sizeof(int));
			write(writepipeReporter[1],&executando->tabelaPcb.tempo_cpu_utilizada,sizeof(int));
		}
		//BLOQUEADOS
        comando = 'B';
        write(writepipeReporter[1], &comando,sizeof(char));//comando PARA IMPRIMIR BLOQUEADO
        if(fila_bloqueados == NULL){
        	comando = 'V';
        	write(writepipeReporter[1], &comando,sizeof(char));//comando PARA IMPRIMIR SEM PROCESSO
        }else{
        	aux = fila_bloqueados;
			while(aux != NULL){// PERCORRER FILA DE BLOQUEADOS
				processo = aux->referenceTabelaPcb->tabelaPcb;
				//ATRIBUTOS DO PROCESSO BLOQUEADO
				comando = 'S';
	        	write (writepipeReporter[1],&comando, 1);//comando PARA PROCESSO
			 	write (writepipeReporter[1],&processo.id_processo,sizeof(int));
				write (writepipeReporter[1],&processo.id_processo_pai,sizeof(int));
 				write (writepipeReporter[1],&processo.prioridade,sizeof(int));
				write (writepipeReporter[1],&processo.valor_inteiro,sizeof(int));
 				write (writepipeReporter[1],&processo.tempo_inicio,sizeof(int));
 				write (writepipeReporter[1],&processo.tempo_cpu_utilizada,sizeof(int));
				aux = aux->prox;
			}
        }
        //PRONTOS:
        comando = 'P';
        close (writepipeReporter[0]);
        write (writepipeReporter[1], &comando, 1);//comando PARA IMPRIMIR PRONTO
        if(fila_prontos == NULL){
        	comando = 'V';
       		close (writepipeReporter[0]);
        	write (writepipeReporter[1], &comando, 1);//comando PARA IMPRIMIR SEM PROCESSO
        }else{
        	aux = fila_prontos;
			while(aux != NULL){// PERCORRER FILA DE PRONTO
				processo = aux->referenceTabelaPcb->tabelaPcb;
				//ATRIBUTOS DO PROCESSO BLOQUEADO
				comando = 'S';
	        	write (writepipeReporter[1],&comando,sizeof(char));//comando PARA PROCESSO
			 	write (writepipeReporter[1],&processo.id_processo,sizeof(int));
				write (writepipeReporter[1],&processo.id_processo_pai,sizeof(int));
 				write (writepipeReporter[1],&processo.prioridade,sizeof(int));
				write (writepipeReporter[1],&processo.valor_inteiro,sizeof(int));
 				write (writepipeReporter[1],&processo.tempo_inicio,sizeof(int));
 				write (writepipeReporter[1],&processo.tempo_cpu_utilizada,sizeof(int));
				aux = aux->prox;
			}
        }
        //FINALIZAR:
        comando = 'F';
        write (writepipeReporter[1], &comando, 1);
    }
    wait(0);
}

int definirPrioridade(int prioridade)
{
	if(prioridade >= prioridadeMAX)
	{
		return prioridade;
	}
	if(prioridade <= 1){
		return 1;
	}
	return prioridade;
}


// void printFILA(Fila *fila){
// 	if(fila == NULL){
// 		printf("fila vazia\n");
// 		return;
// 	}
// 	Fila *aux = fila;
// 	TabelaPcb processo;
// 	while(aux != NULL){// sempre inserir no FIM
// 		processo = aux->referenceTabelaPcb->tabelaPcb;
// 		printf("pid : %d, ppid : %d, prioridade: %d, valor: %d, tempo inicio: %d, CPU USADA ATE O MOMENTO %d\n",
// 			processo.id_processo,
// 			processo.id_processo_pai,
// 			processo.prioridade,
// 			processo.valor_inteiro,
// 			processo.tempo_inicio,
// 			processo.tempo_cpu_utilizada
// 		);
// 		aux = aux->prox;
// 	}
// 	printf("\n");
// }


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
		//printf("indice %d:\n",aux->indice);
		printf("ID : %d\n",aux->tabelaPcb.id_processo);
		printf("ID PAI: %d\n",aux->tabelaPcb.id_processo_pai);
		printf("ESTADO : %d\n",aux->tabelaPcb.estado);
		printf("CONTADOR : %d\n",aux->tabelaPcb.cont_programa);
		imprimaCpu(aux->tabelaPcb.programa);
		aux = aux->prox;
	}
	printf("___________________________\n");
}
