#include <stdio.h>
#include <stdlib.h>
#include "src/funcoes.h"

int main(){

  srand(0);

  Mundo *M = malloc(sizeof(Mundo));

  if(! M){
    printf("Erro de alocacao de memoria\n");
    exit(1);
  }

  evento_t * e = malloc(sizeof(evento_t));

  if (! e){
    printf("Erro de alocacao de memoria\n");
    exit(1);
  }

  lef_t * lef = malloc(sizeof(lef_t));

  if (! lef){
    printf("Erro de alocacao de memoria\n");
    exit(1);
  }

  inicializar_mundo(M);

  inicializar_lef(lef);

  while(1){

    e = obtem_primeiro_lef(lef);

    switch(e->tipo){
      case EV_ENTRADA:
        tratar_ev_chegada(M, lef,((ev_entrada_e_saida_t*)e->dados)->pessoaid, ((ev_entrada_e_saida_t*)e->dados)->localid, e->tempo);
      break;

      case EV_SAIDA:
        trata_ev_saida(M, lef,((ev_entrada_e_saida_t*)e->dados)->pessoaid, ((ev_entrada_e_saida_t*)e->dados)->localid, e->tempo);
      break;

      
      case EV_RUMOR:
        trata_ev_disseminacao(M, lef,((ev_entrada_e_saida_t*)e->dados)->pessoaid, ((ev_entrada_e_saida_t*)e->dados)->localid, e->tempo);
      break;
      

      case EV_FIM:
        printf("Fim da simulacao\n");
        exit(0);
      break;
    }
    

  }

  return 0;

}