#include "kernel/types.h"
//#include "kernel/stat.h"
#include "user/user.h"

#define READER 0
#define WRITER 1

int main(void)
{
    int pwcr[2];//parent write child read
    int cwpr[2];//child write parent read
    pipe(pwcr);
    pipe(cwpr);
    //create two pips

    int pid=fork();
    if (pid == 0){//child process
        char msg;
        read(pwcr[READER],&msg,1);
        close(pwcr[READER]);
        printf("%d: received ping\n", getpid());

        write(cwpr[WRITER],&msg, 1);
        close(cwpr[WRITER]);
        //avoid the read end being blocked
    }
    else if(pid>0){//parent process
        write(pwcr[WRITER], "b", 1);
        close(pwcr[WRITER]);
        //avoid the read end being blocked

        char msg;
        read(cwpr[READER],&msg, 1);
        printf("%d: received pong\n", getpid());

        wait(0);
    }
    else{//handles error
        fprintf(2, "fork error\n");
        exit(1);
    }

    exit(0);
}