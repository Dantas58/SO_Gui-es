#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


struct RegistoF{
    char nome[20];
    char cargo[20];
    int salario;
}RegistoF;

void aumentaSalarios(char* ficheiro, char* cargo, int valor, long N, int p){

    int fd = open(ficheiro, O_RDWR, 0666);

    struct RegistoF r;
    int active_proc = 0;

    for(int i = 0; i<N; i++){

        if(active_proc >= p){

            wait(NULL);
            active_proc--;
        }

        read(fd, &r, sizeof(RegistoF));

        if(fork()==0){

            if(!strcmp(cargo, r.cargo)){

                r.salario += valor;
                lseek(fd, -sizeof(RegistoF), SEEK_CUR);
                write(fd, &r, sizeof(RegistoF));
            }

            _exit(0);
        }

        active_proc++;
    }

    while(wait(NULL)>0);

    close(fd);
}

int validaSalarios(char* ficheiro, char* cargo){

    int fd = open(ficheiro, O_RDONLY, 0666);

    struct RegistoF r;
    int status;
    int pp[2];
    int result = 1;

    pipe(pp);

    if(fork()==0){

        close(pp[0]);
        dup2(pp[1], STDOUT_FILENO); 
        close(pp[1]);

        execl("./filtraCargo", "filtraCargo", ficheiro, cargo, NULL);
    }   

    wait(NULL);
    close(pp[1]);

    if(fork()==0){

        dup2(pp[0], STDIN_FILENO);
        close(pp[0]);

        execl("./validaMin", "validaMin", NULL);
    }

    close(fd);
    while(wait(&status)>0){

        if(WIFEXITED(status) && WEXITSTATUS(status)==1) result = 0;
    }

    return r;
}