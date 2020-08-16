#include "cartola_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct rodada{
    int id;
    Atleta **gol;
    int n_gol;
    Atleta **lat;
    int n_lat;
    Atleta **zag;
    int n_zag;
    Atleta **mei;
    int n_mei;
    Atleta **ata;
    int n_ata;
    Atleta **tec;
    int n_tec;
};

struct atleta{
    char nome[100];
    char apelido[50];
    int id_atleta;
    int id_clube; //tem os ids dos clubes
    char nome_clube[50];
    char posicao[4];
    //Os seguintes dados são vetores de MAX_RODADAS posições pois representam valores entre as rodadas 0 e 38;
    //OBS: rodada 0 são os dados iniciais dos atletas antes da primeira rodada
    /*
    Tipos de status:
    'P' -> provável
    'C' -> contundido
    'S' -> suspenso
    */
    char status[MAX_RODADAS];
    int jogou_rodada[MAX_RODADAS]; //vetor de "boolean" para indicar se o atleta participou da rodada x ou não
    float pontos[MAX_RODADAS];
    float preco[MAX_RODADAS];
    float valorizacao[MAX_RODADAS];
    float media[MAX_RODADAS];
    //dados do jogo(acumulativos com as rodadas):
    //obs: no csv daqui pra frente pode estar em qualquer ordem
    int fs[MAX_RODADAS]; //faltas sofridas: +0.5
    int rb[MAX_RODADAS]; //roubadas de bola: +1.5
    int pe[MAX_RODADAS]; //passes errados: -0.3
    int fc[MAX_RODADAS]; //faltas cometidas: -0.5 
    int g[MAX_RODADAS]; //gols: +8.0
    int ff[MAX_RODADAS]; //finalizações para fora: +0.8
    int ft[MAX_RODADAS]; //finalizações na trave: +3.0
    int fd[MAX_RODADAS]; //finalizações defendidas: +1.2
    int dd[MAX_RODADAS]; //defesas difíceis: +3.0
    int gs[MAX_RODADAS]; //gols sofridos: -2.0
    int sg[MAX_RODADAS]; //jogos sem sofrer gol: +5.0
    int a[MAX_RODADAS]; //assistências: +5.0
    int ca[MAX_RODADAS]; //cartões amarelos: -2.0
    int i[MAX_RODADAS]; //impedimentos: -0.5
    int cv[MAX_RODADAS]; //cartões vermelhos: -5.0
    int pp[MAX_RODADAS]; //pênatils perdidos: -4.0
    int gc[MAX_RODADAS]; //gols contra: -5.0
    int dp[MAX_RODADAS]; //defesa de pênalti: +7.0

    int tec_qtd;
};

Rodada *rodadas18; //Vetor de rodadas do campeonato
Atleta **atletas18; //Vetor com todos os atletas do jogo
int n_atletas = 0;
int id_atleta_atual = -1;

Rodada *rodadas19; //Vetor de rodadas do campeonato
Atleta **atletas19; //Vetor com todos os atletas do jogo

Rodada *rodadas20; //Vetor de rodadas do campeonato
Atleta **atletas20; //Vetor com todos os atletas do jogo

#define MAX_ID_TIMES 500

struct time_escolhido{
    int id_time;
    int qtd;
};

#define MENOR_ID 0
#define MAIOR_ID 19
#define TAM_TIMES 20

float get_pontuacao_media(int quant_pos[6], int pref_gol[3], int pref_zag[3], int pref_lat[3], int pref_mei[3], int pref_ata[3], int capitao){

    float pontuacao_acumulada = 0;
    int total_rodadas = 0;

    pontuacao_acumulada += escala_campeonato(rodadas18, atletas18, MAX_RODADAS, quant_pos, pref_gol, pref_zag, pref_lat, pref_mei, pref_ata, capitao);
    total_rodadas += MAX_RODADAS-2; //tirando a primeira rodada

    pontuacao_acumulada += escala_campeonato(rodadas19, atletas19, MAX_RODADAS, quant_pos, pref_gol, pref_zag, pref_lat, pref_mei, pref_ata, capitao);
    total_rodadas += MAX_RODADAS-2;

    return (pontuacao_acumulada / total_rodadas);

}

