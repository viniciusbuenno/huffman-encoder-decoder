#ifndef TABELA_DE_FREQUENCIAS_H
#define TABELA_DE_FREQUENCIAS_H

#include "meustipos.h"

typedef struct tabela_de_frequencias {
    Ptr_de_no_de_arvore_binaria vetor[256];
    U16 quantidade_de_posicoes_preenchidas;
} Tabela_de_frequencias;

void nova_tabela_de_frequencias(Tabela_de_frequencias* tab);
int inclua_byte(U8 byte, Tabela_de_frequencias* tab);
void junte_nodos_no_inicio_do_vetor(Tabela_de_frequencias* tab);
void liberar_no_arvore(Ptr_de_no_de_arvore_binaria no);
int novo_no_de_arvore_binaria(Ptr_de_no_de_arvore_binaria* no, Ptr_de_no_de_arvore_binaria esq, Elemento info, Ptr_de_no_de_arvore_binaria dir);

#endif