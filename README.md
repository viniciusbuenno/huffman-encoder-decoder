# huffman-encoder-decoder
Algoritmo de Huffman para compactação e descompactação de arquivos.


# Algoritmo de Huffman para Compactação e Descompactação de Arquivos

Este projeto implementa o algoritmo de **Huffman** em linguagem C, permitindo a **compactação** e **descompactação** de arquivos binários. A compressão é baseada na frequência de ocorrência de bytes, atribuindo códigos binários menores aos mais frequentes, reduzindo o tamanho total do arquivo.

---

📁 Estrutura do Projeto

- `compactador.c`: Realiza a leitura do arquivo original, calcula frequências, gera a árvore de Huffman, cria os códigos e grava o arquivo compactado.
- `descompactador.c`: Lê o arquivo compactado, reconstrói a árvore de Huffman e decodifica os dados para gerar o arquivo original.
- `codigo.c` / `codigo.h`: Manipula estruturas de código binário (sequência de bits) para representar os bytes codificados.
- `tabela_de_frequencias.c` / `tabela_de_frequencias.h`: Gera a tabela de frequências e constrói a árvore de Huffman.
- `meustipos.h`: Contém definições de tipos auxiliares como `U8`, `U16`, `U64`, etc.

---

⚙️ Compilação

É possível compilar os arquivos utilizando `gcc`:

```
gcc compactador.c codigo.c tabela_de_frequencias.c -o compactador
gcc descompactador.c codigo.c tabela_de_frequencias.c -o descompactador

```
---

🚀 Uso

./compactador entrada.txt saida.huff

./descompactador entrada.huff recuperado.txt

--

🧠 Como Funciona?

Leitura do Arquivo Original: Lê byte a byte e conta a frequência de cada um.

Construção da Árvore de Huffman: A partir das frequências, é criada uma árvore binária onde os bytes mais frequentes ficam mais próximos da raiz.

Geração dos Códigos: Cada byte recebe um código binário único com base no caminho na árvore.

Codificação: Os bytes do arquivo original são substituídos pelos seus códigos binários.

Gravação do Arquivo Compactado: Escreve o cabeçalho com a tabela de frequências e os dados codificados.

Descompactação: Reverte o processo reconstruindo a árvore e lendo bit a bit.

---

✅ Requisitos

Compilador C compatível (gcc, clang, etc.)

Sistema que permita manipulação de arquivos binários (Linux, Windows ou macOS)

---

🧹 Gerenciamento de Memória

O código foi desenvolvido com atenção à liberação de memória, utilizando free() para limpar buffers e estruturas como a árvore de Huffman e vetores dinâmicos após o uso.

---

📜 Licença

Este projeto foi desenvolvido com fins educacionais e pode ser reutilizado ou adaptado livremente.

---

👨‍💻 Autor

Desenvolvido por Vinicius Bueno como parte de um projeto de estudo em Estruturas de Dados e Algoritmos em C.
