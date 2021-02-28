#include "tpmanager.h"

int main() {//PM
	char string ;
	Instrucao *programa;
	CPU cpu;
	programa = lerArq("init");
	cpu.cont_programa = 0;
    cpu.valor_inteiro = 0;
    cpu.tempo_processo = 0;
    cpu.tempo_atual = tempo;
    cpu.programa = programa;
    addTabelaPCB(programa,0,0,0);
    
	do {
		scanf("%c", &string);
		//Comandos
		switch(string){
			case 'Q': // Fim de uma unidade de tempo
					printf("Q\n");
				break;
			case 'U': // Desbloqueie o primeiro processo simulado que esta na fila de bloqueados
					printf("U\n");
				break;
			case 'P': // Imprima o estado atual do sistema
					printf("P\n");
				break;
			case 'T': // Imprima o tempo de retorno medio e finaliza o simulador
					printf("T\n");
				break;
			default:
				//printf("FILHO : %c\n", string);
				break;
		}
	} while(string != 't' && string != 'T' );

}

void addTabelaPCB(Instrucao * programan,int id,int id_pai, int valor){
	tabelaPcb[id].id_processo = 0;
    tabelaPcb[id].id_processo_pai = id_pai;
	tabelaPcb[id].cont_programa = 0;
	tabelaPcb[id].valor_inteiro = valor;
	tabelaPcb[id].estado = 1;// Pronto
	tabelaPcb[id].tempo_inicio = tempo;
	tabelaPcb[id].tempo_cpu_utilizada = 0;
	tabelaPcb[id].programa = programa;
    id ++;
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
    	 	instrucao.arquivo = (char*) malloc(sizeof(char)*50);
    	 	instrucao.arquivo = string;
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

void imprimaCpu(CPU cpu,int n){

	printf("instrucoes: (%d)\n",n);
	for (int i = 0; i < n; i++)
	{
		printf("Intrucao: %c  ",cpu.programa[i].instrucao);
		switch(cpu.programa[i].instrucao){
		case 'S': // n: Atualiza o valor da variavel inteira para n.
			printf("Valor : %d \n",cpu.programa[i].valor);
			break;
		case 'A': // n: Soma n na variavel inteira.
			printf("Valor : %d \n",cpu.programa[i].valor);
			break;
		case 'D':// D n: Subtrai n na variavel inteira.
			printf("Valor : %d \n",cpu.programa[i].valor);
			break;
		case 'B': // B: Bloqueia o processo simulado.
			printf("B :  \n");
			break;
		case 'E': // Termina o processo simulado.
			printf("E  \n");
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			printf("Valor : %d \n",cpu.programa[i].valor);
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			printf("Arquivo : %s \n",cpu.programa[i].arquivo);
			break;
		default:
			break;
		}
	}
}

void processoSimulado()
{
//Processo Simulado
	char string = ' ';
	int n = 0;
	n++;
	switch(string){
		case 'S': // n: Atualiza o valor da variavel inteira para n.
			printf("s\n");
			break;
		case 'A': // n: Soma n na variavel inteira.
			break;
		case 'D':// D n: Subtrai n na variavel inteira.
			break;
		case 'B': // B: Bloqueia o processo simulado.
			break;
		case 'E': // Termina o processo simulado.
			break;
		case 'F': //n: Cria um novo processo simulado. O novo processo uma copia exata do pai. O novo processo executa
		//da instução imediatamente apos a instução F, enquanto o pai continua n instrucões apos F
			break;
		case 'R':// R nome do arquivo: Substitui o programa do processo simulado com o programa no arquivo nome do
		// arquivo, e atualiza o valor do contador de programa para a primeira instrução do novo programa.
			break;
		default:
			break;
	}
}
