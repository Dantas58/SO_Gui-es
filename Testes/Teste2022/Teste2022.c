#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


int mensagens(char* palavra, char* ficheiro){

    int pp[2];
    if(pipe(pp)<0){
        perror("Error: could not create pipe\n");
        return -1;
    }

    if(fork()==0){

        close(pp[0]);
        dup2(pp[1], STDOUT_FILENO);
        close(pp[1]);

        execlp("grep", "grep", palavra, ficheiro, NULL);

        perror("[CHILD] Error: could not execute\n");
        _exit(-1);
    }

    close(pp[1]);
    wait(NULL);

    int pipe2[2];
    if(pipe(pipe2)<0){
        perror("Error: could not create pipe\n");
        return -1;
    }
    
    if(fork()==0){

        close(pipe2[0]);

        dup2(pp[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pp[0]);
        close(pipe2[1]);

        execlp("wc", "wc", "-l", NULL);
        perror("[CHILD] Error: could not execute\n");
        _exit(-1);
    }

    close(pp[0]);
    close(pipe2[1]);

    int status;
    wait(&status);
    if(!WIFEXITED(status) || WEXITSTATUS(status)<0) return -1;

    char* res = malloc(sizeof(char));
    read(pipe2[0], res, sizeof(char));
    close(pipe2[0]);

    return atoi(res);
}



int autores_que_usaram(char* palavra, int n, char* autores[n]){

    int r = 0;
    int status;

    for(int i = 0; i<n; i++){

        if(fork()==0){
            if(mensagens(palavra, autores[i])>0) _exit(1);
            _exit(0);
        }
    }

    for(int j = 0; j<n; j++){

        wait(&status);
        if(WIFEXITED(status) && WEXITSTATUS(status)>0) r++;
    }

    return r;
}



int main (int argc, char** argv){

    int n;
    char* autores[2]; 
    
    autores[0] = "afonso.txt"; 
    autores[1] = "joao.txt"; 
    n = mensagens("ola","test1.txt");


    int t = autores_que_usaram("ola",2,autores);

    printf("O numero foi: %d\n",n);
    printf("O numero foi: %d\n",t);

    return 0;
}
