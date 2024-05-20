#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <wait.h>

char** separarComando(char comando[], int *numeroEnderecos) {
    int i = 0;
    *numeroEnderecos = 1;
    char** enderecosComando;
    while (comando[i] != '\0') {
        if (comando[i] == ' ') {
            comando[i] = '\0';
            while (comando[i + 1] == ' ')
                i++;
            if (comando[i + 1] != '\0')
                (*numeroEnderecos)++;
        }
        i++;
    }
    i = 1;
    int j = 0;
    enderecosComando = malloc((*numeroEnderecos) * sizeof(char*));
    enderecosComando[0] = comando;

    while (i < (*numeroEnderecos)) {
        if (comando[j] == '\0') {
            while (comando[j + 1] == ' ')
                j++;
            enderecosComando[i] = &comando[j + 1];
            i++;
        }
        j++;
    }
    printf("\n");
    return enderecosComando;
}

int contaPalavras(char** enderecosComando, int numeroEnderecos) {
    int i = 0;
    int numeroPalavras = 0;
    int flag = 0;
    while (i < numeroEnderecos && strcmp(enderecosComando[i], "&") != 0) {
        i++;
    }
    numeroPalavras = i;
    return numeroPalavras;
}

int main(int argc, char *argv[]) {
    
    char comando[8191];
    char diretorio[256];
    char **path;
    int numeroEnderecos = 0;
    getcwd(diretorio, sizeof(diretorio));
    char** enderecosComando;
    int numeroPalavras;
    int enderecoAtual = 0;
    int i;
    int numeroProcessosPricipal = 0;
    int pathTamanho = 1;
    pid_t pidPrincipal = 1;
    pid_t pid;
    path = malloc(1 * sizeof(char*));
    path[0] = malloc(256 * sizeof(char));
    strcpy(path[0], "/usr/bin");
    char ls_path[512];
    char cat_path[512];
    realpath("./ls", ls_path);
    realpath("./cat", cat_path);

    printf("-- Bob shell --\n\n\n");
    while (1) {
        printf("\n%s>", diretorio);
        fgets(comando, 8191, stdin);
        comando[strcspn(comando, "\n")] = 0;
        enderecosComando = separarComando(comando, &numeroEnderecos);
        for(i = 0;i<numeroEnderecos;i++){
            if(strcmp(enderecosComando[i], "&") == 0 && pidPrincipal != 0){
                numeroProcessosPricipal++;
                pidPrincipal = fork();
                if(pidPrincipal == 0){
                    enderecoAtual = i + 1;
                }
            }
        }

        numeroPalavras = contaPalavras(&enderecosComando[enderecoAtual], numeroEnderecos - enderecoAtual);
    
        if (strcmp(enderecosComando[enderecoAtual], "cd") == 0) {
            enderecoAtual++;
            if (chdir(enderecosComando[enderecoAtual]) == 0) {
                getcwd(diretorio, sizeof(diretorio));
            } 
            else {
                perror("chdir");
            }
        } else if (strcmp(enderecosComando[enderecoAtual], "ls") == 0) {
            pid = fork();
            if (pid == 0) {
                char *args[numeroPalavras+1]; 
                args[0] = enderecosComando[enderecoAtual];

                for (int i = 1; i < numeroPalavras; i++) {
                    args[i] = enderecosComando[enderecoAtual+i];
                }
                args[numeroPalavras] = NULL;
                printf("\n%s",ls_path);
                execv(ls_path, args);

                perror("execv");
                exit(EXIT_FAILURE);
            }
            wait(NULL);
            }
            else if (strcmp(enderecosComando[enderecoAtual], "path") == 0) {
                enderecoAtual++;
                if (numeroPalavras == 1) {
                    printf("PATH = ");
                    for (i = 0; i < pathTamanho; i++) {
                        printf("%s ", path[i]);
                    }
                    printf("\n");
                } else {
                    for (i = 0; i < pathTamanho; i++) {
                        free(path[i]);
                    } 
                    free(path);
                    pathTamanho = 0;
                    path = malloc((numeroPalavras - 1) * sizeof(char*));
                    for (i = 0; i < (numeroPalavras - 1); i++) {
                        path[i] = malloc(256 * sizeof(char));
                        strncpy(path[i],enderecosComando[enderecoAtual + i],255);
                        
                    }
                    pathTamanho = i;
                }
            } else if(strcmp(enderecosComando[enderecoAtual],"cat") == 0){
                enderecoAtual++;
                pid_t pid = fork(); // cria um novo processo
                if (pid == -1) {
                    perror("fork");
                } else if (pid == 0) {
                    char *argv[] = {cat_path, enderecosComando[enderecoAtual], NULL}; // argumentos para o programa cat
                    execvp(argv[0], argv); // executa o programa cat no novo processo
                    perror("execvp"); // execvp retorna apenas se ocorrer um erro
                    exit(EXIT_FAILURE);
                } else {
                    int status;
                    wait(NULL);
                }
            }
            else if(strcmp(enderecosComando[0],"clear") == 0){
                        system("clear");

            } else if(strcmp(enderecosComando[0],"exit") == 0){
                printf("\nTerminou o BobShell\n");
                free(path);
                return 0;
            }else{
                int numeroProcessos = 0;
                char diretorioProcurado[256];
                
                for(i = 0;i<pathTamanho;i++){
                    snprintf(diretorioProcurado, sizeof(diretorioProcurado), "%s/%s", path[i], enderecosComando[enderecoAtual]);
                    if(access(diretorioProcurado, X_OK) == 0){
                        pid = fork();
                        if(pid == 0){
                            execl(diretorioProcurado, enderecosComando[enderecoAtual] , NULL, NULL);
                        }
                        numeroProcessos++;
                    }
                }
                    
                for(i = 0; i<numeroProcessos;i++)
                    wait(NULL);
                if(numeroProcessos == 0){
                    printf("%s não é reconhecido como comando interno\n",enderecosComando[enderecoAtual]);
                }
        }

        free(enderecosComando);
        
        
        numeroPalavras = 0;
        enderecoAtual = 0;

        if(pidPrincipal == 0){
            return 0;
        }
        for(i = 0; i<numeroProcessosPricipal;i++)
            wait(NULL);

        numeroProcessosPricipal = 0;
    }

   
    
}
