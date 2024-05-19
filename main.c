#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


char** separarComando(char comando[],int *numeroEnderecos){

    int i = 0;
    *numeroEnderecos = 1;
    char** enderecosComando;
    while(comando[i] != '\0'){
        if(comando[i] == ' '){
            comando[i] = '\0';
            
            while(comando[i+1] == ' ')
                i++;
            if(comando[i+1] != '\0')
                (*numeroEnderecos)++;
        } 
        i++;       
    }
    i = 1;
    int j = 0;
    enderecosComando = malloc ((*numeroEnderecos) * sizeof(char*));
    enderecosComando[0] = comando;
    
    while(i<(*numeroEnderecos)){
        if(comando[j] == '\0'){
            while(comando[j+1] == ' ')
                j++;
            enderecosComando[i] = &comando[j+1];
            i++;
        }

        j++;

    }

    printf("comando inteiro:");
    for(i=0;i<(*numeroEnderecos);i++){
        printf("%s ",enderecosComando[i]);
    }
    printf("\n");
    return enderecosComando;
}


int contaPalavras(char** enderecosComando, int numeroEnderecos){
    int i = 0;
    int numeroPalavras = 0;
    while(i<numeroEnderecos && strcmp(enderecosComando[i],"&") != 0){
        numeroPalavras++;
        i++;
    }

    return numeroPalavras;
}



int main(){

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
    
    while(1){

        printf("\n%s>",diretorio);
        fgets(comando,8191,stdin);
        comando[strcspn(comando, "\n")] = 0;
        enderecosComando = separarComando(comando,&numeroEnderecos);


        numeroPalavras = contaPalavras(&enderecosComando[enderecoAtual],numeroEnderecos);

        printf("\nEndereço atual: %d\n",enderecoAtual);
        printf("\nNumero de palavras: %d\n",numeroPalavras);
       
        
        if(strcmp(enderecosComando[enderecoAtual],"cd") == 0) {  
            
            enderecoAtual++;
            
            if (chdir(enderecosComando[enderecoAtual]) == 0) {
                getcwd(diretorio, sizeof(diretorio));
                
            } 
            else {
                perror("chdir");
                
            }

            
        
        }else if(strcmp(enderecosComando[enderecoAtual],"path") == 0){
            enderecoAtual++;
            if(numeroPalavras == (enderecoAtual)){
                printf("PATH = ");
                for(i = 0;i<pathTamanho;i++){
                    printf("%s ",path[i]);
                }
                printf("\n");
            }else{
                for(i = 0; i < pathTamanho; i++){
                    free(path[i]);
                } 
               
               free(path);
                pathTamanho = 0;
                path = malloc((numeroPalavras-1) * sizeof(char*));
                
                for(i = 0;i<(numeroPalavras-1);i++){
                    path[i] = malloc(256 * sizeof(char));
                    strncpy(path[i],enderecosComando[enderecoAtual + i],255);
                    
                }
                pathTamanho = i;

            }
   
        }
        else if(strcmp(enderecosComando[0],"exit") == 0){
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