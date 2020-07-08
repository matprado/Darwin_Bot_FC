#ifndef ALGORITMO_GENETICO_H
#define ALGORITMO_GENETICO_H

#include <time.h>

#define TAM_POP 10
#define LIMITE_PROBABILIDADE 10000
#define TAXA_MUT_INI 0.0001
#define FATOR_MUTACAO 1.5
#define LIMITE_TAXA_MUT 1
#define N_IGUAIS_CONSECUTIVOS 5
#define GERACOES_PREDACAO 10

//Estrutura de um cromossomo(ou indivíduo)
typedef struct cromossomo Cromossomo;

void definir_seed(time_t seed);

void evolucao();

void big_bang();

void limpar_memoria_alocada_ag();

void definir_formacao(int pos, int formacao);

int *get_3_aleatorios_diferentes_entre(int a, int b);

void copia_the_bester_cromossomo();

void avalia_populacao();

void avalia_individuo(int i);

void elitismo();

void crossover();

int media_simples(int f1, int f2);

void mutacao();

void ajuste();

void mutacao_dinamica();

void criar_grafico_fitness();

void atualizar_grafico_fitness();

void predacao_sintese();

void predacao_aleatoria();

void genocidio_total();

void genocidio_parcial();

void predacao();

void imprimir_melhor_cromossomo();

void imprimir_cromossomo(Cromossomo c);

float get_fitness_melhor();

#endif