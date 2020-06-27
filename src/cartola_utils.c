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

struct time{
    int mando_de_campo[MAX_RODADAS];
};


Rodada *rodadas; //Vetor de rodadas do campeonato
Atleta **atletas; //Vetor com todos os atletas do jogo
int n_atletas = 0;
int id_atleta_atual = -1;

Time *times; //Vetor com todos os times
#define MAX_ID_TIMES 356
#define MIN_ID_TIMES 262


int ler_csv_campeonato(){

    char nome_csv[100];
    printf("Nome do arquivo .csv com dados dos atletas: ");
    scanf("%s", nome_csv);

    FILE *csv = fopen(nome_csv, "r");
    
    if(!csv){
        printf("ERRO! FALHA AO ABRIR ARQUIVO CSV!\n");
        return 0;
    }

    printf("Nome do arquivo .csv com a tabela: ");
    scanf("%s", nome_csv);

    FILE *tabela = fopen(nome_csv, "r");
    
    if(!tabela){
        printf("ERRO! FALHA AO ABRIR ARQUIVO DA TABELA!\n");
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

    //inicializa todos os times
    times = (Time *) calloc(MAX_ID_TIMES - MIN_ID_TIMES, sizeof(Time));

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

        //printf("linhas: %d, atletas: %d\n", ++j, n_atletas);
        
    }

    fclose(csv);
    
    //pula a primeira linha do csv, header
    fscanf(tabela, "%*[^\n]%*c");

    while((teste = fgetc(tabela)) != EOF){

        fseek(tabela, -1, SEEK_CUR);

        ler_linha_csv_tabela(tabela);
        
    }

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

void ler_linha_csv_tabela(FILE *csv){

    int id;
    int rodada;
    int mando;

    fscanf(csv, "%*[^,]%*c%d%*c%d%*c%d%*[^\n]%*c", &id, &rodada, &mando);

    times[id-MIN_ID_TIMES].mando_de_campo[rodada] = mando;

    //printf("Time %d na rodada %d jogou em casa? %d ;\n", id, rodada, mando);

}

void limpar_memoria_alocada_cartola(){

    for(int i=0; i<MAX_ATLETAS; i++){
        free(atletas[i]);
    }
    free(atletas);

    free(rodadas);

    free(times);

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
        if(comparador(aux[i], aux[j], rodada))
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