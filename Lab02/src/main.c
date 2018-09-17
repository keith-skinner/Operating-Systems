//Keith Skinner
//Lab 02

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include "help.h"

void substitute(const char *from, const char *to, size_t size);

void consumeArg(int *argc, char **argv[]);

void getFile(char *filename, char *mode, FILE *file);

void parseArguments(char **from, char **to, size_t *length, int argc, char *argv[]);

int main(int argc, char *argv[]) {
    char *from = NULL;
    char *to = NULL;
    size_t length = 0;

    if (argc == 1) {
        puts(NOTHING_TO_DO);
        puts(MORE_INFORMATION);
    }
    else if (2 <= argc && argc <= 7) {
        parseArguments(&from, &to, &length, argc, argv);
        substitute(from, to, length);
    }
    else {
        puts(UNEXPECTED_ARGUMENTS);
        puts(MORE_INFORMATION);
    }
    return 0;
}


inline void substitute(const char *from, const char *to, size_t size) {
    char c = (char) getchar();
    while (!feof(stdin) && !ferror(stdin) && !ferror(stdout)) {
        if (from != NULL && to != NULL) {
            size_t pos = strchr(from, c) - from;
            c = (0 <= pos && pos <= size) ? to[pos] : c;
        }
        putc(c, stdout);
        c = (char) getchar();
    }
}

inline void consumeArg(int *argc, char **argv[]) {
    --(*argc);
    ++(*argv);
}

inline void getFile(char *filename, char *mode, FILE *file) {
    freopen(filename, mode, file);
    if (file == NULL)
        exit(errno);
}

void parseArguments(char **from, char **to, size_t *length, int argc, char *argv[]) {
    consumeArg(&argc, &argv);
    while (argc > 0) {
        if (argv[0][0] == '-') {
            switch (argv[0][1]) {
                case 'h':
                    puts(getHelpMessage());
                    exit(0);
                case '-':
                    *from = argv[0] + 2;
                    break;
                case '+':
                    *to = argv[0] + 2;
                    *length = strlen(*to);
                    break;
                case 'i':
                    getFile(argv[1], "r", stdin);
                    consumeArg(&argc, &argv);
                    break;
                case 'o':
                    getFile(argv[1], "w", stdout);
                    consumeArg(&argc, &argv);
                    break;
                default:
                    break;
            }
        }
        consumeArg(&argc, &argv);
    }
}