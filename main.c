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
    printf("comando inteiro:");
    for (i = 0; i < (*numeroEnderecos); i++) {
        printf("%s ", enderecosComando[i]);
    }
    printf("\n");
    return enderecosComando;
}

int contaPalavras(char** enderecosComando, int numeroEnderecos) {
    int i = 0;
    int numeroPalavras = 0;
    while (i < numeroEnderecos && strcmp(enderecosComando[i], "&") != 0) {
        numeroPalavras++;
        i++;
    }
    return numeroPalavras;
}

int main(int argc, char *argv[]) {
    
    char comando[8191];
    char diretorio[256];
    char **path;
    int numeroEnderecos = 0;
    getcwd(diretorio, sizeof(diretorio));
    printf("-- Bob shell --\n\n\n");
    char** enderecosComando;
    int numeroPalavras;
    int enderecoAtual = 0;
    int i;
    int pathTamanho = 1;
    
    pid_t pid;
    path = malloc(1 * sizeof(char*));
path[0] = malloc(256 * sizeof(char));
    strcpy(path[0], "/usr/bin");
    // Obtém o caminho absoluto do executável ls
    char ls_path[512];
    realpath("./ls", ls_path);

    while (1) {
        printf("\n%s>", diretorio);
        fgets(comando, 8191, stdin);
        comando[strcspn(comando, "\n")] = 0;
        enderecosComando = separarComando(comando, &numeroEnderecos);

        numeroPalavras = contaPalavras(&enderecosComando[enderecoAtual], numeroEnderecos);

        printf("\nEndereço atual: %d\n", enderecoAtual);
        printf("\nNumero de palavras: %d\n", numeroPalavras);

       // Verifica por redirecionamento de saída
        int redirect_output = 0;
        char *output_file = NULL;
        for (i = 0; i < numeroEnderecos; i++) {
            if (strcmp(enderecosComando[i], ">") == 0 && i + 1 < numeroEnderecos) {
                redirect_output = 1;
                output_file = enderecosComando[i + 1];
                enderecosComando[i] = NULL; // Termina o comando antes do '>'
                numeroEnderecos = i; // Ajusta o número de endereços para não incluir o redirecionamento
                break;
            }
        }

        int stdout_backup;
        if (redirect_output) {
            stdout_backup = dup(fileno(stdout));
            int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("Erro ao abrir o arquivo de saída");
                continue;
            }
            if (dup2(output_fd, fileno(stdout)) == -1) {
                perror("Erro ao redirecionar stdout");
                close(output_fd);
                continue;
            }
            close(output_fd);
        }

        if (numeroEnderecos == 0) {
            continue;  // No command to execute
        }

        if (strcmp(enderecosComando[0], "cd") == 0) {
            enderecoAtual++;
            if (chdir(enderecosComando[enderecoAtual]) == 0) {
                getcwd(diretorio, sizeof(diretorio));
            } 
            else {
                perror("chdir");
            }
        } else if (strcmp(enderecosComando[0], "ls") == 0) {
    pid = fork();
    if (pid == 0) {
        // Count the number of arguments to pass to execl
        int arg_count = 1; // For "ls"

        // Create an array to hold the arguments
        char *args[numeroPalavras]; // +1 for NULL termination

        // Set the first argument to "ls"
        args[0] = enderecosComando[0];

        // Copy the rest of the arguments
        for (int i = 1; i < numeroPalavras; i++) {
            args[i] = enderecosComando[i];
        }

        // Null-terminate the arguments array
        args[numeroPalavras] = NULL;

       // Executa o comando com os argumentos
        execv(ls_path, args);

        // If execv fails
        perror("execv");
        exit(EXIT_FAILURE);
    }
    wait(NULL);
}
else if (strcmp(enderecosComando[enderecoAtual], "path") == 0) {
            enderecoAtual++;
            if (numeroPalavras == (enderecoAtual)) {
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
            FILE *file;
            int c;
            if(enderecoAtual >= numeroEnderecos) {
                printf("Por favor, forneça o nome do arquivo como argumento.\n");
            } else {
                file = fopen(enderecosComando[enderecoAtual], "r");
                if(file == NULL) {
                    printf("Não foi possível abrir o arquivo %s\n", enderecosComando[enderecoAtual]);
                } else {
                    while((c = fgetc(file)) != EOF) {
                        putchar(c);
                    }
                    fclose(file);
                }
            }
        } else if(strcmp(enderecosComando[0],"clear") == 0){
            system("cls");
        } else if(strcmp(enderecosComando[0],"exit") == 0){
            printf("\nTerminou o BobShell\n");
            free(path);
            return 0;
        }else{
            int numeroProcessos = 0;
            char diretorioProcurado[256];
    
            for(i = 0;i<pathTamanho;i++){
                snprintf(diretorioProcurado, sizeof(diretorioProcurado), "%s/%s", path[i], enderecosComando[0]);
                printf("%s\n",diretorioProcurado);
                if(access(diretorioProcurado, X_OK) == 0){
                    pid = fork();
                    if(pid == 0){
                        execl(diretorioProcurado, enderecosComando[0] , NULL, NULL);
                    }
                    numeroProcessos++;
                }
            }
            
            for(i = 0; i<numeroProcessos;i++)
                wait(NULL);
            if(numeroProcessos == 0){
                printf("%s não é reconhecido como comando interno\n",enderecosComando[0]);
            }
        }

        free(enderecosComando);
        numeroPalavras = 0;
        enderecoAtual = 0;
    }

   
    
}
