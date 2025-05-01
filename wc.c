#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>

bool mode_bytes = false;
bool mode_chars = false;
bool mode_lines = false;
bool mode_mllen = false;
bool mode_words = false;

int intlen(int number) {
    // return ceil(log10(number+1));
    char buffer[10];
    sprintf(buffer, "%d", number);
    return strlen(buffer);
}

void wc(char **list, int count) {
    char buffer[BUFSIZ];
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
    for (int i = 0; i < count; i++) {
        items[i] = calloc(5, sizeof(int));
    }
    int len;
    for (int i = 1; i < count; i++) {
        FILE *stream = fopen(list[i], "r");
        if (stream == NULL) {
            perror("Erro ao abrir o arquivo");
            exit(EXIT_FAILURE);
        }
        while (fgets(buffer, BUFSIZ, stream) != NULL) {
            /* bytes     */
            if (mode_bytes)
                items[i][3] += strlen(buffer);
            char *str;
            /* words     */
            if (mode_words) {
                str = strdup(buffer);
                while (strtok_r(str, " \t\n", &str) != NULL)
                    items[i][1]++;
            }
            /* max-line-length     */
            if (mode_mllen) {
                str = strdup(buffer);
                len = strlen(str);
                if (len > 0 && str[len-1] == '\n')
                    str[len-1] = '\0';
                len = strlen(str);
                items[i][4] = items[i][4] > len ? items[i][4] : len;
            }
            /* lines     */
            if (mode_lines)
                items[i][0]++;
            /* chars
            if (mode_chars)             */
        }
        if (strcmp(list[i], "/dev/stdin") == 0)
            list[i] = "";
        items[0][0] += items[i][0];
        items[0][1] += items[i][1];
        items[0][3] += items[i][3];
        fclose(stream);
    }

    for (int i = 1; i < count; i++)
        items[0][4] = items[0][4] > items[i][4] ? items[0][4] : items[i][4];
                                            /*  Ordem das colunas   */
    for (int i = 1; i < count; i++) {       /*  -l lines -w words -m chars -c bytes -L max-line-length   */
        if (mode_lines)
            printf("%*d ", intlen(items[0][0]), items[i][0]);
        if (mode_words)
            printf("%*d ", intlen(items[0][1]), items[i][1]);
        if (mode_chars)
            printf("%*d ", intlen(items[0][2]), items[i][2]);
        if (mode_bytes)
            printf("%*d ", intlen(items[0][3]), items[i][3]);
        if (mode_mllen)
            printf("%*d ", intlen(items[0][4]), items[i][4]);
        printf("%s\n", list[i]);
    }
    if (count > 2) {                        /*  Print total */
        if (mode_lines)
            printf("%*d ", intlen(items[0][0]), items[0][0]);
        if (mode_words)
            printf("%*d ", intlen(items[0][1]), items[0][1]);
        if (mode_chars)
            printf("%*d ", intlen(items[0][2]), items[0][2]);
        if (mode_bytes)
            printf("%*d ", intlen(items[0][3]), items[0][3]);
        if (mode_mllen)
            printf("%*d ", intlen(items[0][4]), items[0][4]);
        printf("total\n");
    }
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
