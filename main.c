#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(){

    char comando[100];
    printf("-- Bob shell --\n\n\n");
    
    
    while(1){
        fgets(comando,100,stdin);
        comando[strcspn(comando, "\n")] = 0;
        if(strcmp(comando,"exit") == 0){
            printf("\nTerminou o BobShell\n");
            return 0;
        }
        else{
            printf("%s não é reconhecido como comando interno\n",comando);
        }
    }
}