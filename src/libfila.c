#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t * cria_fila(){

  fila_t *f = malloc(sizeof(fila_t));

  if(! f){
    perror("Erro de alocacao de memoria!\n");
    return NULL;
  }

  f->tamanho = 0;
  f->ini = NULL;
  f->fim = NULL;

  return f;
}

fila_t * destroi_fila(fila_t *f){
  struct nodo_f *excluir = NULL;

  if(f->ini){
    while(f->ini->prox){
      excluir = f->ini;
      f->ini = f->ini->prox;
      f->ini->prev = NULL;
      free(excluir);
    }

    excluir = f->ini;
    f->ini->prox = NULL;
    f->ini->prev = NULL;
    free(excluir);
  }

  free(f);

  return NULL;
}

int fila_vazia(fila_t *f){
  if(f->ini)
    return 0;
  else
    return 1;
}

int tamanho_fila(fila_t *f){
  return f->tamanho;
}

int insere_fila(fila_t *f, int elemento){
  nodo_f_t *novo = malloc(sizeof(nodo_f_t));

  if(! novo){
    perror("Erro de alocacao de memoria!\n");
    return 0;
  }

  novo->chave = elemento;
  novo->prox = NULL;
  novo->prev = f->fim;

  if(fila_vazia(f)){
    f->ini = novo;
    f->fim = novo;
  }else{
    f->fim->prox = novo;
    f->fim = novo;
  }

  f->tamanho++;

  return 1;
}

int retira_fila(fila_t *f, int *elemento){

  if(fila_vazia(f)){
    perror("Fila vazia!\n");
    return 0;
  }

  *elemento = f->ini->chave;

  nodo_f_t *excluir = f->ini;
  f->ini = f->ini->prox;

  if(f->ini)
    f->ini->prev = NULL;
  else
    f->fim = NULL;

  free(excluir);
  f->tamanho--;

  return 1;
}