#include <stdlib.h>
#include "tabela_de_frequencias.h"

void nova_tabela_de_frequencias(Tabela_de_frequencias* tab) {
    for (U16 i = 0; i < 256; i++) {
        tab->vetor[i] = NULL;
    }
    tab->quantidade_de_posicoes_preenchidas = 0;
}

int inclua_byte(U8 byte, Tabela_de_frequencias* tab) {
    if (tab->vetor[byte] == NULL) {
        tab->vetor[byte] = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(No_de_arvore_binaria));
        if (tab->vetor[byte] == NULL) return 0;
        tab->vetor[byte]->esq = NULL;
        tab->vetor[byte]->dir = NULL;
        tab->vetor[byte]->informacao.byte = byte;
        tab->vetor[byte]->informacao.frequencia = 0;
        tab->quantidade_de_posicoes_preenchidas++;
    }
    tab->vetor[byte]->informacao.frequencia++;
    return 1;
}

void junte_nodos_no_inicio_do_vetor(Tabela_de_frequencias* tab) {
    U16 primeiro_NULL, primeiro_nao_NULL;
    for(;;) {
        primeiro_NULL = 0;
        while (primeiro_NULL < 256 && tab->vetor[primeiro_NULL] != NULL)
            primeiro_NULL++;
        if (primeiro_NULL == 256) break;
        primeiro_nao_NULL = primeiro_NULL + 1;
        while (primeiro_nao_NULL < 256 && tab->vetor[primeiro_nao_NULL] == NULL)
            primeiro_nao_NULL++;
        if (primeiro_nao_NULL == 256) break;
        tab->vetor[primeiro_NULL] = tab->vetor[primeiro_nao_NULL];
        tab->vetor[primeiro_nao_NULL] = NULL;
    }
}

void liberar_no_arvore(Ptr_de_no_de_arvore_binaria no) {
    if (no == NULL) return;
    liberar_no_arvore(no->esq);
    liberar_no_arvore(no->dir);
    free(no);
}

int novo_no_de_arvore_binaria(Ptr_de_no_de_arvore_binaria* no, Ptr_de_no_de_arvore_binaria esq, Elemento info, Ptr_de_no_de_arvore_binaria dir) {
    *no = (Ptr_de_no_de_arvore_binaria)malloc(sizeof(No_de_arvore_binaria));
    if (*no == NULL) return 0;
    (*no)->esq = esq;
    (*no)->dir = dir;
    (*no)->informacao = info;
    return 1;
}