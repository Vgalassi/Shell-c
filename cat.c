#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *file;
    int c;

    if(argc < 2) {
        printf("Por favor, forneça o nome do arquivo como argumento.\n");
        return 1;
    }

    file = fopen(argv[1], "r");
    if(file == NULL) {
        printf("Não foi possível abrir o arquivo %s\n", argv[1]);
        return 2;
    }

    while((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    fclose(file);

    return 0;
}
