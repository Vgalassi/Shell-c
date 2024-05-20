
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>


int main()
{
    pid_t pid;

    //Criando filho
    pid = fork();

    //Tratamento de erro
    if(pid < 0){
        printf("Ocorreu um erro");
        return 1;
    }
     //Bloco de execução do filho
    if(pid == 0){
        printf("Eu sou seu filho\n");
        return 0;

    }
    //Bloco de execução do pai
        printf("Eu sou seu pai\n");
        wait(NULL);
   

    return 0;
}
