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

int ler_csv_campeonato();
void ler_linha_csv(FILE *csv);
void ler_linha_csv_tabela(FILE *csv);
void limpar_memoria_alocada();
void imprimir_atleta_rodada(Atleta *atleta, int rodada);
void imprimir_atleta_campeonato(Atleta *atleta);
void imprimir_todos_atletas();

#endif