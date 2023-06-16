#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


int comando(const char* filtro, const char* entrada, const char* saida){

    int fd_saida = open(saida, O_CREAT | O_WRONLY, 0660);
    int fd_entrada = open(entrada, O_CREAT | O_RDONLY, 0660);

    int status;

    if(fork()==0){

        dup2(fd_saida, STDOUT_FILENO);
        dup2(fd_entrada, STDIN_FILENO);

        close(fd_entrada);
        close(fd_saida);

        execlp("filtro", "filtro", NULL);
        perror("[CHILD] Error during 'filtro' execution\n");
        _exit(-1);
    }

    close(fd_entrada);
    close(fd_saida);

    wait(&status);

    if(!WIFEXITED(status) || WEXITSTATUS(status)<0) return 0;

    return 1;
}


// ex 2 xdxd   