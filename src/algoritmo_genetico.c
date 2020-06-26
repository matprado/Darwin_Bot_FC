#include "algoritmo_genetico.h"
#include "cartola_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct cromossomo{

    //Informações adicionais sobre o time durante a simulação

    float cartoletas;
    float media_pontuacao;
    int quant_pos[6];

    //Informações genéticas: são cruzadas com outros cromossomos
    
    int formacao;
    /* 1 : 3-2-4-1 ou 5-4-1
       2 : 2-2-5-1 ou 4-5-1
       3 : 3-2-3-2 ou 5-3-2
       4 : 3-0-5-2 ou 3-5-2
       5 : 2-2-4-2 ou 4-4-2
       6 : 3-0-4-3 ou 3-4-3
       7 : 2-2-3-3 ou 4-3-3*/

    //Inicialmente, considera o gasto uniforme de cartoletas
    
    int pref_tec[3];
    int pref_gol[3];
    int pref_lat[3];
    int pref_zag[3];
    int pref_mei[3];
    int pref_ata[3];

    //Preferencia por mando de campo em cada posição
    int mando_pos[6];

    //Preferencia por jogadores valorizados ou não em cada posição
    int valorizacao_pos[6];

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

void definir_seed(time_t seed){

    srand(seed);

}

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
        pop[i] = criar_cromossomo_aleatorio();    
    }

}

Cromossomo *criar_cromossomo_aleatorio(){

    Cromossomo *c = (Cromossomo *) malloc(sizeof(Cromossomo));

    //No começo do game, o time tem 100 cartoletas
    c->cartoletas = 100;
    c->media_pontuacao = 0;

    //formacao: entre 1 e 7
    int formacao = (rand() % 7) + 1;

    //Sempre temos 1 tecnico e 1 goleiro
    c->quant_pos[0] = 1;
    c->quant_pos[1] = 1;

    //define o resto da formacao
    definir_formacao(c, formacao);

    for(int i=0; i<6; i++){
        c->mando_pos[i] = rand() % 2;
    }

    int random = get_3_ale_dif_entre();

    definir_preferencia_tec(c);

    return c;

}

void definir_formacao(Cromossomo *c, int formacao){
    
    /* 1 : 3-2-4-1 ou 5-4-1
       2 : 2-2-5-1 ou 4-5-1
       3 : 3-2-3-2 ou 5-3-2
       4 : 3-0-5-2 ou 3-5-2
       5 : 2-2-4-2 ou 4-4-2
       6 : 3-0-4-3 ou 3-4-3
       7 : 2-2-3-3 ou 4-3-3*/

    c->formacao = formacao;

    switch(formacao){

        case 1:
            c->quant_pos[2] = 3;
            c->quant_pos[3] = 2;
            c->quant_pos[4] = 4;
            c->quant_pos[5] = 1;
            break;

        case 2:
            c->quant_pos[2] = 2;
            c->quant_pos[3] = 2;
            c->quant_pos[4] = 5;
            c->quant_pos[5] = 1;
            break;

        case 3:
            c->quant_pos[2] = 3;
            c->quant_pos[3] = 2;
            c->quant_pos[4] = 3;
            c->quant_pos[5] = 2;
            break;

        case 4:
            c->quant_pos[2] = 3;
            c->quant_pos[3] = 0;
            c->quant_pos[4] = 5;
            c->quant_pos[5] = 2;
            break;
        
        case 5:
            c->quant_pos[2] = 2;
            c->quant_pos[3] = 2;
            c->quant_pos[4] = 4;
            c->quant_pos[5] = 2;
            break;

        case 6:
            c->quant_pos[2] = 3;
            c->quant_pos[3] = 0;
            c->quant_pos[4] = 4;
            c->quant_pos[5] = 3;
            break;

        case 7:
            c->quant_pos[2] = 2;
            c->quant_pos[3] = 2;
            c->quant_pos[4] = 3;
            c->quant_pos[5] = 3;
            break;

        default:
            printf("ERRO na formação");
            break;
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

    /*if(pop[the_bester] > the_best_of_the_bester_fitness)
        the_best_of_the_bester_fitness = pop[the_bester];
    fitness_medio /= TAM_POP;*/
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
    //pop[i] = (pop[i] + pop[the_bester]) / 2;    //Crossover(soma e divide por 2)
}

int crossover_formacao(int f1, int f2){
    int f3 = f1 + f2;
    if(f3 % 2 > 0) f3 += 1;
    return f3 / 2;
}

void mutacao(){
    //pop[i] = pop[i] + ((rand() % VALOR_MAX - VALOR_MEIO) * taxa_mutacao); //Mutação
}

void ajuste(){
   /* if(pop[i] < 0.0) pop[i] = (-pop[i]);
    if(pop[i] > VALOR_MAX){
        pop[i] = VALOR_MAX - (pop[i] - VALOR_MAX); 
        if(pop[i] < 0.0) pop[i] = (int)(-pop[i]) % VALOR_MAX;
    }*/
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