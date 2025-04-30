#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int intlen(int number) {
    // return ceil(log10(number+1));
    char buffer[10];
    sprintf(buffer, "%d", number);
    return strlen(buffer);
}

void wc(char **list, int count) {
    char buffer[BUFSIZ];
    int clines = 0, cwords = 0, cbytes = 0;
    for (int i = 1; i < count; i++) {
        int lines = 0, words = 0, bytes = 0;
        FILE *stream = fopen(list[i], "r");
        if (stream == NULL) {
            perror("Erro ao abrir o arquivo");
            exit(EXIT_FAILURE);
        }
        while (fgets(buffer, BUFSIZ, stream) != NULL) {
            /* bytes     */
            bytes += strlen(buffer);
            char *str;
            /* words     */
            str = strdup(buffer);
            while (strtok_r(str, " \t\n", &str) != NULL)
                words++;
            /* lines     */
            lines++;
        }
        if (strcmp(list[i], "/dev/stdin") == 0)
            list[i] = "";
        clines += lines;
        cwords += words;
        cbytes += bytes;
        printf("%d %d %d %s\n", lines, words, bytes, list[i]);
        fclose(stream);
    }
    if (count > 2)
        printf("%d %d %d total\n", clines, cwords, cbytes);
}

int main(int argc, char **argv) {
    char *list[2];
    if (argc < 2) {
        list[1] = "/dev/stdin";
        wc(list, 2);
    } else {
        wc(argv, argc);
    }
    return EXIT_SUCCESS;
}
