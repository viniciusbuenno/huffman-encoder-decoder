#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "codigo.h"

bool novo_codigo(Codigo* c) {
    c->byte = (U8*)calloc(1, sizeof(U8));
    if (c->byte == NULL) return false;
    c->tamanho = 0;
    c->capacidade_em_bytes = 1;
    return true;
}

void free_codigo(Codigo* c) {
    if (c->byte != NULL) {
        free(c->byte);
        c->byte = NULL;
    }
    c->tamanho = 0;
    c->capacidade_em_bytes = 0;
}

bool adiciona_bit(Codigo* c, U8 valor) {
    if (c->tamanho / 8 >= c->capacidade_em_bytes) {
        U16 nova_capacidade = c->capacidade_em_bytes * 2;
        U8* novo = (U8*)realloc(c->byte, nova_capacidade * sizeof(U8));
        if (novo == NULL) return false;
        memset(novo + c->capacidade_em_bytes, 0, c->capacidade_em_bytes);
        c->byte = novo;
        c->capacidade_em_bytes = nova_capacidade;
    }
    if (valor) {
        c->byte[c->tamanho / 8] |= (1 << (7 - (c->tamanho % 8)));
    }
    c->tamanho++;
    return true;
}

bool joga_fora_bit(Codigo* c) {
    if (c->tamanho == 0) return false;
    c->tamanho--;
    if (c->tamanho % 8 == 0 && c->tamanho > 0) {
        c->byte[c->tamanho / 8] = 0;
    } else {
        c->byte[c->tamanho / 8] &= ~(1 << (7 - (c->tamanho % 8)));
    }
    return true;
}

bool clone(Codigo* original, Codigo* copia) {
    copia->byte = (U8*)malloc(original->capacidade_em_bytes * sizeof(U8));
    if (copia->byte == NULL) return false;
    memcpy(copia->byte, original->byte, original->capacidade_em_bytes);
    copia->tamanho = original->tamanho;
    copia->capacidade_em_bytes = original->capacidade_em_bytes;
    return true;
}

void gerar_codigos(Ptr_de_no_de_arvore_binaria no, Codigo* codigos[], Codigo* codigo_atual) {
    if (no == NULL) return;
    if (!no->esq && !no->dir) {
        // Alocar novo Codigo se necessário
        if (codigos[no->informacao.byte] == NULL) {
            codigos[no->informacao.byte] = (Codigo*)malloc(sizeof(Codigo));
            if (codigos[no->informacao.byte] == NULL) return;
            if (!novo_codigo(codigos[no->informacao.byte])) {
                free(codigos[no->informacao.byte]);
                codigos[no->informacao.byte] = NULL;
                return;
            }
        }
        // Clonar o código atual para o byte correspondente
        clone(codigo_atual, codigos[no->informacao.byte]);
        return;
    }
    if (no->esq) {
        adiciona_bit(codigo_atual, 0);
        gerar_codigos(no->esq, codigos, codigo_atual);
        joga_fora_bit(codigo_atual);
    }
    if (no->dir) {
        adiciona_bit(codigo_atual, 1);
        gerar_codigos(no->dir, codigos, codigo_atual);
        joga_fora_bit(codigo_atual);
    }
}