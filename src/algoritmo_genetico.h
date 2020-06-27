#ifndef ALGORITMO_GENETICO_H
#define ALGORITMO_GENETICO_H

#include <time.h>

#define TAM_POP 100
#define TAXA_MUT_INI 0.001
#define VALOR_MAX 1000
#define VALOR_MEIO (VALOR_MAX / 2)
#define N_IGUAIS_CONSECUTIVOS 5
#define FATOR_MUTACAO 2 // DOBRA
#define LIMITE_TAXA_MUT 3

//Estrutura de um cromossomo(ou indivíduo)
typedef struct cromossomo Cromossomo;

void definir_seed(time_t seed);

void evolucao();

void big_bang();

void limpar_memoria_alocada_ag();

void definir_formacao(Cromossomo *c, int formacao);

int *get_3_aleatorios_diferentes_entre(int a, int b);

void geracao();

void avalia_populacao();

void avalia_individuo(int i);

void elitismo();

void crossover();

void mutacao();

void ajuste();

void mutacao_dinamica();

void atualizar_grafico_fitness();

void predacao_sintese();

void predacao_aleatoria();

void genocidio_total();

void genocidio_parcial();

void imprime_pop();

#endif