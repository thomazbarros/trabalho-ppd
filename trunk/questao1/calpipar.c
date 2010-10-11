/*
 * Paralelização
 * Calpi
 * calpi.c
 * Ultima revisão GPS 18/08/04

  Versao paralela
 
*/
#include "pvm3.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define f(x) ((double)(4.0/(1.0+x*x)))
#define pi ((double)(4.0*atan(1.0)))

#define NUMPROCS 10


int solicita (void);
void imprime (double sum);
double calcula(int N, int gid);


int main(int argc, char* argv[])     
{
  /* Este programa bem simples aproxima pi calculando pi = integral 
   * de 0 até 1 de 4/(1+x*x)dx que é aproximando pela soma de 
   * k=1 até N de 4 / ((1+[(1/N)*(k-1/2)]**2) e então
   * multiplicando a soma por (1/N). (Esta regra de integração numérica
   * é chamada de "Midpoint rule" e pode ser encontrada em vários livros
   * de análise numérica). O único dado de entrada necessário é N.                                  
   */
  double sum, sumtemp, w;
  int i, N;
  int gid, numt;
  char* grupo  = "g";
  int tids[NUMPROCS-1];


  /* 
   * A rotina solicita irá pegar e propagar o valor de N
   */

  N = 1;

  while (N > 0) 
  {    

	    pvm_initsend(PvmDataDefault);
	    pvm_catchout(stdout);
	    gid = pvm_joingroup(grupo);
	    
	    if(gid == 0)
	    {
		N = solicita ();
		if(N == 0) break;

		numt =  pvm_spawn("calpipar", NULL, PvmTaskDefault, NULL, NUMPROCS-1, tids);
		pvm_pkint(&N,1,1);
	   	pvm_mcast(tids, NUMPROCS-1, 1);
	    }

	    pvm_barrier(grupo, NUMPROCS);

	    if(gid != 0)
	    {
	       pvm_recv(-1, 1);
	       pvm_upkint(&N, 1, 1);  
	    }
	  
	    sum = calcula(N,gid);
	     
	    if(gid != 0)
	    {
		pvm_pkdouble( &sum, 1, 1 );
		pvm_send(pvm_parent(), 2);
	    }

	    pvm_barrier(grupo, NUMPROCS);

	    if(gid == 0)
	    {
		for(i=0;i<NUMPROCS-1;i++)
		{
			pvm_recv(-1, 2);
			pvm_upkdouble(&sumtemp, 1,1);
			sum += sumtemp;
		}

		w = 1.0/(double)N;
		sum *= w;
		imprime(sum);
	    } 

	 if(gid == 0) N = 1;
	 else N = 0;  

         pvm_barrier(grupo, NUMPROCS);
   	 pvm_lvgroup(grupo);
    }   

   pvm_exit();
  return (0);
}


/*  --------------------------------------------------------------  */
int solicita (void)
{
  int N;
  printf ("Entre o numero de intervalos para a  aproximacaoo:(0 para terminar)\n");
  scanf("%d",&N);
  return (N);
}

/*  --------------------------------------------------------------  */
void imprime(double sum)
{
  double err;
  err = sum - pi;
  printf("soma, erro = %7.5f, %10e\n", sum, err);
}


double calcula(int N, int gid)
{
        double sum, w;
        int i;
	
	int i_inicial, i_final;

	i_inicial = (gid * (N / NUMPROCS)) + 1 ;
	i_final = (gid * (N / NUMPROCS)) + (N / NUMPROCS);

	sum = 0.0;
	w = 1.0/(double)N;
	
	for (i = i_inicial; i <= i_final; i++)  sum = sum + f(((double)i-0.5)*w);
	
	return sum ;
}

