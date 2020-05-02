#include "cartola_utils.h"

struct rodada{
    int id;
    Atleta *goleiro[MAX_GOLEIROS];
    Atleta *lateral[MAX_LATERAIS];
    Atleta *zagueiro[MAX_ZAGUEIROS];
    Atleta *meia[MAX_MEIAS];
    Atleta *atacante[MAX_ATACANTES];
    Atleta *tecnico[MAX_TECNICO];
};

struct atleta{
    char nome[100];
    int id;
    int rodada;
    int clube_id; //tem os ids dos clubes
    int status;
    /*0 -> provável
    1 -> contundido
    2 -> nulo*/
    float pontos;
    float preco;
    float valorizacao;
    float media;
    char nome_clube[50];
    //dados do jogo(acumulativos com as rodadas):
    //obs: no csv daqui pra frente pode estar em qualquer ordem
    int fs; //faltas sofridas: +0.5
    int rb; //roubadas de bola: +1.5
    int pe; //passes errados: -0.3
    int fc; //faltas cometidas: -0.5 
    int g; //gols: +8.0
    int ff; //finalizações para fora: +0.8
    int ft; //finalizações na trave: +3.0
    int fd; //finalizações defendidas: +1.2
    int dd; //defesas difíceis: +3.0
    int gs; //gols sofridos: -2.0
    int sg; //jogos sem sofrer gol: +5.0
    int a; //assistências: +5.0
    int ca; //cartões amarelos: -2.0
    int i; //impedimentos: -0.5
    int cv; //cartões vermelhos: -5.0
    int pp; //pênatils perdidos: -4.0
    int gc; //gols contra: -5.0
    int dp; //defesa de pênalti: +7.0
};
