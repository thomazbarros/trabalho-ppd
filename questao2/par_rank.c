#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pvm3.h"


/* Rank sort paralelizado*/
int main (int argc, char *argv[]){
	int tam, i, j, rank, id, tag, processos;
	int *numeros, *rankeados, *tids;
	char *grupo = "grupo";

	srand(time(NULL));

	printf("Digite numero de processos a serem disparados:\n");
	scanf("%d", &processos);

	printf("Digite o tamanho do vetor a ser ordenado:\n");
	scanf("%d", &tam );


	numeros=(int *) malloc (tam*sizeof(int));
	rankeados=(int*) malloc (tam*sizeof(int));
	tids=(int*) malloc (processos*sizeof(int));


	if((!numeros)||(!rankeados)||(!tids))
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
