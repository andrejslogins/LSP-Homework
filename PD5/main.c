#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <zconf.h>
#include <memory.h>


jmp_buf buffer;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        printf("Recieved signal\n");
    longjmp(buffer,1);
}

int main(void)
{
    if(setjmp(buffer)){
        return 0;
    }

    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\nerror\n");

    while(1)
        sleep(1);

    // taalaako kodu nemainiit!

    // taimeris uz 3 sekundeem
    alarm(3);

    // bezgaliigs cikls
    while (1);

    // atgriezh kaut kaadu nenulles kodu, lai kompilators nesuudzeetos
    return -1;
}

