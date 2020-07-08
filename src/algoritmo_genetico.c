#include "algoritmo_genetico.h"
#include "cartola_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct cromossomo{

    //Informações adicionais sobre o time durante a simulação

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
    
    //int *pref_tec;
    int *pref_gol;
    int *pref_lat;
    int *pref_zag;
    int *pref_mei;
    int *pref_ata;

    //Preferencia por mando de campo em cada posição
    //int mando_pos[6];

};

//Melhor EVER, TEM QUE SABER QUEM É ESSE CARA SEMPRE, NÃO PODE MUTAR ELE
Cromossomo the_best_of_the_bester;
float the_best_of_the_bester_fitness = 0;

int the_bester; //Melhor atual da população
Cromossomo pop[TAM_POP]; //População de indivíduos/cromossomos
float fitness[TAM_POP]; //Nota de cada cromossomo da população
float fitness_medio; //Nota média da população
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

    //reproduz e muta
    elitismo(); //Seleciona o melhor individuo para transar com todos
 
    //Dinamiza a mutação e faz genocídio
    //mutacao_dinamica();

    //Predação a cada 10 gerações alternando o tipo de predação:
    /*if(n_geracao % 10 == 0){
        if(tipo_predacao){
            predacao_sintese();
        }else{
            predacao_aleatoria();
        }
        tipo_predacao = (tipo_predacao + 1) % 2;
    }*/

    //avalia população nova
    avalia_populacao();

    //atualiza gráfico de fitness
    atualizar_grafico_fitness();

    n_geracao++;

}

void big_bang(){

    for (int i = 0; i < TAM_POP; i++){

        //formacao: entre 1 e 7
        int formacao = (rand() % 7) + 1;

        //define a formacao
        definir_formacao(i, formacao);

        /*for(int i=0; i<6; i++){
            pop[i].mando_pos[i] = rand() % 2;
        }*/

        //Define, aleatoriamente, as 3 preferências de escalação para cada posição
        pop[i].pref_gol = get_3_aleatorios_diferentes_entre(1, get_n_ids_gol());
        pop[i].pref_zag = get_3_aleatorios_diferentes_entre(1, get_n_ids_zag());
        pop[i].pref_lat = get_3_aleatorios_diferentes_entre(1, get_n_ids_lat());
        pop[i].pref_mei = get_3_aleatorios_diferentes_entre(1, get_n_ids_mei());
        pop[i].pref_ata = get_3_aleatorios_diferentes_entre(1, get_n_ids_ata());  
    }

    n_geracao = 0;

    the_best_of_the_bester.pref_gol = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_zag = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_lat = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_mei = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_ata = (int *) malloc(3 * sizeof(int));

}

void limpar_memoria_alocada_ag(){
    for(int i=0; i<TAM_POP; i++){
        free(pop[i].pref_gol);
        free(pop[i].pref_zag);
        free(pop[i].pref_lat);
        free(pop[i].pref_mei);
        free(pop[i].pref_ata);
    }
    free(the_best_of_the_bester.pref_gol);
    free(the_best_of_the_bester.pref_zag);
    free(the_best_of_the_bester.pref_lat);
    free(the_best_of_the_bester.pref_mei);
    free(the_best_of_the_bester.pref_ata);
    fclose(melhor_fitness);
    fclose(media_fitness);
}

