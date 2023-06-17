#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

void memoria(int argc, char* argv[]){

    pid_t pid_save[10];

    for(int i = 0; i<10; i++){

        if((pid_save[i]=fork())==0){

            execvp(argv[1], &argv[1]);
            perror("[CHILD] Error: could not execute\n");
            _exit(1);
        }

        wait(NULL);
    }

    int pipe_1[2];
    int pipe_2[2];

    pipe(pipe_1);
    pipe(pipe_2);

    int time[10];

    for(int j = 0; j<10; j++){

        if(fork()==0){

            close(pipe_2[0]);
            close(pipe_2[1]);
            close(pipe_1[0]);
            dup2(pipe_1[1], STDOUT_FILENO);
            close(pipe_1[1]);

            char command[50];
            sprintf(command, "/proc/%d/memstats", pid_save[j]);

            execlp("grep", "grep", "VmPeak", command, NULL);
            perror("[CHILD] Error: could not execute\n");
            _exit(1);
        }

        if(fork()==0){

            close(pipe_1[1]);
            close(pipe_2[0]);
            dup2(pipe_1[0], STDIN_FILENO);
            close(pipe_1[0]);
            dup2(pipe_2[1], STDOUT_FILENO);
            close(pipe_2[1]);

            execlp("cut", "cut", "-d", "" "", "-f4", NULL);
            perror("[CHILD] Error: could not execute\n");
        }

        close(pipe_1[0]);
        close(pipe_1[1]);
        close(pipe_2[1]);
        dup2(pipe_2[0], STDIN_FILENO);
        close(pipe_2[0]);
        // fechar fora do ciclo (?)

        char r[10];
        read(STDIN_FILENO, r, sizeof(char));
        time[j] = atoi(r);
    }

    while(wait(NULL)>0);

    int max = -1;
    int min = 10000;
    int media;

    for (int i = 0; i < 10; i++) {
        if (time[i] > max) max = time[i];
        if (time[i] < min) min = time[i];
        media += time[i];
    }

    media /= 10;

    printf("MAX: %d\n MIN: %d\n MEDIA: %d\n", max, min, media);
}

int main() {
    char* argv[] = {"./memoria", "ls", "-l", NULL};
    int argc = sizeof(argv) / sizeof(argv[0]) - 1;

    memoria(argc, argv);

    return 0;
}

// nao saem sinais