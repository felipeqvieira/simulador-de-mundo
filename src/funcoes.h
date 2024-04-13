#include "liblef.h"
#include "libconjunto.h"
#include "libfila.h"

/*

Esqueleto da simulação:

  O tempo inicial é 0

  O tamanho do nosso mundo (N_TAMANHO_MUNDO) é 20000

  O tamanho do conjunto de (N_RUMORES_MUNDO) rumores é 30

  A nossa população (N_PESSOAS) é 100

  O número de locais (N_LOCAIS) é 8

  O final da simulação (FIM_DO_MUNDO) é no instante 34944

Criar o conjunto de rumores com ID sequencia de 0 até N_RUMORES_MUNDO-1

Criar um vetor de N_PESSOAS, cada PESSOA é inicializada com:

  id = número sequencial 0-N_PESSOAS-1

  extroversao = ALEAT(0,100)

  paciencia = ALEAT(0,100)

  idade = ALEAT (18,100)

  rumores = subconjunto com tamanho ALEAT(1,5) de CJ_RUMORES_MUNDO

Criar um vetor de N_LOCAIS, cada LOCAL é inicializada com:

  id = número sequencial 0-N_LOCAIS-1

  lotacao_max = ALEAT (5,30)

  localizacao = ALEAT(0,N_TAMANHO_MUNDO-1), ALEAT(0,N_TAMANHO_MUNDO-1)

  publico = conjunto vazio

Criar para cada cada PESSOA da população:

  EVENTO de CHEGADA em ID_LUGAR_DEST=ALEAT(0,N_LOCAIS-1)
  inserir na LEF com tempo: ALEAT(0,96*7)

Criar e inserir o evento de FIM no tempo FIM_DO_MUNDO

*/

/* Tempo inicial */
#define TEMPO_INI 0

/* Tamanho do mundo */
#define N_TAMANHO_MUNDO 20000

/* Numero de rumores */
#define N_RUMORES_MUNDO 30

/* Numero de pessoas */
#define N_PESSOAS 100

/*Numero de locais */
#define N_LOCAIS 8

/* Numero de rumores iniciais de cada pessoa */
#define RUMOR_MAX 5

/* Tempo Final */
#define FIM_DO_MUNDO 34944

/* Constante do evento entrada */
#define EV_ENTRADA 1

/* Constante do evento saida */
#define EV_SAIDA 2

/* Constante do evento saida */
#define EV_RUMOR 3

/* Constante do evento fim */
#define EV_FIM 4

/*
Pessoa: 

id: Inteiro que identifica de maneira única uma pessoa;

extroversão: Inteiro entre 0 e 100 que indica o grau de extroversão da pessoa; em nosso modelo pessoas mais extrovertidas (números mais altos) são mais propensas à disseminar e a ouvir rumores;

paciência: Inteiro entre 0 e 100 que indica quanto paciente uma pessoa é. Em nosso modelo isso afeta as decisões de permanência em
locais e filas;

idade: Inteiro entre 18 e 100 indicando a idade em anos de uma pessoa. Em nossa simulação a idade afeta o tempo de deslocamento
entre lugares;

conjunto de rumores conhecidos (CRC): Rumores conhecidos por uma determinada pessoa e passíveis de disseminação.
*/

typedef struct Pessoa{

  int id;
  int extroversao;
  int paciencia;
  int idade;
  conjunto_t *rumores;

}Pessoa;

/*
Local: 

id: Inteiro que identifica o local;

lotação máxima: Número de pessoas máximo que estão em um lugar;

pessoas no lugar: Conjunto de identificadores das pessoas que estão atualmente no lugar;

fila: Fila onde as pessoas esperam para poderem entrar caso o mesmo esteja lotado;

localização: Par de inteiros (x, y) indicando uma coordenada em um plano cartesiano. Vamos considerar que o mapa de nossa cidade é representado por um plano cartesiano de tamanho tal que cada unidade representa 1 metro.

*/

typedef struct Coordenada{
  
    int x;
    int y;

}Coordenada;

typedef struct Local{

  int id;

  int lotmax;

  conjunto_t * pnolocal;

  fila_t * fila;

  Coordenada Localizacao;

}Local;

/*
Mundo:

TempoAtual: Inteiro positivo indicando o tempo atual da simulação. Cada unidade representa 15 minutos de tempo real;

Pessoas: Todas as pessoas;

Locais: Todos os locais;

Rumores: Conjunto de todos os rumores que podem ser conhecidos;

NPessoas: Número total de pessoas no mundo;

NLocais: Número total de locais no mundo;

NTamanhoMundo: Coordenadas máximas do plano cartesiano dos locais.

*/

typedef struct Mundo{

  int TempoAtual;

  Pessoa *Pessoas;

  Local *Locais;

  conjunto_t *Rumores;

  int NPessoas;

  int NLocais;

  int NTamanhoMundo;

}Mundo;

/*

Eventos:

Nosso relógio (TEMPO_ATUAL) inicia em 0.

Os eventos são registrados em uma Lista de Eventos Futuros (LEF) ordenados pelo seu tempo lógico.

Cada vez que um evento é lido o TEMPO_ATUAL é atualizado para o tempo do evento.

Em nossa simulação temos 4 tipos de eventos diferentes: CHEGADA, SAIDA, DISSEMINACAO e FIM.

*/

typedef struct ev_entrada_e_saida_t{

  int pessoaid;

  int localid;

}ev_entrada_e_saida_t;


typedef struct ev_rumor_t {

  int pessoaid;

  int localid;

}ev_rumor_t;

void cria_evento_entrada_saida(lef_t *lef, int pessoaid, int localid, int tempo, int tipo, int inicio);

void cria_evento_rumor(Mundo *M, lef_t *lef, int pessoaid, int localid, int NRD, int tempoatual);

void cria_evento_fim (lef_t *lef);

lef_t * inicializar_lef(lef_t *lef);

int MAX(int a, int b);

int MIN(int a, int b);

void tratar_ev_chegada(Mundo *M, lef_t *lef, int pessoaid, int localid, int tempoatual);

int distancia(Coordenada a, Coordenada b);

void trata_ev_saida(Mundo *M, lef_t *lef, int pessoaid, int localid, int tempoatual);

void trata_ev_disseminacao(Mundo *M, lef_t *lef, int pessoaid, int localid, int tempoatual);

void inicializar_mundo (Mundo *M);