#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void imprimirLinha()
{//imprimi uma 100 *
	for (int i = 0; i <= 100; i++)
	{
		printf("*");
	}
	printf("\n");
}


int main() 
{
	char comando;
	int inteiro,id,pid,prioridade,valor,tempoInicio,tempoCPU;
	printf("\n");
	imprimirLinha();
	printf("ESTADO DO SISTEMA\n");
	imprimirLinha();
	//primeiro interiro lido é o TEMPO do sistema
	read(0,&inteiro,sizeof(int));
	printf("\nTEMPO ATUAL: %d\n",inteiro);
	do{
		//le os comandos para saber quais operações deve realizar
		read(0,&comando,sizeof(char));
		//Comandos
		switch(comando){
			case 'E':
				printf("PROCESSO EXECUTANDO\n");
				break;
			case 'B':
				printf("PROCESSOS BLOQUEADOS\n");
				break;
			case 'P':
				printf("PROCESSOS PRONTOS\n");
				break;
			case 'V':
				printf("SEM PROCESSO\n");
				break;
			case 'S': //simuladado
				read(0,&id,sizeof(int));
				read(0,&pid,sizeof(int));
				read(0,&prioridade,sizeof(int));
				read(0,&valor,sizeof(int));
				read(0,&tempoInicio,sizeof(int));
				read(0,&tempoCPU,sizeof(int));
				printf("pid : %d",id);
				printf(", ppid : %d",pid);
				printf(", prioridade: %d",prioridade);
				printf(", valor: %d",valor);
				printf(", tempo inicio: %d",tempoInicio);
				printf(", CPU USADA ATE O MOMENTO %d \n",tempoCPU);
				break;
			case 'F':
				imprimirLinha();
				break;
			default:
				break;
		}
	}while(comando != 'F');
}
