#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



char** separarComando(char comando[]){

    int i = 0;
    int numeroEnderecos = 0;
    char** enderecosComando;
    for(i = 0;i<100;i++){
        if(comando[i] == ' '){
            comando[i] = '\0';
            numeroEnderecos++;
        }        
    }
    i = 1;
    int j = 0;
    enderecosComando = malloc (numeroEnderecos * sizeof(char));
    enderecosComando[0] = &comando[0];
    while(i<numeroEnderecos){
        if(comando[j] = '\0'){
            enderecosComando[0] = &comando[i+1];
            i++;
        }
        j++;

    }

    return enderecosComando;
}




int main(){

    char comando[100];
    char diretório[100] = "/";
    printf("-- Bob shell --\n\n\n");
    char** enderecosComando;
    
    
    while(1){
        fgets(comando,100,stdin);
        comando[strcspn(comando, "\n")] = 0;
        enderecosComando = separarComando(comando);




        if(strcmp(enderecosComando[0],"exit") == 0){
            printf("\nTerminou o BobShell\n");
            return 0;
        }else if(1) {  
            if (chdir(comando) == 0) {
            printf("Mudou para o diretório %s\n",comando);
            } else {
                perror("chdir");
            }
        
        }

        
        else{
            printf("%s não é reconhecido como comando interno\n",comando);
        }
    }
}