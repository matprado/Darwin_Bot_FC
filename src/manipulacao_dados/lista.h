#ifndef LISTA_H
#define LISTA_H

#define ERRO -404
#define TIPO int  //Customização do tipo

typedef struct LISTA_ LISTA;
typedef struct NO_ NO;

/*Funções básicas:*/
LISTA *lista_criar(void);
void lista_apagar(LISTA *L);

/*Funções de inserção, busca e remoção:*/
TIPO lista_buscar_item(LISTA *L, TIPO chave);
//TIPO lista_buscar_recursivo(LISTA *L, TIPO chave);
int lista_buscar_posicao(LISTA *L, TIPO chave);
int lista_inserir_ordenadamente(LISTA *L, TIPO item);
int lista_remover_item(LISTA *L, TIPO chave);

/*Outras funções:*/
//void lista_imprimir(LISTA *L);
int lista_vazia(LISTA *L);
int lista_tamanho(LISTA *L);

#endif