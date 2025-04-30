# wc

wc - Imprime a contagem de novas linhas, palavras e bytes de cada arquivo

Uma cópia da ferramenta da suíte GNU Coreutils, wc (word counter) escrita em C (fins didáticos de aprendizado da linguagem).

```help
Uso: wc [OPÇÃO]... [ARQUIVO]...
 ou: wc [OPÇÃO]... --files0-from=A
Imprime totais de novas linhas, palavras e bytes para cada FICHEIRO, e uma linha
de totais se mais de um FICHEIRO for especificado. Uma palavra é uma sequência
não-vazia de espaços não-brancos delimitada por espaços ou pelo início ou fim da entrada.

Se ARQUIVO não for especificado ou for -, lê a entrada padrão.

As opções abaixo podem ser usadas para selecionar qual quantidade será
mostrada, sempre na seguinte ordem: linha, palavra, byte, tamanho máximo
de linha.
  -c, --bytes            mostra a quantidade de bytes
  -m, --chars            mostra a quantidade de caracteres
  -l, --lines            mostra a quantidade de linhas
     --files0-from=A     lê a entrada dos arquivos especificados pelos nomes
                           terminados com NULO contidos no arquivo A;
                           Se A for -, lê nomes da entrada padrão
  -L, --max-line-length  emite o comprimento da linha mais longa
  -w, --words            emite a quantidade de palavras
      --total=QUANDO     quando imprimir uma linha com totais;
                           QUANDO pode ser: auto, always, only, never
      --help        mostra esta ajuda e sai
      --version     mostra informação da versão e sai

Página de ajuda do GNU coreutils: <https://www.gnu.org/software/coreutils/>
Relate erros de tradução para <https://translationproject.org/team/pt_BR.html>
Documentação completa em <https://www.gnu.org/software/coreutils/wc>
ou disponível localmente via: info "(coreutils) wc invocation"
```

```man
WC(1)                                                        User Commands                                                        WC(1)

NAME
       wc - print newline, word, and byte counts for each file

SYNOPSIS
       wc [OPTION]... [FILE]...
       wc [OPTION]... --files0-from=F

DESCRIPTION
       Print  newline,  word, and byte counts for each FILE, and a total line if more than one FILE is specified.  A word is a nonempty
       sequence of non white space delimited by white space characters or by start or end of input.

       With no FILE, or when FILE is -, read standard input.

       The options below may be used to select which counts are printed, always in the following order: newline, word, character, byte,
       maximum line length.

       -c, --bytes
              print the byte counts

       -m, --chars
              print the character counts

       -l, --lines
              print the newline counts

       --files0-from=F
              read input from the files specified by NUL-terminated names in file F; If F is - then read names from standard input

       -L, --max-line-length
              print the maximum display width

       -w, --words
              print the word counts

       --total=WHEN
              when to print a line with total counts; WHEN can be: auto, always, only, never

       --help display this help and exit

       --version
              output version information and exit

AUTHOR
       Written by Paul Rubin and David MacKenzie.

REPORTING BUGS
       GNU coreutils online help: <https://www.gnu.org/software/coreutils/>
       Report any translation bugs to <https://translationproject.org/team/>

SEE ALSO
       Full documentation <https://www.gnu.org/software/coreutils/wc>
       or available locally via: info '(coreutils) wc invocation'

       Packaged by Debian (9.7-2)
       Copyright © 2025 Free Software Foundation, Inc.
       License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.
       This is free software: you are free to change and redistribute it.
       There is NO WARRANTY, to the extent permitted by law.

GNU coreutils 9.7                                              April 2025                                                         WC(1)
```
