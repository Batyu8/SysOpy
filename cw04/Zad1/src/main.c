#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int direction;

void SIGTSTP_Handler(int signo){
    direction = -direction;
}
void SIGINT_Handler(int signo){
    printf("\nOdebrano sygnal SIGINT\n");
    raise(SIGINT);
}


int main() {

    char letter = 'A';
    direction = 1;
    signal(SIGTSTP,SIGTSTP_Handler);
    struct sigaction sa;
    sa.sa_handler = SIGINT_Handler;
    sa.sa_flags = SA_RESETHAND;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    printf("%c ",letter);
    fflush(stdout);

    while(1){
        sleep(1);
        if(letter == 'Z' && direction == 1){
            letter = 'A';
        }
        else if(letter == 'A' && direction == -1) {
            letter = 'Z';
        }
        else {
            letter += direction;
        }
        printf("%c ",letter);
        fflush(stdout);
    }


    return 0;
}