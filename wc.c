#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char buffer[BUFSIZ];
    int lines = 0, words = 0, bytes = 0;
    FILE *stream = fopen(argv[1], "r");
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
    printf("%d %d %d %s\n", lines, words, bytes, argv[1]);
    fclose(stream);
    return EXIT_SUCCESS;
}