void definir_formacao(int pos, int formacao){
    
    /* 1 : 3-2-4-1 ou 5-4-1
       2 : 2-2-5-1 ou 4-5-1
       3 : 3-2-3-2 ou 5-3-2
       4 : 3-0-5-2 ou 3-5-2
       5 : 2-2-4-2 ou 4-4-2
       6 : 3-0-4-3 ou 3-4-3
       7 : 2-2-3-3 ou 4-3-3*/

    //Sempre temos 1 tecnico e 1 goleiro
    pop[pos].quant_pos[0] = 1;
    pop[pos].quant_pos[1] = 1;

    pop[pos].formacao = formacao;

    switch(formacao){

        case 1:
            pop[pos].quant_pos[2] = 3;
            pop[pos].quant_pos[3] = 2;
            pop[pos].quant_pos[4] = 4;
            pop[pos].quant_pos[5] = 1;
            break;

        case 2:
            pop[pos].quant_pos[2] = 2;
            pop[pos].quant_pos[3] = 2;
            pop[pos].quant_pos[4] = 5;
            pop[pos].quant_pos[5] = 1;
            break;

        case 3:
            pop[pos].quant_pos[2] = 3;
            pop[pos].quant_pos[3] = 2;
            pop[pos].quant_pos[4] = 3;
            pop[pos].quant_pos[5] = 2;
            break;

        case 4:
            pop[pos].quant_pos[2] = 3;
            pop[pos].quant_pos[3] = 0;
            pop[pos].quant_pos[4] = 5;
            pop[pos].quant_pos[5] = 2;
            break;
        
        case 5:
            pop[pos].quant_pos[2] = 2;
            pop[pos].quant_pos[3] = 2;
            pop[pos].quant_pos[4] = 4;
            pop[pos].quant_pos[5] = 2;
            break;

        case 6:
            pop[pos].quant_pos[2] = 3;
            pop[pos].quant_pos[3] = 0;
            pop[pos].quant_pos[4] = 4;
            pop[pos].quant_pos[5] = 3;
            break;

        case 7:
            pop[pos].quant_pos[2] = 2;
            pop[pos].quant_pos[3] = 2;
            pop[pos].quant_pos[4] = 3;
            pop[pos].quant_pos[5] = 3;
            break;

        default:
            printf("ERRO na formação");
            break;
    }

}

int *get_3_aleatorios_diferentes_entre(int a, int b){

    int *vet = (int *) malloc(3 * sizeof(int));

    int range = b - a;

    vet[0] = rand() % range;

    while((vet[1] = rand() % range) == vet[0]);

    while((vet[2] = rand() % range) == vet[1] || vet[2] == vet[0]);

    for(int i=0; i<3; i++) vet[i] += a;

    return vet;

}

void avalia_populacao(){

    float maior = -100000;
    float menor = 100000;
    fitness_medio = 0;
    //media_pop = 0;

    //registra maior e menor fitness
    for(int i=0; i<TAM_POP; i++){

        //fitness é a pontuação média do time em um campeonato
        printf("----------------------TIME %d-----------------------\n", i+1);
        printf("*** Formacao: %d-%d-%d ***\n", pop[i].quant_pos[2] + pop[i].quant_pos[3], pop[i].quant_pos[4], pop[i].quant_pos[5]);
        printf("*** Pref. Goleiro: %d, %d e %d ***\n", pop[i].pref_gol[0], pop[i].pref_gol[1], pop[i].pref_gol[2]);
        printf("*** Pref. Zagueiro: %d, %d e %d ***\n", pop[i].pref_zag[0], pop[i].pref_zag[1], pop[i].pref_zag[2]);
        printf("*** Pref. Lateral: %d, %d e %d ***\n", pop[i].pref_lat[0], pop[i].pref_lat[1], pop[i].pref_lat[2]);
        printf("*** Pref. Meia: %d, %d e %d ***\n", pop[i].pref_mei[0], pop[i].pref_mei[1], pop[i].pref_mei[2]);
        printf("*** Pref. Atacante: %d, %d e %d ***\n", pop[i].pref_ata[0], pop[i].pref_ata[1], pop[i].pref_ata[2]);
        
        fitness[i] = get_pontuacao_media(pop[i].quant_pos, pop[i].pref_gol, pop[i].pref_zag, pop[i].pref_lat, pop[i].pref_mei, pop[i].pref_ata);

        if(fitness[i] > maior){
            maior = fitness[i];
            the_bester = i;
        }else if(fitness[i] < menor){
            menor = fitness[i];
            the_worst = i;
        }

        fitness_medio += fitness[i];
        //media_pop += pop[i];

        printf("Em média, time %d conseguiu %.2f pontos\n\n", i+1, fitness[i]);

    }

    if(fitness[the_bester] > the_best_of_the_bester_fitness){
        the_best_of_the_bester_fitness = fitness[the_bester];
        copia_the_bester_cromossomo();
        //the_best_of_the_bester = pop[the_bester];
    }
    
    fitness_medio /= TAM_POP;
    
    //media_pop /= TAM_POP;

}

//a recebe a copia de b
void copia_the_bester_cromossomo(){
    the_best_of_the_bester.formacao = pop[the_bester].formacao;
    for(int i=0; i<6; i++) the_best_of_the_bester.quant_pos[i] = pop[the_bester].quant_pos[i];

    for(int i=0; i<3; i++){
        the_best_of_the_bester.pref_gol[i] = pop[the_bester].pref_gol[i];
        the_best_of_the_bester.pref_zag[i] = pop[the_bester].pref_zag[i];
        the_best_of_the_bester.pref_lat[i] = pop[the_bester].pref_lat[i];
        the_best_of_the_bester.pref_mei[i] = pop[the_bester].pref_mei[i];
        the_best_of_the_bester.pref_ata[i] = pop[the_bester].pref_ata[i];
    }
}

