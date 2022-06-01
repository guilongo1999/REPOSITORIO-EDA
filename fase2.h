#include "fase1.h"

typedef struct pp {
    job Job;
    char nome_tipo_job[100];
    struct pp *next;
} pp, *process_plan;

typedef struct wp {
    char nome_tipo_job[100];
    int seq;
    int maquina;
    int tempo_inicial;
    int tempo_final;
    struct wp *next;
} wp, *work_plan;

process_plan LePP(process_plan p, char *nome);
int EscrevePP(process_plan p, char *nome);
process_plan ResetPP(process_plan p);
void percorrePP(process_plan p);
void ResetWP();
