#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "fase2.h"


int menu() {
	int opc = 0;
	int try = 0;

	while(( opc < 0 || opc > 7 ) || try != 1) {
		printf("\n Gestao de Process Plans (PP)");
		printf("\n ----------------------------");
		printf("\n 1 - Ler PP");
		printf("\n 2 - Escrever PP");
				
		printf("\n 3 - Editar um job do PP");
		printf("\n 4 - Calculo de Escalonamento");
		
		printf("\n 5 - Lista Escalonamento");
		printf("\n 6 - Lista PP");
		printf("\n 7 - Limpa PP carregado");
		printf("\n 0 - Sair");
		printf("\n digite a opcao e prima enter --> ");
		try = scanf("%d",&opc);
		getchar();
	}
	return opc;
}

process_plan opcaoLePP(process_plan p) {
	char nome_do_pp[250];

	printf("\n Introduza pf o nome do Process Plan:\n");
	scanf("%s",nome_do_pp);
	/* faltam validações */
	p = LePP(p,nome_do_pp);
	
	return(p);
}

int opcaoEscrevePP(process_plan p) {
	int res;
	char nome_do_pp[250];

	printf("\n Introduza pf o nome do Process Plan: \n");
	scanf("%s",nome_do_pp);
	res = EscrevePP(p,nome_do_pp);
	return(res);
}


void opcaoEditaPP(process_plan p) {
	char path[250],
		 path1[50],
		 path3[50];

	char editor[20],
		 cl[200];

	printf("\n Qual o process plan que quer editar:\n");
	scanf("%s",path1);
	strcpy(cl,"ls ");
	strcat(cl,"./");
	strcat(cl,path1);
	system(cl);
	printf("\n Qual o process type que quer editar:\n");
	scanf("%s",path3); //faltam validacoes
	strcpy(path,"./"); 
	strcat(path,path1); 
	strcat(path,"/"); 
	strcat(path,path3);
		
	strcpy(editor,getenv("EDITOR"));
	printf("\n\n\n EDITOR usado =  %s",editor);
	strcpy(cl,editor); 
	strcat(cl," "); 
	strcat(cl,path);
	printf("\n Invocando ... %s",cl);
	system(cl);
	p = ResetPP(p);
	printf("\n p = LePP(%p,%s);",p,path1);
	p = LePP(p,path1);	
	printf("\n p = LePP(%p,%s);",p,path1);
}

void opcaoCalculaEscalonamento(process_plan p) {
 percorrePP(p);
}

void opcaoListaPP(process_plan p) {
	printf("\n Listar Process Plan Carregado\n");
	
	if(p == NULL) printf("\n Esse PP esta vazio");
	while(p != NULL) {
		printf("\nNOME DO TIPO DE JOB = %s\n",p->nome_tipo_job);
		listaJob(p->Job);
		p = p->next;
	}
}

void opcaoListaEscalonamento() {
 printf("\nListar Escalonamento \n\n");
 printf("\n S=sequencia da operacao");
 printf("\n M=maquina");
 printf("\n TI=inicio");
 printf("\n TF=fim");
 printf("\n P=tipo de Processo\n\n");
 printf("S;M;TI;TF;P\n");
 system("cat ./output/* | sort");
}

int main(void) {
	int opc;

	process_plan plan=NULL;
	ResetWP();
while(1) {
	opc = menu();
	
	switch (opc)
	{
	case 1:
		plan = opcaoLePP(plan);
		break;
	case 2:
		opcaoEscrevePP(plan);
		break;
	case 3:
		opcaoEditaPP(plan);
		break;
	case 4:
		opcaoCalculaEscalonamento(plan);
		break;
	case 5:
		opcaoListaEscalonamento();
		break;
	case 6:
		opcaoListaPP(plan);
		break;
	case 7:
		plan = ResetPP(plan);
		break;
	case 0:
		printf("\n Ok. Vamos sair.");
		exit(0);
		break;
	default:
		return(-1);
		break;
	}
	}
}



