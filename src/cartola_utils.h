#ifndef CARTOLA_UTILS_H
#define CARTOLA_UTILS_H

#define MAX_GOLEIROS 100
#define MAX_LATERAIS 150
#define MAX_ZAGUEIROS 150
#define MAX_MEIAS 300
#define MAX_ATACANTES 300
#define MAX_TECNICOS 20
#define MAX_ATLETAS MAX_GOLEIROS + MAX_LATERAIS + MAX_ZAGUEIROS + MAX_MEIAS + MAX_ATACANTES + MAX_TECNICOS
#define MAX_RODADAS 39

typedef struct rodada Rodada;
typedef struct atleta Atleta;

int ler_csv_campeonato();
void ler_linha_csv(FILE *csv);

#endif