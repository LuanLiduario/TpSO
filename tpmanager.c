#include "tpmanager.h"

int main() 
{
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
    executando->tabelaPcb.estado = 1;
    cpu.prioridade = definirPrioridade(round((cpu.nInstrucoes/quantum)));
	do {
		read(0,&comando,sizeof(char));
		//Comandos
		switch(comando){
			case 'Q': // Fim de uma unidade de tempo
					if (executando != NULL)
					{
						cpu = processoSimulado(cpu);
					}
					cpu = escalonar(cpu);// verifica se pode ser escalonado 
					TEMPO++;
				break;
			case 'U': // Desbloqueie o primeiro processo simulado que esta na fila de bloqueados
					desbloquerProcesso();
					if(executando == NULL)
					{// se executando for vazio, já faz uma troca de contexto para o processo desbloqueado ser o executando
						cpu = trocaContexto(cpu);
					}
				break;
			case 'P': // Imprima o estado atual do sistema
					if(executando != NULL)
					{
						//Atualiza o processo executando
						executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
				    	executando->tabelaPcb.programa = cpu.programa;
				    	executando->tabelaPcb.prioridade = cpu.prioridade;
				    	executando->tabelaPcb.nInstrucoes = cpu.nInstrucoes;
					}
					reporter();
				break;
			case 'T': // Imprima o tempo de retorno medio e finaliza o simulador
					if(executando != NULL)
					{
						//Atualiza o processo executando
						executando->tabelaPcb.cont_programa = cpu.cont_programa;
					    executando->tabelaPcb.valor_inteiro = cpu.valor_inteiro;
					    executando->tabelaPcb.tempo_cpu_utilizada = cpu.tempo_processo + cpu.tempo_atual;
				    	executando->tabelaPcb.programa = cpu.programa;
				    	executando->tabelaPcb.prioridade = cpu.prioridade;
				    	executando->tabelaPcb.nInstrucoes = cpu.nInstrucoes;
					}
					reporter();
				break;
			default:
				break;
		}
	} while(comando != 'T' );
}

CPU processoSimulado(CPU cpu)
{
//Processo Simulado
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
			cpu.prioridade--;
			cpu.prioridade =  definirPrioridade(cpu.prioridade); // se o processo é bloqueado ele perde prioridade e é necessario definir a prioridade nova
			cpu = trocaContexto(cpu);//salva as informações na tabela e a cpu recebe um novo processo
			break;
		case 'E': // Termina o processo simulado.
			terminarProcessoSimulado();
			executando = NULL;// será trocado no escalonador
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			cpu.cont_programa++;
			ListaTabelaPcb *aux;
			aux = addTabelaPCB(cpu.programa,nProcessos,executando->tabelaPcb.id_processo,cpu.valor_inteiro,cpu.cont_programa,cpu.nInstrucoes);
			fila_prontos = addFila(fila_prontos,aux);//ordenar
			ordenarFila(fila_prontos);
			cpu.cont_programa =  cpu.cont_programa + cpu.programa[cpu.cont_programa-1].valor;
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			cpu.nInstrucoes = 0;
			cpu.programa = lerArq(cpu.programa[cpu.cont_programa].filename,&cpu.nInstrucoes);
			cpu.cont_programa = 0;
			cpu.valor_inteiro = 0;
			cpu.prioridade =  definirPrioridade(round((cpu.nInstrucoes/quantum)));// como há um novo arquivo é necessário definir a prioridade novamente
			break;
		default:
			printf("ERRO INSTRUCAO NAO RECONHECIDA\n");
			printf("CONTADOR %d\n",cpu.cont_programa );
			exit(1);
			break;
	}
	return cpu;
}

void bloquearProcessoSimulado()
{
	executando->tabelaPcb.estado = 2;//troco o estado direto na tabela
	fila_bloqueados = addFila(fila_bloqueados,executando);//adiciona executando na fila de bloqueados
}

void desbloquerProcesso()
{
	if(fila_bloqueados == NULL)
	{//verifica se existe processos bloqueados
		printf("NAO HA PROCESSOS BLOQUEADOS\n");
		return;
	}
	fila_prontos = addFila(fila_prontos,fila_bloqueados->referenceTabelaPcb);//coloca o primeiro processo da filade bloqueados na fila de prontos
	ordenarFila(fila_prontos);//ordenar fila
	fila_bloqueados->referenceTabelaPcb->tabelaPcb.estado = 3;//troco o estado direto na tabela
	fila_bloqueados = retirarFila(fila_bloqueados);// retira o primeiro processo da fila de bloqueados
}

