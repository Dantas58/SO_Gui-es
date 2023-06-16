#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

// ex 1
int exec_ls(){

    execlp("ls", "ls", "-l", (char*)NULL);

    return 1;
}

// ex 2

int exec_ls_filho(){

    if(fork()==0){
        execlp("ls","ls","-l", NULL);
        _exit(0);
    }
    else wait(NULL);

    return 1;
}


// ex 3
int exec_varios(int argc, int** argv){

    for(int i = 1; i<argc; i++){

        if(fork()==0){
            exexlp(argv[i], argv[i], NULL);
            _exit(i);
        }
    }

    while(wait(NULL)>0);

    return 1;
}


// ex 4
int new_system(char* command){

    char* args[30];

    char* token = strtok(command, " ");
    int i;

    for(i = 0; token; i++){
        
        args[i] = token;
        token = strtok(NULL, " ");
    }

    args[i] = NULL;

    pid_t pid;
    int exec_r; 

    if((pid = fork()) == 0){

        exec_r = execvp(args[0], args);
        _exit(exec_r);
    }

    int status;

    wait(&status);
    if(WIFEXITED(status)) return WEXITSTATUS(status);

    return -1;
}   