float escala_campeonato(Rodada *rodadas, Atleta **atletas, int n_rodadas, int quant_pos[6], int pref_gol[3], int pref_zag[3], int pref_lat[3], int pref_mei[3], int pref_ata[3], int capitao){

    float cartoletas = 100, valorizacao = 0, cart_atual, pontuacao_acumulada = 0;
    int faltam;

    Atleta **copia_gol, **copia_zag, **copia_lat, **copia_mei, **copia_ata, **copia_tec;

    TimeEscolhido *times;

    //para cada rodada do campeonato...
    //pulando a primeira...
    for(int i=2; i<n_rodadas; i++){

        times = (TimeEscolhido *) calloc(TAM_TIMES, sizeof(TimeEscolhido));

        for(int j=0; j<rodadas[i-1].n_tec; j++){
            times[rodadas[i-1].tec[j]->id_clube].id_time = rodadas[i-1].tec[j]->id_clube;
        }

        //Escala um time e registra a pontuação na rodada
       
        //PARA O TÉCNICO, ESCOLHE O TECNICO DO TIME COM MAIS JOGADORES ESCALADOS

        faltam = 12;
        valorizacao = 0;
        cart_atual = cartoletas;

        printf("\t*****rodada %d:*****\n", i);    

        copia_gol = get_copia_goleiros_rodada(i, rodadas);
        printf("  (goleiro):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[1], rodadas[i].n_gol, copia_gol, i, get_comparador_gol(pref_gol[0]), get_comparador_gol(pref_gol[1]), get_comparador_gol(pref_gol[2]), &cartoletas, &faltam, &valorizacao, (capitao==1), times);
        free(copia_gol);

        copia_zag = get_copia_zagueiros_rodada(i, rodadas);
        printf("  (zagueiros):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[2], rodadas[i].n_zag, copia_zag, i, get_comparador_zag(pref_zag[0]), get_comparador_zag(pref_zag[1]), get_comparador_zag(pref_zag[2]), &cartoletas, &faltam, &valorizacao, (capitao==2), times);
        free(copia_zag);

        copia_lat = get_copia_laterais_rodada(i, rodadas);
        printf("  (laterais):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[3], rodadas[i].n_lat, copia_lat, i, get_comparador_lat(pref_lat[0]), get_comparador_lat(pref_lat[1]), get_comparador_lat(pref_lat[2]), &cartoletas, &faltam, &valorizacao, (capitao==3), times);
        free(copia_lat);

        copia_mei = get_copia_meias_rodada(i, rodadas);
        printf("  (meias):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[4], rodadas[i].n_mei, copia_mei, i, get_comparador_mei(pref_mei[0]), get_comparador_mei(pref_mei[1]), get_comparador_mei(pref_mei[2]), &cartoletas, &faltam, &valorizacao, (capitao==4), times);
        free(copia_mei);
        
        copia_ata = get_copia_atacantes_rodada(i, rodadas);
        printf("  (atacantes):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[5], rodadas[i].n_ata, copia_ata, i, get_comparador_ata(pref_ata[0]), get_comparador_ata(pref_ata[1]), get_comparador_ata(pref_ata[2]), &cartoletas, &faltam, &valorizacao, (capitao==5), times);     
        free(copia_ata);

        copia_tec = get_copia_tecnicos_rodada(i, rodadas);
        printf("  (técnico):\n");
        pontuacao_acumulada += escolhe_tecnico_rodada(rodadas[i].n_tec, copia_tec, i, &cartoletas, &valorizacao, times);
        free(copia_tec);

        cartoletas = cart_atual + valorizacao;

        printf("\n");

        free(times);

    }

    printf("O time terminou com %.2f cartoletas e pontuou, em média, %.2f pontos\n\n", cartoletas, pontuacao_acumulada/(n_rodadas-2));

    return pontuacao_acumulada;

}

void escala_campeonato_rodada(int rodada, float cartoletas, int quant_pos[6], int pref_gol[3], int pref_zag[3], int pref_lat[3], int pref_mei[3], int pref_ata[3], int capitao){

    int faltam;
    float valorizacao = 0;

    Atleta **copia_gol, **copia_zag, **copia_lat, **copia_mei, **copia_ata, **copia_tec;

    TimeEscolhido *times;

    //para cada rodada do campeonato...
    //pulando a primeira...

    times = (TimeEscolhido *) calloc(TAM_TIMES, sizeof(TimeEscolhido));

    for(int j=0; j<rodadas20[rodada].n_tec; j++){
        times[rodadas20[rodada].tec[j]->id_clube].id_time = rodadas20[rodada].tec[j]->id_clube;
    }

    //Escala um time e registra a pontuação na rodada
    
    //PARA O TÉCNICO, ESCOLHE O TECNICO DO TIME COM MAIS JOGADORES ESCALADOS

    faltam = 12;

    printf("\t*****rodada %d:*****\n", rodada+1);    

    copia_gol = get_copia_goleiros_rodada(rodada, rodadas20);
    printf("  (goleiro):\n");
    escolhe_atletas_rodada_2(quant_pos[1], rodadas20[rodada].n_gol, copia_gol, rodada, get_comparador_gol(pref_gol[0]), get_comparador_gol(pref_gol[1]), get_comparador_gol(pref_gol[2]), &cartoletas, &faltam, &valorizacao, (capitao==1), times);
    free(copia_gol);

    copia_zag = get_copia_zagueiros_rodada(rodada, rodadas20);
    printf("  (zagueiros):\n");
    escolhe_atletas_rodada_2(quant_pos[2], rodadas20[rodada].n_zag, copia_zag, rodada, get_comparador_zag(pref_zag[0]), get_comparador_zag(pref_zag[1]), get_comparador_zag(pref_zag[2]), &cartoletas, &faltam, &valorizacao, (capitao==2), times);
    free(copia_zag);

    copia_lat = get_copia_laterais_rodada(rodada, rodadas20);
    printf("  (laterais):\n");
    escolhe_atletas_rodada_2(quant_pos[3], rodadas20[rodada].n_lat, copia_lat, rodada, get_comparador_lat(pref_lat[0]), get_comparador_lat(pref_lat[1]), get_comparador_lat(pref_lat[2]), &cartoletas, &faltam, &valorizacao, (capitao==3), times);
    free(copia_lat);

    copia_mei = get_copia_meias_rodada(rodada, rodadas20);
    printf("  (meias):\n");
    escolhe_atletas_rodada_2(quant_pos[4], rodadas20[rodada].n_mei, copia_mei, rodada, get_comparador_mei(pref_mei[0]), get_comparador_mei(pref_mei[1]), get_comparador_mei(pref_mei[2]), &cartoletas, &faltam, &valorizacao, (capitao==4), times);
    free(copia_mei);
    
    copia_ata = get_copia_atacantes_rodada(rodada, rodadas20);
    printf("  (atacantes):\n");
    escolhe_atletas_rodada_2(quant_pos[5], rodadas20[rodada].n_ata, copia_ata, rodada, get_comparador_ata(pref_ata[0]), get_comparador_ata(pref_ata[1]), get_comparador_ata(pref_ata[2]), &cartoletas, &faltam, &valorizacao, (capitao==5), times);     
    free(copia_ata);

    copia_tec = get_copia_tecnicos_rodada(rodada, rodadas20);
    printf("  (técnico):\n");
    escolhe_tecnico_rodada_2(rodadas20[rodada].n_tec, copia_tec, rodada, &cartoletas, &valorizacao, times);
    free(copia_tec);

    printf("\n");

    free(times);

}

float escolhe_atletas_rodada(int qtd_atl, int total, Atleta **atletas, int rodada, 
    int (*comparador1)(Atleta *, Atleta *, int), 
    int (*comparador2)(Atleta *, Atleta *, int), 
    int (*comparador3)(Atleta *, Atleta *, int),
    float *cartoletas, int *faltam, float *valorizacao, int eh_capitao, TimeEscolhido *times){

    if(qtd_atl == 0){
        printf("Sem atletas desta posição!\n");
        return 0;
    }

    int (*comparador)(Atleta *, Atleta *, int) = comparador1;

    ordena_vetor_rodada(atletas, total, comparador, rodada-1);

    float cartoletasDisponiveis;

    int i, j, k, l, m;

    //Neste momento, está ordenado com a preferência 1.
    //Caso necessário, vamos desempatar com a preferência 2
    i=0;
    while(i<(total-1)){

        j=i;

        comparador = comparador1;
        while(i<(total-1) && (!comparador(atletas[i], atletas[i+1], rodada))){
            //testa com o comparador: se o teste falhar, só pode significar empate.
            i++;
        }

        m=i;

        if(j!=m){
            //desempate dos iguais
            comparador = comparador2;
            merge_sort(atletas, j, m, comparador, rodada);
            while(j<m){
                
                k = j;

                comparador = comparador2;
                while(j<m && (!comparador(atletas[j], atletas[j+1], rodada))){
                    //testa com o comparador: se o teste falhar, só pode significar empate.
                    j++;
                }

                l = j;

                if(k!=l){
                    //desempate dos iguais
                    comparador = comparador3;
                    merge_sort(atletas, k, l, comparador, rodada);
                }

                j++;

            }
        }

        i++;
    }

    int ja_escolhidos=0;
    float pontuacao = 0, preco;
    cartoletasDisponiveis = (*cartoletas) / (*faltam);
    i=0;
    while(i<(total-1) && ja_escolhidos<qtd_atl){
        
        preco = atletas[i]->preco[rodada-1];

        if(preco <= cartoletasDisponiveis){
            //Pode comprar o jogador       
            imprimir_atleta_rodada(atletas[i], rodada);
            
            times[atletas[i]->id_clube].qtd += 1;

            (*cartoletas) = (*cartoletas) - preco;
            (*faltam) = (*faltam) - 1;
            if(*faltam != 0){
                cartoletasDisponiveis = (*cartoletas) / (*faltam);
            }
            (*valorizacao) = (*valorizacao) + atletas[i]->valorizacao[rodada];

            if(eh_capitao && ja_escolhidos == 0){
                pontuacao += ((atletas[i])->media[rodada] * 2);
            }else{
                pontuacao += (atletas[i])->media[rodada];
            }
            
            ja_escolhidos++;
        }

        i++;

    }

    return pontuacao;

}

float escolhe_tecnico_rodada(int total, Atleta **atletas, int rodada, float *cartoletas, float *valorizacao, TimeEscolhido *times){

    merge_sort_2(times, 0, TAM_TIMES-1, compara_time_mais_escalado);

    int i=0, j=0, ja_escolhidos=0;
    float preco, pontuacao=0;
    while(ja_escolhidos<1 && i<(TAM_TIMES)){ 

        j=0;
        while(ja_escolhidos<1 && j<(total-1)){
                
            if(times[i].id_time == atletas[j]->id_clube){
            
                preco = atletas[j]->preco[rodada-1];

                if(preco <= (*cartoletas)){

                    //Pode comprar o jogador       
                    imprimir_atleta_rodada(atletas[j], rodada);

                    (*cartoletas) = (*cartoletas) - preco;

                    (*valorizacao) = (*valorizacao) + atletas[j]->valorizacao[rodada];
                    
                    pontuacao += (atletas[j])->media[rodada];
                    
                    ja_escolhidos++;

                }else{
                    //Tenta outro time
                    j=(total-1);
                }
                
            }else{

                j++;
            
            }      

        }

        i++;

    }

    return pontuacao;

}

void escolhe_atletas_rodada_2(int qtd_atl, int total, Atleta **atletas, int rodada, 
    int (*comparador1)(Atleta *, Atleta *, int), 
    int (*comparador2)(Atleta *, Atleta *, int), 
    int (*comparador3)(Atleta *, Atleta *, int),
    float *cartoletas, int *faltam, float *valorizacao, int eh_capitao, TimeEscolhido *times){

    if(qtd_atl == 0){
        printf("Sem atletas desta posição!\n");
        return;
    }

    int (*comparador)(Atleta *, Atleta *, int) = comparador1;

    ordena_vetor_rodada(atletas, total, comparador, rodada);

    float cartoletasDisponiveis;

    int i, j, k, l, m;

    //Neste momento, está ordenado com a preferência 1.
    //Caso necessário, vamos desempatar com a preferência 2
    i=0;
    while(i<(total-1)){

        j=i;

        comparador = comparador1;
        while(i<(total-1) && (!comparador(atletas[i], atletas[i+1], rodada))){
            //testa com o comparador: se o teste falhar, só pode significar empate.
            i++;
        }

        m=i;

        if(j!=m){
            //desempate dos iguais
            comparador = comparador2;
            merge_sort(atletas, j, m, comparador, rodada);
            while(j<m){
                
                k = j;

                comparador = comparador2;
                while(j<m && (!comparador(atletas[j], atletas[j+1], rodada))){
                    //testa com o comparador: se o teste falhar, só pode significar empate.
                    j++;
                }

                l = j;

                if(k!=l){
                    //desempate dos iguais
                    comparador = comparador3;
                    merge_sort(atletas, k, l, comparador, rodada);
                }

                j++;

            }
        }

        i++;
    }
    
    /*for(int i=0; i<total; i++){
        printf("Nome: %s / ", atletas[i]->apelido);
        printf("Valorização: %.2f;", atletas[i]->valorizacao[rodada]);
        printf("CA: %d;", atletas[i]->ca[rodada]);
        printf("PI: %d;", atletas[i]->pe[rodada]);
        printf("\n"); 
    }
    printf("\n");*/

    int ja_escolhidos=0;
    float preco;
    cartoletasDisponiveis = (*cartoletas) / (*faltam);
    i=0;
    while(i<(total-1) && ja_escolhidos<qtd_atl){
        
        preco = atletas[i]->preco[rodada];

        if(preco <= cartoletasDisponiveis){
            //Pode comprar o jogador       
            imprimir_atleta_rodada(atletas[i], rodada);
            
            times[atletas[i]->id_clube].qtd += 1;

            (*cartoletas) = (*cartoletas) - preco;
            (*faltam) = (*faltam) - 1;
            if(*faltam != 0){
                cartoletasDisponiveis = (*cartoletas) / (*faltam);
            }
            
            ja_escolhidos++;
        }

        i++;

    }
}

void escolhe_tecnico_rodada_2(int total, Atleta **atletas, int rodada, float *cartoletas, float *valorizacao, TimeEscolhido *times){

    merge_sort_2(times, 0, TAM_TIMES-1, compara_time_mais_escalado);

    int i=0, j=0, ja_escolhidos=0;
    float preco;
    while(ja_escolhidos<1 && i<(TAM_TIMES)){ 

        j=0;
        while(ja_escolhidos<1 && j<(total-1)){
                
            if(times[i].id_time == atletas[j]->id_clube){
            
                preco = atletas[j]->preco[rodada];

                if(preco <= (*cartoletas)){

                    //Pode comprar o jogador       
                    imprimir_atleta_rodada(atletas[j], rodada);

                    (*cartoletas) = (*cartoletas) - preco;
                    
                    ja_escolhidos++;

                }else{
                    //Tenta outro time
                    j=(total-1);
                }
                
            }else{

                j++;
            
            }      

        }

        i++;

    }

}

int compara_time_mais_escalado(TimeEscolhido a, TimeEscolhido b){
    return (a.qtd > b.qtd);
}


int ler_csv_campeonato(){

    //char nome_csv[100];

    //----------------------- 2018 --------------------------

    //printf("Nome do arquivo .csv com dados dos atletas: ");
    //scanf("%s%*c", nome_csv);
    //printf("%s\n", nome_csv);

    FILE *csv = fopen("src/manipulacao_dados/2018/dados_2018.csv", "r");
    
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    //inicializa todas as rodadas
    rodadas18 = (Rodada *) calloc(MAX_RODADAS, sizeof(Rodada));
    //LEMBRA DE LIMPAR DEPOIS
    
    for(int i=0; i<MAX_RODADAS; i++){
        rodadas18[i].id = i;
        rodadas18[i].tec = (Atleta **) malloc(MAX_TECNICOS * sizeof(Atleta*));
        rodadas18[i].gol = (Atleta **) malloc(MAX_GOLEIROS * sizeof(Atleta*));
        rodadas18[i].zag = (Atleta **) malloc(MAX_ZAGUEIROS * sizeof(Atleta*));
        rodadas18[i].lat = (Atleta **) malloc(MAX_LATERAIS * sizeof(Atleta*));
        rodadas18[i].mei = (Atleta **) malloc(MAX_MEIAS * sizeof(Atleta*));
        rodadas18[i].ata = (Atleta **) malloc(MAX_ATACANTES * sizeof(Atleta*));
    }

    atletas18 = (Atleta **) calloc(MAX_ATLETAS, sizeof(Atleta *));
    for(int i=0; i<MAX_ATLETAS; i++){
        atletas18[i] = (Atleta *) calloc(1, sizeof(Atleta));
    }

    //LEMBRA DE LIMPAR DEPOIS 
  
    char teste;
    //int j=0; 
    
    //pula a primeira linha do csv, header
    fscanf(csv, "%*[^\n]%*c");

    /*PARA ESSA FUNÇÃO, EU CONSIDERO QUE O CSV ESTÁ ORGANIZADO DA SEGUINTE FORMA:
    -ORDENADO PRIMEIRAMENTE PELO ID DO ATLETA
    -ORDENADO SEGUNDAMENTE PELA RODADA
    */

    while((teste = fgetc(csv)) != EOF){

        fseek(csv, -1, SEEK_CUR);

        ler_linha_csv(csv, rodadas18, atletas18);

        //printf("linhas: %d, atletas: %d\n", ++j, n_atletas);
        
    }

    fclose(csv);
    
    //----------------------- 2019 --------------------------
    n_atletas = 0;
    id_atleta_atual = -1;

    //printf("Nome do arquivo .csv com dados dos atletas: ");
    //scanf("%s%*c", nome_csv);
    //printf("%s\n", nome_csv);
    
    csv = fopen("src/manipulacao_dados/2019/dados_2019.csv", "r");
    
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    //inicializa todas as rodadas
    rodadas19 = (Rodada *) calloc(MAX_RODADAS, sizeof(Rodada));
    //LEMBRA DE LIMPAR DEPOIS
    
    for(int i=0; i<MAX_RODADAS; i++){
        rodadas19[i].id = i;
        rodadas19[i].tec = (Atleta **) malloc(MAX_TECNICOS * sizeof(Atleta*));
        rodadas19[i].gol = (Atleta **) malloc(MAX_GOLEIROS * sizeof(Atleta*));
        rodadas19[i].zag = (Atleta **) malloc(MAX_ZAGUEIROS * sizeof(Atleta*));
        rodadas19[i].lat = (Atleta **) malloc(MAX_LATERAIS * sizeof(Atleta*));
        rodadas19[i].mei = (Atleta **) malloc(MAX_MEIAS * sizeof(Atleta*));
        rodadas19[i].ata = (Atleta **) malloc(MAX_ATACANTES * sizeof(Atleta*));
    }

    atletas19 = (Atleta **) calloc(MAX_ATLETAS, sizeof(Atleta *));
    for(int i=0; i<MAX_ATLETAS; i++){
        atletas19[i] = (Atleta *) calloc(1, sizeof(Atleta));
    }

    //LEMBRA DE LIMPAR DEPOIS 
  
    //int j=0; 
    
    //pula a primeira linha do csv, header
    fscanf(csv, "%*[^\n]%*c");

    /*PARA ESSA FUNÇÃO, EU CONSIDERO QUE O CSV ESTÁ ORGANIZADO DA SEGUINTE FORMA:
    -ORDENADO PRIMEIRAMENTE PELO ID DO ATLETA
    -ORDENADO SEGUNDAMENTE PELA RODADA
    */

    while((teste = fgetc(csv)) != EOF){

        fseek(csv, -1, SEEK_CUR);

        ler_linha_csv(csv, rodadas19, atletas19);

        //printf("linhas: %d, atletas: %d\n", ++j, n_atletas);
        
    }

    fclose(csv);

    //----------------------- 2020 --------------------------
    n_atletas = 0;
    id_atleta_atual = -1;

    //printf("Nome do arquivo .csv com dados dos atletas: ");
    //scanf("%s%*c", nome_csv);
    //printf("%s\n", nome_csv);
    
    csv = fopen("src/manipulacao_dados/2020/dados_2020.csv", "r");
    
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    //inicializa todas as rodadas
    rodadas20 = (Rodada *) calloc(MAX_RODADAS, sizeof(Rodada));
    //LEMBRA DE LIMPAR DEPOIS
    
    for(int i=0; i<MAX_RODADAS; i++){
        rodadas20[i].id = i;
        rodadas20[i].tec = (Atleta **) malloc(MAX_TECNICOS * sizeof(Atleta*));
        rodadas20[i].gol = (Atleta **) malloc(MAX_GOLEIROS * sizeof(Atleta*));
        rodadas20[i].zag = (Atleta **) malloc(MAX_ZAGUEIROS * sizeof(Atleta*));
        rodadas20[i].lat = (Atleta **) malloc(MAX_LATERAIS * sizeof(Atleta*));
        rodadas20[i].mei = (Atleta **) malloc(MAX_MEIAS * sizeof(Atleta*));
        rodadas20[i].ata = (Atleta **) malloc(MAX_ATACANTES * sizeof(Atleta*));
    }

    atletas20 = (Atleta **) calloc(MAX_ATLETAS, sizeof(Atleta *));
    for(int i=0; i<MAX_ATLETAS; i++){
        atletas20[i] = (Atleta *) calloc(1, sizeof(Atleta));
    }

    //LEMBRA DE LIMPAR DEPOIS 
  
    //int j=0; 
    
    //pula a primeira linha do csv, header
    fscanf(csv, "%*[^\n]%*c");

    /*PARA ESSA FUNÇÃO, EU CONSIDERO QUE O CSV ESTÁ ORGANIZADO DA SEGUINTE FORMA:
    -ORDENADO PRIMEIRAMENTE PELO ID DO ATLETA
    -ORDENADO SEGUNDAMENTE PELA RODADA
    */

    while((teste = fgetc(csv)) != EOF){

        fseek(csv, -1, SEEK_CUR);

        ler_linha_csv(csv, rodadas20, atletas20);

        //printf("linhas: %d, atletas: %d\n", ++j, n_atletas);
        
    }

    fclose(csv);

    return 1;

}

void ler_linha_csv(FILE *csv, Rodada *rodadas, Atleta **atletas){

    int rodada;

    char pos[4];
    fscanf(csv, "%[^,]%*c", pos);
    //printf("%s ", pos);

    int id_clube;
    fscanf(csv, "%d%*c", &id_clube);
    //printf("%d ", id_clube);

    int id_atleta;
    fscanf(csv, "%d%*c", &id_atleta);
    //printf("%d ", id_atleta);

    //verifica se é um novo atleta ou não
    if(id_atleta == id_atleta_atual){

        //MESMO ATLETA
        fscanf(csv, "%d%*c", &rodada);
        //printf("%d ", rodada);
        atletas[n_atletas-1]->jogou_rodada[rodada] = 1;

        fscanf(csv, "%*[^,]%*c");
        fscanf(csv, "%*[^,]%*c%*[^,]%*c%*[^,]%*c");
        fscanf(csv, "%c%*[^,]%*c", &(atletas[n_atletas-1]->status[rodada]));
        //printf("%c ", atletas[n_atletas-1]->status[rodada]);

        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->pontos[rodada]));
        //printf("%f ", atletas[n_atletas-1]->pontos[rodada]);

        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->preco[rodada]));
        //printf("%f ", atletas[n_atletas-1]->preco[rodada]);

        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->valorizacao[rodada]));
        //printf("%f ", atletas[n_atletas-1]->valorizacao[rodada]);

        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->media[rodada]));
        //printf("%f ", atletas[n_atletas-1]->media[rodada]);

        fscanf(csv, "%*[^,]%*c");        
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->fs[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->rb[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->pe[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->fc[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->g[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->ff[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->ft[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->fd[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->dd[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->gs[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->sg[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->a[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->ca[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->i[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->cv[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->pp[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->gc[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->dp[rodada]));
        
        //le até o fim da linha
        fscanf(csv, "%*[^\n]%*c");

    }else{
        
        //NOVO ATLETA
        id_atleta_atual = id_atleta;
        n_atletas++;

        //registra os dados do novo atleta
        strcpy(atletas[n_atletas-1]->posicao, pos);
        atletas[n_atletas-1]->id_clube = id_clube;
        atletas[n_atletas-1]->id_atleta = id_atleta;

        fscanf(csv, "%d%*c", &rodada);
        atletas[n_atletas-1]->jogou_rodada[rodada] = 1;
              
        fscanf(csv, "%[^,]%*c", atletas[n_atletas-1]->nome);
        //pula o slug antes e a foto depois
        fscanf(csv, "%*[^,]%*c%[^,]%*c%*[^,]%*c", atletas[n_atletas-1]->apelido);
        fscanf(csv, "%c%*[^,]%*c", &(atletas[n_atletas-1]->status[rodada]));

        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->pontos[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->preco[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->valorizacao[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas-1]->media[rodada]));

        fscanf(csv, "%[^,]%*c", atletas[n_atletas-1]->nome_clube);

        if(rodada == 0){
            //ignora ate o fim da linha porque ja acabou os dados
            fscanf(csv, "%*[^\n]%*c");
        }else{
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->fs[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->rb[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->pe[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->fc[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->g[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->ff[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->ft[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->fd[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->dd[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->gs[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->sg[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->a[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->ca[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->i[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->cv[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->pp[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->gc[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas-1]->dp[rodada]));
            //le até o fim da linha
            fscanf(csv, "%*[^\n]%*c");
        }

    }

    //ASSOCIA COM RODADAS

    //Apenas se teve status provavel na ultima rodada:
    
    if(rodada != 0 && atletas[n_atletas-1]->status[rodada-1] == 'P'){
    
        switch(atletas[n_atletas-1]->posicao[0]){
            case 't':
                rodadas[rodada].tec[rodadas[rodada].n_tec++] = atletas[n_atletas-1];
                break;

            case 'g':
                rodadas[rodada].gol[rodadas[rodada].n_gol++] = atletas[n_atletas-1];
                break;

            case 'z':
                rodadas[rodada].zag[rodadas[rodada].n_zag++] = atletas[n_atletas-1];
                break;

            case 'l':
                rodadas[rodada].lat[rodadas[rodada].n_lat++] = atletas[n_atletas-1];
                break;

            case 'm':
                rodadas[rodada].mei[rodadas[rodada].n_mei++] = atletas[n_atletas-1];
                break;

            case 'a':
                rodadas[rodada].ata[rodadas[rodada].n_ata++] = atletas[n_atletas-1];
                break;
        }

    }

}

void limpar_memoria_alocada_cartola(){

    for(int i=0; i<MAX_ATLETAS; i++){
        free(atletas18[i]);
    }
    free(atletas18);
    for(int i=0; i<MAX_RODADAS; i++){
        free(rodadas18[i].tec);
        free(rodadas18[i].gol);
        free(rodadas18[i].zag);
        free(rodadas18[i].lat);
        free(rodadas18[i].mei);
        free(rodadas18[i].ata);
    }
    free(rodadas18);


    for(int i=0; i<MAX_ATLETAS; i++){
        free(atletas19[i]);
    }
    free(atletas19);
    for(int i=0; i<MAX_RODADAS; i++){
        free(rodadas19[i].tec);
        free(rodadas19[i].gol);
        free(rodadas19[i].zag);
        free(rodadas19[i].lat);
        free(rodadas19[i].mei);
        free(rodadas19[i].ata);
    }
    free(rodadas19);

    for(int i=0; i<MAX_ATLETAS; i++){
        free(atletas20[i]);
    }
    free(atletas20);
    for(int i=0; i<MAX_RODADAS; i++){
        free(rodadas20[i].tec);
        free(rodadas20[i].gol);
        free(rodadas20[i].zag);
        free(rodadas20[i].lat);
        free(rodadas20[i].mei);
        free(rodadas20[i].ata);
    }
    free(rodadas20);

}

void imprimir_atleta_rodada(Atleta *atleta, int rodada){

    if(!atleta){
        printf("Atleta não existe.\n");
        return;
    }

    if(atleta->jogou_rodada[rodada]){
        //printf("Nome: %s / ", atleta->nome);
        printf("Nome: %s / ", atleta->apelido);
        //printf("Rodada %d / ", rodada);
        //printf("Status: %c / ", atleta->status[rodada]);
        printf("Time: %s / ", atleta->nome_clube);
        //printf("Preço: %.2f /", atleta->preco[rodada-1]);
        //printf("Posição: %s / ", atleta->posicao);
        printf("Pontos: %.2f / ", atleta->pontos[rodada]);
        //printf("Média: %.2f / ", atleta->media[rodada-1]);
        printf("Valorização: %.2f;", atleta->valorizacao[rodada]);    
        printf("\n");
    }else{
        printf("O Atleta %s não jogou na rodada %d.\n", atleta->nome, rodada);
        return;
    }

}

void imprimir_atleta_campeonato(Atleta *atleta){

    if(!atleta){
        printf("Atleta não existe.");
        return;
    }

    for(int rodada=0; rodada<MAX_RODADAS; rodada++){
        imprimir_atleta_rodada(atleta, rodada);
    }

}

void imprimir_todos_atletas(){

    printf("%d\n", n_atletas);

    for(int atleta=0; atleta<n_atletas; atleta++){
        imprimir_atleta_campeonato(atletas18[atleta]);
    }

}


int compara_media_maior(Atleta *a, Atleta *b, int rodada){
    return (round(a->media[rodada]) > round(b->media[rodada]));
}

int compara_preco_maior(Atleta *a, Atleta *b, int rodada){
    return (round(a->preco[rodada]) > round(b->preco[rodada]));
}

int compara_valorizacao_maior(Atleta *a, Atleta *b, int rodada){
    return (round(a->valorizacao[rodada]) > round(b->valorizacao[rodada]));
}

int compara_valorizacao_menor(Atleta *a, Atleta *b, int rodada){
    return (round(a->valorizacao[rodada]) < round(b->valorizacao[rodada]));
}

int compara_sg_maior(Atleta *a, Atleta *b, int rodada){
    return (a->sg[rodada] > b->sg[rodada]);
}

int compara_gs_menor(Atleta *a, Atleta *b, int rodada){
    return (a->gs[rodada] < b->gs[rodada]);
}

int compara_dd_maior(Atleta *a, Atleta *b, int rodada){
    return (a->dd[rodada] > b->dd[rodada]);
}

int compara_dp_maior(Atleta *a, Atleta *b, int rodada){
    return (a->dp[rodada] > b->dp[rodada]);
}

int compara_ca_menor(Atleta *a, Atleta *b, int rodada){
    return (a->ca[rodada] < b->ca[rodada]);
}

int compara_fs_maior(Atleta *a, Atleta *b, int rodada){
    return (a->fs[rodada] > b->fs[rodada]);
}

int compara_pe_menor(Atleta *a, Atleta *b, int rodada){
    return (a->pe[rodada] < b->pe[rodada]);
}

int compara_rb_maior(Atleta *a, Atleta *b, int rodada){
    return (a->rb[rodada] > b->rb[rodada]);
}

int compara_fc_menor(Atleta *a, Atleta *b, int rodada){
    return (a->fc[rodada] < b->fc[rodada]);
}

int compara_ff_maior(Atleta *a, Atleta *b, int rodada){
    return (a->ff[rodada] > b->ff[rodada]);
}

int compara_fd_maior(Atleta *a, Atleta *b, int rodada){
    return (a->fd[rodada] > b->fd[rodada]);
}

int compara_ft_maior(Atleta *a, Atleta *b, int rodada){
    return (a->ft[rodada] > b->ft[rodada]);
}

int compara_g_maior(Atleta *a, Atleta *b, int rodada){
    return (a->g[rodada] > b->g[rodada]);
}

int compara_a_maior(Atleta *a, Atleta *b, int rodada){
    return (a->a[rodada] > b->a[rodada]);
}

int compara_i_menor(Atleta *a, Atleta *b, int rodada){
    return (a->i[rodada] < b->i[rodada]);
}

int (*get_comparador_gol(int id))(Atleta *, Atleta *, int){
    switch (id){
        case 1:
            return compara_media_maior;
        case 2:
            return compara_preco_maior;
        case 3:
            return compara_valorizacao_maior;
        case 4:
            return compara_sg_maior;
        case 5:
            return compara_gs_menor;
        case 6:
            return compara_dd_maior;
        case 7:
            return compara_dp_maior;
        case 8:
            return compara_ca_menor;
        case 9:
            return compara_fs_maior;
        case 10:
            return compara_pe_menor;
        case 11:
            return compara_valorizacao_menor;
    }
    return NULL;
}

int get_n_ids_gol(){
    return 11;
}

int (*get_comparador_zag(int id))(Atleta *, Atleta *, int){
    switch (id){
        case 1:
            return compara_media_maior;
        case 2:
            return compara_preco_maior;
        case 3:
            return compara_valorizacao_maior;
        case 4:
            return compara_sg_maior;
        case 5:
            return compara_rb_maior;
        case 6:
            return compara_fc_menor;
        case 7:
            return compara_ca_menor;
        case 8:
            return compara_pe_menor;
        case 9:
            return compara_ff_maior;
        case 10:
            return compara_fd_maior;
        case 11:
            return compara_ft_maior;
        case 12:
            return compara_g_maior;
        case 13:
            return compara_valorizacao_menor;
    }
    return NULL;
}

int get_n_ids_zag(){
    return 13;
}

int (*get_comparador_lat(int id))(Atleta *, Atleta *, int){
    switch (id){
        case 1:
            return compara_media_maior;
        case 2:
            return compara_preco_maior;
        case 3:
            return compara_valorizacao_maior;
        case 4:
            return compara_sg_maior;
        case 5:
            return compara_rb_maior;
        case 6:
            return compara_fc_menor;
        case 7:
            return compara_ca_menor;
        case 8:
            return compara_fs_maior;
        case 9:
            return compara_pe_menor;
        case 10:
            return compara_a_maior;
        case 11:
            return compara_ff_maior;
        case 12:
            return compara_fd_maior;
        case 13:
            return compara_ft_maior;
        case 14:
            return compara_g_maior;
        case 15:
            return compara_valorizacao_menor;
    }
    return NULL;
}

int get_n_ids_lat(){
    return 15;
}

int (*get_comparador_mei(int id))(Atleta *, Atleta *, int){
    switch (id){
        case 1:
            return compara_valorizacao_menor;
        case 2:
            return compara_rb_maior;
        case 3:
            return compara_fc_menor;
        case 4:
            return compara_ca_menor;
        case 5:
            return compara_fs_maior;
        case 6:
            return compara_pe_menor;
        case 7:
            return compara_a_maior;
        case 8:
            return compara_ff_maior;
        case 9:
            return compara_fd_maior;
        case 10:
            return compara_ft_maior;
        case 11:
            return compara_g_maior;
        case 12:
            return compara_valorizacao_maior;
        case 13:
            return compara_preco_maior;
        case 14:
            return compara_media_maior;
    }
    return NULL;
}

int get_n_ids_mei(){
    return 14;
}

int (*get_comparador_ata(int id))(Atleta *, Atleta *, int){
    switch (id){
        case 1:
            return compara_valorizacao_menor;
        case 2:
            return compara_rb_maior;
        case 3:
            return compara_fc_menor;
        case 4:
            return compara_ca_menor;
        case 5:
            return compara_fs_maior;
        case 6:
            return compara_pe_menor;
        case 7:
            return compara_a_maior;
        case 8:
            return compara_i_menor;
        case 9:
            return compara_ff_maior;
        case 10:
            return compara_fd_maior;
        case 11:
            return compara_ft_maior;
        case 12:
            return compara_g_maior;
        case 13:
            return compara_valorizacao_maior;
        case 14:
            return compara_preco_maior;
        case 15:
            return compara_media_maior;
    }
    return NULL;
}

int get_n_ids_ata(){
    return 15;
}

void ordena_vetor_rodada(Atleta **vet, int tam, int (*comparador)(Atleta *, Atleta *, int), int rodada){
    merge_sort(vet, 0, tam-1, comparador, rodada);
}

void merge_sort(Atleta **vet, int ini, int fim, int (*comparador)(Atleta *, Atleta *, int), int rodada){
	if(fim <= ini)	return;
	int meio = (int) (ini+fim)/2;	
	merge_sort(vet, ini, meio, comparador, rodada);
	merge_sort(vet, meio+1, fim, comparador, rodada);
	merge(vet, ini, meio, fim, comparador, rodada);
}
/*Função merge(intercala):
 Pega dois vetores previamente ordenados e os intercala formando um só vetor ordenado*/
void merge(Atleta **vet, int ini, int meio, int fim, int (*comparador)(Atleta *, Atleta *, int), int rodada){
	Atleta **aux = (Atleta **) malloc((fim-ini+1) * sizeof(Atleta *));
	int i, j, k;	
    for(i=ini, j=meio+1, k=0; i<=meio && j<=fim; k++){	//Ordena os dois 'blocos'
    	//QUEM EU QUERO ANTES? O MENOR OU O MAIOR?
        //printf("veti: %p\n", vet[i]);
        //printf("vetj: %p\n", vet[j]);
        if(comparador(vet[i], vet[j], rodada))
    		aux[k] = vet[i++];
    	else
    		aux[k] = vet[j++];
    }
    while(i <= meio){  //Caso ainda haja elementos na primeira metade
        aux[k++] = vet[i++];
    }
    while(j <= fim){   //Caso ainda haja elementos na segunda metade
        aux[k++] = vet[j++];
    }
    for(k = ini; k <= fim; k++){//Move os elementos de volta para o vetor original
        vet[k] = aux[k-ini];
    }
    free(aux);
}

Atleta **get_copia_goleiros_rodada(int i, Rodada *rodadas){
    int n = rodadas[i].n_gol;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].gol[j];
    }
    return copia;
}

Atleta **get_copia_zagueiros_rodada(int i, Rodada *rodadas){
    int n = rodadas[i].n_zag;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].zag[j];
    }
    return copia;
}

Atleta **get_copia_laterais_rodada(int i, Rodada *rodadas){
    int n = rodadas[i].n_lat;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].lat[j];
    }
    return copia;
}

Atleta **get_copia_meias_rodada(int i, Rodada *rodadas){
    int n = rodadas[i].n_mei;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].mei[j];
    }
    return copia;
}

Atleta **get_copia_atacantes_rodada(int i, Rodada *rodadas){
    int n = rodadas[i].n_ata;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].ata[j];
    }
    return copia;
}

Atleta **get_copia_tecnicos_rodada(int i, Rodada *rodadas){
    int n = rodadas[i].n_tec;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].tec[j];
    }
    return copia;
}

void merge_sort_2(TimeEscolhido *vet, int ini, int fim, int (*comparador)(TimeEscolhido , TimeEscolhido)){
	if(fim <= ini)	return;
	int meio = (int) (ini+fim)/2;	
	merge_sort_2(vet, ini, meio, comparador);
	merge_sort_2(vet, meio+1, fim, comparador);
	merge_2(vet, ini, meio, fim, comparador);
}
/*Função merge(intercala):
 Pega dois vetores previamente ordenados e os intercala formando um só vetor ordenado*/
void merge_2(TimeEscolhido *vet, int ini, int meio, int fim, int (*comparador)(TimeEscolhido, TimeEscolhido)){
	TimeEscolhido*aux = (TimeEscolhido*) malloc((fim-ini+1) * sizeof(TimeEscolhido));
	int i, j, k;	
    for(i=ini, j=meio+1, k=0; i<=meio && j<=fim; k++){	//Ordena os dois 'blocos'
    	//QUEM EU QUERO ANTES? O MENOR OU O MAIOR?
        //printf("veti: %p\n", vet[i]);
        //printf("vetj: %p\n", vet[j]);
        if(comparador(vet[i], vet[j]))
    		aux[k] = vet[i++];
    	else
    		aux[k] = vet[j++];
    }
    while(i <= meio){  //Caso ainda haja elementos na primeira metade
        aux[k++] = vet[i++];
    }
    while(j <= fim){   //Caso ainda haja elementos na segunda metade
        aux[k++] = vet[j++];
    }
    for(k = ini; k <= fim; k++){//Move os elementos de volta para o vetor original
        vet[k] = aux[k-ini];
    }
    free(aux);
}