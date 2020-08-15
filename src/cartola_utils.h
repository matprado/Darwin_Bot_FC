#ifndef CARTOLA_UTILS_H
#define CARTOLA_UTILS_H

#include <stdio.h>

#define MAX_GOLEIROS 200
#define MAX_LATERAIS 300
#define MAX_ZAGUEIROS 300
#define MAX_MEIAS 500
#define MAX_ATACANTES 500
#define MAX_TECNICOS 40
#define MAX_ATLETAS 2000
#define MAX_RODADAS 39

typedef struct rodada Rodada;
typedef struct atleta Atleta;
typedef struct time Time;

typedef struct time_escolhido TimeEscolhido;

float get_pontuacao_media(int quant_pos[6], int pref_gol[3], int pref_zag[3], int pref_lat[3], int pref_mei[3], int pref_ata[3], int capitao);

float escala_campeonato(Rodada *rodadas, Atleta **atletas, int n_rodadas, int quant_pos[6], int pref_gol[3], int pref_zag[3], int pref_lat[3], int pref_mei[3], int pref_ata[3], int capitao);

float escolhe_atletas_rodada(int qtd_atl, int total, Atleta **atletas, int rodada, 
    int (*comparador1)(Atleta *, Atleta *, int), 
    int (*comparador2)(Atleta *, Atleta *, int), 
    int (*comparador3)(Atleta *, Atleta *, int),
    float *cartoletas, int *faltam, float *valorizacao, int eh_capitao, TimeEscolhido *times);

float escolhe_tecnico_rodada(int total, Atleta **atletas, int rodada, float *cartoletas, float *valorizacao, int eh_capitao, TimeEscolhido *times);

int ler_csv_campeonato();
void ler_linha_csv(FILE *csv, Rodada *rodadas, Atleta **atletas);
void limpar_memoria_alocada_cartola();
void imprimir_atleta_rodada(Atleta *atleta, int rodada);
void imprimir_atleta_campeonato(Atleta *atleta);
void imprimir_todos_atletas();

int compara_time_mais_escalado(TimeEscolhido a, TimeEscolhido b);
int compara_media_maior(Atleta *a, Atleta *b, int rodada);
int compara_preco_maior(Atleta *a, Atleta *b, int rodada);
int compara_valorizacao_maior(Atleta *a, Atleta *b, int rodada);
int compara_valorizacao_menor(Atleta *a, Atleta *b, int rodada);
int compara_sg_maior(Atleta *a, Atleta *b, int rodada);
int compara_gs_menor(Atleta *a, Atleta *b, int rodada);
int compara_dd_maior(Atleta *a, Atleta *b, int rodada);
int compara_dp_maior(Atleta *a, Atleta *b, int rodada);
int compara_ca_menor(Atleta *a, Atleta *b, int rodada);
int compara_fs_maior(Atleta *a, Atleta *b, int rodada);
int compara_pe_menor(Atleta *a, Atleta *b, int rodada);
int compara_rb_maior(Atleta *a, Atleta *b, int rodada);
int compara_fc_menor(Atleta *a, Atleta *b, int rodada);
int compara_ff_maior(Atleta *a, Atleta *b, int rodada);
int compara_fd_maior(Atleta *a, Atleta *b, int rodada);
int compara_ft_maior(Atleta *a, Atleta *b, int rodada);
int compara_g_maior(Atleta *a, Atleta *b, int rodada);
int compara_a_maior(Atleta *a, Atleta *b, int rodada);
int compara_i_menor(Atleta *a, Atleta *b, int rodada);

int (*get_comparador_gol(int id))(Atleta *, Atleta *, int);
int get_n_ids_gol();
int (*get_comparador_zag(int id))(Atleta *, Atleta *, int);
int get_n_ids_zag();
int (*get_comparador_lat(int id))(Atleta *, Atleta *, int);
int get_n_ids_lat();
int (*get_comparador_mei(int id))(Atleta *, Atleta *, int);
int get_n_ids_mei();
int (*get_comparador_ata(int id))(Atleta *, Atleta *, int);
int get_n_ids_ata();

void ordena_vetor_rodada(Atleta **vet, int tam, int (*comparador)(Atleta *, Atleta *, int), int rodada);
void merge_sort(Atleta **vet, int ini, int fim, int (*comparador)(Atleta *, Atleta *, int), int rodada);
void merge(Atleta **vet, int ini, int meio, int fim, int (*comparador)(Atleta *, Atleta *, int), int rodada);

Atleta **get_copia_goleiros_rodada(int i, Rodada *rodadas);
Atleta **get_copia_zagueiros_rodada(int i, Rodada *rodadas);
Atleta **get_copia_laterais_rodada(int i, Rodada *rodadas);
Atleta **get_copia_meias_rodada(int i, Rodada *rodadas);
Atleta **get_copia_atacantes_rodada(int i, Rodada *rodadas);
Atleta **get_copia_tecnicos_rodada(int i, Rodada *rodadas);

void merge_sort_2(TimeEscolhido *vet, int ini, int fim, int (*comparador)(TimeEscolhido , TimeEscolhido));
void merge_2(TimeEscolhido *vet, int ini, int meio, int fim, int (*comparador)(TimeEscolhido, TimeEscolhido));

#endif