#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

#define size 50

int modifier(){

    int fd_passwd;
    int fd_saida;
    int fd_erros;

    if((fd_passwd = open("/etc/passwd", O_RDONLY, 0660))<0){

        perror("Error: could not open passwd\n");
        return -1;
    }

    if((fd_saida = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660))<0){

        perror("Error: could not open saida.txt\n");
        return -1;
    }

    if((fd_erros = open("erros.txt", O_WRONLY | O_CREAT | O_TRUNC, 0660))<0){

        perror("Error: could not open erros.txt\n");
        return -1;
    }

    dup2(fd_passwd, STDIN_FILENO);
    close(fd_passwd);

    dup2(fd_saida, STDOUT_FILENO);
    close(fd_saida);

    dup2(fd_erros, STDERR_FILENO);
    close(fd_erros);

    //printf("Terminei\n");
    //return 1; 

    //ex 2

    if(fork()==0){

        printf("[CHILD] My PID is: %d\n", getpid());
        int buffer[size];
        int read_bytes;
        while((read_bytes = read(STDIN_FILENO, buffer, size))>0){
            write(STDOUT_FILENO, buffer, read_bytes);
            write(STDERR_FILENO, buffer, read_bytes);
        }

        _exit(0);
    }

    wait(NULL);

    //printf("Terminei\n");
    //return 1;

    // ex 3

    int status;

    if(fork()==0){

        if(execlp("wc","wc",NULL)<0) _exit(-1);
        _exit(0);
    }

    wait(&status);
    
    if(!WIFEXITED(status)) return -1;
    if(WEXITSTATUS(status)==-1) return -1;
    
    printf("Terminei");
    return 1;
}


int ex4(){

    int pp[2];

    if(pipe(pp)<0){
        perror("Error: could not create pipe\n");
        return -1;
    }

    if(fork()==0){

        close(pp[1]);

        if(dup2(pp[0], STDIN_FILENO)<0){
            perror("[CHILD] could not execute dup2");
            _exit(-1);
        }

        if(execlp("wc","wc",NULL)==-1){
            
            perror("[CHILD] could not execute execlp\n");
            _exit(-1);
        }

        close(pp[0]);
        _exit(0);
    }

    int status;
    close(pp[0]);
    char buffer[size];
    size_t read_bytes;

    while((read_bytes = read(STDIN_FILENO, buffer, size))>0) write(pp[1], buffer, read_bytes);

    close(pp[1]);
    wait(NULL);
    if(!WIFEXITED(status)) return -1;
    if(WEXITSTATUS(status) == -1) return -1;

    return 1;
}

int ex5(){

    int pp[2];

    if(pipe(pp)<0){
        perror("Error: could not create pipe\n");
        return -1;
    }

    if(fork()==0){

        close(pp[0]);
        if(dup2(pp[1], STDIN_FILENO)<0){
            perror("[CHILD] Error: could not execute dup2\n");
            _exit(-1);
        }
        
        close(pp[1]);

        execlp("ls", "ls", "/etc", NULL);

        _exit(0);
    }

    if(fork()==0){

        close(pp[1]);
        if(dup2(pp[0], STDIN_FILENO)<0){
            perror("[CHILD] Error: could not execute dup2\n");
            _exit(-1);
        }

        close(pp[0]);

        execlp("wc", "wc", "-l", NULL);

        _exit(0);
    }
}