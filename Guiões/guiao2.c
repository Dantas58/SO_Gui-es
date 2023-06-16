#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


// ex 1
int main(){

    printf("%d\n", getpid());

    printf("%d\n", getppid());

    return 0;
}


// ex 2

int main(){

    pid_t pid;

    if(pid = fork() == 0){
        printf("[FILHO] PID do processo: %d\n", getpid());
        printf("[FILHO] PID do processo pai: %d\n", getppid());
        _exit(0);
    }
    else{
        printf("[PAI] PID do processo: %d\n", getpid());
        printf("[PAI] PID do processo filho: %d\n", pid);
        printf("[PAI] PID do processo pai: %d\n", getppid());
    }

    return 1;
}

// ex 3

int main(){

    int status;

    for(int i = 1; i<=10; i++){

        if(fork() == 0){
            printf("[FILHO %d] PID do processo: %d\n", i, getpid());
            printf("[FILHO %d] PID do processo pai: %d\n", i, getppid());
            _exit(i);
        }
        else{
            wait(&status);
            printf("[PAI] Código de sáida do processo filho: %d\n", WEXITSTATUS(status));
        }
    }

    return 1;
}


// ex 4

int main(){

    int status;

    for(int i = 1; i<=10; i++){
        if(fork()==0){
            printf("[FILHO] PID do processo: %d\n", getpid());
            printf("[FILHO] PID do processo pai: %d\n", getppid());
            _exit(i);
        }
    }

    pid_t pid_filho;

    while(pid_filho = wait(&status)>0){

        if(WIFEXITED(status)) printf("[PAI] O processo %d tem valor de saída: %d\n", pid_filho, WEXITSTATUS(status));
        else printf("ERRO!\n");
    }
    
    return 1;
}


// ex 5

#define linhas 5
#define colunas 120

int find_int (int matriz[linhas][colunas], int x){

    pid_t pid;
    int save[linhas];
    int status;

    for(int i = 0; i<linhas; i++){

        if((pid = fork())==0){
            for(int j = 0; j<colunas; j++) if(matriz[i][j] == x) _exit(i+1);
            _exit(0);
        }

        else save[i] = pid;
    }

    for(int l = 0; l<linhas; l++){

        waitpid(save[l],&status,0);
        if(WIFEXITED(status))
            if(WEXITSTATUS(status)>0){
            printf("Inteiro encontrado na linha: %d\n", WEXITSTATUS(status));
            return 1;
            }
    }

    return 0;
}