Instrucao* lerArq(char * filename,int *nInstrucoes)
{
	FILE *arq = fopen(filename, "r");
    if(arq == NULL)
    { 
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
    while(!feof(arq))
    {//percorre o arquivo
    	 fgets(string,50,arq);// le a linha por linha
    	 instrucao.instrucao = string[0];// a instrução sempre será a posição 0
    	 if(instrucao.instrucao != 'B' && instrucao.instrucao != 'E')
    	 {// verifico se a instrução é alguma que tem algum valor
			i = 2; 
			while(string[i] != '\n')
			{// ler o valor
				valor[i-2] = string[i];
				i++;
			}
			valor[i-3]='\0';
			if(instrucao.instrucao == 'R')
			{// se for R salvo o nome do arquivo
				instrucao.filename = (char*) malloc(sizeof(char)*TamanhoString);
				strcpy (instrucao.filename, valor);
			}else
			{// transforma o valor em inteiro
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

CPU escalonar(CPU cpu)
{
	if(executando == NULL)
	{//processo terminou
		cpu = trocaContexto(cpu);// deve haver troca de contexto
	}
	else if(fila_prontos != NULL)
	{//existe processo na filade prontos
		if(cpu.tempo_atual >= quantum)
		{//processo chegou no seu limite de tempo na cpu
			ListaTabelaPcb*aux = executando;
			cpu.prioridade--;//perde prioridade
			cpu.prioridade = definirPrioridade(cpu.prioridade);//defini a nova prioridade
			cpu = trocaContexto(cpu);
			aux->tabelaPcb.estado = 3;
			fila_prontos = addFila(fila_prontos,aux);//adiciona executando na fila de prontoss
			ordenarFila(fila_prontos);//ordena fila
		}
	}
	return cpu;
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
    	executando->tabelaPcb.prioridade = cpu.prioridade;
	}
    if(fila_prontos != NULL)
    {//existe processo prontos
    	executando = fila_prontos->referenceTabelaPcb;//o primeiro da fila de prontos sera o novo executando
    	fila_prontos = retirarFila(fila_prontos);//retiro o primeiro da fila de executando
    	ordenarFila(fila_prontos);//ordena fila de prontos
	    //Troca processo na CPU
	    executando->tabelaPcb.estado = 1;
	    cpu.prioridade =  executando->tabelaPcb.prioridade;
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

int definirPrioridade(int prioridade)
{
	// se a prioridade for maior que a prioridade maxima, a prioridade definida dever a maxima
	if(prioridade >= prioridadeMAX)
	{
		return prioridadeMAX;
	}
	// se a prioridade for menor que a prioridade minima, a prioridade definida dever a minima
	if(prioridade <= prioridadeMIN){
		return prioridadeMIN;
	}
	// se respeitar as regras acima deve definir a propria prioridade recebida
	return prioridade;
}

ListaTabelaPcb* addTabelaPCB(Instrucao * programa,int id,int id_pai, int valor,int cont, int nInstrucoes)
{
	// atribui os dados a uma variavel ques será salva na lista
	TabelaPcb processo;
	processo.id_processo = id;
    processo.id_processo_pai = id_pai;
	processo.cont_programa = cont;
	processo.valor_inteiro = valor;
	processo.estado = 3;// Pronto
	processo.tempo_inicio = TEMPO;
	processo.tempo_cpu_utilizada = 0;
	processo.nInstrucoes = nInstrucoes;
	processo.prioridade = definirPrioridade(round((nInstrucoes/quantum)));
	processo.programa = programa;
	nProcessos ++;
	if(tabelaPcb == NULL)
	{// tabela está vazia aloca espeço para a lista e seus apontadores iram apontar pra NULL
		tabelaPcb = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		tabelaPcb->tabelaPcb = processo;
		tabelaPcb->prox = NULL;
		tabelaPcb->anterior = NULL; 
		return tabelaPcb; 
	}else
	{// encontrar fim da lista
		ListaTabelaPcb *aux = NULL;
		aux = tabelaPcb;
		while(aux->prox != NULL)
		{
			aux = aux->prox;
		}
		//aloca espaço para o próxima da lista
		aux->prox = (ListaTabelaPcb*) malloc(sizeof(ListaTabelaPcb));
		aux->prox->tabelaPcb = processo;
		aux->prox->prox = NULL;
		aux->prox->anterior = aux;
		return aux->prox;
	}
}

void terminarProcessoSimulado()
{
	ListaTabelaPcb *aux;
	aux = executando;
	//Retirar da tabela PCB
	if (executando->anterior == NULL && executando->prox == NULL)
	{ // existe apenas um elemento na lista
		free(tabelaPcb);
		tabelaPcb = NULL;
	}else if(executando->anterior == NULL)
	{// o elemento executando é o primeiro da fila
		tabelaPcb = tabelaPcb->prox;
		tabelaPcb->anterior = NULL;
		free(aux);
	}else if(executando->prox == NULL)
	{// o elemento executando é o ultimo da fila
		aux->anterior->prox = NULL;
		free(aux);
	}else
	{
		aux->prox->anterior = aux->anterior;
		aux->anterior->prox = aux->prox;
		free(aux);
	}
}




Fila* addFila(Fila *fila,ListaTabelaPcb *processo)
{
	if(fila == NULL)
	{//fila vazia, logo aloo fila
		fila = (Fila*) malloc(sizeof(Fila));
		fila->referenceTabelaPcb = processo;
		fila->prox = NULL;
		fila->anterior = NULL;
	}else
	{
		Fila *aux = fila;
		while(aux->prox != NULL)
		{// sempre inserir no FIM
			aux = aux->prox;
		}
		aux->prox = (Fila*) malloc(sizeof(Fila));
		aux->prox->referenceTabelaPcb = processo;
		aux->prox->prox = NULL;
		aux->prox->anterior = aux;
	}
	return fila;
}

Fila* retirarFila(Fila *fila)
{
	if(fila == NULL)
	{//não existe fila
		// printf("FILA VAZIA NO RETIRA\n");
	}else if(fila->prox == NULL)
	{//só tem um elemento na fila
		free(fila);
		fila = NULL;
	}else{//ta no meio da fila
		Fila *aux = fila;
		fila->prox->anterior = NULL;
		fila = fila->prox;
		free(aux);
	}
	return fila;
}

void ordenarFila (Fila *fila)
{//insert sort
	if(fila == NULL)
	{//fila vazia não é necessario ordenar 
		return;
	}
	if(fila->prox == NULL)
	{//fila com um elemento não é necessario ordenar 
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
			anterior = anterior->anterior;//anda na fila para tras
		}
		if(anterior == NULL)
		{// Se anterior for null, a primeira posição da fila deve receber aux
			fila->referenceTabelaPcb = aux;
		}else
		{
			anterior->prox->referenceTabelaPcb = aux;
		}
		atual = atual->prox;
	}
}

void reporter()
{
	int writepipeReporter[2] = {-1, -1};
    pid_t id_filho;
    if(pipe(writepipeReporter) < 0)
     {
    	printf("ERRO NO PIPE NO REPORTER\n");
        perror("pipe");
        exit(1);
    }
    if((id_filho = fork()) == -1)
     {
    	printf("ERRO NO REPORTER\n");
        perror("fork");
    }
    if(id_filho == 0)
     {
        close(writepipeReporter[1]);
        //bloqueia teclado
        dup2(writepipeReporter[0], STDIN_FILENO);
        close(writepipeReporter[0]);
        execlp("./reporter", "./reporter", NULL);

    } else 
    {
    	//enviar as informações para o processo reporter
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
        if(executando == NULL)
        {
        	comando = 'V';
        	write(writepipeReporter[1],&comando,sizeof(char));//comando PARA IMPRIMIR SEM PROCESSO
        }else
        {
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
        if(fila_bloqueados == NULL)
        {
        	comando = 'V';
        	write(writepipeReporter[1], &comando,sizeof(char));//comando PARA IMPRIMIR SEM PROCESSO
        }else{
        	aux = fila_bloqueados;
			while(aux != NULL)
			{// PERCORRER FILA DE BLOQUEADOS
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
        if(fila_prontos == NULL)
        {
        	comando = 'V';
       		close (writepipeReporter[0]);
        	write (writepipeReporter[1], &comando, 1);//comando PARA IMPRIMIR SEM PROCESSO
        }else
        {
        	aux = fila_prontos;
			while(aux != NULL)
			{// PERCORRER FILA DE PRONTO
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

