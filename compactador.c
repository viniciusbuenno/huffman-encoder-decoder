#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tabela_de_frequencias.h"
#include "meustipos.h"
#include "codigo.h"

void liberar_arvore_huffman(Ptr_de_no_de_arvore_binaria raiz) {
    if (raiz == NULL) return;
    liberar_arvore_huffman(raiz->esq);
    liberar_arvore_huffman(raiz->dir);
    free(raiz);
}

void compactar_arquivo(const char* arquivo_entrada, const char* arquivo_saida) {
    FILE* entrada = fopen(arquivo_entrada, "rb");
    if (!entrada) {
        printf("Erro ao abrir arquivo de entrada\n");
        return;
    }

    Tabela_de_frequencias tab;
    nova_tabela_de_frequencias(&tab);
    U8 byte;
    U64 total_bytes = 0;
    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        if (!inclua_byte(byte, &tab)) {
            fclose(entrada);
            return;
        }
        total_bytes++;
    }
    rewind(entrada);

    if (tab.quantidade_de_posicoes_preenchidas == 0) {
        fclose(entrada);
        return;
    }

    U16 num_bytes_distintos = 0;
    U8 bytes_distintos[256];
    unsigned int frequencias[256];
    for (U16 i = 0; i < 256; i++) {
        if (tab.vetor[i] != NULL) {
            bytes_distintos[num_bytes_distintos] = tab.vetor[i]->informacao.byte;
            frequencias[num_bytes_distintos] = (unsigned int)tab.vetor[i]->informacao.frequencia;
            num_bytes_distintos++;
        }
    }

    while (tab.quantidade_de_posicoes_preenchidas > 1) {
        junte_nodos_no_inicio_do_vetor(&tab);
        Elemento elem;
        elem.frequencia = tab.vetor[0]->informacao.frequencia + tab.vetor[1]->informacao.frequencia;
        elem.byte = 0;
        Ptr_de_no_de_arvore_binaria novo;
        if (!novo_no_de_arvore_binaria(&novo, tab.vetor[0], elem, tab.vetor[1])) {
            fclose(entrada);
            return;
        }
        tab.vetor[0] = novo;
        tab.vetor[1] = NULL;
        tab.quantidade_de_posicoes_preenchidas--;
    }
    Ptr_de_no_de_arvore_binaria raiz = tab.vetor[0];

    Codigo* tabela_codigos[256] = {NULL};
    Codigo codigo_atual;
    if (!novo_codigo(&codigo_atual)) {
        liberar_arvore_huffman(raiz);
        fclose(entrada);
        return;
    }
    gerar_codigos(raiz, tabela_codigos, &codigo_atual);

    FILE* saida = fopen(arquivo_saida, "wb");
    if (!saida) {
        for (U16 i = 0; i < 256; i++) {
            if (tabela_codigos[i] != NULL) {
                free_codigo(tabela_codigos[i]);
                free(tabela_codigos[i]);
            }
        }
        free_codigo(&codigo_atual);
        liberar_arvore_huffman(raiz);
        fclose(entrada);
        return;
    }

    fwrite(&num_bytes_distintos, sizeof(U16), 1, saida);
    U8 bits_validos = 0; // Será calculado depois
    fwrite(&bits_validos, sizeof(U8), 1, saida);
    for (U16 i = 0; i < num_bytes_distintos; i++) {
        fwrite(&bytes_distintos[i], sizeof(U8), 1, saida);
        fwrite(&frequencias[i], sizeof(unsigned int), 1, saida);
    }

    Codigo buffer;
    if (!novo_codigo(&buffer)) {
        fclose(saida);
        for (U16 i = 0; i < 256; i++) {
            if (tabela_codigos[i] != NULL) {
                free_codigo(tabela_codigos[i]);
                free(tabela_codigos[i]);
            }
        }
        free_codigo(&codigo_atual);
        liberar_arvore_huffman(raiz);
        fclose(entrada);
        return;
    }
    U64 bits_escritos = 0;
    while (fread(&byte, sizeof(U8), 1, entrada) == 1) {
        if (tabela_codigos[byte] == NULL) {
            fclose(saida);
            free_codigo(&buffer);
            for (U16 i = 0; i < 256; i++) {
                if (tabela_codigos[i] != NULL) {
                    free_codigo(tabela_codigos[i]);
                    free(tabela_codigos[i]);
                }
            }
            free_codigo(&codigo_atual);
            liberar_arvore_huffman(raiz);
            fclose(entrada);
            return;
        }
        for (U16 i = 0; i < tabela_codigos[byte]->tamanho; i++) {
            U8 bit = (tabela_codigos[byte]->byte[i / 8] >> (7 - (i % 8))) & 1;
            adiciona_bit(&buffer, bit);
            bits_escritos++;
            if (buffer.tamanho >= 8) {
                U8 byte_saida = buffer.byte[0];
                fwrite(&byte_saida, sizeof(U8), 1, saida);
                for (U16 j = 0; j < buffer.tamanho - 8; j++) {
                    buffer.byte[j] = buffer.byte[j + 1];
                }
                buffer.tamanho -= 8;
                buffer.byte[buffer.tamanho / 8] = 0;
            }
        }
    }
    if (buffer.tamanho > 0) {
        bits_validos = buffer.tamanho;
        fwrite(buffer.byte, sizeof(U8), (buffer.tamanho + 7) / 8, saida);
    }

    fseek(saida, sizeof(U16), SEEK_SET);
    fwrite(&bits_validos, sizeof(U8), 1, saida);

    fclose(saida);
    free_codigo(&buffer);
    for (U16 i = 0; i < 256; i++) {
        if (tabela_codigos[i] != NULL) {
            free_codigo(tabela_codigos[i]);
            free(tabela_codigos[i]);
        }
    }
    free_codigo(&codigo_atual);
    liberar_arvore_huffman(raiz);
    fclose(entrada);
    printf("Arquivo compactado: %s criado\n", arquivo_saida);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }
    compactar_arquivo(argv[1], argv[2]);
    return 0;
}
