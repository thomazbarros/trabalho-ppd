#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define NUMPROCS 10

/* Rank sort paralelo v1*/



int *numeros;
int *rankeados;
int *rank_local;
int tam;
int numprocs;
void mestre(void);
void escravo(void);

void rank(int myi){

	int i, j, rank;

	int inicio, fim;

	inicio = myi * tam/numprocs;
	fim  = myi * tam/numprocs + tam/numprocs;

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
	int *tids;
	int i, j;


	srand(time(NULL));
	printf("Digite o tamanho do vetor que deseja ordenar:\n Digite 1 para mil numeros.\n Digite 2 para dez mil números.\n Digite 3 para cem mil números.\n");
	scanf("%d", &tam );
	while((tam<1)||(tam>3))
	{
		printf("Opcão incorreta. Digite novamente.\n");
		scanf("%d", &tam);
	}
	switch(tam)
	{
		case 1:
			tam=1000;
			break;
		case 2:
			tam=10000;		
			break;
		case 3:
			tam=100000;
			break;
		default:
			break;
	}

	printf("Digite o número de processos:\n Digite 1 para 4 processos.\n Digite 2 para 6 processos.\n Digite 3 para 16 processos.\n");

	 scanf("%d", &numprocs );
        while((numprocs<1)||(numprocs>3))
        {
                printf("Opcão incorreta. Digite novamente.\n");
                scanf("%d", &numprocs);
        }
        switch(numprocs)
        {
                case 1:
                        numprocs=1000;
                        break;
                case 2:
                        numprocs=10000;
                        break;
                case 3:
                        numprocs=100000;
                        break;
                default:
                        break;
        }

		
	numeros=(int *) malloc (tam*sizeof(int));
	rankeados=(int*) malloc (tam*sizeof(int));
	rank_local=(int*) malloc ((tam/numprocs)*sizeof(int));
	tids=(int*)malloc(numprocs*sizeof(int));
	if((!numeros)||(!rankeados)||(!rank_local)||(!tids))
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
	//	for(j=0

	}
	printf("\n");


	// Depois de gerar o vetor, vamos criar os filhos.

	numt =  pvm_spawn("par_rank1", NULL, PvmTaskDefault, NULL, numprocs-1, tids);
	if(numt != numprocs-1) exit(0);

	// Distribuindo tarefas

	for(i = 0; i < numprocs-1; i++){

        	pvm_pkint(&tam, 1, 1);
		pvm_pkint(&numeros[0], tam, 1);
		pvm_send(tids[i],1);	

		
		pvm_send(tids[i],1);
	}

	// calcula parte do mestre
		rank(0);
	
	// apanha resultados do mestre 

	for(i=0; i < tam/numprocs; i++)		
		rankeados[rank_local[i]] = numeros[i];

	//apanha resultados dos escravos

	int mytid;
	mytid = pvm_mytid();


	for(i=0; i < numprocs-1; i++){
	    pvm_recv(tids[i],3);
	    pvm_upkint(rank_local, tam/numprocs, 1);

	    for(j=0; j < tam/numprocs; j++){		
			rankeados[rank_local[j]] = numeros[j + ((tids[i] - mytid) * tam/numprocs)];
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
	rank_local=(int*) malloc ((tam/numprocs)*sizeof(int));

	pvm_upkint(numeros, tam, 1);	
	

	rank(tid - ptid);

	pvm_pkint(&rank_local[0], tam/numprocs, 1);
	pvm_send(pvm_parent(),3);	

}


