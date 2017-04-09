#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#include <stdbool.h>

int call_counter;
pid_t *pids;
bool got_enough_calls_flag;
int children_terminated;

void SIGUSR1_handler(int signo, siginfo_t* info, void* vp){
    pid_t sender = info->si_pid;
    sleep(1);
    if(!got_enough_calls_flag){
        pids[call_counter] = sender;
        call_counter++;
    }
    else{
        kill(sender,SIGCONT);
    }
}

void SIGRT_handler(int signo, siginfo_t* info, void* vp){
    pid_t sender = info->si_pid;
    printf("Received real-time signal %i from child with pid: %i\n",signo,sender);
}

void SIGCHLD_handler(int signo,siginfo_t* info, void* vp){
    pid_t sender = info->si_pid;
    int status;
    waitpid(sender,&status,0);
    printf("Child with pid: %i terminated with code: %i\n",sender,WEXITSTATUS(status));
    children_terminated++;
}

void SIGCONT_handler(int signo){
    printf("Child with pid: %i received signal SIGCONT\n",getpid());
}


int main(int argc, char **argv) {

    if(argc != 3){
        printf("Wrong number of parameters\n");
        return 1;
    }

    long children = strtol(argv[1],NULL,10);
    if(errno == EINVAL || errno == ERANGE){
        printf("Incorrect first argument\n");
        return 1;
    }
    long calls = strtol(argv[2],NULL,10);
    if(errno == EINVAL || errno == ERANGE){
        printf("Incorrect second argument\n");
        return 1;
    }

    pids = malloc(children*sizeof(pid_t));
    got_enough_calls_flag = false;
    children_terminated = 0;
    call_counter = 0;

    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGUSR1);
    sigaddset(&sigset,SIGCHLD);
    sigaddset(&sigset,SIGCONT);
    for(int i=SIGRTMIN;i<=SIGRTMAX;i++){
        sigaddset(&sigset,i);
    }
    struct sigaction sa;
    sa.sa_handler = NULL;
    sa.sa_sigaction = SIGUSR1_handler;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_mask = sigset;
    sigaction(SIGUSR1,&sa,NULL);

    sa.sa_sigaction = NULL;
    sa.sa_handler = SIGCONT_handler;
    sa.sa_flags = 0;
    sa.sa_mask = sigset;
    sigaction(SIGCONT,&sa,NULL);

    sa.sa_handler = NULL;
    sa.sa_sigaction = SIGCHLD_handler;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_mask = sigset;
    sigaction(SIGCHLD,&sa,NULL);

    sa.sa_handler = NULL;
    sa.sa_sigaction = SIGRT_handler;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_mask = sigset;

    for(int i=SIGRTMIN;i<=SIGRTMAX;i++){
        sigaction(i,&sa,NULL);
    }
    for(int i=0;i<children;i++){

        if(fork() == 0){
            srand(i);
            int sleep_time = rand()%11;
            pid_t pid = getpid();
            printf("Child with pid: %i is going to sleep for: %i seconds\n",pid,sleep_time);
            sleep(sleep_time);
            pid_t ppid = getppid();
            union sigval rt;
            rt.sival_int = 0;
            rt.sival_ptr = NULL;
            time_t start = time(NULL);
            printf("Child with pid: %i is sending signal SIGUSR1\n",pid);
            kill(ppid,SIGUSR1);
            pause();
            printf("Child eith pid: %i unpaused\n",pid);
            int rtsig = rand()%(SIGRTMAX-SIGRTMIN+1)+SIGRTMIN;
            sigqueue(ppid,rtsig,rt);
            printf("Child with pid: %i sent rt signal %i \n",pid,rtsig);
            time_t end = time(NULL);
            int time_taken = end-start;
            printf("Children with PID: %i ending. Time taken: %d\n", pid,time_taken);
            _exit(time_taken);
        }
    }

    while(call_counter < calls)
        pause();


    got_enough_calls_flag = true;
    printf("Got enough signals\n");

    for(int i=0;i<call_counter;i++){
        kill(pids[i],SIGCONT);
    }

    printf("Waiting for children to terminate\n");
    while(children_terminated < children){
        pause();
    }

    free(pids);

    return 0;
}