#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pvm3.h"


/* Rank sort paralelizado*/
int main (int argc, char *argv[]){
	int tam, i, j, rank, id, tag=100, processos, retorno;
	int *numeros, *rankeados, *tids, *ranks;
	char *grupo = "grupo";

	srand(time(NULL));

	printf("Digite numero de processos a serem disparados:\n");
	scanf("%d", &processos);

	printf("Digite o tamanho do vetor a ser ordenado:\n");
	scanf("%d", &tam );

	retorno=(tam/processos);
	
	numeros=(int *) malloc (tam*sizeof(int));
	rankeados=(int*) malloc (tam*sizeof(int));
	tids=(int*) malloc (processos*sizeof(int));
	ranks=(int*)malloc ((tam)*sizeof(int));
	


	if((!numeros)||(!rankeados)||(!tids)||(!ranks))
	{
		printf("Erro na alocacao de memoria\n");
		return 1;
	}


	printf("Vetor não ordenado:\n");
	printf("Números: ");
	for(i=0;i<tam;i++)
	{
		numeros[i]=rand() % 1001;
		printf(" %d ", numeros[i] );
		
	}
	printf("\n");
		
	pvm_initsend(PvmDataDefault);
	pvm_cachout(stdout);
	id = pvm_joingroup(grupo);
	
	if(id==0)
	{
		if(pvm_spawn("tarefas", NULL, PvmTaskDefault, "", processos-1,tids)!=(processos-1))
		{
			printf("Erro: Não foi possível disparar os processos\n");
			return 2;
		}
		else
		{
			
	  		for(i=0;i<tam;i++)
	              	{
				pvm_barrier(grupo, NPROCS);
				pvm_scatter(ranks,numeros,(tam/processos),PVM_INT,tag,grupo,0);
              			rank=0;
                    			if((numeros[i]<numeros[j]) || ((numeros[i]==numeros[j])&&(j<i)))
                       			{
                               			 rank++;
					}
//
		                  rankeados[rank]=numeros[i];
	        	          printf("rank:%d ", rank);
			}
        //		pvm_barrier(grupo, NPROCS);
	
		}
			
	}

/*	for(i=0;i<tam;i++)
	{
		rank=0;
		for(j=0;j<tam;j++)
		{
			if((numeros[i]<numeros[j]) || ((numeros[i]==numeros[j])&&(j<i)))
			{
				rank++;
			}
		}
		rankeados[rank]=numeros[i];
		printf("rank:%d ", rank);
	}
*/	

	printf("Vetor ordenado:\n");
	printf("Números: ");
	for(i=0;i<tam;i++)
	{
		printf(" %d ", rankeados[i] );
		
	}
	printf("\n");

	free(numeros);
	free(rankeados);
	return 0;
}
