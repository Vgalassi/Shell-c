#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <wait.h>



void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
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
    FILE *file;
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

    if(argc != 1){
        
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("Não foi possível abrir o arquivo.\n");
            return 1;
        }
    }

    printf("-- Bob shell --\n");
    while (1) {
        int redirect_output = 0;
        int redirect_input = 0;

        if (!redirect_output) {
            printf("\n%s> ", diretorio);
        }

        if(file != NULL && fgets(comando, sizeof(comando), file) != NULL){
            comando[strcspn(comando, "\r")] = 0;
        }else{
            fgets(comando, 8191, stdin);
        }
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

        char *output_file = NULL;
        // Adicione uma variável para armazenar o nome do arquivo de entrada
char *input_file = NULL;

for (i = 0; i < numeroEnderecos; i++) {
    if (strcmp(enderecosComando[i], "<") == 0 && i + 1 < numeroEnderecos) {
        // Se encontrarmos o operador de redirecionamento de entrada
        input_file = enderecosComando[i];
        enderecosComando[i] = NULL; 
        numeroEnderecos = i; 
        break;
    } else if (strcmp(enderecosComando[i], ">") == 0 && i + 1 < numeroEnderecos) {
        // Se encontrarmos o operador de redirecionamento de saída
        redirect_output = 1;
        output_file = enderecosComando[i + 1];
        enderecosComando[i] = NULL; 
        numeroEnderecos = i; 
        break;
    }
}

int stdin_backup, stdout_backup;

if (redirect_output) {
    stdin_backup = dup(fileno(stdin));
    stdout_backup = dup(fileno(stdout));

    if (redirect_input) {
                if (access(input_file, F_OK) == -1) {
                    fprintf(stderr, "Erro: Arquivo de entrada '%s' não existe\n", input_file);
                    continue;
                }
                int input_fd = open(input_file, O_RDONLY);
                if (input_fd == -1) {
                    perror("Erro ao abrir arquivo de entrada");
                    continue;
                }
                if (dup2(input_fd, fileno(stdin)) == -1) {
                    perror("Erro ao redirecionar stdin");
                    close(input_fd);
                    continue;
                }
                close(input_fd);
            }

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd == -1) {
        perror("ERROR");
        printf("Não foi possível abrir ou criar o arquivo %s\n", enderecosComando[i+1]);
        dup2(stdout_backup, fileno(stdout));

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
            continue;  
        }
    
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
            else if(strcmp(enderecosComando[enderecoAtual],"clear") == 0){
                clear_screen();
                printf("-- Bob shell --\n");

            } else if(strcmp(enderecosComando[enderecoAtual],"exit") == 0){
                printf("\nTerminou o BobShell\n");
                free(path);
                if(file != NULL)
                    fclose(file);
                return 0;
            }else if(strcmp(enderecosComando[enderecoAtual],"") == 0){

            }
            else{
                int numeroProcessos = 0;
                char diretorioProcurado[256];
                
                for(i = 0;i<pathTamanho;i++){
                    snprintf(diretorioProcurado, sizeof(diretorioProcurado), "%s/%s", path[i], enderecosComando[enderecoAtual]);
                    if(access(diretorioProcurado, X_OK) == 0 ){
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

        if (redirect_output) {

            fflush(stdout);
            dup2(stdout_backup, fileno(stdout));
            close(stdout_backup);
            printf("Redirecionamento concluído!\n");

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
        pidPrincipal = 1;
    }

   
    
}