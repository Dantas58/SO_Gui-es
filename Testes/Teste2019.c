#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


int mail(char* ficheiro){

    int fd = open(ficheiro, O_RDONLY, 0660);

    int active_proc = 0;
    char line[10];

    while(read(fd, line, 10)>0){

        active_proc++;
        if(active_proc>10){
            wait(NULL);
            active_proc--;
        }

        if(fork()==0){

            char num[7];
            strncpy(num, line, 6);
            num[7] = 0;
            char email[24];
            sprintf(email, "%s@alunos.uminho.pt", num);

            execlp("mail", "mail", "-s", "Sistemas_Operativos", email, line, NULL);
            perror("[CHILD] Error: could not execute\n");
        }
    }

    close(fd);
    while(active_proc>0){
        wait(NULL);
        active_proc--;
    }

    return 1;
}