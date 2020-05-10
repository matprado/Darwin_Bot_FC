#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cartola_utils.h"

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
    'N' -> nulo
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

    atletas = (Atleta **) malloc(MAX_ATLETAS * sizeof(Atleta *));
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
    while(fscanf(csv, "%*c")){
        
        fseek(csv, -1, SEEK_CUR);

        ler_linha_csv(csv);
        
    }

    fclose(csv);
    return 1;

}

void ler_linha_csv(FILE *csv){

    int rodada;

    char pos[4];
    fscanf(csv, "%[^,]%*c", pos);

    int id_clube;
    fscanf(csv, "%d%*c", &id_clube);

    int id_atleta;
    fscanf(csv, "%d%*c", &id_atleta);

    //verifica se é um novo atleta ou não
    if(id_atleta == id_atleta_atual){

        fscanf(csv, "%d%*c", &rodada);
        atletas[n_atletas]->jogou_rodada[rodada] = 1;

        fscanf(csv, "%*[^,]%*c");
        fscanf(csv, "%*[^,]%*c%*[^,]%*c%*[^,]%*c");
        fscanf(csv, "%c%*[^,]%*c", &(atletas[n_atletas]->status[rodada]));

        fscanf(csv, "%f%*c", &(atletas[n_atletas]->pontos[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas]->preco[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas]->valorizacao[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas]->media[rodada]));

        fscanf(csv, "%*[^,]%*c");        
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->fs[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->rb[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->pe[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->fc[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->g[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->ff[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->ft[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->fd[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->dd[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->gs[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->sg[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->a[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->ca[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->i[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->cv[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->pp[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->gc[rodada]));
        fscanf(csv, "%d%*c", &(atletas[n_atletas]->dp[rodada]));
        
        //le até o fim da linha
        fscanf(csv, "%*[^\n]%*c");

    }else{
        
        id_atleta_atual = id_atleta;
        n_atletas++;
        //registra os dados do novo atleta
        strcpy(atletas[n_atletas]->posicao, pos);
        atletas[n_atletas]->id_clube = id_clube;
        atletas[n_atletas]->id_atleta = id_atleta;

        fscanf(csv, "%d%*c", &rodada);
        atletas[n_atletas]->jogou_rodada[rodada] = 1;
              
        fscanf(csv, "%[^,]%*c", atletas[n_atletas]->nome);
        //pula o slug antes e a foto depois
        fscanf(csv, "%*[^,]%*c%[^,]%*c%*[^,]%*c", atletas[n_atletas]->apelido);
        fscanf(csv, "%c%*[^,]%*c", &(atletas[n_atletas]->status[rodada]));

        fscanf(csv, "%f%*c", &(atletas[n_atletas]->pontos[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas]->preco[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas]->valorizacao[rodada]));
        fscanf(csv, "%f%*c", &(atletas[n_atletas]->media[rodada]));

        fscanf(csv, "%[^,]%*c", atletas[n_atletas]->nome_clube);

        if(rodada == 0){
            //ignora ate o fim da linha porque ja acabou os dados
            fscanf(csv, "%*[^\n]%*c");
        }else{
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->fs[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->rb[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->pe[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->fc[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->g[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->ff[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->ft[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->fd[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->dd[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->gs[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->sg[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->a[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->ca[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->i[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->cv[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->pp[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->gc[rodada]));
            fscanf(csv, "%d%*c", &(atletas[n_atletas]->dp[rodada]));
            //le até o fim da linha
            fscanf(csv, "%*[^\n]%*c");
        }

    }

    //ASSOCIA COM RODADAS
    switch(atletas[n_atletas]->posicao[0]){

        case 't':
            rodadas[rodada].tec[rodadas[rodada].n_tec++] = atletas[n_atletas];
            break;

        case 'g':
            rodadas[rodada].gol[rodadas[rodada].n_gol++] = atletas[n_atletas];
            break;

        case 'z':
            rodadas[rodada].zag[rodadas[rodada].n_zag++] = atletas[n_atletas];
            break;

        case 'l':
            rodadas[rodada].lat[rodadas[rodada].n_lat++] = atletas[n_atletas];
            break;

        case 'm':
            rodadas[rodada].mei[rodadas[rodada].n_mei++] = atletas[n_atletas];
            break;

        case 'a':
            rodadas[rodada].ata[rodadas[rodada].n_ata++] = atletas[n_atletas];
            break;

    }
    

}