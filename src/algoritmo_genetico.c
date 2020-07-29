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

    int capitao;
    /*
    1-gol
    2-zag
    3-lat
    4-mei
    5-ata
    6-tec
    */

    //Preferencia por mando de campo em cada posição
    //int mando_pos[6];

};

//Melhor EVER, TEM QUE SABER QUEM É ESSE CARA SEMPRE, NÃO PODE MUTAR ELE
Cromossomo the_best_of_the_bester;
float the_best_of_the_bester_fitness = -10000;

int the_bester; //Melhor atual da população
Cromossomo pop[TAM_POP]; //População de indivíduos/cromossomos
float fitness[TAM_POP]; //Nota de cada cromossomo da população
float fitness_medio; //Nota média da população
int n_geracao = 0;
float besters[100000];
FILE *melhor_fitness;
FILE *media_fitness;
FILE *melhor_fitness_atual;
FILE *backup;
int iguais_consecutivos = 0;
double taxa_mutacao = TAXA_MUT_INI;
int the_worst;
int tipo_predacao = 0;
int tipo_genocidio = 0;
Cromossomo sintese;
int geracao_total;

double taxa_mutacao_for = TAXA_MUT_INI_FOR;

void definir_seed(time_t seed){
    srand(seed);
}

void evolucao(){

    //reproduz e muta
    elitismo(); //Seleciona o melhor individuo para transar com todos
 
    //Dinamiza a mutação e faz genocídio
    mutacao_dinamica();
    mutacao_dinamica_delta();

    //preda
    predacao();

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

        //capitao: entre 1 e 6
        pop[i].capitao = (rand() % 6) + 1;

        //Define, aleatoriamente, as 3 preferências de escalação para cada posição
        pop[i].pref_gol = get_3_aleatorios_diferentes_entre(1, get_n_ids_gol());
        pop[i].pref_zag = get_3_aleatorios_diferentes_entre(1, get_n_ids_zag());
        pop[i].pref_lat = get_3_aleatorios_diferentes_entre(1, get_n_ids_lat());
        pop[i].pref_mei = get_3_aleatorios_diferentes_entre(1, get_n_ids_mei());
        pop[i].pref_ata = get_3_aleatorios_diferentes_entre(1, get_n_ids_ata());  
    }

    n_geracao = 0;
    geracao_total = 0;

    the_best_of_the_bester.pref_gol = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_zag = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_lat = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_mei = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_ata = (int *) malloc(3 * sizeof(int));

    sintese.pref_gol = (int *) malloc(3 * sizeof(int));
    sintese.pref_zag = (int *) malloc(3 * sizeof(int));
    sintese.pref_lat = (int *) malloc(3 * sizeof(int));
    sintese.pref_mei = (int *) malloc(3 * sizeof(int));
    sintese.pref_ata = (int *) malloc(3 * sizeof(int));

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
    free(sintese.pref_gol);
    free(sintese.pref_zag);
    free(sintese.pref_lat);
    free(sintese.pref_mei);
    free(sintese.pref_ata);
    fclose(melhor_fitness);
    fclose(media_fitness);
    fclose(melhor_fitness_atual);
    fclose(backup);
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

