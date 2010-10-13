#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
        Questão 2 do Trabalho 1 de Programacao Paralela e Distribuída
        Renan da Costa Garrot DRE:108055521
        Thomaz Avila Barros DRE:107363028
	Versão sequencial do Rank Sort
*/

int main (int argc, char *argv[]){
	int tam, i, j, rank;
	int *numeros, *rankeados;

	srand(time(NULL));

	printf("Digite o tamanho do vetor a ser ordenado:\n");
	scanf("%d", &tam );

	numeros=(int *) malloc (tam*sizeof(int));
	rankeados=(int*) malloc (tam*sizeof(int));

	if((!numeros)||(!rankeados))
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
	}
	
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
