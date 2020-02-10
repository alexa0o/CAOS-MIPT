#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void handler(int s)
{
    flag = 1;
}

int main()
{
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    sigaction(SIGINT, 
                &(struct sigaction){ .sa_handler = handler, 
                                    .sa_flags = SA_RESTART }, NULL);

    while (1) {
        // здесь ошибка race condition
        // сигнал может прийти в промежутке после проверки flag
        // но до вызова pause()
        sigprocmask(SIG_BLOCK, &mask, &oldmask);
        while (!flag) { sigsuspend(&oldmask); }
        printf("signal %d\n", getpid());
        flag = 0;
        sigprocmask(SIG_UNBLOCK, &mask, &oldmask);
    }
}