void definir_formacao_bester(int formacao){
    
    /* 1 : 3-2-4-1 ou 5-4-1
       2 : 2-2-5-1 ou 4-5-1
       3 : 3-2-3-2 ou 5-3-2
       4 : 3-0-5-2 ou 3-5-2
       5 : 2-2-4-2 ou 4-4-2
       6 : 3-0-4-3 ou 3-4-3
       7 : 2-2-3-3 ou 4-3-3*/

    //Sempre temos 1 tecnico e 1 goleiro
    the_best_of_the_bester.quant_pos[0] = 1;
    the_best_of_the_bester.quant_pos[1] = 1;

    the_best_of_the_bester.formacao = formacao;

    switch(formacao){

        case 1:
            the_best_of_the_bester.quant_pos[2] = 3;
            the_best_of_the_bester.quant_pos[3] = 2;
            the_best_of_the_bester.quant_pos[4] = 4;
            the_best_of_the_bester.quant_pos[5] = 1;
            break;

        case 2:
            the_best_of_the_bester.quant_pos[2] = 2;
            the_best_of_the_bester.quant_pos[3] = 2;
            the_best_of_the_bester.quant_pos[4] = 5;
            the_best_of_the_bester.quant_pos[5] = 1;
            break;

        case 3:
            the_best_of_the_bester.quant_pos[2] = 3;
            the_best_of_the_bester.quant_pos[3] = 2;
            the_best_of_the_bester.quant_pos[4] = 3;
            the_best_of_the_bester.quant_pos[5] = 2;
            break;

        case 4:
            the_best_of_the_bester.quant_pos[2] = 3;
            the_best_of_the_bester.quant_pos[3] = 0;
            the_best_of_the_bester.quant_pos[4] = 5;
            the_best_of_the_bester.quant_pos[5] = 2;
            break;
        
        case 5:
            the_best_of_the_bester.quant_pos[2] = 2;
            the_best_of_the_bester.quant_pos[3] = 2;
            the_best_of_the_bester.quant_pos[4] = 4;
            the_best_of_the_bester.quant_pos[5] = 2;
            break;

        case 6:
            the_best_of_the_bester.quant_pos[2] = 3;
            the_best_of_the_bester.quant_pos[3] = 0;
            the_best_of_the_bester.quant_pos[4] = 4;
            the_best_of_the_bester.quant_pos[5] = 3;
            break;

        case 7:
            the_best_of_the_bester.quant_pos[2] = 2;
            the_best_of_the_bester.quant_pos[3] = 2;
            the_best_of_the_bester.quant_pos[4] = 3;
            the_best_of_the_bester.quant_pos[5] = 3;
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
    sintese.formacao = 0;
    sintese.capitao = 0;
    for(int i=0; i<3; i++){
        sintese.pref_gol[i] = 0;
        sintese.pref_zag[i] = 0;
        sintese.pref_lat[i] = 0;
        sintese.pref_mei[i] = 0;
        sintese.pref_ata[i] = 0;
    }

    for(int i=0; i<TAM_POP; i++){
       
        printf("----------------------TIME %d-----------------------\n", i+1);
        imprimir_cromossomo(pop[i]);
        
        //fitness é a pontuação média do time em um campeonato
        fitness[i] = get_pontuacao_media(pop[i].quant_pos, pop[i].pref_gol, pop[i].pref_zag, pop[i].pref_lat, pop[i].pref_mei, pop[i].pref_ata, pop[i].capitao);

        //registra maior e menor fitness
        if(fitness[i] > maior){
            maior = fitness[i];
            the_bester = i;
        }
        if(fitness[i] < menor){
            menor = fitness[i];
            the_worst = i;
        }

        fitness_medio += fitness[i];
        
        sintese.formacao += pop[i].formacao;
        sintese.capitao += pop[i].capitao;
        for(int j=0; j<3; j++){
            sintese.pref_gol[j] += pop[i].pref_gol[j];
            sintese.pref_zag[j] += pop[i].pref_zag[j];
            sintese.pref_lat[j] += pop[i].pref_lat[j];
            sintese.pref_mei[j] += pop[i].pref_mei[j];
            sintese.pref_ata[j] += pop[i].pref_ata[j];
        }

        printf("Em média, time %d conseguiu %.2f pontos\n\n", i+1, fitness[i]);

    }

    if(fitness[the_bester] > the_best_of_the_bester_fitness){
        the_best_of_the_bester_fitness = fitness[the_bester];
        copia_the_bester_cromossomo();
    }
    
    fitness_medio /= TAM_POP;
    
    sintese.formacao /= TAM_POP;
    sintese.capitao /= TAM_POP;
    for(int j=0; j<3; j++){
        sintese.pref_gol[j] /= TAM_POP;
        sintese.pref_zag[j] /= TAM_POP;
        sintese.pref_lat[j] /= TAM_POP;
        sintese.pref_mei[j] /= TAM_POP;
        sintese.pref_ata[j] /= TAM_POP;
    }

    verificar_preferencias_iguais_sintese();

    printf("O melhor conseguiu, em média, %f pontos!\n", fitness[the_bester]);

}

//a recebe a copia de b
void copia_the_bester_cromossomo(){
    the_best_of_the_bester.formacao = pop[the_bester].formacao;
    the_best_of_the_bester.capitao = pop[the_bester].capitao;
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

        verificar_preferencias_iguais(i);

    }
}

