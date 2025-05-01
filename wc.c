#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    int *lines = calloc(count, sizeof(int));
    int *words = calloc(count, sizeof(int));
    int *bytes = calloc(count, sizeof(int));
    int *chars = calloc(count, sizeof(int));
    int *mllen = calloc(count, sizeof(int));
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
                bytes[i] += strlen(buffer);
            char *str;
            /* words     */
            if (mode_words) {
                str = strdup(buffer);
                while (strtok_r(str, " \t\n", &str) != NULL)
                    words[i]++;
            }
            /* max-line-length     */
            if (mode_mllen) {
                str = strdup(buffer);
                len = strlen(str);
                if (len > 0 && str[len-1] == '\n')
                    str[len-1] = '\0';
                len = strlen(str);
                mllen[i] = mllen[i] > len ? mllen[i] : len;
            }
            /* lines     */
            if (mode_lines)
                lines[i]++;
            /* chars
            if (mode_chars)             */
        }
        if (strcmp(list[i], "/dev/stdin") == 0)
            list[i] = "";
        lines[0] += lines[i];
        words[0] += words[i];
        bytes[0] += bytes[i];
        fclose(stream);
    }

    for (int i = 1; i < count; i++)
        mllen[0] = mllen[0] > mllen[i] ? mllen[0] : mllen[i];
    for (int i = 1; i < count; i++) {       /*  -l lines -w words -m chars -c bytes -L max-line-length   */
        if (mode_lines)
            printf("%*d ", intlen(lines[0]), lines[i]);
        if (mode_words)
            printf("%*d ", intlen(words[0]), words[i]);
        if (mode_chars)
            printf("%*d ", intlen(chars[0]), chars[i]);
        if (mode_bytes)
            printf("%*d ", intlen(bytes[0]), bytes[i]);
        if (mode_mllen)
            printf("%*d ", intlen(mllen[0]), mllen[i]);
        printf("%s\n", list[i]);
    }
    if (count > 2) {                        /*  Print total */
        if (mode_lines)
            printf("%*d ", intlen(lines[0]), lines[0]);
        if (mode_words)
            printf("%*d ", intlen(words[0]), words[0]);
        if (mode_chars)
            printf("%*d ", intlen(chars[0]), chars[0]);
        if (mode_bytes)
            printf("%*d ", intlen(bytes[0]), bytes[0]);
        if (mode_mllen)
            printf("%*d ", intlen(mllen[0]), mllen[0]);
        printf("total\n");
    }
}

int main(int argc, char **argv) {
    char *list[2];
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
        wc(argv, argc);
    }
    return EXIT_SUCCESS;
}
