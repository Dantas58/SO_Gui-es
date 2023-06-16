#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

int simple_pipe(int x){

    int pp[2];
    int save;
    
    if(pipe(pp)<0){
        perror("Error during pipe creation");
        return -1;
    }

    if(fork()==0){

        close(pp[1]);

        read(pp[0], &save, sizeof(int));
        printf("[CHILD] INT %d read from pipe.\n", save);

        close(pp[0]);
        _exit(0);
    }
    else{

        close(pp[0]);

        write(pp[1], &x, sizeof(int));
        printf("[PARENT] INT %d written to pipe.\n", x);

        close(pp[1]);

        wait(NULL);
    }

    return 1;
}


//ex 6 

#define lines 5
#define columns 120
typedef struct vetor{
    int line;
    int column;
    struct vetor* next;
}Vetor;

int find_int(int matriz[lines][columns], int x, Vetor *result){

    int pp[2];

    if(pipe(pp)<0){
        printf("Error during pipe creation\n");
        return -1;
    }

    for(int l = 0; l<lines; l++){

        if(fork()==0){

            close(pp[0]);
            Vetor* temp = malloc(sizeof(Vetor));

            for(int c = 0; c<columns; c++){
                if(matriz[l][c] == x){
                    temp->line = l;
                    temp->column = c;
                    temp->next = NULL;
                    write(pp[1], temp, sizeof(Vetor));
                }
            }

            close(pp[1]);
            _exit(0);
        }
    }

    close(pp[1]);
    Vetor *aux;

    while(read(pp[0], result, sizeof(Vetor))>0){

        aux = result;
        result->next = malloc(sizeof(Vetor));
        result = result->next;
    }

    aux->next = NULL;
    close(pp[0]);
    wait(NULL);

    return 1;
}