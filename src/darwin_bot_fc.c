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
    
    printf("MELHOR INDIVÍDUO COM FITNESS = %f\n", get_fitness_melhor());
    imprimir_melhor_cromossomo();

    armazenar_dados_ag();

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

    int repeticoes = 0;

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
                recuperar_dados_ag();
                incrementar_grafico_fitness();
                break;
            case 2:
                printf("CUIDADO -> lembre-se de ter um backup\n");
                scanf("%*c");
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
    int rodada;
    float cartoletas;

    do{
        printf("----------------------------------------------\n");
        printf("0 - Sair;\n");
        printf("1 - Simular algoritmo evolutivo;\n");
        printf("2 - Testar Melhor de Todos;\n");
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
                printf("Digite a rodada: ");
                scanf("%d", &rodada);
                printf("Quantas cartoletas? : ");
                scanf("%f", &cartoletas);
                testar_melhor_de_todos(rodada, cartoletas);
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