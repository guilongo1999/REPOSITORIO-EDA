#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "include/fase1.h"

job criaJob(int m, int t) {
    job novaLista;

    novaLista = malloc(sizeof(operacao)); //alocar memoria para tamanho da operacao
    novaLista->seq=1;
    novaLista->maquina=m;
    novaLista->tempo=t;
    novaLista->next=NULL;
    return(novaLista);
}

void listaJob(job l) {

    while(l!=NULL) {
        printf("\n[%d] maq=%d, tempo=%d",l->seq,l->maquina,l->tempo);
        l=l->next;
        }
}

operacao *tailListaOperacao(job head) {
    operacao *tail;

    tail=head; //tail começa como sendo assumindo head
    if(head == NULL)
        return NULL; //caso não exista ainda primeiro nodulo
    while(tail->next != NULL)/* caso os seguintes sejam nulos */ 
		tail = tail->next; //buscar o ultimo termo
    return(tail); //a função devolve tail
}

void insereOperacao(int seq, int m, int t, job l) {
    job novo,aux;

    aux=tailListaOperacao(l); //aux assume o valor do ultimo nodulo
    novo=malloc(sizeof(operacao));
    novo->seq=seq;
    novo->maquina=m;
    novo->tempo=t;
    novo->next=NULL;
    aux->next=novo;
}

job removeOperacao(int seq, int m, job l) {
	job ret,prev,temp=l;

    if (temp != NULL && temp->seq == seq && temp->maquina == m) {
        l = temp->next; // changed head
        free(temp); // free old head
		return(l);
    }
    while (temp != NULL && (temp->maquina!=m || temp->seq!=seq )) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
		perror("\nNó não foi encontrado");
	else {
    prev->next = temp->next;
    free(temp); // free memory
	}
    return(l);
}

void alteraOperacao(int seq, int om, int ot, int nm, int nt, job j) {
	/* 	seq = n de sequencia da operacao
		om = codigo de máquina a substituir
		ot = antigo tempo
		nm = nova maquina
		nt = novo tempo
	*/
	job aux=j;

	if(aux == NULL) perror("\n Foi passado um job vazio");

	while(aux != NULL) {
		if(aux->seq == seq && aux->maquina == om && aux->tempo == ot) {
			aux->maquina = nm;
			aux->tempo = nt;
		}
		else
			aux=aux->next;
	}
}

int calculaMinTempoJob(job j){ //parametro j
	job jaux=j;
	int tempo_min=INT_MAX;  // a fim de comparar os tempos, assume um valor max para depois ir substituindo por menor munero
	int tempo_total=0; //começa com um valor 0 posteriormente soma
	int seq_actual=j->seq; //continua a sequência que será a continuação

	while(jaux != NULL){ // enquanto não chega ao fim da lista
		if(jaux->tempo <= tempo_min) //se tempo_min inferior aos valores da lista
			tempo_min=jaux->tempo;
		if(jaux->next == NULL) {
			tempo_total=tempo_total+tempo_min;
			return(tempo_total);
		}
		else if(jaux->next->seq != seq_actual) {
			seq_actual=jaux->next->seq;
			tempo_total=tempo_total+tempo_min;
			tempo_min=INT_MAX;
		}
		jaux=jaux->next;
	}
}
int calculaMaxTempoJob(job j){

	job jaux=j;
	int tempo_max = INT_MIN;
	int tempo_total=0;
	int seq_actual=j->seq;

	while (jaux != NULL){
 		if (jaux->tempo >= tempo_max)
 			tempo_max = jaux->tempo;
 		if(jaux->next == NULL) {
			tempo_total=tempo_total+tempo_max;
	 		return(tempo_total);
 }
 		else if(jaux->next->seq != seq_actual){

	 		seq_actual=jaux->next->seq;
	 		tempo_total=tempo_total+tempo_max;
	 		tempo_max=INT_MIN; //assume o valor alterado
 		}
 		jaux=jaux->next;
	}
}

job leFicheiroJob(char *nome_fich) {
	FILE *fp;
	int maquina,tempo,seq;
	job novo;

	fp=fopen(nome_fich,"r");
	if(fp!=NULL) {
		fscanf(fp,"[%d]%d,%d",&seq,&maquina,&tempo);
		novo=criaJob(maquina,tempo);
		while(fscanf(fp,"\n[%d]%d,%d",&seq,&maquina,&tempo) == 3) {
			insereOperacao(seq,maquina,tempo,novo);
		}
	}
	fclose(fp);
	return(novo);
}

int escreveFicheiroJob(char *nome_fich, job j) {
	FILE *fp=fopen(nome_fich,"w+"); //so escrita
	job aux=j; //aux e o job

	while(aux != NULL) {
		fprintf(fp,"[%d]%d,%d\n",aux->seq,aux->maquina,aux->tempo); //imprime os valores criados da struct
		aux=aux->next;
	}
	fclose(fp);
}
