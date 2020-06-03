#include "cartola_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rodada{
    int id;
    Atleta *gol[MAX_GOLEIROS];
    int n_gol;
    Atleta *lat[MAX_LATERAIS];
    int n_lat;
    Atleta *zag[MAX_ZAGUEIROS];
    int n_zag;
    Atleta *mei[MAX_MEIAS];
    int n_mei;
    Atleta *ata[MAX_ATACANTES];
    int n_ata;
    Atleta *tec[MAX_TECNICOS];
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
};


Rodada *rodadas; //Vetor de rodadas do campeonato
Atleta **atletas; //Vetor com todos os atletas do jogo
int n_atletas = 0;
int id_atleta_atual = -1;


int ler_csv_campeonato(){

    char nome_csv[100];
    printf("Nome do arquivo .csv: ");
    scanf("%s", nome_csv);

    FILE *csv = fopen(nome_csv, "r");
    
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    //inicializa todas as rodadas
    rodadas = (Rodada *) calloc(MAX_RODADAS, sizeof(Rodada));
    //LEMBRA DE LIMPAR DEPOIS
    
    for(int i=0; i<MAX_RODADAS; i++){
        rodadas[i].id = i;
    }

    atletas = (Atleta **) calloc(MAX_ATLETAS, sizeof(Atleta *));
    for(int i=0; i<MAX_ATLETAS; i++){
        atletas[i] = (Atleta *) calloc(1, sizeof(Atleta));
    }

    //LEMBRA DE LIMPAR DEPOIS 

    //pula a primeira linha do csv, header
    fscanf(csv, "%*[^\n]%*c");

    /*PARA ESSA FUNÇÃO, EU CONSIDERO QUE O CSV ESTÁ ORGANIZADO DA SEGUINTE FORMA:
    -ORDENADO PRIMEIRAMENTE PELO ID DO ATLETA
    -ORDENADO SEGUNDAMENTE PELA RODADA
    */
    char teste;
    int j=0;

    while((teste = fgetc(csv)) != EOF){

        fseek(csv, -1, SEEK_CUR);

        ler_linha_csv(csv);

        printf("linhas: %d, atletas: %d\n", ++j, n_atletas);
        
    }

    fclose(csv);
    return 1;

}

void ler_linha_csv(FILE *csv){

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

void limpar_memoria_alocada(){

    for(int i=0; i<MAX_ATLETAS; i++){
        free(atletas[i]);
    }
    free(atletas);

    free(rodadas);

}

void imprimir_atleta_rodada(Atleta *atleta, int rodada){

    if(!atleta){
        printf("Atleta não existe.\n");
        return;
    }

    if(atleta->jogou_rodada[rodada]){
        printf("Nome: %s / ", atleta->nome);
        printf("Rodada %d / ", rodada);
        printf("Status: %c / ", atleta->status[rodada]);
        printf("pontos: %f / ", atleta->pontos[rodada]);
        printf("media: %f / ", atleta->media[rodada]);
        printf("valorização: %f / ", atleta->valorizacao[rodada]);
        printf("preço: %f / \n", atleta->preco[rodada]);

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
        imprimir_atleta_campeonato(atletas[atleta]);
    }

}