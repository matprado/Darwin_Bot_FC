#include "cartola_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct time{
    int mando_de_campo[MAX_RODADAS];
};


Rodada *rodadas; //Vetor de rodadas do campeonato
Atleta **atletas; //Vetor com todos os atletas do jogo
int n_atletas = 0;
int id_atleta_atual = -1;

Time *times; //Vetor com todos os times
#define MAX_ID_TIMES 500

struct time_escolhido{
    int id_time;
    int qtd;
};

#define MOD 100
#define MENOR_ID 14
#define MAIOR_ID 93
#define TAM_TIMES (MAIOR_ID - MENOR_ID)+1

float get_pontuacao_media(int quant_pos[6], int pref_gol[3], int pref_zag[3], int pref_lat[3], int pref_mei[3], int pref_ata[3], int capitao){
 
    float pontuacao_acumulada = 0;
    float cartoletas = 100, valorizacao = 0, cart_atual;
    int faltam;

    Atleta **copia_gol, **copia_zag, **copia_lat, **copia_mei, **copia_ata, **copia_tec;

    //Posso usar mod100 para vetor de 100 posicoes para tecnico. Em outros anos, precisa verificar isso...
    TimeEscolhido *times;

    //para cada rodada do campeonato...
    //pulando a primeira...
    for(int i=2; i<MAX_RODADAS; i++){

        times = (TimeEscolhido *) calloc(TAM_TIMES, sizeof(TimeEscolhido));

        for(int j=0; j<rodadas[i-1].n_tec; j++){
            times[((rodadas[i-1].tec[j]->id_clube)%MOD)-MENOR_ID].id_time = rodadas[i-1].tec[j]->id_clube;
        }

        //Escala um time e registra a pontuação na rodada
       
        //PARA O TÉCNICO, ESCOLHE O TECNICO DO TIME COM MAIS JOGADORES ESCALADOS

        faltam = 12;
        valorizacao = 0;
        cart_atual = cartoletas;

        printf("\t*****rodada %d:*****\n", i);    

        copia_gol = get_copia_goleiros_rodada(i-1);
        printf("  (goleiro):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[1], rodadas[i-1].n_gol, copia_gol, i-1, get_comparador_gol(pref_gol[0]), get_comparador_gol(pref_gol[1]), get_comparador_gol(pref_gol[2]), &cartoletas, &faltam, &valorizacao, (capitao==1), times);
        free(copia_gol);

        copia_zag = get_copia_zagueiros_rodada(i-1);
        printf("  (zagueiros):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[2], rodadas[i-1].n_zag, copia_zag, i-1, get_comparador_zag(pref_zag[0]), get_comparador_zag(pref_zag[1]), get_comparador_zag(pref_zag[2]), &cartoletas, &faltam, &valorizacao, (capitao==2), times);
        free(copia_zag);

        copia_lat = get_copia_laterais_rodada(i-1);
        printf("  (laterais):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[3], rodadas[i-1].n_lat, copia_lat, i-1, get_comparador_lat(pref_lat[0]), get_comparador_lat(pref_lat[1]), get_comparador_lat(pref_lat[2]), &cartoletas, &faltam, &valorizacao, (capitao==3), times);
        free(copia_lat);

        copia_mei = get_copia_meias_rodada(i-1);
        printf("  (meias):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[4], rodadas[i-1].n_mei, copia_mei, i-1, get_comparador_mei(pref_mei[0]), get_comparador_mei(pref_mei[1]), get_comparador_mei(pref_mei[2]), &cartoletas, &faltam, &valorizacao, (capitao==4), times);
        free(copia_mei);
        
        copia_ata = get_copia_atacantes_rodada(i-1);
        printf("  (atacantes):\n");
        pontuacao_acumulada += escolhe_atletas_rodada(quant_pos[5], rodadas[i-1].n_ata, copia_ata, i-1, get_comparador_ata(pref_ata[0]), get_comparador_ata(pref_ata[1]), get_comparador_ata(pref_ata[2]), &cartoletas, &faltam, &valorizacao, (capitao==5), times);     
        free(copia_ata);

        copia_tec = get_copia_tecnicos_rodada(i-1);
        printf("  (técnico):\n");
        pontuacao_acumulada += escolhe_tecnico_rodada(rodadas[i-1].n_tec, copia_tec, i-1, &cartoletas, &valorizacao, (capitao==6), times);
        free(copia_tec);

        cartoletas = cart_atual + valorizacao;

        printf("\n");

        free(times);

    }

    printf("O time terminou com %.2f cartoletas\n\n", cartoletas);

    return (pontuacao_acumulada / (MAX_RODADAS-1)); //tirando a primeira rodada

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

    ordena_vetor_rodada(atletas, total, comparador, rodada);

    float cartoletasDisponiveis;

    int i, j, k;

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

        k=i;

        if(j!=k){
            //desempate dos iguais
            comparador = comparador2;
            merge_sort(atletas, j, k, comparador, rodada);
        }

        i++;
    }
    
    //Neste momento, está ordenado com as preferências 1 e 2.
    //Caso necessário, vamos desempatar com a preferência 3
    i=0;
    while(i<(total-1)){

        j=i;

        comparador = comparador2;
        while(i<(total-1) && (!comparador(atletas[i], atletas[i+1], rodada))){
            //testa com o comparador: se o teste falhar, só pode significar empate.
            i++;
        }

        k=i;

        if(j!=k){
            //desempate dos iguais
            comparador = comparador3;
            merge_sort(atletas, j, k, comparador, rodada);
        }

        i++;
    }

    int ja_escolhidos=0;
    float pontuacao = 0, preco;
    cartoletasDisponiveis = (*cartoletas) / (*faltam);
    i=0;
    while(i<(total-1) && ja_escolhidos<qtd_atl){
        
        preco = atletas[i]->preco[rodada];

        if(preco <= cartoletasDisponiveis){
            //Pode comprar o jogador       
            imprimir_atleta_rodada(atletas[i], rodada+1);
            
            times[((atletas[i]->id_clube)%MOD)-MENOR_ID].qtd += 1;

            (*cartoletas) = (*cartoletas) - preco;
            (*faltam) = (*faltam) - 1;
            if(*faltam != 0){
                cartoletasDisponiveis = (*cartoletas) / (*faltam);
            }
            (*valorizacao) = (*valorizacao) + atletas[i]->valorizacao[rodada+1];

            if(eh_capitao && ja_escolhidos == 0){
                pontuacao += ((atletas[i])->media[rodada+1] * 2);
            }else{
                pontuacao += (atletas[i])->media[rodada+1];
            }
            
            ja_escolhidos++;
        }

        i++;

    }

    return pontuacao;

    /*j=0;
    while(j < qtd_atl){

        //Sei que o atual é, realmente, o melhor
        if(comparador(atletas[j], atletas[qtd_atl], rodada)){
       
            //Tenta comprar
            preco = atletas[j]->preco[rodada];
            if(preco <= cartoletasDisponiveis){
                //Pode comprar o jogador       
                imprimir_atleta_rodada(atletas[j], rodada);
                
                (*cartoletas) = (*cartoletas) - preco;
                (*faltam) = (*faltam) - 1;
                if(*faltam != 0){
                    cartoletasDisponiveis = (*cartoletas) / (*faltam);
                }
                (*valorizacao) = (*valorizacao) + atletas[j]->valorizacao[rodada];

                pontuacao += (atletas[j])->media[rodada];

                ja_escolhidos++;
            }

        }else{
            //Sei que tem um cara mais pra frente que pode ser melhor
            //e todos os outros até ele também estão empatados
            break;
        }

        j++;
    }

    Atleta **aux = (Atleta **) malloc(total * sizeof(Atleta *));
    
    int n=0;

    //testa com o comparador: se o teste falhar, só pode significar empate. Pega os primeiros empatados e desempata
    while ((j < total) && (!comparador(atletas[j], atletas[j+1], rodada))){
        preco = atletas[j]->preco[rodada];
        if(preco <= cartoletasDisponiveis){
            aux[n++] = atletas[j];
        }
        j++;
    }

    //Tenho no aux, todo mundo que falta
    if(n < qtd_atl)

    if(j > qtd_atl){

        //tem que desempatar com a proxima preferencia!
        comparador = comparador2;
        merge_sort(atletas, ja_escolhidos, j-1, comparador, rodada);

        //TEM QUE CONFERIR TUDO DE NOVO PARA A PROXIMA PREFERENCIA        
        
        total = j;
        j = ja_escolhidos;

        while(j < qtd_atl){
            if(comparador(atletas[j], atletas[qtd_atl-1], rodada)){
                ja_escolhidos++;
            }
            j++;
        }

        // j == qtd_atl
    
        while ((j < total) && (!comparador(atletas[j-1], atletas[j], rodada))){
            //testa com o comparador: se o teste falhar, só pode significar empate. Pega os primeiros empatados e desempata
            j++;
        }

        if(j > qtd_atl){
            //tem que desempatar com a proxima preferencia!
            comparador = comparador3;
            merge_sort(atletas, ja_escolhidos, j-1, comparador, rodada);
        }
    }
    return pontuacao;
    */

}

float escolhe_tecnico_rodada(int total, Atleta **atletas, int rodada, float *cartoletas, float *valorizacao, int eh_capitao, TimeEscolhido *times){

    merge_sort_2(times, 0, TAM_TIMES-1, compara_time_mais_escalado);

    int i=0, j=0, ja_escolhidos=0;
    float preco, pontuacao=0;
    while(ja_escolhidos<1 && i<(TAM_TIMES)){ 

        j=0;
        while(ja_escolhidos<1 && j<(total-1)){
                
            if(times[i].id_time == atletas[j]->id_clube){
            
                preco = atletas[j]->preco[rodada];

                if(preco <= (*cartoletas)){

                    //Pode comprar o jogador       
                    imprimir_atleta_rodada(atletas[j], rodada+1);

                    (*cartoletas) = (*cartoletas) - preco;

                    (*valorizacao) = (*valorizacao) + atletas[j]->valorizacao[rodada+1];

                    if(eh_capitao){
                        pontuacao += ((atletas[j])->media[rodada+1] * 2);
                    }else{
                        pontuacao += (atletas[j])->media[rodada+1];
                    }
                    
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

int compara_time_mais_escalado(TimeEscolhido a, TimeEscolhido b){
    return (a.qtd > b.qtd);
}


int ler_csv_campeonato(){

    //char nome_csv[100];
    
    //printf("Nome do arquivo .csv com a tabela: ");
    //scanf("%s%*c", nome_csv);
    //printf("%s\n", nome_csv);

    FILE *tabela = fopen("src/manipulacao_dados/2019/tabela_2019.csv", "r");
    
    if(!tabela){
        printf("ERRO! FALHA AO ABRIR ARQUIVO DA TABELA!\n");
        return 0;
    }

    //printf("Nome do arquivo .csv com dados dos atletas: ");
    //scanf("%s%*c", nome_csv);
    //printf("%s\n", nome_csv);

    FILE *csv = fopen("src/manipulacao_dados/2019/dados_2019.csv", "r");
    
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    //inicializa todas as rodadas
    rodadas = (Rodada *) calloc(MAX_RODADAS, sizeof(Rodada));
    //LEMBRA DE LIMPAR DEPOIS
    
    for(int i=0; i<MAX_RODADAS; i++){
        rodadas[i].id = i;
        rodadas[i].tec = (Atleta **) malloc(MAX_TECNICOS * sizeof(Atleta*));
        rodadas[i].gol = (Atleta **) malloc(MAX_GOLEIROS * sizeof(Atleta*));
        rodadas[i].zag = (Atleta **) malloc(MAX_ZAGUEIROS * sizeof(Atleta*));
        rodadas[i].lat = (Atleta **) malloc(MAX_LATERAIS * sizeof(Atleta*));
        rodadas[i].mei = (Atleta **) malloc(MAX_MEIAS * sizeof(Atleta*));
        rodadas[i].ata = (Atleta **) malloc(MAX_ATACANTES * sizeof(Atleta*));
    }

    atletas = (Atleta **) calloc(MAX_ATLETAS, sizeof(Atleta *));
    for(int i=0; i<MAX_ATLETAS; i++){
        atletas[i] = (Atleta *) calloc(1, sizeof(Atleta));
    }

    //inicializa todos os times
    times = (Time *) calloc(MAX_ID_TIMES, sizeof(Time));

    //LEMBRA DE LIMPAR DEPOIS 
  
    char teste;
    //int j=0; 
    
    //pula a primeira linha do csv, header
    fscanf(tabela, "%*[^\n]%*c");

    while((teste = fgetc(tabela)) != EOF){

        fseek(tabela, -1, SEEK_CUR);

        ler_linha_csv_tabela(tabela);
        
    }

    fclose(tabela);

    //pula a primeira linha do csv, header
    fscanf(csv, "%*[^\n]%*c");

    /*PARA ESSA FUNÇÃO, EU CONSIDERO QUE O CSV ESTÁ ORGANIZADO DA SEGUINTE FORMA:
    -ORDENADO PRIMEIRAMENTE PELO ID DO ATLETA
    -ORDENADO SEGUNDAMENTE PELA RODADA
    */

    while((teste = fgetc(csv)) != EOF){

        fseek(csv, -1, SEEK_CUR);

        ler_linha_csv(csv);

        //printf("linhas: %d, atletas: %d\n", ++j, n_atletas);
        
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

void ler_linha_csv_tabela(FILE *csv){

    int id;
    int rodada;
    int mando;

    fscanf(csv, "%*[^,]%*c%d%*c%d%*c%d%*[^\n]%*c", &id, &rodada, &mando);

    times[id].mando_de_campo[rodada] = mando;

    //printf("Time %d na rodada %d jogou em casa? %d ;\n", id, rodada, mando);

}

void limpar_memoria_alocada_cartola(){

    for(int i=0; i<MAX_ATLETAS; i++){
        free(atletas[i]);
    }
    free(atletas);

    for(int i=0; i<MAX_RODADAS; i++){
        free(rodadas[i].tec);
        free(rodadas[i].gol);
        free(rodadas[i].zag);
        free(rodadas[i].lat);
        free(rodadas[i].mei);
        free(rodadas[i].ata);
    }
    free(rodadas);

    free(times);

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
        imprimir_atleta_campeonato(atletas[atleta]);
    }

}


int compara_media_maior(Atleta *a, Atleta *b, int rodada){
    return (a->media[rodada] > b->media[rodada]);
}

int compara_preco_maior(Atleta *a, Atleta *b, int rodada){
    return (a->preco[rodada] > b->preco[rodada]);
}

int compara_valorizacao_maior(Atleta *a, Atleta *b, int rodada){
    return (a->valorizacao[rodada] > b->valorizacao[rodada]);
}

int compara_valorizacao_menor(Atleta *a, Atleta *b, int rodada){
    return (a->valorizacao[rodada] < b->valorizacao[rodada]);
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

Atleta **get_copia_goleiros_rodada(int i){
    int n = rodadas[i].n_gol;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].gol[j];
    }
    return copia;
}

Atleta **get_copia_zagueiros_rodada(int i){
    int n = rodadas[i].n_zag;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].zag[j];
    }
    return copia;
}

Atleta **get_copia_laterais_rodada(int i){
    int n = rodadas[i].n_lat;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].lat[j];
    }
    return copia;
}

Atleta **get_copia_meias_rodada(int i){
    int n = rodadas[i].n_mei;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].mei[j];
    }
    return copia;
}

Atleta **get_copia_atacantes_rodada(int i){
    int n = rodadas[i].n_ata;
    Atleta **copia = (Atleta **) malloc(n * sizeof(Atleta *));
    for(int j=0; j<n; j++){
        copia[j] = rodadas[i].ata[j];
    }
    return copia;
}

Atleta **get_copia_tecnicos_rodada(int i){
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