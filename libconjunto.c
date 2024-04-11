#include <stdio.h>
#include <stdlib.h>
#include "libconjunto.h"

conjunto_t * cria_conjunto(int max){

  conjunto_t *c = malloc(sizeof(conjunto_t));

  if (! c){
    perror("Erro na alocacao de memoria!\n");
    return NULL;
  }

  c->max = max;

  c->ptr = -1;

  c->v = malloc(sizeof(int)*c->max);

  if (! c->v){
    perror("Erro na alocacao de memoria!\n");
    free(c);
    return NULL;
  }

  c->card = 0;

  return c;

}

int conjunto_vazio(conjunto_t *c){

  if (c->card == 0)
    return 1;
  return 0;

}

int cardinalidade (conjunto_t *c){

  return c->card;

}

int pertence (conjunto_t *c, int elemento){

  int ini = 0;

  int fim = c->card - 1;

  int meio = (ini + fim)/2;

  if(conjunto_vazio(c))
    return -2;
  else{

    while(ini <= fim && elemento != c->v[meio]){
      
      if(elemento < c->v[meio])
        fim = meio - 1;

      else
        ini = meio + 1;

      meio = (ini + fim)/2;

    }
    
    if(fim < ini)
      return -1;
    else 
      return meio;
  }

}

int insere_conjunto(conjunto_t *c, int elemento){

  int i = 0;

  if (pertence(c,elemento) > -1)
    return 0;

  if (conjunto_vazio(c) == 1 && c->max != 0){

      c->v[i] = elemento;

      c->card++;

      return 1;

  } else if (c->card < c->max){

    i = c->card -1;

    while (i >= 0 && elemento < c->v[i]){

      c->v[i+1] = c->v[i];
      i--;

    }

    c->v[i+1] = elemento;

    c->card++;

    return 1;

  }

  return 0;

}

int retira_conjunto (conjunto_t *c, int elemento){
  
  int pos = pertence(c, elemento);
  
  if (pos > -1){
  
    for (int i = pos; i < c->card - 1; i++)
      c->v[i] = c->v[i+1];
  
    c->card--;
  
    return elemento;
  }
  
    return -1;
}

int sao_iguais (conjunto_t *c1, conjunto_t *c2){

  if (c1->card != c2->card)
    return 0;

  for (int i = 0; i < c1->card; i++)
    if (c1->v[i] != c2->v[i])
      return 0;

  return 1;

}

conjunto_t *cria_uniao(conjunto_t *c1, conjunto_t *c2){
  
    conjunto_t *c = cria_conjunto(c1->card + c2->card);
  
    if (! c)
      return NULL;
  
    for (int i = 0; i < c1->card; i++)
      insere_conjunto(c, c1->v[i]);
  
    for (int i = 0; i < c2->card; i++)
      insere_conjunto(c, c2->v[i]);
  
    return c;
}

conjunto_t * cria_interseccao (conjunto_t *c1, conjunto_t *c2){

  conjunto_t *intersec = cria_conjunto(c1->card);

  if (! intersec)
    return NULL;

  for (int i = 0; i < c1->card; i++)
    if (pertence(c2, c1->v[i]) > -1)
      insere_conjunto(intersec, c1->v[i]);

  return intersec;

}

conjunto_t * cria_diferenca (conjunto_t *c1, conjunto_t *c2){

  conjunto_t *dif = cria_conjunto(c1->card);

  if (! dif)
    return NULL;

  for (int i = 0; i < c1->card; i++)
    if (pertence(c2, c1->v[i]) == -1)
      insere_conjunto(dif, c1->v[i]);

  return dif;

}

int contido(conjunto_t *c1, conjunto_t *c2){

  for (int i = 0; i < c1->card; i++)
    if (pertence(c2, c1->v[i]) == -1)
      return 0;

  return 1;

}

conjunto_t *cria_copia(conjunto_t *c1){
  
    conjunto_t *c = cria_conjunto(c1->max);
  
    if (! c)
      return NULL;
  
    for (int i = 0; i < c1->card; i++)
      insere_conjunto(c, c1->v[i]);
  
    return c;

}

int incrementar_iterador (conjunto_t *c, int *elemento){

  if (c->ptr < c->card){

    *elemento = c->v[c->ptr];

    c->ptr++;

    return 1;

  }

  return 0;
}

void iniciar_iterador (conjunto_t * c){

  c->ptr = 0;

}

int retirar_um_elemento(conjunto_t *c){

  int elemento = c->v[c->card];

  c->card--;

  return elemento;

}

conjunto_t * destroi_conjunto(conjunto_t *c){

  free(c->v);

  free(c);

  return NULL;

}

int redimensiona (conjunto_t *c){

  c->v = realloc(c->v, sizeof(int)*c->max*2);

  if (! c->v)
    return 0;

  c->max = c->max*2;

  return 1;

}

/*
 * Cria um subconjunto com elementos aleatorios do conjunto c.
 * Se o conjunto for vazio, retorna um subconjunto vazio.
 * Se o n >= cardinalidade (c) entao retorna o proprio conjunto c.
 * Supoe que a funcao srand () tenha sido chamada antes.
 */

conjunto_t * cria_subconjunto(conjunto_t *c, int n){

  conjunto_t *sub = cria_conjunto(n);

  if (! sub)
    return NULL;

  if (conjunto_vazio(c) == 1)
    return sub;

  if (n >= c->card)
    return cria_copia(c);

  while (n > 0){

    int pos = rand() % c->card;

    if (insere_conjunto(sub, c->v[pos]) == 1)
      n--;

  }

  return sub;

  
}