void elitismo(){
    for(int i=0; i<TAM_POP; i++){
        if(i == the_bester) continue;
        //Crossover:
        crossover(i, the_bester); 
        //Mutação
        mutacao(i);
        //Ajeitar Valores fora dos limites
        //ajuste();
    }
}

void crossover(int a, int b){
    //pop[i] = (pop[i] + pop[the_bester]) / 2;    //Crossover(soma e divide por 2)
    
    //Crossover(soma e divide por 2):

    int formacao = media_simples(pop[a].formacao, pop[b].formacao);
    definir_formacao(a, formacao);
    
    for(int i=0; i<3; i++){
        pop[a].pref_gol[i] = media_simples(pop[a].pref_gol[i], pop[b].pref_gol[i]);
        pop[a].pref_zag[i] = media_simples(pop[a].pref_zag[i], pop[b].pref_zag[i]);
        pop[a].pref_lat[i] = media_simples(pop[a].pref_lat[i], pop[b].pref_lat[i]);
        pop[a].pref_mei[i] = media_simples(pop[a].pref_mei[i], pop[b].pref_mei[i]);
        pop[a].pref_ata[i] = media_simples(pop[a].pref_ata[i], pop[b].pref_ata[i]);
    }

}

int media_simples(int f1, int f2){
    int f3 = f1 + f2;
    if(f3 % 2 > 0) f3 += 1;
    return f3 / 2;
}

void mutacao(int i){
    //pop[i] = pop[i] + ((rand() % VALOR_MAX - VALOR_MEIO) * taxa_mutacao); //Mutação

    int teste;

    teste = (rand() % LIMITE_PROBABILIDADE)+1;
    if(taxa_mutacao*LIMITE_PROBABILIDADE <= teste){
        int formacao = pop[i].formacao + ((rand() % 7 - 3));    
        if(formacao < 0) formacao = (-formacao);
        else if(formacao > 6){
            formacao = 6 - (formacao - 6);
            if(formacao < 0) formacao = (-formacao) % VALOR_MAX;
        }
    }

    teste = (rand() % LIMITE_PROBABILIDADE)+1;
    if(taxa_mutacao*LIMITE_PROBABILIDADE <= teste){

    }
    
    if(pop[i] > VALOR_MAX){
        pop[i] = VALOR_MAX - (pop[i] - VALOR_MAX); 
        if(pop[i] < 0.0) pop[i] = (int)(-pop[i]) % VALOR_MAX;
    }

    definir_formacao(i, formacao);
    
    for(int i=0; i<3; i++){
        get_n_ids_gol()
        pop[a].pref_gol[i] = media_simples(pop[a].pref_gol[i], pop[b].pref_gol[i]);
        pop[a].pref_zag[i] = media_simples(pop[a].pref_zag[i], pop[b].pref_zag[i]);
        pop[a].pref_lat[i] = media_simples(pop[a].pref_lat[i], pop[b].pref_lat[i]);
        pop[a].pref_mei[i] = media_simples(pop[a].pref_mei[i], pop[b].pref_mei[i]);
        pop[a].pref_ata[i] = media_simples(pop[a].pref_ata[i], pop[b].pref_ata[i]);
    }

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

void criar_grafico_fitness(){
    char nome[30];
    printf("Digite o nome do arquivo dos melhores fitness: ");
    scanf("%s%*c", nome);
    melhor_fitness = fopen(nome, "w+");
    printf("Digite o nome do arquivo da média de fitness: ");
    scanf("%s%*c", nome);
    media_fitness = fopen(nome, "w+");
}

//recuperar

void atualizar_grafico_fitness(){
    //Vetor de melhores a cada geração
    besters[n_geracao] = fitness[the_bester];

    //Guarda o melhor por geração no arquivo melhor_fitness.dat
    fprintf(melhor_fitness, "%d\t%f\n", n_geracao, fitness[the_bester]);
    
    //Guarda a média de fitness por geração no arquivo media_fitness.dat
    fprintf(media_fitness, "%d\t%f\n", n_geracao, fitness_medio);
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