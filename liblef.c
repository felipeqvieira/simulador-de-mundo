#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liblef.h"

lef_t *cria_lef(){

  lef_t *l = malloc(sizeof(lef_t));

  if (! l){
    perror("Erro de alocacao de memoria!\n");
    return NULL;
  }

  l -> Primeiro = NULL;

  return l;

}

lef_t * destroi_lef (lef_t *l){

  nodo_lef_t *aux;

  while (l->Primeiro){

    aux = l -> Primeiro;

    if (aux -> evento) {
      aux -> evento -> destroidados(aux->evento->dados);
      free(aux->evento);
    }

    l -> Primeiro = aux -> prox;
    free (aux);
  }  

  return NULL;
}

int adiciona_inicio_lef(lef_t *l, evento_t *evento){

  nodo_lef_t *novo;

  novo = malloc(sizeof(nodo_lef_t));

  if (! novo){
    perror("Erro de alocacao de memoria!\n");
    return 0;
  }

  novo -> evento = malloc(sizeof(evento_t));

  if (! novo -> evento){
    perror("Erro de alocacao de memoria!\n");
    return 0;
  }

  novo -> evento -> tempo = evento -> tempo;
  novo -> evento -> tipo = evento -> tipo;
  novo -> evento -> tamanho = evento -> tamanho;
  novo -> evento -> dados = malloc(evento -> tamanho);

  if (! novo -> evento -> dados){
    perror("Erro de alocacao de memoria!\n");
    return 0;
  }

  memcpy(novo -> evento -> dados, evento -> dados, evento -> tamanho);
  novo -> evento -> destroidados = evento -> destroidados;

  novo -> prox = l -> Primeiro;
  l -> Primeiro = novo;

  return 1;

}

int adiciona_ordem_lef(lef_t *l, evento_t *evento){
  
    nodo_lef_t *novo;
  
    novo = malloc(sizeof(nodo_lef_t));
  
    if (! novo){
      perror("Erro de alocacao de memoria!\n");
      return 0;
    }
  
    novo -> evento = malloc(sizeof(evento_t));
  
    if (! novo -> evento){
      perror("Erro de alocacao de memoria!\n");
      return 0;
    }
  
    novo -> evento -> tempo = evento -> tempo;
    novo -> evento -> tipo = evento -> tipo;
    novo -> evento -> tamanho = evento -> tamanho;
    novo -> evento -> dados = malloc(evento -> tamanho);
  
    if (! novo -> evento -> dados){
      perror("Erro de alocacao de memoria!\n");
      return 0;
    }
  
    memcpy(novo -> evento -> dados, evento -> dados, evento -> tamanho);
    novo -> evento -> destroidados = evento -> destroidados;
  
    novo -> prox = NULL;
  
    if (! l -> Primeiro){
      l -> Primeiro = novo;
      return 1;
    }
  
    if (novo -> evento -> tempo < l -> Primeiro -> evento -> tempo){
      novo -> prox = l -> Primeiro;
      l -> Primeiro = novo;
      return 1;
    }
  
    nodo_lef_t *aux;
  
    aux = l -> Primeiro;
  
    while (aux -> prox != NULL && aux -> prox -> evento -> tempo < novo -> evento -> tempo){
      aux = aux -> prox;
    }
  
    novo -> prox = aux -> prox;
    aux -> prox = novo;
  
    return 1;
}

evento_t * obtem_primeiro_lef (lef_t *l){

  evento_t *aux = malloc(sizeof(evento_t));

  if(! aux){
    perror("Erro de alocacao de memoria!\n");
    return NULL;
  }

  aux = l -> Primeiro -> evento;

  nodo_lef_t *remove = malloc(sizeof(nodo_lef_t));

  if (! remove){
    perror("Erro de alocacao de memoria!\n");
    return NULL;
  }

  remove = l -> Primeiro;

  l -> Primeiro = l -> Primeiro -> prox;

  free(remove);

  return aux;
  
}