void crossover(int a, int b){
    //pop[i] = (pop[i] + pop[the_bester]) / 2;    //Crossover(soma e divide por 2)
    
    //Crossover(soma e divide por 2):

    int formacao = media_simples(pop[a].formacao, pop[b].formacao);
    definir_formacao(a, formacao);

    pop[a].capitao = media_simples(pop[a].capitao, pop[b].capitao);
    
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

/*void mutacao(int i){
    //pop[i] = pop[i] + ((rand() % VALOR_MAX - VALOR_MEIO) * taxa_mutacao); //Mutação

    int teste;

    teste = (rand() % LIMITE_PROBABILIDADE)+1;
    if(teste <= (taxa_mutacao*LIMITE_PROBABILIDADE)){
        int formacao = pop[i].formacao + (rand() % 7 - 3); 
        //Ajeitar Valores fora dos limites
        if(formacao < 1) formacao = (-formacao) + 2;
        else if(formacao > 7){
            formacao = 14 - formacao;
            //if(formacao < 0) formacao = (-formacao) % VALOR_MAX;
        }
        definir_formacao(i, formacao);
    }

    int n_gol = get_n_ids_gol(), n_zag = get_n_ids_zag(), n_lat = get_n_ids_lat(), n_mei = get_n_ids_mei(), n_ata = get_n_ids_ata();
    
    for(int j=0; j<3; j++){

        teste = (rand() % LIMITE_PROBABILIDADE)+1;
        if(teste <= (taxa_mutacao*LIMITE_PROBABILIDADE)){
            int mut = pop[i].pref_gol[j] + ((rand() % n_gol) - n_gol/2);
            //Ajeitar Valores fora dos limites
            if(mut < 1) mut = -(mut) + 2;
            else if(mut > (n_gol)){
                mut = 2*(n_gol) - mut;
            }
            pop[i].pref_gol[j] = mut;
        }
        teste = (rand() % LIMITE_PROBABILIDADE)+1;
        if(teste <= (taxa_mutacao*LIMITE_PROBABILIDADE)){
            int mut = pop[i].pref_zag[j] + ((rand() % n_zag) - n_zag/2);
            //Ajeitar Valores fora dos limites
            if(mut < 1) mut = -(mut) + 2;
            else if(mut > (n_zag)){
                mut = 2*(n_zag) - mut;
            }
            pop[i].pref_zag[j] = mut;
        }
        teste = (rand() % LIMITE_PROBABILIDADE)+1;
        if(teste <= (taxa_mutacao*LIMITE_PROBABILIDADE)){
            int mut = pop[i].pref_lat[j] + ((rand() % n_lat) - n_lat/2);
            //Ajeitar Valores fora dos limites
            if(mut < 1) mut = -(mut) + 2;
            else if(mut > (n_lat)){
                mut = 2*(n_lat) - mut;
            }
            pop[i].pref_lat[j] = mut;
        }
        teste = (rand() % LIMITE_PROBABILIDADE)+1;
        if(teste <= (taxa_mutacao*LIMITE_PROBABILIDADE)){
            int mut = pop[i].pref_mei[j] + ((rand() % n_mei) - n_mei/2);
            //Ajeitar Valores fora dos limites
            if(mut < 1) mut = -(mut) + 2;
            else if(mut > (n_mei)){
                mut = 2*(n_mei) - mut;
            }
            pop[i].pref_mei[j] = mut;
        }
        teste = (rand() % LIMITE_PROBABILIDADE)+1;
        if(teste <= (taxa_mutacao*LIMITE_PROBABILIDADE)){
            int mut = pop[i].pref_ata[j] + ((rand() % n_ata) - n_ata/2);
            //Ajeitar Valores fora dos limites
            if(mut < 1) mut = -(mut) + 2;
            else if(mut > (n_ata)){
                mut = 2*(n_ata) - mut;
            }
            pop[i].pref_ata[j] = mut;
        }
    }
}*/


int converte(float num){
    float resto = num - (int)num;
    if(resto<0.5){
        num = (int)num;
    }else{
        num = ((int)num) + 1;        
    }
    return num;
}

void mutacao(int i){
    //Mutação

    int formacao = pop[i].formacao + converte(((rand() % 7 - 3) * taxa_mutacao_for));
    //Ajeitar Valores fora dos limites
    if(formacao < 1) formacao = (-formacao) + 2;
    else if(formacao > 7){
        formacao = 14 - formacao;
        //if(formacao < 0) formacao = (-formacao) % VALOR_MAX;
    }
    definir_formacao(i, formacao);

    int capitao = pop[i].capitao + converte(((rand() % 6 - 2) * taxa_mutacao_for));
    //Ajeitar Valores fora dos limites
    if(capitao < 1) capitao = (-capitao) + 2;
    else if(capitao > 6){
        capitao = 12 - capitao;
    }

    int n_gol = get_n_ids_gol(), n_zag = get_n_ids_zag(), n_lat = get_n_ids_lat(), n_mei = get_n_ids_mei(), n_ata = get_n_ids_ata();
    
    for(int j=0; j<3; j++){

        int mut = pop[i].pref_gol[j] + converte(((rand() % n_gol - n_gol/2) * taxa_mutacao));
        //Ajeitar Valores fora dos limites
        if(mut < 1) mut = -(mut) + 2;
        else if(mut > (n_gol)){
            mut = 2*(n_gol) - mut;
        }
        pop[i].pref_gol[j] = mut;
    
        
        mut = pop[i].pref_zag[j] + converte(((rand() % n_zag - n_zag/2) * taxa_mutacao));
        //Ajeitar Valores fora dos limites
        if(mut < 1) mut = -(mut) + 2;
        else if(mut > (n_zag)){
            mut = 2*(n_zag) - mut;
        }
        pop[i].pref_zag[j] = mut;
    
        
        mut = pop[i].pref_lat[j] + converte(((rand() % n_lat - n_lat/2) * taxa_mutacao));
        //Ajeitar Valores fora dos limites
        if(mut < 1) mut = -(mut) + 2;
        else if(mut > (n_lat)){
            mut = 2*(n_lat) - mut;
        }
        pop[i].pref_lat[j] = mut;
    
    
        mut = pop[i].pref_mei[j] + converte(((rand() % n_mei - n_mei/2) * taxa_mutacao));
        //Ajeitar Valores fora dos limites
        if(mut < 1) mut = -(mut) + 2;
        else if(mut > (n_mei)){
            mut = 2*(n_mei) - mut;
        }
        pop[i].pref_mei[j] = mut;
        
        
        mut = pop[i].pref_ata[j] + converte(((rand() % n_ata - n_ata/2) * taxa_mutacao));
        //Ajeitar Valores fora dos limites
        if(mut < 1) mut = -(mut) + 2;
        else if(mut > (n_ata)){
            mut = 2*(n_ata) - mut;
        }
        pop[i].pref_ata[j] = mut;
        
    }
}


void mutacao_dinamica(){
    if(n_geracao>0 && besters[n_geracao] == besters[n_geracao-1]){
        iguais_consecutivos++;
        if(iguais_consecutivos == N_IGUAIS_CONSECUTIVOS){
            //taxa_mutacao += SOMA_MUTACAO;
            taxa_mutacao *= FATOR_MUTACAO;
            //Taxa de Mutação chega no seu limite máximo:
            if(taxa_mutacao > LIMITE_TAXA_MUT){
                //Reseta:
                taxa_mutacao = TAXA_MUT_INI;
                //Genocídio:
                if(tipo_genocidio == NUM_GEN_TOTAL-1){
                    genocidio_total();
                }else{
                    genocidio_parcial();
                }
                tipo_genocidio = (tipo_genocidio + 1) % NUM_GEN_TOTAL;
            }
            iguais_consecutivos = 0;
        }
    }else{
        taxa_mutacao = TAXA_MUT_INI;
        iguais_consecutivos = 0;
        tipo_genocidio = 0;
    }
}

void mutacao_dinamica_delta(){
    if((fitness[the_bester] - fitness_medio) <= DELTA){
        //taxa_mutacao *= FATOR_MUTACAO;
        taxa_mutacao_for *= FATOR_MUTACAO_FOR;
        //Taxa de Mutação chega no seu limite máximo:
        if(taxa_mutacao_for > LIMITE_TAXA_MUT_FOR){
            taxa_mutacao_for = TAXA_MUT_INI_FOR;
            //Genocídio:
            /*if(tipo_genocidio){
                genocidio_total();
            }else{
                genocidio_parcial();
            }
            tipo_genocidio = (tipo_genocidio + 1) % 2;*/
        }
    }else{
        taxa_mutacao_for = TAXA_MUT_INI_FOR;
    }
}

void criar_grafico_fitness(){
    //char nome[30];
    //printf("Digite o nome do arquivo dos melhores fitness: ");
    //scanf("%s%*c", nome);
    melhor_fitness = fopen("data/melhor_fitness.dat", "w+");
    //printf("Digite o nome do arquivo da média de fitness: ");
    //scanf("%s%*c", nome);
    media_fitness = fopen("data/media_fitness.dat", "w+");

    melhor_fitness_atual = fopen("data/melhor_fitness_atual.dat", "w+");
}

void incrementar_grafico_fitness(){
    //char nome[30];
    //printf("Digite o nome do arquivo dos melhores fitness: ");
    //scanf("%s%*c", nome);
    melhor_fitness = fopen("data/melhor_fitness.dat", "a+");
    //printf("Digite o nome do arquivo da média de fitness: ");
    //scanf("%s%*c", nome);
    media_fitness = fopen("data/media_fitness.dat", "a+");

    melhor_fitness_atual = fopen("data/melhor_fitness_atual.dat", "a+");

}

void atualizar_grafico_fitness(){
    //Vetor de melhores a cada geração
    besters[n_geracao] = fitness[the_bester];
    //besters[n_geracao] = the_best_of_the_bester_fitness;

    //Guarda o melhor de todos no arquivo melhor_fitness.dat
    //fprintf(melhor_fitness, "%d\t%f\n", n_geracao+geracao_total, fitness[the_bester]);
    fprintf(melhor_fitness, "%d\t%f\n", n_geracao+geracao_total, the_best_of_the_bester_fitness);
    
    //Guarda a média de fitness por geração no arquivo media_fitness.dat
    fprintf(media_fitness, "%d\t%f\n", n_geracao+geracao_total, fitness_medio);

    //Guarda o melhor por geração no arquivo melhor_fitness_atual.dat
    fprintf(melhor_fitness_atual, "%d\t%f\n", n_geracao+geracao_total, fitness[the_bester]);
}

void predacao_sintese(){
    //predacao por síntese

    //define a formacao
    definir_formacao(the_worst, sintese.formacao);

    pop[the_worst].capitao = sintese.capitao;

    for(int i=0; i<3; i++){
        pop[the_worst].pref_gol[i] = sintese.pref_gol[i];
        pop[the_worst].pref_zag[i] = sintese.pref_zag[i];
        pop[the_worst].pref_lat[i] = sintese.pref_lat[i];
        pop[the_worst].pref_mei[i] = sintese.pref_mei[i];
        pop[the_worst].pref_ata[i] = sintese.pref_ata[i];
    }
}

void predacao_aleatoria(){
    //predacao aleatoria
    //formacao: entre 1 e 7
    int formacao = (rand() % 7) + 1;

    //define a formacao
    definir_formacao(the_worst, formacao);

    //capitao: entre 1 e 6
    pop[the_worst].capitao = (rand() % 6) + 1;


    //Define, aleatoriamente, as 3 preferências de escalação para cada posição
    free(pop[the_worst].pref_gol);
    free(pop[the_worst].pref_zag);
    free(pop[the_worst].pref_lat);
    free(pop[the_worst].pref_mei);
    free(pop[the_worst].pref_ata);
    pop[the_worst].pref_gol = get_3_aleatorios_diferentes_entre(1, get_n_ids_gol());
    pop[the_worst].pref_zag = get_3_aleatorios_diferentes_entre(1, get_n_ids_zag());
    pop[the_worst].pref_lat = get_3_aleatorios_diferentes_entre(1, get_n_ids_lat());
    pop[the_worst].pref_mei = get_3_aleatorios_diferentes_entre(1, get_n_ids_mei());
    pop[the_worst].pref_ata = get_3_aleatorios_diferentes_entre(1, get_n_ids_ata());
}

void genocidio_total(){
     //Mata o melhor também
    for (int i = 0; i < TAM_POP; i++){

        //formacao: entre 1 e 7
        int formacao = (rand() % 7) + 1;

        //define a formacao
        definir_formacao(i, formacao);

        /*for(int i=0; i<6; i++){
            pop[i].mando_pos[i] = rand() % 2;
        }*/

        //capitao: entre 1 e 6
        pop[i].capitao = (rand() % 6) + 1;

        //Define, aleatoriamente, as 3 preferências de escalação para cada posição
        free(pop[i].pref_gol);
        free(pop[i].pref_zag);
        free(pop[i].pref_lat);
        free(pop[i].pref_mei);
        free(pop[i].pref_ata);
        pop[i].pref_gol = get_3_aleatorios_diferentes_entre(1, get_n_ids_gol());
        pop[i].pref_zag = get_3_aleatorios_diferentes_entre(1, get_n_ids_zag());
        pop[i].pref_lat = get_3_aleatorios_diferentes_entre(1, get_n_ids_lat());
        pop[i].pref_mei = get_3_aleatorios_diferentes_entre(1, get_n_ids_mei());
        pop[i].pref_ata = get_3_aleatorios_diferentes_entre(1, get_n_ids_ata());  
    }
    
}

void genocidio_parcial(){
    //Não mata o melhor
    for (int i = 0; i < TAM_POP; i++){
        
        if(i == the_bester) continue;
        
        //formacao: entre 1 e 7
        int formacao = (rand() % 7) + 1;

        //define a formacao
        definir_formacao(i, formacao);

        /*for(int i=0; i<6; i++){
            pop[i].mando_pos[i] = rand() % 2;
        }*/

        //capitao: entre 1 e 6
        pop[i].capitao = (rand() % 6) + 1;

        //Define, aleatoriamente, as 3 preferências de escalação para cada posição
        free(pop[i].pref_gol);
        free(pop[i].pref_zag);
        free(pop[i].pref_lat);
        free(pop[i].pref_mei);
        free(pop[i].pref_ata);
        pop[i].pref_gol = get_3_aleatorios_diferentes_entre(1, get_n_ids_gol());
        pop[i].pref_zag = get_3_aleatorios_diferentes_entre(1, get_n_ids_zag());
        pop[i].pref_lat = get_3_aleatorios_diferentes_entre(1, get_n_ids_lat());
        pop[i].pref_mei = get_3_aleatorios_diferentes_entre(1, get_n_ids_mei());
        pop[i].pref_ata = get_3_aleatorios_diferentes_entre(1, get_n_ids_ata());  
    }
}

void predacao(){
    //Predação a cada x gerações alternando o tipo de predação:
    if(n_geracao % GERACOES_PREDACAO == 0){
        if(tipo_predacao){
            predacao_sintese();
        }else{
            predacao_aleatoria();
        }
        tipo_predacao = (tipo_predacao + 1) % 2;
    }
}

void imprimir_melhor_cromossomo(){
    imprimir_cromossomo(the_best_of_the_bester);
}

void imprimir_cromossomo(Cromossomo c){
    printf("*** Formacao: %d-%d-%d ***\n", c.quant_pos[2] + c.quant_pos[3], c.quant_pos[4], c.quant_pos[5]);
    printf("*** Capitão: %d ***\n", c.capitao);
    printf("*** Pref. Goleiro: %d, %d e %d ***\n", c.pref_gol[0], c.pref_gol[1], c.pref_gol[2]);
    printf("*** Pref. Zagueiro: %d, %d e %d ***\n", c.pref_zag[0], c.pref_zag[1], c.pref_zag[2]);
    printf("*** Pref. Lateral: %d, %d e %d ***\n", c.pref_lat[0], c.pref_lat[1], c.pref_lat[2]);
    printf("*** Pref. Meia: %d, %d e %d ***\n", c.pref_mei[0], c.pref_mei[1], c.pref_mei[2]);
    printf("*** Pref. Atacante: %d, %d e %d ***\n", c.pref_ata[0], c.pref_ata[1], c.pref_ata[2]);
}

float get_fitness_melhor(){
    return the_best_of_the_bester_fitness;
}

void armazenar_dados_ag(){

    //armazeno, na sequencia:
    /*
        Cromossomo the_best_of_the_bester;
        float the_best_of_the_bester_fitness;
        Cromossomo pop[TAM_POP];
        int geracao_total;
        int iguais_consecutivos;
        double taxa_mutacao;
        int tipo_predacao;
        int tipo_genocidio;
    */

    backup = fopen("data/backup.txt", "w+");

    fprintf(backup, "%d\n", the_best_of_the_bester.formacao);
    fprintf(backup, "%d\n", the_best_of_the_bester.capitao);
    for(int i=0; i<3; i++){
        fprintf(backup, "%d\n", the_best_of_the_bester.pref_gol[i]);
        fprintf(backup, "%d\n", the_best_of_the_bester.pref_zag[i]);
        fprintf(backup, "%d\n", the_best_of_the_bester.pref_lat[i]);
        fprintf(backup, "%d\n", the_best_of_the_bester.pref_mei[i]);
        fprintf(backup, "%d\n", the_best_of_the_bester.pref_ata[i]);
    }

    fprintf(backup, "%f\n", the_best_of_the_bester_fitness);

    for(int j=0; j<TAM_POP; j++){
        fprintf(backup, "%d\n", pop[j].formacao);
        fprintf(backup, "%d\n", pop[j].capitao);
        for(int i=0; i<3; i++){
            fprintf(backup, "%d\n", pop[j].pref_gol[i]);
            fprintf(backup, "%d\n", pop[j].pref_zag[i]);
            fprintf(backup, "%d\n", pop[j].pref_lat[i]);
            fprintf(backup, "%d\n", pop[j].pref_mei[i]);
            fprintf(backup, "%d\n", pop[j].pref_ata[i]);
        }
    }

    geracao_total += n_geracao;
    fprintf(backup, "%d\n", geracao_total);
    fprintf(backup, "%d\n", iguais_consecutivos);
    fprintf(backup, "%lf\n", taxa_mutacao);
    fprintf(backup, "%lf\n", taxa_mutacao_for);
    fprintf(backup, "%d\n", tipo_predacao);
    fprintf(backup, "%d\n", tipo_genocidio);

}

void recuperar_dados_ag(){

    backup = fopen("data/backup.txt", "r+");

    fscanf(backup, "%d%*c", &the_best_of_the_bester.formacao);
    definir_formacao_bester(the_best_of_the_bester.formacao);
    fscanf(backup, "%d%*c", &the_best_of_the_bester.capitao);
    the_best_of_the_bester.pref_gol = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_zag = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_lat = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_mei = (int *) malloc(3 * sizeof(int));
    the_best_of_the_bester.pref_ata = (int *) malloc(3 * sizeof(int));
    for(int i=0; i<3; i++){
        fscanf(backup, "%d%*c", &(the_best_of_the_bester.pref_gol[i]));
        fscanf(backup, "%d%*c", &(the_best_of_the_bester.pref_zag[i]));
        fscanf(backup, "%d%*c", &(the_best_of_the_bester.pref_lat[i]));
        fscanf(backup, "%d%*c", &(the_best_of_the_bester.pref_mei[i]));
        fscanf(backup, "%d%*c", &(the_best_of_the_bester.pref_ata[i]));
    }

    fscanf(backup, "%f%*c", &the_best_of_the_bester_fitness);

    for(int j=0; j<TAM_POP; j++){
        fscanf(backup, "%d%*c", &(pop[j].formacao));
        definir_formacao(j, pop[j].formacao);
        fscanf(backup, "%d%*c", &(pop[j].capitao));
        pop[j].pref_gol = (int *) malloc(3 * sizeof(int));
        pop[j].pref_zag = (int *) malloc(3 * sizeof(int));
        pop[j].pref_lat = (int *) malloc(3 * sizeof(int));
        pop[j].pref_mei = (int *) malloc(3 * sizeof(int));
        pop[j].pref_ata = (int *) malloc(3 * sizeof(int));
        for(int i=0; i<3; i++){
            fscanf(backup, "%d%*c", &(pop[j].pref_gol[i]));
            fscanf(backup, "%d%*c", &(pop[j].pref_zag[i]));
            fscanf(backup, "%d%*c", &(pop[j].pref_lat[i]));
            fscanf(backup, "%d%*c", &(pop[j].pref_mei[i]));
            fscanf(backup, "%d%*c", &(pop[j].pref_ata[i]));
        }
    }

    fscanf(backup, "%d%*c", &geracao_total);
    fscanf(backup, "%d%*c", &iguais_consecutivos);
    fscanf(backup, "%lf%*c", &taxa_mutacao);
    fscanf(backup, "%lf%*c", &taxa_mutacao_for);
    fscanf(backup, "%d%*c", &tipo_predacao);
    fscanf(backup, "%d%*c", &tipo_genocidio);

    fclose(backup);

    n_geracao = 0;
    
    sintese.pref_gol = (int *) malloc(3 * sizeof(int));
    sintese.pref_zag = (int *) malloc(3 * sizeof(int));
    sintese.pref_lat = (int *) malloc(3 * sizeof(int));
    sintese.pref_mei = (int *) malloc(3 * sizeof(int));
    sintese.pref_ata = (int *) malloc(3 * sizeof(int));

}

void verificar_preferencias_iguais(int i){

    int r, fator;

    int n_gol = get_n_ids_gol(), n_zag = get_n_ids_zag(), n_lat = get_n_ids_lat(), n_mei = get_n_ids_mei(), n_ata = get_n_ids_ata();

    if(pop[i].pref_gol[1] == pop[i].pref_gol[0]){
        r = rand()%2;
        if(r){
            pop[i].pref_gol[1] += 1;
            if(pop[i].pref_gol[1] > n_gol){
                pop[i].pref_gol[1] = 2*(n_gol) - pop[i].pref_gol[1];
            }
        }else{
            pop[i].pref_gol[1] -= 1;
            if(pop[i].pref_gol[1] < 1){
                pop[i].pref_gol[1] = -(pop[i].pref_gol[1]) + 2;
            }
        }
    }

    fator = 1;
    while(pop[i].pref_gol[2] == pop[i].pref_gol[1] || pop[i].pref_gol[2] == pop[i].pref_gol[0]){
        pop[i].pref_gol[2] += fator;
        if(pop[i].pref_gol[2] > n_gol){
            pop[i].pref_gol[2] -= 1;
            fator *= -1;
        }
    }

    if(pop[i].pref_zag[1] == pop[i].pref_zag[0]){
        r = rand()%2;
        if(r){
            pop[i].pref_zag[1] += 1;
            if(pop[i].pref_zag[1] > n_zag){
                pop[i].pref_zag[1] = 2*(n_zag) - pop[i].pref_zag[1];
            }
        }else{
            pop[i].pref_zag[1] -= 1;
            if(pop[i].pref_zag[1] < 1){
                pop[i].pref_zag[1] = -(pop[i].pref_zag[1]) + 2;
            }
        }
    }

    fator = 1;
    while(pop[i].pref_zag[2] == pop[i].pref_zag[1] || pop[i].pref_zag[2] == pop[i].pref_zag[0]){
        pop[i].pref_zag[2] += fator;
        if(pop[i].pref_zag[2] > n_zag){
            pop[i].pref_zag[2] -= 1;
            fator *= -1;
        }
    }

    if(pop[i].pref_lat[1] == pop[i].pref_lat[0]){
        r = rand()%2;
        if(r){
            pop[i].pref_lat[1] += 1;
            if(pop[i].pref_lat[1] > n_lat){
                pop[i].pref_lat[1] = 2*(n_lat) - pop[i].pref_lat[1];
            }
        }else{
            pop[i].pref_lat[1] -= 1;
            if(pop[i].pref_lat[1] < 1){
                pop[i].pref_lat[1] = -(pop[i].pref_lat[1]) + 2;
            }
        }
    }

    fator = 1;
    while(pop[i].pref_lat[2] == pop[i].pref_lat[1] || pop[i].pref_lat[2] == pop[i].pref_lat[0]){
        pop[i].pref_lat[2] += fator;
        if(pop[i].pref_lat[2] > n_lat){
            pop[i].pref_lat[2] -= 1;
            fator *= -1;
        }
    }

    if(pop[i].pref_mei[1] == pop[i].pref_mei[0]){
        r = rand()%2;
        if(r){
            pop[i].pref_mei[1] += 1;
            if(pop[i].pref_mei[1] > n_mei){
                pop[i].pref_mei[1] = 2*(n_mei) - pop[i].pref_mei[1];
            }
        }else{
            pop[i].pref_mei[1] -= 1;
            if(pop[i].pref_mei[1] < 1){
                pop[i].pref_mei[1] = -(pop[i].pref_mei[1]) + 2;
            }
        }
    }

    fator = 1;
    while(pop[i].pref_mei[2] == pop[i].pref_mei[1] || pop[i].pref_mei[2] == pop[i].pref_mei[0]){
        pop[i].pref_mei[2] += fator;
        if(pop[i].pref_mei[2] > n_mei){
            pop[i].pref_mei[2] -= 1;
            fator *= -1;
        }
    }

    if(pop[i].pref_ata[1] == pop[i].pref_ata[0]){
        r = rand()%2;
        if(r){
            pop[i].pref_ata[1] += 1;
            if(pop[i].pref_ata[1] > n_ata){
                pop[i].pref_ata[1] = 2*(n_ata) - pop[i].pref_ata[1];
            }
        }else{
            pop[i].pref_ata[1] -= 1;
            if(pop[i].pref_ata[1] < 1){
                pop[i].pref_ata[1] = -(pop[i].pref_ata[1]) + 2;
            }
        }
    }

    fator = 1;
    while(pop[i].pref_ata[2] == pop[i].pref_ata[1] || pop[i].pref_ata[2] == pop[i].pref_ata[0]){
        pop[i].pref_ata[2] += fator;
        if(pop[i].pref_ata[2] > n_ata){
            pop[i].pref_ata[2] -= 1;
            fator *= -1;
        }
    }

}

void verificar_preferencias_iguais_sintese(){

    int r, fator;

    int n_gol = get_n_ids_gol(), n_zag = get_n_ids_zag(), n_lat = get_n_ids_lat(), n_mei = get_n_ids_mei(), n_ata = get_n_ids_ata();

    if(sintese.pref_gol[1] == sintese.pref_gol[0]){
        r = rand()%2;
        if(r){
            sintese.pref_gol[1] += 1;
            if(sintese.pref_gol[1] > n_gol){
                sintese.pref_gol[1] = 2*(n_gol) - sintese.pref_gol[1];
            }
        }else{
            sintese.pref_gol[1] -= 1;
            if(sintese.pref_gol[1] < 1){
                sintese.pref_gol[1] = -(sintese.pref_gol[1]) + 2;
            }
        }
    }

    fator = 1;
    while(sintese.pref_gol[2] == sintese.pref_gol[1] || sintese.pref_gol[2] == sintese.pref_gol[0]){
        sintese.pref_gol[2] += fator;
        if(sintese.pref_gol[2] > n_gol){
            sintese.pref_gol[2] -= 1;
            fator *= -1;
        }
    }

    if(sintese.pref_zag[1] == sintese.pref_zag[0]){
        r = rand()%2;
        if(r){
            sintese.pref_zag[1] += 1;
            if(sintese.pref_zag[1] > n_zag){
                sintese.pref_zag[1] = 2*(n_zag) - sintese.pref_zag[1];
            }
        }else{
            sintese.pref_zag[1] -= 1;
            if(sintese.pref_zag[1] < 1){
                sintese.pref_zag[1] = -(sintese.pref_zag[1]) + 2;
            }
        }
    }

    fator = 1;
    while(sintese.pref_zag[2] == sintese.pref_zag[1] || sintese.pref_zag[2] == sintese.pref_zag[0]){
        sintese.pref_zag[2] += fator;
        if(sintese.pref_zag[2] > n_zag){
            sintese.pref_zag[2] -= 1;
            fator *= -1;
        }
    }

    if(sintese.pref_lat[1] == sintese.pref_lat[0]){
        r = rand()%2;
        if(r){
            sintese.pref_lat[1] += 1;
            if(sintese.pref_lat[1] > n_lat){
                sintese.pref_lat[1] = 2*(n_lat) - sintese.pref_lat[1];
            }
        }else{
            sintese.pref_lat[1] -= 1;
            if(sintese.pref_lat[1] < 1){
                sintese.pref_lat[1] = -(sintese.pref_lat[1]) + 2;
            }
        }
    }

    fator = 1;
    while(sintese.pref_lat[2] == sintese.pref_lat[1] || sintese.pref_lat[2] == sintese.pref_lat[0]){
        sintese.pref_lat[2] += fator;
        if(sintese.pref_lat[2] > n_lat){
            sintese.pref_lat[2] -= 1;
            fator *= -1;
        }
    }

    if(sintese.pref_mei[1] == sintese.pref_mei[0]){
        r = rand()%2;
        if(r){
            sintese.pref_mei[1] += 1;
            if(sintese.pref_mei[1] > n_mei){
                sintese.pref_mei[1] = 2*(n_mei) - sintese.pref_mei[1];
            }
        }else{
            sintese.pref_mei[1] -= 1;
            if(sintese.pref_mei[1] < 1){
                sintese.pref_mei[1] = -(sintese.pref_mei[1]) + 2;
            }
        }
    }

    fator = 1;
    while(sintese.pref_mei[2] == sintese.pref_mei[1] || sintese.pref_mei[2] == sintese.pref_mei[0]){
        sintese.pref_mei[2] += fator;
        if(sintese.pref_mei[2] > n_mei){
            sintese.pref_mei[2] -= 1;
            fator *= -1;
        }
    }

    if(sintese.pref_ata[1] == sintese.pref_ata[0]){
        r = rand()%2;
        if(r){
            sintese.pref_ata[1] += 1;
            if(sintese.pref_ata[1] > n_ata){
                sintese.pref_ata[1] = 2*(n_ata) - sintese.pref_ata[1];
            }
        }else{
            sintese.pref_ata[1] -= 1;
            if(sintese.pref_ata[1] < 1){
                sintese.pref_ata[1] = -(sintese.pref_ata[1]) + 2;
            }
        }
    }

    fator = 1;
    while(sintese.pref_ata[2] == sintese.pref_ata[1] || sintese.pref_ata[2] == sintese.pref_ata[0]){
        sintese.pref_ata[2] += fator;
        if(sintese.pref_ata[2] > n_ata){
            sintese.pref_ata[2] -= 1;
            fator *= -1;
        }
    }

}