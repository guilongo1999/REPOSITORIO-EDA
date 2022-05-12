#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "include/fase1.h"


void main(void) {
	job job1,job2,job3,job4;
	int te;
	int ti;
	
	job1=criaJob(1,2);
	insereOperacao(1,3,1,job1);
	insereOperacao(1,5,1,job1);
	insereOperacao(2,6,2,job1);
	insereOperacao(3,8,3,job1);
	insereOperacao(3,1,4,job1);
	listaJob(job1);
	printf("\n********\n");
	getchar();

	escreveFicheiroJob("p91.job",job1);
	job2=leFicheiroJob("p91.job");
	listaJob(job2);
	printf("\n********\n");
	getchar();

	job2=removeOperacao(3,8,job2); 
	job3=criaJob(3,7);
	listaJob(job3);
	listaJob(job2);
	printf("\n********\n");
	getchar();

	job2=leFicheiroJob("p12.job");
	job3=leFicheiroJob("p91.job");
	listaJob(job3);
	listaJob(job2);
	te=calculaMinTempoJob(job3);
	ti=calculaMaxTempoJob(job3);
	printf("\n O TEMPO MIN É: %d",te);
	printf("\n O TEMPO MAX É: %d\n",ti);
	
}
