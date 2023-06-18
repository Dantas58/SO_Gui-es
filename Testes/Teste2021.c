#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

int vacinados(char* regiao, int idade){

    int status;
    int pp[2];
    pipe(pp);

    if(fork()==0){

        close(pp[0]);
        dup2(pp[1], STDOUT_FILENO);
        close(pp[1]);
        char idadeS[10];

        sprintf(idadeS, "%d", idade);
        
        execlp("grep", "grep", idadeS, strcat(regiao, ".txt"));
        perror("[CHILD] Error: could not execute\n");
    }

    close(pp[1]);
    //wait(NULL);

    int pp2[2];
    pipe(pp2);

    if(fork()==0){

        close(pp2[0]);
        dup2(pp[0], STDIN_FILENO);
        dup2(pp2[1], STDOUT_FILENO);
        close(pp[0]);
        close(pp2[1]);

        execlp("wc", "wc", "-l", NULL);
        perror("[CHILD] Error: could not execute\n");
    }

    close(pp[0]);
    close(pp2[1]);

    wait(NULL);
    wait(&status);

    if(!WIFEXITED(status)) return -1;

    char r[10];
    read(pp2[0], r, sizeof(char));

    return atoi(r);
}

int vacinado(char* cidadao){

    int r = 0;
    pid_t pid_save[10];

    for(int i = 0; i<10; i++){

        if((pid_save[i]=fork())==0){

            char ficheiro[10];
            sprintf(ficheiro, "%d", i);
            strcat(ficheiro, ".txt");

            execlp("grep", "grep", cidadao, ficheiro, NULL);
            perror("[CHILD] Error: could not execute\n");
        }
    }

    int status;
    for(int j = 0; j<10; j++){

        waitpid(pid_save[j], &status, 0);

        if(WIFEXITED(status) && WEXITSTATUS(status)==0){
            r = 1;
            //for(int k = j+1; k<10; k++) kill(pid_save[k], SIGTERM); // tinha de ser com sinais acho
            break;
        }
    }

    return r;
}