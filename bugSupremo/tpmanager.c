#include "tpmanager.h"

int main() {//PM
	char comando ;
	//Iniciar cpu
	CPU cpu;
	cpu.nintrucoes = 0;
	cpu.cont_programa = 0;
    cpu.valor_inteiro = 0;
    cpu.tempo_processo = 0;
    cpu.tempo_atual = 0;
    cpu.programa = lerArq("init",&cpu.nintrucoes);
    estado_executando = addTabelaPCB(cpu.programa,0,0,0,0,cpu.nintrucoes);
	do {
		scanf("%c", &comando);
		//Comandos
		switch(comando){
			case 'Q': // Fim de uma unidade de tempo
					if (estado_executando != NULL)
					{
						cpu = processoSimulado(cpu);
						//printTabelaPCB();
					}else{
						if(tabelaPcb != NULL){
							//printTabelaPCB();
						}else{
							//printf("TabelaPcb vazia\n");
						}
					}
					cpu = escalonar(cpu);
					TEMPO++;
					//printf("Q\n");
				break;
			case 'U': // Desbloqueie o primeiro processo simulado que esta na fila de bloqueados
					desbloquerProcesso();
					if(estado_executando == NULL){// POSSO FAZER ISSO?
						cpu = trocaContexto(cpu);
					}
				break;
			case 'P': // Imprima o estado atual do sistema
					if(estado_executando != NULL){
						estado_executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    estado_executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    estado_executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
				    	estado_executando->tabelaPcb.programa = cpu.programa;
				    	estado_executando->tabelaPcb.prioridade = (int)(cpu.nintrucoes/quantum);
					}
					processoReporter();
				break;
			case 'T': // Imprima o tempo de retorno medio e finaliza o simulador
					if(estado_executando != NULL){
						estado_executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    estado_executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    estado_executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
				    	estado_executando->tabelaPcb.programa = cpu.programa;
				    	estado_executando->tabelaPcb.prioridade = (int)(cpu.nintrucoes/quantum);
					}
					processoReporter();
				break;
			default:
				break;
		}
	} while(comando != 't' && comando != 'T' );

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
	processo.nintrucoes = nintrucoes;
	processo.prioridade = (int)(nintrucoes/quantum);
	processo.programa = programa;
	int indice = nProcessos;////tirar o indice
	nProcessos ++;
	if(tabelaPcb == NULL){// tabela está vazia
		tabelaPcb = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		tabelaPcb->indice = indice;
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

Instrucao* lerArq(char * filename,int *nintrucoes){
	FILE *arq = fopen(filename, "r");
	//printf("%s\n",filename);
    if(arq == NULL){ 
        printf("\nERRO NA ABERTURA DO ARQUIVO\n\n\n");
        exit(1);
    };
     (*nintrucoes) = 1;
    char *string;
    string = (char*) malloc(sizeof(char)*TamanhoString);
    char *valor;
    valor = (char*) malloc(sizeof(char)*TamanhoString);
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
				instrucao.filename = (char*) malloc(sizeof(char)*TamanhoString);
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
    free(valor);
    free(string);
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
			terminarProcessoSimulado();//trocar contexto
			estado_executando = NULL;
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			cpu.cont_programa++;
			ListaTabelaPcb *aux;
			aux = addTabelaPCB(cpu.programa,nProcessos,estado_executando->tabelaPcb.id_processo,cpu.valor_inteiro,cpu.cont_programa,cpu.nintrucoes);
			estado_pronto = addFila(estado_pronto,aux);//ordenar
			//ordenarFila(estado_pronto);
			// if(cpu.programa[cpu.cont_programa].instrucao == 'R'){
				// printf("SOMA %d ", cpu.cont_programa);
				cpu.cont_programa =  cpu.cont_programa + cpu.programa[cpu.cont_programa-1].valor;
				// printf("+ %d = %d\n",cpu.programa[cpu.cont_programa-1].valor, cpu.cont_programa);
			// }
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			cpu.nintrucoes = 0;
			cpu.programa = lerArq(cpu.programa[cpu.cont_programa].filename,&cpu.nintrucoes);
			cpu.cont_programa = 0;
			cpu.valor_inteiro = 0;
			break;
		default:
			printf("ERRO INSTRUCAO NAO RECONHECIDA\n");
			printf("CONTADOR %d\n",cpu.cont_programa );
			exit(1);
			break;
	}
	//cpu = escalonar();
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
	estado_pronto = addFila(estado_pronto,estado_bloqueado->referenceTabelaPcb);//ordenar fila
	//ordenarFila(estado_pronto);
	estado_bloqueado->referenceTabelaPcb->tabelaPcb.estado = 3;
	estado_bloqueado = retirarFila(estado_bloqueado);
}

void ordenarFila (Fila *fila){//insert sort
	Fila *percorre = fila->prox;
	//int i = 1;
	while(percorre != 0)
	{

	}
	Fila aux;
	int i;
	int j = 1;
	while(percorre != NULL){
		aux = fila[j];
		for (i = j-1; i >= 0 && 
			fila[i].referenceTabelaPcb->tabelaPcb.prioridade > aux.referenceTabelaPcb->tabelaPcb.prioridade; --i){//analisa a posiçao anterior a que se está no vetor, se o anterior for maior os dois trocam de lugar e o i é decrementado
			fila[i+1] = fila[i];
		}
		fila[i+1] = aux;
		percorre = percorre->prox;
		j++;
	}
}

// void insercaoVetorPequeno (int t, Pequeno *vetor){
//    long int mov=0,comparacao=0;
//    for (int j = 1; j < t; ++j) {//percorre o vetor começando do segundo elemento da lista
//       Pequeno x = vetor[j];
//       mov++;//incrementa um na variavel de movimentaçao
//       int i;
//       for (i = j-1; i >= 0 && vetor[i].chave > x.chave; --i){//analisa a posiçao anterior a que se está no vetor, se o anterior for maior os dois trocam de lugar e o i é decrementado
//          vetor[i+1] = vetor[i];
//          mov++;
//          comparacao++;
//       }
//       comparacao++;
//       vetor[i+1] = x;
//       mov++;
//       //printf("Teste:");
//       //printVetorPequeno(vetor,t);
//    }
//   printf("Comparacao : %ld\n",comparacao);
//   printf("Movimentacao : %ld\n",mov);
// }


CPU trocaContexto(CPU cpu)
{
	if(estado_executando != NULL)
	{// existe processo sendo executado
		//Atualizar tabela PCB
		estado_executando->tabelaPcb.cont_programa = cpu.cont_programa;
	    estado_executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
	    estado_executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
    	estado_executando->tabelaPcb.programa = cpu.programa;
    	estado_executando->tabelaPcb.prioridade = (int)(cpu.nintrucoes/quantum);
	}
    if(estado_pronto != NULL)
    {//existe processo prontos
    	estado_executando = estado_pronto->referenceTabelaPcb;//o primeiro da fila de prontos sera o novo executando
    	estado_pronto = retirarFila(estado_pronto);//retiro o primeiro da fila de executando
	    //Troca processo na CPU
	    cpu.nintrucoes = estado_executando->tabelaPcb.nintrucoes;
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

CPU escalonar(CPU cpu){
	//int prioridade_atual = (int)(cpu.nintrucoes/quantum);
	if(estado_executando == NULL)
	{//processo terminou
		cpu = trocaContexto(cpu);
	}
	else if(estado_pronto != NULL)
	{//existe um processo com prioridade maior em prontos
		// if (prioridade_atual < estado_pronto->referenceTabelaPcb->tabelaPcb.prioridade)// devo fazer?
		// {
		// 	cpu = trocaContexto(cpu);
		// }else 

		if(cpu.tempo_atual >= quantum)
		{//processo chegou no seu limite de cpu
			ListaTabelaPcb*aux = estado_executando;
			cpu = trocaContexto(cpu);
			aux->tabelaPcb.estado = 3;
			estado_pronto = addFila(estado_pronto,aux);//add executando na fila de bloqueados
		}

	}
	return cpu;
}

// void reporter(){
// 	int writepipe[2] = {-1, -1};
//     pid_t childpid;
//     if(pipe(writepipe) < 0) {
//         perror("pipe");
//         exit(1);
//     }
//     if((childpid = fork()) == -1) {
//         perror("fork");
//     }
//     if(childpid == 0) {
//         close(writepipe[1]);
//         dup2(writepipe[0], STDIN_FILENO);
//         close(writepipe[0]);
//         execlp("./reporter", "./reporter", NULL);

//     } else {
//         close(writepipe[0]);
//         char string;
//         while(string != 'T'){
//         //  printf("commander\n");
//           scanf("%c", &string);
//           write(writepipe[1], &string, 1);
//           sleep(1);
//         }
//     }

//     wait(0);
// }


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
	}else{
		printf("NENHUM PROCESSO EXECUTANDO\n");
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
