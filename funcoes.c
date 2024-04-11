#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "funcoes.h"

void inicializar_mundo (Mundo *M){

  int i;

  M->TempoAtual = TEMPO_INI;
  
  M->NPessoas = N_PESSOAS;
  
  M->NLocais = N_LOCAIS;

  M->Rumores = cria_conjunto(N_RUMORES_MUNDO);

  for(int i = 0; i < N_RUMORES_MUNDO; i++)
    insere_conjunto(M->Rumores, i);

  M->Pessoas = (Pessoa *) malloc (M->NPessoas * sizeof(Pessoa));

  M->Locais = (Local *) malloc (M->NLocais * sizeof(Local));


  for (i = 0; i < M->NPessoas; i++){

    M->Pessoas[i].id = i;

    M->Pessoas[i].extroversao = rand() % 100;

    M->Pessoas[i].paciencia = rand() % 100;

    M->Pessoas[i].idade = rand() % 100 + 18;

    M->Pessoas[i].rumores = cria_subconjunto(M->Rumores, 5);

  }

  for (i = 0; i < M->NLocais; i++){

    M->Locais[i].id = i;

    M->Locais[i].lotmax = rand() %10 + 5;

    M->Locais[i].pnolocal = cria_conjunto(M->Locais[i].lotmax);

    M->Locais[i].Localizacao.x = rand() % N_TAMANHO_MUNDO-1;

    M->Locais[i].Localizacao.y = rand() % N_TAMANHO_MUNDO-1;

    M->Locais[i].fila = cria_fila();

  }

}
void cria_evento_entrada_saida(lef_t *lef, int pessoaid, int localid, int tempo, int tipo, int inicio){

  evento_t chegada;

  ev_entrada_e_saida_t temp;

  temp.pessoaid = pessoaid;

  temp.localid = localid;

  chegada.tempo = tempo;

  chegada.tipo = tipo;

  chegada.tamanho = sizeof(ev_entrada_e_saida_t);

  chegada.dados = malloc(sizeof(ev_entrada_e_saida_t));

  if(! chegada.dados){
      printf("Erro ao alocar memória para o evento de entrada\n");
      exit(1);
  }

  memcpy(chegada.dados, &temp, chegada.tamanho);

  if(inicio == 1)
    adiciona_inicio_lef(lef, &chegada);
  else 
    adiciona_ordem_lef(lef, &chegada);

}

void cria_evento_rumor(Mundo *M, lef_t *lef, int pessoaid, int localid, int NRD, int tempoatual){

  evento_t rumor;

  ev_rumor_t temp;

  temp.pessoaid = pessoaid;

  temp.localid = localid;

  rumor.tempo = tempoatual;

  rumor.tipo = EV_RUMOR;

  rumor.tamanho = sizeof(ev_rumor_t);

  rumor.dados = malloc(sizeof(ev_rumor_t));

  if(! rumor.dados){
      printf("Erro ao alocar memória para o evento de rumor\n");
      exit(1);
  }

  memcpy(rumor.dados, &temp, rumor.tamanho);

  adiciona_ordem_lef(lef, &rumor);

}


void cria_evento_fim (lef_t *lef){

	evento_t fim;

	fim.tipo = EV_FIM;

	fim.tempo = FIM_DO_MUNDO;

	fim.tamanho = 0;

	fim.dados = NULL;

	adiciona_ordem_lef(lef, &fim);

}


/* Funcao que inicializa a lef */
lef_t * inicializar_lef(lef_t *lef){
	
	int i;
	for (i = 0; i < N_PESSOAS; i++)
		cria_evento_entrada_saida(lef,i,rand()%N_LOCAIS,rand()%96*7,EV_ENTRADA,0);

  cria_evento_fim(lef);

	return lef;
}



int ALEAT(int a, int b){

  return rand()%(b-a+1) + a;
  
}

int MAX(int a, int b){

  if(a > b)
    return a;
  else
    return b;

}

int MIN(int a, int b){
  
    if(a < b)
      return a;
    else
      return b; 
  
}

void tratar_ev_chegada(Mundo *M, lef_t *lef, int pessoaid, int localid, int tempoatual){

  int TPL, NRD;

  //Se o ID_LOCAL está lotado a ID_PESSOA deve decidir entre ficar na fila ou ir embora:
  if(cardinalidade(M->Locais[localid].pnolocal) == M->Locais[localid].lotmax){

    //Se (PESSOA[ID_PESSOA].PACIENCIA/4 - tamanho_fila(ID_LOCAL) > 0
    if(M->Pessoas[pessoaid].paciencia/4 - tamanho_fila(M->Locais[localid].fila) > 0){
      //Adiciona ID_PESSOA em ID_LOCAL.fila
      insere_fila(M->Locais[localid].fila, pessoaid);

      printf("%6d:CHEGA Pessoa %4d Local %d ( %2d/%2d), FILA %d\n", tempoatual, pessoaid, localid, cardinalidade(M->Locais[localid].pnolocal), M->Locais[localid].lotmax, tamanho_fila(M->Locais[localid].fila));
    }
    else{
      //senão
      //Cria um evento de SAIDA de ID_PESSOA e insere na LEF com TEMPO_ATUAL+0
      cria_evento_entrada_saida(lef, pessoaid, localid, tempoatual+0, EV_SAIDA, 0);

      retira_fila(M->Locais[localid].fila, &pessoaid);
      
      printf("%6d:CHEGA Pessoa %4d Local %d ( %2d/%2d), DESISTE\n", tempoatual, pessoaid, localid, cardinalidade(M->Locais[localid].pnolocal), M->Locais[localid].lotmax);

    }

  } else {

    TPL = MAX(1, M->Pessoas[pessoaid].paciencia/10+ALEAT(-2,6));

    insere_conjunto(M->Locais[localid].pnolocal, pessoaid);

    printf("%6d:CHEGA Pessoa %4d Local %d ( %2d/%2d), ENTRA\n", tempoatual, pessoaid, localid, cardinalidade(M->Locais[localid].pnolocal), M->Locais[localid].lotmax);

    cria_evento_entrada_saida(lef, pessoaid, localid, tempoatual+TPL, EV_SAIDA, 0);

    NRD = MIN(M->Pessoas[pessoaid].extroversao/10, TPL);

    cria_evento_rumor(M, lef, pessoaid, localid, NRD, tempoatual + ALEAT(0,TPL));
    
  }
}


