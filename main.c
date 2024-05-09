#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void exec_comando_cd(const char *caminho) {
    if(caminho == NULL) {
        printf("Uso: cd <caminho>\n");
    } else {
        if(chdir(caminho) == 0) {
            printf("Mudou para o diretório %s\n", caminho);
        } else {
            perror("chdir");
        }
    }
}

int main() {
    char comando[100];
    printf("-- Bob shell --\n\n\n");

    while(1) {
        printf("> ");
        fgets(comando, 100, stdin);
        comando[strcspn(comando, "\n")] = 0;

        if(strcmp(comando, "exit") == 0) {
            printf("\nTerminou o BobShell\n");
            return 0;
        }
        else if(strcmp(comando, "cd") == 0) {
            printf("Uso: cd <caminho>\n");
        } 
        else if(strncmp(comando, "cd ", 3) == 0) {
            char *caminho = comando + 3;
            exec_comando_cd(caminho);
        } 
        else {
            printf("%s não é reconhecido como comando interno\n", comando);
        }
    }

    return 0;
}
