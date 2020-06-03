#include "algoritmo_genetico.h"
#include "cartola_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cromossomo{


};

int the_best_of_the_bester_fitness = 0;  //Melhor EVER, TEM QUE SABER QUEM É ESSE CARA SEMPRE, NÃO PODE MUTAR ELE
int the_bester; //Melhor atual da população
Cromossomo pop[TAM_POP]; //População de indivíduos/cromossomos
int fitness[TAM_POP]; //Nota de cada cromossomo da população
int fitness_medio; //Nota média da população
char aux[20];
int n_geracao = 0;
int besters[100000];
FILE *melhor_fitness;
FILE *media_fitness;
int iguais_consecutivos = 0;
double taxa_mutacao = TAXA_MUT_INI;
int the_worst;
int tipo_predacao = 0;
int tipo_genocidio = 0;
double media_pop = 0;

void evolucao(){

    atualizar_grafico_fitness();

    mutacao_dinamica();

    //Predação a cada 10 gerações alternando o tipo de predação:
    if(n_geracao % 10 == 0){
        if(tipo_predacao){
            predacao_sintese();
        }else{
            predacao_aleatoria();
        }
        tipo_predacao = (tipo_predacao + 1) % 2;
    }

    geracao();

}

void big_bang(){
    for (int i = 0; i < TAM_POP; i++){
        //pop[i] = 
    }
}

void geracao(){
    elitismo(); //Seleciona o melhor individuo para transar com todos
    avalia_populacao();
    n_geracao++;
}

void avalia_populacao(){
    int maior = -1;
    int menor = fitness[0];
    fitness_medio = 0;
    //media_pop = 0;
    for(int i=0; i<TAM_POP; i++){
        //fitness[i] = f_de_x_teto_de_casa(pop[i]);
        avalia_individuo(i);
        if(fitness[i] > maior){
            maior = fitness[i];
            the_bester = i;
        }else if(fitness[i] < menor){
            menor = fitness[i];
            the_worst = i;
        }
        fitness_medio += fitness[i];
        //media_pop += pop[i];
    }
    if(pop[the_bester] > the_best_of_the_bester_fitness)
        the_best_of_the_bester_fitness = pop[the_bester];
    fitness_medio /= TAM_POP;
    //media_pop /= TAM_POP;
}

void avalia_individuo(int i){

    

}

void elitismo(){
    for(int i=0; i<TAM_POP; i++){
        if(i == the_bester) continue;
        //Crossover:
        //crossover(i, the_bester);    
        //Mutação
        //mutacao();
        //Ajeitar Valores fora dos limites
        //ajuste();
    }
}

void crossover(){
    pop[i] = (pop[i] + pop[the_bester]) / 2;    //Crossover(soma e divide por 2)
}

void mutacao(){
    pop[i] = pop[i] + ((rand() % VALOR_MAX - VALOR_MEIO) * taxa_mutacao); //Mutação
}

void ajuste(){
    if(pop[i] < 0.0) pop[i] = (-pop[i]);
    if(pop[i] > VALOR_MAX){
        pop[i] = VALOR_MAX - (pop[i] - VALOR_MAX); 
        if(pop[i] < 0.0) pop[i] = (int)(-pop[i]) % VALOR_MAX;
    }
}

void mutacao_dinamica(){
    if(n_geracao>0 && besters[n_geracao] == besters[n_geracao-1]){
        iguais_consecutivos++;
        if(iguais_consecutivos == N_IGUAIS_CONSECUTIVOS){
            taxa_mutacao *= FATOR_MUTACAO;
            //Taxa de Mutação chega no seu limite máximo:
            if(taxa_mutacao > LIMITE_TAXA_MUT){
                //Reseta:
                taxa_mutacao = TAXA_MUT_INI;
                //Genocídio:
                if(tipo_genocidio){
                    genocidio_total();
                }else{
                    genocidio_parcial();
                }
                tipo_genocidio = (tipo_genocidio + 1) % 2;
            }
            iguais_consecutivos = 0;
        }
    }else{
        taxa_mutacao = TAXA_MUT_INI;
        iguais_consecutivos = 0;
    }
}

void atualizar_grafico_fitness(){
    //Vetor de melhores a cada geração
    besters[n_geracao] = fitness[the_bester];

    //Guarda o melhor por geração no arquivo melhor_fitness.dat
    fprintf(melhor_fitness, "%d\t%d\n", n_geracao, fitness[the_bester]);
    
    //Guarda a média de fitness por geração no arquivo media_fitness.dat
    fprintf(media_fitness, "%d\t%d\n", n_geracao, fitness_medio);
}

void predacao_sintese(){
    //predacao por síntese
    //pop[the_worst] = media_pop;
}

void predacao_aleatoria(){
    //predacao aleatoria
    //pop[the_worst] = (rand() % VALOR_MAX);
}

void genocidio_total(){
    for(int i=0; i<TAM_POP; i++){
        //Mata o melhor
        //pop[i] = (rand() % VALOR_MAX);
    }
}

void genocidio_parcial(){
    for(int i=0; i<TAM_POP; i++){
        //Não mata o melhor
        if(i == the_bester) continue;
        //pop[i] = (rand() % VALOR_MAX);
    }
}

void imprime_pop(){
    /*for (int i = 0; i < TAM_POP; i++){
        printf("Ser nº %d --> x = %.2lf, y = %.2lf\n", i, pop[i], fitness[i]);
    }*/
}