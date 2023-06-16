#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>


// 1
/*
int main(int argc, char **argv){

    if(argc<3){
        printf("Não há 3 argumentos paneleiro.");
        return -1;
    }

    int src = open(argv[1],O_RDONLY);
    if(src<0){
        perror("Não dá pa abrir paneleiro.");
        return -1;
    }

    int dest = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0640);
    if(dest<0){
        perror("Não dá pa abrir paneleiro.");
        return -1;
    }

    char *buffer = malloc(20*sizeof(char));
    int count = 0;
    while(count = read(src, buffer, 20)>0){
        write(dest, buffer, count);
    }

    close(src);
    close(dest);
    return 0;
} 
*/


// 2
/*
int main(int argc, char **argv){

    int count = 0;
    char *buffer = malloc(20*sizeof(char));
    while(count = read(0, buffer, 20)>0) write(1, buffer, count);

    free(buffer);

    return 0;
}
*/

/*
// 3
int readChar(int fd, char *buffer){

    return read(fd, buffer, 1);
}

ssize_t readln(int fd, char* line, size_t size) {

    int next_pos = 0;
    int read_bytes = 0;

    while(next_pos < size && readChar(fd, line+next_pos) > 0){

        read_bytes++;
        if(!line[next_pos]) return read_bytes;

        next_pos++; 
    }
    return read_bytes;
}
*/

/*
// 4
#define MAX_BUFFER 4096

int main (int agrc, char *argv[]){

    int line_counter = 0;
    int buffer[MAX_BUFFER];
    int bytes_read = 0;
    int new_line = 1;

    while(bytes_read = readln(0, buffer, MAX_BUFFER) > 0){

        char line_number[10] = "";

        // escreve o indice da linha
        if(new_line && buffer[0]!='\n'){
            snprintf(line_number, 10, "%d", line_counter);
            write(1, line_number, sizeof(line_number));
            line_counter++;
        }
        write(1, buffer, bytes_read);

        // não encontra '\n', continua a ler
        if(buffer[bytes_read-1]!='\n') new_line = 0;
        else new_line = 1;
    }
    return 0;
}
*/

// 5
int new_person(char *nome, int age){

    Person pessoa;
    pessoa.age = age;

    if(strlen(nome)>Max){
        perror("Nome excede o limite de caractéres");
        return -1;
    }

    strncpy(pessoa.name, nome, Max);

    int fd = open(FILENAME, O_CREAT | O_APPEND | O_WRONLY, 0640);

    if(fd<0){
        perror("Erro na abertura do ficheiro");
        return -1;
    }

    write(fd, &pessoa, sizeof(Person));

    int r = lseek(fd, -sizeof(Person), SEEK_END)/(sizeof(Person));
    if(r<0){
        perror("Erro no lseek");
        return -1;
    }

    close(fd);
    return r;

}

// abrir em RD e WR
// while ate encontrar a pessoa (com strcmp) ou ate final do ficheiro
// se encontrarmos a pessoa atualizar idade e lseek para tras e atualizar em disco
// fechar ficheiro
int person_change_age(char *nome, int age){


}

// abrir na mesma em RD e WR
// lseek para a posiçao da pessoa
// ler a pessoa para o buffer
// atualizar idade
// fechar fihceiro
int person_change_age_v2(long pos, int age){


}


int main(int argc, char * argv[]){

    char id[20] = "";

    if(strcmp(argv[1], "-i") == 0){
        int res = new_person(argv[2], atoi(argv[3]));

    }
}
