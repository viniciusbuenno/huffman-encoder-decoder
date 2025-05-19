#ifndef MEUSTIPOS_H
#define MEUSTIPOS_H

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned long long U64;

typedef struct elemento {
    U8 byte;
    U64 frequencia;
} Elemento;

typedef struct no_de_arvore_binaria {
    struct no_de_arvore_binaria* esq;
    Elemento informacao;
    struct no_de_arvore_binaria* dir;
} No_de_arvore_binaria;

typedef No_de_arvore_binaria* Ptr_de_no_de_arvore_binaria;

#endif