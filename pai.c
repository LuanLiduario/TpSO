#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int writepipe[2] = {-1, -1};
    pid_t childpid;

    if(pipe(writepipe) < 0) {
        perror("pipe");
        exit(1);
    }

    if((childpid = fork()) == -1) {
        perror("fork");
    }

    if(childpid == 0) {
        close(writepipe[1]);

        dup2(writepipe[0], STDIN_FILENO);
        close(writepipe[0]);
        execlp("./filho", "./filho", NULL);

    } else {
        char s = 'a';
	   
		close(writepipe[0]);
        write(writepipe[1], &s, 1);
        s = 'q';
        write(writepipe[1], &s, 1);
    }

    wait(0);
}
