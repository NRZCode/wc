#include <getopt.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIELDS 5

bool mode_bytes = false;
bool mode_chars = false;
bool mode_lines = false;
bool mode_mllen = false;
bool mode_words = false;

enum { TOTAL };
enum { LINES, WORDS, CHARS, BYTES, MLLEN };

int intlen(int number) {
    // return ceil(log10(number+1));
    char buffer[10];
    sprintf(buffer, "%d", number);
    return strlen(buffer);
}

void wc(char **list, int count) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    char buffer[BUFSIZ];
    size_t mblen;
    /**
     *  Items = [
     *      [0] => lines,
     *      [1] => words,
     *      [2] => chars,
     *      [3] => bytes,
     *      [4] => max-line-length
     *  ]
     */
    int **items = malloc(count * sizeof(int *));
    if (!items) {
        perror("Erro na alocação de items");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; i++) {
        items[i] = calloc(FIELDS, sizeof(int));
        if (!items[i]) {
            perror("Erro na alocação de items[i]");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 1; i < count; i++) {
        FILE *stream = fopen(list[i], "r");
        if (stream == NULL) {
            perror("Erro ao abrir o arquivo");
            exit(EXIT_FAILURE);
        }
        while (fgets(buffer, BUFSIZ, stream) != NULL) {
            /* bytes     */
            if (mode_bytes)
                items[i][BYTES] += strlen(buffer);
            char *str;
            /* words     */
            if (mode_words) {
                str = strdup(buffer);
                while (strtok_r(str, " \t\n", &str) != NULL)
                    items[i][WORDS]++;
            }
            /* chars                        */
            if (mode_chars) {
                str = strdup(buffer);
                mblen = mbstowcs(NULL, str, 0);
                if (mblen == (size_t)-1) {
                    perror("Erro ao converter multibyte");
                    exit(EXIT_FAILURE);
                }
                if (mblen > INT_MAX) {
                    fprintf(stderr, "Erro valor maior que INT_MAX!\n");
                    exit(EXIT_FAILURE);
                }
                items[i][CHARS] += (int)mblen;
            }
            /* max-line-lenght              */
            if (mode_mllen) {
                str = strdup(buffer);
                int len = strlen(str);
                if (len > 0 && str[len-1] == '\n')
                    str[len-1] = '\0';
                mblen = mbstowcs(NULL, str, 0);
                if (mblen == (size_t)-1) {
                    perror("Erro ao converter multibyte");
                    exit(EXIT_FAILURE);
                }
                if (mblen > INT_MAX) {
                    fprintf(stderr, "Erro valor maior que INT_MAX!\n");
                    exit(EXIT_FAILURE);
                }
                items[i][MLLEN] = items[i][MLLEN] > (int)mblen ? items[i][MLLEN] : (int)mblen;
            }
            /* lines     */
            if (mode_lines)
                items[i][LINES]++;
        }
        if (strcmp(list[i], "/dev/stdin") == 0)
            list[i] = "";
        items[TOTAL][LINES] += items[i][LINES];
        items[TOTAL][WORDS] += items[i][WORDS];
        items[TOTAL][BYTES] += items[i][BYTES];
        items[TOTAL][CHARS] += items[i][CHARS];
        fclose(stream);
    }
    for (int i = 1; i < count; i++)
        items[TOTAL][MLLEN] = items[TOTAL][MLLEN] > items[i][MLLEN] ? items[TOTAL][MLLEN] : items[i][MLLEN];
                                            /*  Ordem das colunas   */
    for (int i = 1; i < count; i++) {       /*  -l lines -w words -m chars -c bytes -L max-line-length   */
        if (mode_lines)
            printf("%*d ", intlen(items[TOTAL][LINES]), items[i][LINES]);
        if (mode_words)
            printf("%*d ", intlen(items[TOTAL][WORDS]), items[i][WORDS]);
        if (mode_chars)
            printf("%*d ", intlen(items[TOTAL][CHARS]), items[i][CHARS]);
        if (mode_bytes)
            printf("%*d ", intlen(items[TOTAL][BYTES]), items[i][BYTES]);
        if (mode_mllen)
            printf("%*d ", intlen(items[TOTAL][MLLEN]), items[i][MLLEN]);
        printf("%s\n", list[i]);
    }
    if (count > 2) {                        /*  Print total */
        if (mode_lines)
            printf("%*d ", intlen(items[TOTAL][LINES]), items[TOTAL][LINES]);
        if (mode_words)
            printf("%*d ", intlen(items[TOTAL][WORDS]), items[TOTAL][WORDS]);
        if (mode_chars)
            printf("%*d ", intlen(items[TOTAL][CHARS]), items[TOTAL][CHARS]);
        if (mode_bytes)
            printf("%*d ", intlen(items[TOTAL][BYTES]), items[TOTAL][BYTES]);
        if (mode_mllen)
            printf("%*d ", intlen(items[TOTAL][MLLEN]), items[TOTAL][MLLEN]);
        printf("total\n");
    }
    for (int i = 0; i < count; i++) {
        free(items[i]);
    }
    free(items);
}

int main(int argc, char **argv) {
    int opt;
    char *list[2];
    // short_options = "c,    m,    l,    L,                                 w"
    // long_options  = "bytes,chars,lines,max-line-length,total,help,version,words"
    while ((opt = getopt(argc, argv, "cmlLw")) != -1) {
        switch (opt) {
            case 'c':
                mode_bytes = true;
                break;
            case 'm':
                mode_chars = true;
                break;
            case 'l':
                mode_lines = true;
                break;
            case 'L':
                mode_mllen = true;
                break;
            case 'w':
                mode_words = true;
                break;
            default:
                abort();
        }
    }
    /* Default mode */
    if ((mode_bytes || mode_chars || mode_lines || mode_mllen || mode_words) == 0) {
        mode_bytes = true;
        mode_lines = true;
        mode_words = true;
    }
    if (argc < 2) {
        list[1] = "/dev/stdin";
        wc(list, 2);
    } else {
        optind--;
        wc(argv + optind, argc - optind);
    }
    return EXIT_SUCCESS;
}
