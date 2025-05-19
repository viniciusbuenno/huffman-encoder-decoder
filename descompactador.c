#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "meustipos.h"
#include "tabela_de_frequencias.h"
#include "codigo.h"

void liberar_arvore(Ptr_de_no_de_arvore_binaria raiz) {
    if (!raiz) return;
    liberar_arvore(raiz->esq);
    liberar_arvore(raiz->dir);
    free(raiz);
}

Ptr_de_no_de_arvore_binaria reconstruir_arvore_huffman(U8* bytes, unsigned int* freq, U16 num) {
    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);

    for (U16 i = 0; i < num; i++) {
        for (unsigned int j = 0; j < freq[i]; j++) {
            inclua_byte(bytes[i], &tab);
        }
    }

    while (tab.quantidade_de_posicoes_preenchidas > 1) {
        junte_nodos_no_inicio_do_vetor(&tab);
        Elemento elem;
        elem.frequencia = tab.vetor[0]->informacao.frequencia + tab.vetor[1]->informacao.frequencia;
        elem.byte = 0;
        Ptr_de_no_de_arvore_binaria novo;
        if (!novo_no_de_arvore_binaria(&novo, tab.vetor[0], elem, tab.vetor[1])) {
            return NULL;
        }
        tab.vetor[0] = novo;
        tab.vetor[1] = NULL;
        tab.quantidade_de_posicoes_preenchidas--;
    }
    return tab.vetor[0];
}

void descompactar_arquivo(const char* entrada_nome, const char* saida_nome) {
    FILE* entrada = fopen(entrada_nome, "rb");
    if (!entrada) {
        printf("Erro ao abrir o arquivo compactado\n");
        return;
    }

    U16 num_bytes_distintos;
    fread(&num_bytes_distintos, sizeof(U16), 1, entrada);

    U8 bits_validos_ultimo_byte;
    fread(&bits_validos_ultimo_byte, sizeof(U8), 1, entrada);

    U8 bytes[256];
    unsigned int frequencias[256];
    for (U16 i = 0; i < num_bytes_distintos; i++) {
        fread(&bytes[i], sizeof(U8), 1, entrada);
        fread(&frequencias[i], sizeof(unsigned int), 1, entrada);
    }

    Ptr_de_no_de_arvore_binaria raiz = reconstruir_arvore_huffman(bytes, frequencias, num_bytes_distintos);
    if (!raiz) {
        printf("Erro ao reconstruir a árvore de Huffman\n");
        fclose(entrada);
        return;
    }

    FILE* saida = fopen(saida_nome, "wb");
    if (!saida) {
        printf("Erro ao abrir o arquivo de saída\n");
        liberar_arvore(raiz);
        fclose(entrada);
        return;
    }

    Ptr_de_no_de_arvore_binaria atual = raiz;
    U8 byte_lido;
    size_t bytes_lidos;
    fseek(entrada, 0, SEEK_END);
    long tamanho_arquivo = ftell(entrada);
    long pos_dados = sizeof(U16) + sizeof(U8) + num_bytes_distintos * (sizeof(U8) + sizeof(unsigned int));
    long tamanho_dados = tamanho_arquivo - pos_dados;
    fseek(entrada, pos_dados, SEEK_SET);

    long total_bits = (tamanho_dados - 1) * 8 + bits_validos_ultimo_byte;
    long bits_lidos = 0;

    while (bits_lidos < total_bits) {
        fread(&byte_lido, sizeof(U8), 1, entrada);
        for (int i = 7; i >= 0 && bits_lidos < total_bits; i--) {
            int bit = (byte_lido >> i) & 1;
            atual = bit == 0 ? atual->esq : atual->dir;

            if (atual->esq == NULL && atual->dir == NULL) {
                fwrite(&atual->informacao.byte, sizeof(U8), 1, saida);
                atual = raiz;
            }
            bits_lidos++;
        }
    }

    printf("Descompactacao concluida. Bits lidos: %ld\n", bits_lidos);

    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_compactado> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    descompactar_arquivo(argv[1], argv[2]);
    return 0;
}
