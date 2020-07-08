#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "cartola_utils.h"
#include "algoritmo_genetico.h"

void algoritmo_genetico();
int imprimir_menu_inicial();
int ler_dados_campeonato();
int imprimir_menu_principal();
int read_number();

int main(int argc, char *argv[]){

    definir_seed(time(NULL));
    //srand(time(NULL));

    if(!imprimir_menu_inicial()) return 0;

    if(!ler_dados_campeonato()) return 0;

    imprimir_menu_principal();

    //imprimir_todos_atletas();
    
    limpar_memoria_alocada_cartola();
    limpar_memoria_alocada_ag();

    return 0;
}

void algoritmo_genetico(){

    //tenho uma população!
    //tenho dados do campeonato para simular rodadas!

    //avalia população inicial
    avalia_populacao();

    //começa a produzir um gráfico de fitness para facilitar o estudo
    atualizar_grafico_fitness();

    int quer_evoluir = 0, repeticoes = 0;

    while(1){

        printf("Gostaria de evoluir quantas vezes? ");
        repeticoes = read_number();

        if(repeticoes == 0) break;

        printf("evoluindo %d vezes:\n\n", repeticoes);

        for(int i=0; i<repeticoes; i++){
            evolucao();
        }
        
    }

}

int imprimir_menu_inicial(){
    
    int opc;

    printf("----------------------------------------------\n");
    printf("\t\tDARWIN_BOT_FC\n");
    printf("----------------------------------------------\n\n");    

    do{
        printf("----------------------------------------------\n");
        printf("0 - Sair;\n");
        printf("1 - Recuperar população já existente;\n");
        printf("2 - Criar nova população;\n");
        printf("----------------------------------------------\n");
        printf("Digite uma opção: ");
        opc = read_number();
        switch(opc){
            case 0:
                return 0;
            case 1:
                //recupera_backup_populacao();
                printf("TODO: recuperar população armazenada");
                break;
            case 2:
                printf("CUIDADO -> lembre-se de ter um backup\n");
                //scanf("%*s");
                //Cria uma nova população
                big_bang();
                criar_grafico_fitness();
                break;
            default:
                printf("Essa opção não existe!\n");
                break;
        }
    }while(opc<0 || opc>2);

    return 1;

}

int ler_dados_campeonato(){
    return ler_csv_campeonato();
}

int imprimir_menu_principal(){

    int opc;

    do{
        printf("----------------------------------------------\n");
        printf("0 - Sair;\n");
        printf("1 - Simular algoritmo evolutivo;\n");
        printf("2 - Imprimir_dados_atletas();\n");
        printf("----------------------------------------------\n");
        printf("Digite uma opção: ");
        opc = read_number();
        switch(opc){
            case 0:
                return 0;
            case 1:
                //----------------------------------------------------------
                //AG;
                algoritmo_genetico();
                //----------------------------------------------------------
                break;
            case 2:
                imprimir_todos_atletas();
                break;
            default:
                printf("Essa opção não existe!\n");
                break;
        }
    }while(opc<0 || opc>2);

    return 1;

}

/*Função read_number:
 Lê um número conferindo se realmente é um número digitado pelo usuário;
 @Retorno:
 -O número inteiro lido*/
int read_number(){
	char input[1000], c;
	input[0] = '\0';
	int i, repeat, size, num=0;
	do{
		scanf("%[^\n]", input);
		if(input[0] != '\n'){
			scanf("%c", &c);
		}
		size = strlen(input);
		i=0;
		repeat=0;
		while(i<size){
			if(!isdigit(input[i++])){
				repeat=1;
			}	
		}
		if(repeat){
			printf("ERRO --> ENTRADA INVÁLIDA!\n");
			printf("Por favor, digite um número inteiro = ");
		}
	}while(repeat);
	num = atoi(input);
	if(input[0] == '\n'){
		return -1;
	}
	return num;	
}