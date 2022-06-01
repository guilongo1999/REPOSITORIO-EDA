#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "fase2.h"


/**
 * @brief funcao regista WP
 */ 
void RegistaWP(char *nome_job,int seq,int maq, int tempo) {
    static const char PATHRESULT[] = "./output/";
    char machinefile[250];
    int ti=0,tf;
    char pad[12];
    int lixo1,lixo2,lixo3,lixo4;

    sprintf(machinefile,"%s%d",PATHRESULT,maq);
    FILE *fp = fopen(machinefile,"r");
    if(fp != NULL) {
        int i=0;
        i = fscanf(fp,"%d;%d;%d;%d;%s",&lixo1,&lixo2,&lixo3,&ti,pad);
        // printf("\n*************** MAQ=%d,SEQ=%d;M=%d;TI=%d;TF=%d",maq,lixo1,lixo2,lixo3,ti);  
        while(i == 4) {
            i = fscanf(fp,"\n%d;%d;%d;%d;%s",&lixo1,&lixo2,&lixo3,&ti,pad);
            //printf("\n ti=%d",ti);
        }
        //printf("\nI=%d MAQ=%d,SEQ=%d;M=%d;TI=%d;TF=%d",i,maq,lixo1,lixo2,lixo3,ti);  
        }
    else {
        ti = 0;
        }
    fclose(fp);
    FILE *fp1 = fopen(machinefile,"a+");
    if(fp1 == NULL) printf("\n\n\n ERRO na escrita de ficheiro");
    fprintf(fp1,"%d;%d;%d;%d;%s\n",seq,maq,ti,ti+tempo,nome_job);
    //printf("\n[%s]%d,%d,%d,%d---%d",nome_job,seq,maq,ti,ti+tempo,tempo);
    fclose(fp1);
}

void ResetWP() {
    char buf[255];

    DIR *d = opendir("output");
    struct dirent *ent;
    
    while ((ent = readdir(d)) != NULL) {
        struct stat st;
        sprintf(buf,"%s/%s","output",ent->d_name);
        int i = stat(buf,&st);
        if(S_ISREG(st.st_mode)) unlink(buf);
    }
    closedir(d);
}

void percorrePP(process_plan p) {
    int seq_ant = -1;
    int tempo_ant = INT_MAX;
    char nome_tipo_job[100];
    job jaux;
    int maquina_preferida = -1;

	printf("\n Percorre Process Plan \n");
    ResetWP();
	
	if(p == NULL) printf("\n Esse PP esta vazio");

	while(p != NULL) {
		strcpy(nome_tipo_job,p->nome_tipo_job);
		jaux = p->Job;
        maquina_preferida = jaux->maquina;
        
        tempo_ant = INT_MAX;
        seq_ant = -1;

        while(jaux != NULL) {
            
            if(jaux->next == NULL) {
                if(jaux->tempo <= tempo_ant && jaux->seq == seq_ant) {
                    printf("\n REGISTA %s s=%d m=%d t=%d",nome_tipo_job,jaux->seq,jaux->maquina,jaux->tempo);
                    RegistaWP(nome_tipo_job,jaux->seq,jaux->maquina,jaux->tempo);
                    tempo_ant = INT_MAX;
                }
                else {
                    printf("\n REGISTA %s s=%d m=%d t=%d",nome_tipo_job,jaux->seq,jaux->maquina,jaux->tempo);
                    RegistaWP(nome_tipo_job,jaux->seq,jaux->maquina,jaux->tempo);
                }
            }
            if(jaux->next != NULL && jaux->next->seq != jaux->seq) {
                if(jaux->tempo <= tempo_ant) {
                    printf("\n REGISTA %s s=%d m=%d t=%d",nome_tipo_job,jaux->seq,jaux->maquina,jaux->tempo);
                    RegistaWP(nome_tipo_job,jaux->seq,jaux->maquina,jaux->tempo);
                    tempo_ant = INT_MAX;
                    seq_ant = jaux->seq;
                }
                else {
                    printf("\n REGISTA %s s=%d m=%d t=%d",nome_tipo_job,jaux->seq,maquina_preferida,tempo_ant);
                    RegistaWP(nome_tipo_job,jaux->seq,maquina_preferida,tempo_ant);
                    seq_ant = jaux->seq;
                    tempo_ant = INT_MAX;
                }
            }
            if(jaux->next != NULL && jaux->next->seq == jaux->seq) {
                if(jaux->tempo <= tempo_ant) {
                    maquina_preferida = jaux->maquina;
                    tempo_ant = jaux->tempo;
                    seq_ant = jaux->seq;
                }
            }

            jaux = jaux->next;
            }
        p = p->next;    
        }
}



job ResetJob(job j) {
    if(j == NULL) {
        return NULL;
    }
    if(j->next == NULL) {
        ResetJob(j->next);
    }
    free(j);
    return(NULL);
}

process_plan ResetPP(process_plan p) {
    if (p == NULL) {
        return NULL;
    }
    if (p->next == NULL) {
        ResetJob(p->Job);
        ResetPP(p->next);
    }
    free(p);
    return NULL;
}

process_plan LePP(process_plan p, char *nome) {
        struct dirent *pDirent;
        DIR *pDir;
        char Path[255];
        process_plan novo_pp;
        process_plan aux = p;
        job jaux;

        pDir = opendir(nome);
        if (pDir == NULL) {
            printf ("Nao consegui abrir o process plan '%s'\n", nome);
            return NULL;
        }

        while ((pDirent = readdir(pDir)) != NULL) {
            if(pDirent->d_type == DT_REG) {
                strcpy(Path,nome);
                strcat(Path,"/");
                strcat(Path,pDirent->d_name);
                
                jaux = leFicheiroJob(Path); 
                novo_pp = malloc(sizeof(struct pp));
                novo_pp->Job = jaux;
                strcpy(novo_pp->nome_tipo_job,pDirent->d_name);
                novo_pp->next = NULL;
                
                if(aux == NULL) {
                    p = novo_pp;
                    aux = p;
                }
                else {
                    while(aux->next != NULL) aux = aux->next;
                        aux->next = novo_pp;
                }             
            }
        }
        closedir(pDir);
        return(p);
}

int EscrevePP(process_plan p,char *nome) {
    DIR *pDir;
    char prefixo[250];
    char path[250];
    job j;
    int res;

    pDir = opendir(nome);
    if (pDir == NULL) {
        printf ("\n Vamos criar um novo process plan '%s'\n", nome);
            res = mkdir(nome);
            if(res == 0) 
                printf("\n Criado um novo PP");
            else {
                printf("\n ERRO: Não consegui criar o PP \n");
                return(res);
            }
        }
    strcpy(prefixo,nome);
    strcat(prefixo,"/");
        
    if(p == NULL) {
        printf("\n ERRO: Estamos a escrever um plano sem conteúdo!");
        exit(0);
    }
    
    while(p != NULL) {
            strcpy(path,prefixo);
            strcat(path,p->nome_tipo_job);
            j = p->Job;
            res=escreveFicheiroJob(path,j);
            p=p->next;
    }
    return(res);
}