#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

void defeitos(char* imagens[], int n, int max){

    int num_proc = 0;

    for(int i = 0; i<n; i++){

        if(num_proc >= max){
            wait(NULL);
            num_proc--;
        }

        if(fork()==0){

            execl("./detectAtom", "detectAtom", imagens[i], NULL);
            perror("Erro na execução do detectAtom\n");
            _exit(-1);
        }

        num_proc++;
    }

    while(wait(NULL)>0);
}


void conta(char* imagens[], int n){

    int pp[2];

    if(pipe(pp)<0){
        perror("Erro na criação do pipe\n");
        return;
    }

    if(fork()==0){

        close(pp[0]);
        dup2(pp[1], STDOUT_FILENO);
        close(pp[1]);

        defeitos(imagens,n,n);
        _exit(0);
    }

    wait(NULL);
    close(pp[1]);

    if(fork()==0){

        dup2(pp[0], STDIN_FILENO);
        close(pp[0]);

        execlp("wc", "wc", "-l", NULL);
        perror("Erro na execução do comando 'wc -l'\n");
        _exit(-1);
    }

    close(pp[0]);
    int status;

    wait(&status); // Espera pelo término do segundo filho
    if (!WIFEXITED(status)) perror("A operação não foi bem sucedida");
    if (WEXITSTATUS(status) == -1) perror("A operação não foi bem sucedida");
}