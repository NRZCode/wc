#include <getopt.h>
#include <limits.h>
#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.1"
#define FIELDS  5

bool mode_bytes = false;
bool mode_chars = false;
bool mode_lines = false;
bool mode_mllen = false;
bool mode_words = false;
bool mode_total = false;

static struct option long_options[] = {
    {"bytes",           no_argument,        NULL, 'c'},
    {"chars",           no_argument,        NULL, 'm'},
    {"lines",           no_argument,        NULL, 'l'},
    {"words",           no_argument,        NULL, 'w'},
    {"max-line-length", no_argument,        NULL, 'L'},
    {"total",           required_argument,  NULL, 1000},
    {"help",            no_argument,        NULL, 'h'},
    {"version",         no_argument,        NULL, 'v'},
    {0, 0, 0, 0}
};

enum { TOTAL };
enum { LINES, WORDS, CHARS, BYTES, MLLEN };

int intlen(int number) {
    // return ceil(log10(number+1));
    char buffer[10];
    sprintf(buffer, "%d", number);
    return strlen(buffer);
}

void count_lines(int *counter) {
    /* lines     */
    (*counter)++;
}

void count_bytes(char *line, int *counter) {
    /* bytes     */
    *counter += strlen(line);
}

void count_words(char *line, int *counter) {
    /* words     */
    char *str = strdup(line);
    if (!str)
        return;
    while (strtok_r(str, " \t\n", &str) != NULL)
        (*counter)++;
}

void count_chars(char *line, int *counter) {
    /* chars                        */
    char *str = strdup(line);
    if (!str)
        return;
    size_t mblen = mbstowcs(NULL, str, 0);
    if (mblen == (size_t)-1 || mblen > INT_MAX) {
        perror("Erro ao converter multibyte");
        exit(EXIT_FAILURE);
    }
    (*counter) += (int)mblen;
}

void count_max_line(char *line, int *max_counter) {
    /* max-line-lenght              */
    char *str = strdup(line);
    if (!str)
        return;
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n')
        str[len-1] = '\0';
    size_t mblen = mbstowcs(NULL, str, 0);
    if (mblen == (size_t)-1 || mblen > INT_MAX) {
        perror("Erro ao converter multibyte");
        exit(EXIT_FAILURE);
    }
    *max_counter = *max_counter > (int)mblen ? *max_counter : (int)mblen;
}

void print_results(char **list, int **items, int count) {
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
    if (mode_total) {                        /*  Print total */
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
}

void wc(char **list, int count) {
    setlocale(LC_ALL, "pt_BR.UTF-8");
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
            if (mode_lines)
                count_lines(&items[i][LINES]);
            if (mode_words)
                count_words(buffer, &items[i][WORDS]);
            if (mode_chars)
                count_chars(buffer, &items[i][CHARS]);
            if (mode_bytes)
                count_bytes(buffer, &items[i][BYTES]);
            if (mode_mllen)
                count_max_line(buffer, &items[i][MLLEN]);
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
    print_results(list, items, count);
    for (int i = 0; i < count; i++) {
        free(items[i]);
    }
    free(items);
}

int main(int argc, char **argv) {
    int opt, option_index;
    char *list[2];
    char *totalarg = "";
    // short_options = "c,    m,    l,    L,                                 w"
    // long_options  = "bytes,chars,lines,max-line-length,total,help,version,words"
    while ((opt = getopt_long(argc, argv, "cmlLwhv", long_options, &option_index)) != -1) {
        if (opt == 0) {
            continue;
        }
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
            case 1000:
                // auto, always, only, never
                if (strcmp(optarg, "auto") == 0 ||
                        strcmp(optarg, "always") == 0 ||
                        strcmp(optarg, "only") == 0 ||
                        strcmp(optarg, "never") == 0) {
                    totalarg = optarg;
                } else {
                    fprintf(stderr, "Erro: argumento inválido para --total: \"%s\"\n", optarg);
                    fprintf(stderr, "Valores permitidos: auto, always, only, never\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                printf("Uso: %s [OPÇÕES] [ARQUIVOS...]\n", argv[0]);
                printf("  -l, --lines             Conta linhas\n");
                printf("  -w, --words             Conta palavras\n");
                printf("  -c, --bytes             Conta bytes\n");
                printf("  -m, --chars             Conta caracteres\n");
                printf("  -L, --max-line-length   Comprimento máximo de linha\n");
                printf("  -h, --help              Exibe esta ajuda\n");
                printf("  -v, --version           Exibe a versão\n");
                exit(EXIT_SUCCESS);
            case 'v':
                printf("wc clone %s\n", VERSION);
                exit(EXIT_SUCCESS);
            default:
                abort();
        }
    }
    /* Default mode */
    if ((mode_bytes || mode_chars || mode_lines || mode_mllen || mode_words) == false) {
        mode_bytes = true;
        mode_lines = true;
        mode_words = true;
    }
    optind--;
    if (((strcmp(totalarg, "") == 0 || strcmp(totalarg, "auto") == 0) && (argc - optind) > 2)
            || strcmp(totalarg, "always") == 0)
        mode_total = true;
    // else if (strcmp(totalarg, "never") == 0)
    //     mode_total = false;
    /*  Ler de stdin ou argumentos */
    if ((argc - optind) < 2) {
        list[1] = "/dev/stdin";
        wc(list, 2);
    } else {
        wc(argv + optind, argc - optind);
    }
    return EXIT_SUCCESS;
}
