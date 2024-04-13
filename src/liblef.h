typedef  struct evento {
    int tempo;   /* tempo logico do evento */
    int tipo;    /* inteiro para diferenciar os diferentes tipos de evento */
    int tamanho; /* tamanho ocupado pela estrutura referente ao evento (dados) */
    void *dados; /* estrutura de dados que representa o evento */
    void (*destroidados)(void *dados); /* ponteiro para funcao que libera evento->dados*/
} evento_t;

typedef struct nodo_lef {
    evento_t *evento;
    struct nodo_lef *prox;
} nodo_lef_t;

/*
 * Representacao da LEF 
 */
typedef struct lef {
    nodo_lef_t  *Primeiro;
} lef_t;

/* 
 * Cria uma LEF vazia 
 */
lef_t * cria_lef ();

/* 
 * Destroi a LEF, sempre retorna NULL. 
 * Para evento eh necessario executar a funcao destroidados para liberar os dados 
 * que representam o evento.
 */ 
lef_t * destroi_lef (lef_t *l);

/* 
 * Adiciona um evento na primeira posicao da LEF. Uma nova copia
 * da estrutura evento sera feita. 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */ 
int adiciona_inicio_lef (lef_t *l, evento_t *evento);

/* 
 * Adiciona um evento de acordo com o valor evento->tempo na LEF. 
 * Uma nova copia da estrutura evento sera feita 
 * Retorna 1 em caso de sucesso, 0 caso contrario.
 */
int adiciona_ordem_lef  (lef_t *l, evento_t *evento);

/* 
 * Retorna o primeiro evento da LEF. A responsabilidade por desalocar
 * a memoria associado eh de quem chama essa funcao 
 */
evento_t * obtem_primeiro_lef (lef_t *l);

