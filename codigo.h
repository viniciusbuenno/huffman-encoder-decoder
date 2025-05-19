#ifndef CODIGO_H
#define CODIGO_H

#include <stdlib.h>
#include <stdbool.h>
#include "meustipos.h"

typedef struct codigo {
    U8* byte;
    U16 tamanho;
    U16 capacidade_em_bytes;
} Codigo;

bool novo_codigo(Codigo* c /* por referência */);
void free_codigo(Codigo* c /* por referência */);
bool adiciona_bit(Codigo* c /* por referência */, U8 valor /* 0 ou 1 */);
bool joga_fora_bit(Codigo* c /* por referência */);
void gerar_codigos(Ptr_de_no_de_arvore_binaria no, Codigo* codigos[], Codigo* codigo_atual);
bool clone(Codigo* original /* por ponteiro */, Codigo* copia /* por referência */);

#endif