int distancia(Coordenada a, Coordenada b){

  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));

}

void trata_ev_saida(Mundo *M, lef_t *lef, int pessoaid, int localid, int tempoatual){

  int ID_PESSOA_FILA, ID_LOCAL_DEST, TDL, D, V;

  // se a fila não está vazia, cria um evento de entrada
  if(fila_vazia(M->Locais[localid].fila) == 0){

    ID_PESSOA_FILA = retira_fila(M->Locais[localid].fila, &ID_PESSOA_FILA);

    cria_evento_entrada_saida(lef, ID_PESSOA_FILA, localid, tempoatual+0, EV_ENTRADA, 1);

    printf("%6d:SAIDA Pessoa %4d Local %d ( %2d/%2d), REMOVE FILA Pessoa %d\n", tempoatual, pessoaid, localid, cardinalidade(M->Locais[localid].pnolocal), M->Locais[localid].lotmax, ID_PESSOA_FILA);

  }

  ID_LOCAL_DEST = rand()%N_LOCAIS;

  D = distancia(M->Locais[localid].Localizacao, M->Locais[ID_LOCAL_DEST].Localizacao);

  V = 100-MAX(0,M->Pessoas[pessoaid].idade-40);

  TDL = D/V;

  cria_evento_entrada_saida(lef, pessoaid, ID_LOCAL_DEST, tempoatual+TDL/15, EV_ENTRADA, 0);

  retira_conjunto(M->Locais[localid].pnolocal, pessoaid);

  printf("%6d:SAIDA Pessoa %4d Local %d ( %2d/%2d)\n", tempoatual, pessoaid, localid, cardinalidade(M->Locais[localid].pnolocal), M->Locais[localid].lotmax);

}


/* 

Estrutura evento disseminação:

Para cada pessoa ID_PESSOA_ESCUTA do conjunto ID_LOCAL.Publico eqto CJ_RUMOR != vazio
  Se ( ALEAT(0,100) < ID_PESSOA_ESCUTA.extroversao )
    ID_RUMOR = retira_evento (CJ_RUMOR)
    insere(ID_PESSOA_ESCUTA.rumores)
senao
  ignora rumor

*/

void trata_ev_disseminacao(Mundo *M, lef_t *lef, int pessoaid, int localid, int tempoatual){

  int i, ID_RUMOR;
  
  iniciar_iterador(M->Pessoas[pessoaid].rumores);

  // para cada pessoa no local
  for(i = 0; i < M->Locais[localid].pnolocal->card; i++){

    // Se o número for menor que a extroversão
    if(ALEAT(0,100) < M->Pessoas[M->Locais[localid].pnolocal->v[i]].extroversao){

      //pega um rumor da pessoaid
      incrementar_iterador(M->Pessoas[pessoaid].rumores, &ID_RUMOR);

      //insere no conjunto de rumores da pessoa que escuta
      insere_conjunto(M->Pessoas[M->Locais[localid].pnolocal->v[i]].rumores, ID_RUMOR);

      printf("%6d:RUMOR Pessoa %4d Local %d (P%2d:R%2d)\n", tempoatual, pessoaid, localid, M->Locais[localid].pnolocal->v[i], ID_RUMOR);

    }

  }

}


/*

Estrutura das mensagens:

8:CHEGA Pessoa 44 Local 1 ( 0/19), ENTRA
16:CHEGA Pessoa 11 Local 5 ( 0/26), ENTRA
23:CHEGA Pessoa 44 Local 2 ( 0/21), ENTRA

232:CHEGA Pessoa 30 Local 3 ( 7/ 7), FILA 1
233:CHEGA Pessoa 74 Local 3 ( 7/ 7), FILA 2
401:CHEGA Pessoa 82 Local 4 ( 5/ 5), DESISTE

22:SAIDA Pessoa 44 Local 1 ( 1/19)
19:SAIDA Pessoa 11 Local 5 ( 1/26)

234:SAIDA Pessoa 35 Local 3 ( 7/ 7), REMOVE FILA Pessoa 30

12:RUMOR Pessoa 44 Local 1

84:RUMOR Pessoa 8 Local 5 (P11:R19) (P31:R13) (P93:R5)

O exemplo acima mostra que a pessoa 8 conseguiu atingir 3 pessoas com seus
rumores no Local 5. A pessoa 11 incorporou o rumor 19; a pessoa 31 incorporou o
rumor 13; e finalmente a pessoa 93 incorporou o rumor 5

tempo %6d
id de pessoas %4d
informa¸c~oes de lota¸c~ao dos locais %2d
tamanho da fila %2d
os pares de rumores incorporados s~ao apenas P%d/R%d

*/