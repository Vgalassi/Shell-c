#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> // Inclua este cabeçalho para usar opendir e readdir

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *ent;

    if (argc < 2) {
        printf("Por favor, forneça o nome do diretório como argumento.\n");
        return 1;
    }

    dir = opendir(argv[1]);
    if (dir == NULL) {
        printf("Não foi possível abrir o diretório %s\n", argv[1]);
        return 2;
    }

    while ((ent = readdir(dir)) != NULL) {
        printf("%s\n", ent->d_name);
    }

    closedir(dir);
    return 0;
}
