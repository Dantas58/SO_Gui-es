#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


int create_fifo(){

    if(mkfifo("fifo",0660)==-1){

        perror("Error during FIFO creation\n");
        return -1;
    }

    return 1;   
}


#define size 30

int read_input(){

    int fd;

    if((fd = open("fifo", O_WRONLY,0660)) == -1){
        perror("Error: could not open FIFO");
        return -1;
    }

    int buffer[size];
    int read_bytes;

    while((read_bytes = read(STDIN_FILENO, buffer, size))>0) write(fd, buffer, read_bytes);

    close(fd);

    return 1;
}

int write_output(){

    int fd;

    if((fd = open("fifo", O_RDONLY, 0660)) == -1){
        perror("Error: could not open FIFO");
        return -1;
    }

    int buffer[size];
    int read_bytes;

    while((read_bytes = read(fd, buffer, size))>0) write(STDOUT_FILENO, buffer, read_bytes);

    close(fd);

    return 1;
}


int client(){

    int fd;

    if((fd = open("fifo", O_WRONLY, 0660))==-1){
        perror("Error: could not open FIFO");
        return -1;
    }

    int buffer[size];
    int read_bytes;

    while((read_bytes = read(STDIN_FILENO, buffer, size))>0) write(fd, buffer, read_bytes);

    close(fd);
    return 1;
}

int server(){

    int fd_FIFO;
    int fd_LOG;

    create_fifo();  // para ser mais rapido
    
    if((fd_FIFO = open("fifo", O_RDONLY, 0660))==-1){

        perror("Error: could not open FIFO\n");
        return -1;
    }

    if ((fd_LOG = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC ,0660))==-1){
        perror("Error: could not open LOGs\n");
        return -1;
    }

    int buffer[size];
    int read_bytes;

    while((read_bytes = read(fd_FIFO, buffer, size))>0) write(fd_LOG, buffer, read_bytes);

    close(fd_FIFO);
    close(fd_LOG);

    return 1;
}