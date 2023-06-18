#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

// III (o II é com sinais)

// supondo que o readline da return 1 caso leia alguma coisa e 0 caso contrario
// vou fingir que upper e lower ta definido mas era so um fazer um for

void rizzler(){

    int fd_PARES = open("PARES", O_CREAT | O_APPEND | O_WRONLY, 0666);
    int fd_IMPARES = open("IMPARES", O_CREAT | O_APPEND | O_WRONLY, 0666);

    char line[20];

    while(readline(line) > 0){

        if(fork()==0){

            if(line[0]%2==0) for(int i = 0; line[i]; i++) lower(line[i]);
            else for(int i = 0; line[i]; i++) upper(line[i]);

            if(line[1]%2==0) write(fd_PARES, line, strlen(line));
            else write(fd_IMPARES, line, strlen(line));

            _exit(0);
        }
    }

    close(fd_IMPARES);
    close(fd_PARES);
}

// 