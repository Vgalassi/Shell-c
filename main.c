#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>




int main(){

    char comando[100];
    printf("-- Bob shell --\n\n\n");
    
    
    while(1){
        fgets(comando,100,stdin);
        comando[strcspn(comando, "\n")] = 0;
        if(strcmp(comando,"exit") == 0){
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