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
    Atleta *tec[MAX_TECNICO];
    int n_tec;
};

struct atleta{
    char nome[100];
    int id;
    int clube_id; //tem os ids dos clubes
    char nome_clube[50];
    //Os seguintes dados são vetores de MAX_RODADAS posições pois representam valores entre as rodadas 0 e 38;
    //OBS: rodada 0 são os dados iniciais dos atletas antes da primeira rodada
    /*
    Tipos de status:
    'P' -> provável
    'C' -> contundido
    'N' -> nulo
    */
    char status[MAX_RODADAS];
    int rodada[MAX_RODADAS]; //vetor de "boolean" para indicar se o atleta participou da rodada x ou não
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
Atleta *atletas[MAX_ATLETAS]; //Vetor com todos os atletas do jogo


void inicializa_rodadas(){

    

        rodadas[i].id = i;

        rodadas[i].n_gol = 0;
        for(int j=0; i<MAX_GOLEIROS; i++)
            rodadas[i].gol[j] = NULL;


    }

}

int ler_csv_campeonato(char *nomeCsv){

    FILE *csv = fopen(nomeCsv, "r");
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    //inicializa todas as rodadas
    rodadas = (Rodada *) calloc(MAX_RODADAS, sizeof(Rodada));
    //LEMBRA DE LIMPAR ESSA PORRA DEPOIS CARALHO
    
    for(int i=0; i<MAX_RODADAS; i++){

        rodadas[i].id = 0;

    }

    //pula a primeira linha do csv, header
    //LEMBRAR DE GARANTIR QUE O CSV TEM QUE TER TERMINAÇÃO DE ARQUIVO PADRÃO LF! (SEM \r)
    fscanf("%[^\n]");

    /*PARA ESSA FUNÇÃO, EU CONSIDERO QUE O CSV ESTÁ ORGANIZADO DA SEGUINTE FORMA:
    -ORDENADO PRIMEIRAMENTE PELO ID DO ATLETA
    -ORDENADO SEGUNDAMENTE PELA RODADA
    */
    while(fscanf("%*c")){
        
        fseek(csv, -1, SEEK_CUR);


    }

    fclose(csv);
    return 1;

}

