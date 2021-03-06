#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int writepipe[2] = {-1, -1};
    pid_t childpid;
    if(pipe(writepipe) < 0)
    {
        perror("pipe");
        exit(1);
    }
    if((childpid = fork()) == -1)
    {
        perror("fork");
    }
    if(childpid == 0)
    {
        close(writepipe[1]);
        //bloqueia a entrada por teclado
        dup2(writepipe[0], STDIN_FILENO);
        close(writepipe[0]);
        execlp("./manager", "./manager", NULL);

    } else 
    {//manda os comandos para o processo manager pelo pipe
        char string;
        close(writepipe[0]);
        while(string != 'T'){
          scanf("%c", &string);
          //escreve os comandos no manger
          write(writepipe[1], &string, 1);
          sleep(1);
        }
    }
    wait(0);
}
