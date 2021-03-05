#include <stdio.h>
#include <stdlib.h>

void imprimirLinha()
{
	for (int i = 0; i <= 100; i++)
	{
		printf("*");
	}
}

int main() {
	char comando;
	char *string;
    string = (char*) malloc(sizeof(char)*100);
	printf("\n");
	imprimirLinha();
	printf("\nESTADO DO SISTEMA\n");
	imprimirLinha();
	do{
		scanf("%c", &comando);
		//Comandos
		switch(comando){
			case 'M': 
				printf("\nTEMPO MEDIO: %d\n",string);
				break;
			case 'T': 
				printf("\nTEMPO ATUAL: %d\n",string);
				break;
			case 'L': 
				imprimirLinha();
				break;
			case 'S': //simuladado
				printf("pid : %d, ppid : %d, prioridade: %d, valor: %d, tempo inicio: %d, CPU USADA ATE O MOMENTO %d \n");
				break;
			case 'E':
				printf("PRONTOS\n");
				break;
			case 'P':
				printf("BLOQUEADOS\n");
				break;
			case 'V':
				printf("SEM PROCESSO\n");
				break
			case 'F':
				imprimirLinha();
				break;
		}
	}while(comando == 'F');
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
