#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMPROCS 4

/*
        Questão 2 do Trabalho 1 de Programacao Paralela e Distribuída
        Renan da Costa Garrot DRE:108055521
        Thomaz Avila Barros DRE:107363028
	Versão paralela do Rank Sort
*/

int *numeros;
int *rankeados;
int *rank_local;
int tam;

void mestre(void);
void escravo(void);

void rank(int myi){

	int i, j, rank;

	int inicio, fim;

	inicio = myi * tam/NUMPROCS;
	fim  = myi * tam/NUMPROCS + tam/NUMPROCS;

	//printf("eu sou o %d | inicio = %d | fim = %d \n",myi, inicio, fim);

	for(i=inicio; i<fim; i++)
	{
		rank=0;
		for(j=0;j<tam;j++)
		{
			if((numeros[i]<numeros[j]) || ((numeros[i]==numeros[j])&&(j<i)))
			{
				rank++;
			}
		}
		rank_local[i - inicio]=rank;
		//printf("rank:%d ", rank);
	}

}


int main (int argc, char **argv){
	
	int parent;

	parent = pvm_parent();

	pvm_initsend(PvmDataDefault);
//	pvm_catchout(stdout);


	if(parent == PvmNoParent){
		mestre();
	} else {
		escravo();		
	}
		
	pvm_exit();
	return 0;
}


void mestre(void){

	int numt;
	int tids[NUMPROCS - 1];
	int i, j;


	srand(time(NULL));
	printf("Digite o tamanho do vetor a ser ordenado:\n");
	scanf("%d", &tam );
	numeros=(int *) malloc (tam*sizeof(int));
	rankeados=(int*) malloc (tam*sizeof(int));
	rank_local=(int*) malloc ((tam/NUMPROCS)*sizeof(int));
	if((!numeros)||(!rankeados))
	{
		printf("Erro na alocacao de memoria\n");
		return;
	}

	printf("Vetor não ordenado:\n");
	printf("Números: ");
	for(i=0;i<tam;i++)
	{
		numeros[i]= rand() % 1001;
		printf(" %d ", numeros[i] );

	}
	printf("\n");


	// Depois de gerar o vetor, vamos criar os filhos.

	numt =  pvm_spawn("par_rank1", NULL, PvmTaskDefault, NULL, NUMPROCS-1, tids);
	if(numt != NUMPROCS-1) exit(0);

	// Distribuindo tarefas

	for(i = 0; i < NUMPROCS-1; i++){

        	pvm_pkint(&tam, 1, 1);
		pvm_pkint(&numeros[0], tam, 1);
		pvm_send(tids[i],1);	

		
		pvm_send(tids[i],1);
	}

	// calcula parte do mestre
		rank(0);
	
	// apanha resultados do mestre 

	for(i=0; i < tam/NUMPROCS; i++)		
		rankeados[rank_local[i]] = numeros[i];

	//apanha resultados dos escravos

	int mytid;
	mytid = pvm_mytid();


	for(i=0; i < NUMPROCS-1; i++){
	    pvm_recv(tids[i],3);
	    pvm_upkint(rank_local, tam/NUMPROCS, 1);

	    for(j=0; j < tam/NUMPROCS; j++){		
			rankeados[rank_local[j]] = numeros[j + ((tids[i] - mytid) * tam/NUMPROCS)];
	    	}
	}

	
	// mestre imprime resultados

	printf("Vetor ordenado:\n");
	printf("Números: ");
	for(i=0;i<tam;i++)
	{
		printf(" %d ", rankeados[i] );

	}
	printf("\n");
	free(numeros);
	free(rankeados);
}


void escravo(void){	
	
	int tid, ptid;

	tid = pvm_mytid();
	ptid = pvm_parent();	

	pvm_recv(pvm_parent(),1);
	pvm_upkint(&tam,1,1);

	numeros=(int *) malloc (tam*sizeof(int));
	rank_local=(int*) malloc ((tam/NUMPROCS)*sizeof(int));

	pvm_upkint(numeros, tam, 1);	
	

	rank(tid - ptid);

	pvm_pkint(&rank_local[0], tam/NUMPROCS, 1);
	pvm_send(pvm_parent(),3);	

}


