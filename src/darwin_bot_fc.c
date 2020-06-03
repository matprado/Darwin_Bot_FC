#include <stdio.h>
#include <stdlib.h>
#include "cartola_utils.h"
#include "algorimo_genetico.h"



int main(int argc, char *argv[]){

    ler_csv_campeonato();

    imprimir_todos_atletas();

    limpar_memoria_alocada();

    return 0;
}