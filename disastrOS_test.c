#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <poll.h>
#include "disastrOS.h"

#define ITERATIONS 3

void producer() {
    char buf[256]; //dichiariamo buffer per messaggio
    printf("[PRODUCER pid = %d] Start!\n", disastrOS_getpid());
    printf("[PRODUCER pid = %d] Allocazione del descrittore associato al message queue\n", disastrOS_getpid());
    int mqdes = disastrOS_msgQueueOpen("/mq"); //coda del messaggio

    printf("[PRODUCER pid = %d] Msg queue con fd = %d opened \n", disastrOS_getpid(), mqdes);
    disastrOS_printStatus();
    printf("[PRODUCER pid = %d] preempt(): CPU to CONSUMER \n\n", disastrOS_getpid()); 
    printf("[CONSUMER] Running!\n");disastrOS_preempt(); //processo successivo
    disastrOS_printStatus();

    unsigned priority;

    for (unsigned i = 0; i < ITERATIONS; ++i) {
        memset(buf, 0, 256); //settiamo buf del messaggio a zero

        priority = rand() % 10; // imposta priorità casuale
        sprintf(buf, "Msg #%u", 1);
        printf("[PRODUCER pid = %d] Scritto msg: '%s', priority: %u\n",disastrOS_getpid(), buf, priority);
        disastrOS_msgQueueWrite(mqdes, buf, strlen(buf), priority); //scrive messaggio in coda 
        disastrOS_printStatus(); //stampa stato sistema

        memset(buf, 0, 256);

        priority = rand() % 10; // imposta priorità casuale
        sprintf(buf, "Msg #%u", 2);
        printf("[PRODUCER pid = %d] Scritto msg: '%s', priority: %u\n", disastrOS_getpid(), buf, priority);
        disastrOS_msgQueueWrite(mqdes, buf, strlen(buf), priority);
        disastrOS_printStatus();

        memset(buf, 0, 256);

        priority = rand() % 10; // imposta priorità casuale
        sprintf(buf, "Msg #%u", 3);
        printf("[PRODUCER pid = %d] Scritto msg: '%s', priority: %u\n", disastrOS_getpid(), buf, priority);
        disastrOS_msgQueueWrite(mqdes, buf, strlen(buf), priority);
        disastrOS_printStatus();


        printf("[PRODUCER pid = %d] preempt(): CPU to CONSUMER\n\n", disastrOS_getpid());
        printf("[CONSUMER] in running\n");
        disastrOS_preempt();

    }

	printf("[PRODUCER (pid = %d)] Deaccollocati subqueues e msg queue. Chiuso msg queue (fd = %d)\n", disastrOS_getpid(), mqdes);
    disastrOS_msgQueueClose(mqdes); //chiudiamo descrittore coda di messaggi
    //printf("[PRODUCER pid = %d] Msg queue (fd = %d) closed \n", disastrOS_getpid(), mqdes);
    disastrOS_printStatus();

    printf("[PRODUCER pid = %d] exit(0)\n", disastrOS_getpid());
    disastrOS_exit(0);

}

void consumer() {
    char buf[256];
    printf("[CONSUMER pid = %d] Start!\n", disastrOS_getpid());
    printf("[CONSUMER pid = %d] Allocazione del descrittore associato al message queue\n", disastrOS_getpid());
    int mqdes = disastrOS_msgQueueOpen("/mq");
    printf("[CONSUMER pid = %d] Msg queue (fd = %d) opened \n", disastrOS_getpid(), mqdes);
    disastrOS_printStatus();
    printf("[CONSUMER pid = %d] preempt: CPU to PRODUCER \n\n", disastrOS_getpid());
    printf("[PRODUCER] in running\n");
    disastrOS_preempt();
    disastrOS_printStatus();

    for (unsigned i = 0; i < ITERATIONS; ++i) {

        memset(buf, 0, 256);
        printf("[CONSUMER pid = %d] Letto primo msg\n", disastrOS_getpid());
        disastrOS_msgQueueRead(mqdes, buf, 256); //legge messaggio dalla coda puntata dal descrittore mqdes
        disastrOS_printStatus();

        memset(buf, 0, 256);
        printf("[CONSUMER pid = %d] Letto secondo msg\n", disastrOS_getpid());
        disastrOS_msgQueueRead(mqdes, buf, 256);
        disastrOS_printStatus();

        memset(buf, 0, 256);
        printf("[CONSUMER pid = %d] Letto terzo msg\n", disastrOS_getpid());
        disastrOS_msgQueueRead(mqdes, buf, 256);
        disastrOS_printStatus();

        printf("[CONSUMER pid = %d] preempt(): CPU to PRODUCER\n", disastrOS_getpid());
        printf("[PRODUCER] in running\n");
        disastrOS_preempt();
    }

    printf("[CONSUMER pid = %d] Msg queue (fd = %d) closed\n", disastrOS_getpid(), mqdes);
    disastrOS_msgQueueClose(mqdes);
    disastrOS_printStatus();

    printf("[CONSUMER pid = %d] exit(0)\n", disastrOS_getpid());
    disastrOS_exit(0);
}

void childFunc(void *argsPtr) {
    printf("[childFunc pid = %d] Start! \n", disastrOS_getpid());

    disastrOS_spawn(consumer, NULL);
    printf("[childFunc pid = %d] Spawning consumer \n", disastrOS_getpid());
    disastrOS_printStatus();

    disastrOS_spawn(producer, NULL);
    printf("[childFunc pid = %d] Spawning producer \n", disastrOS_getpid());
    disastrOS_printStatus();

	printf("[childFunc pid = %d] Allocata subqueue e msg queue \n", disastrOS_getpid());
    disastrOS_msgQueueCreate("/mq");
    disastrOS_printStatus();

    printf("[childFunc pid = %d] Va alla waiting list: Attende terminazione del processo figlio\n", disastrOS_getpid());

    disastrOS_wait(0, NULL);  // attende terminazione di uno qualsiasi dei figli
    printf("[childFunc pid = %d] wait(0, NULL) -> legge valore di ritorno e attende la terminazione del processo figlio\n", disastrOS_getpid());
    disastrOS_printStatus();

    printf("[childFunc pid = %d] Va alla waiting list: Attende la terminazione del prossimo processo figlio\n", disastrOS_getpid());

    disastrOS_wait(0, NULL);
    printf("[childFunc pid = %d] wait(0, NULL) -> legge valore di ritorno e attende la terminazione del processo figlio\n", disastrOS_getpid());
    disastrOS_printStatus();

    printf("[childFunc pid = %d] Prova a rimuovere msg queue\n", disastrOS_getpid());
    disastrOS_msgQueueUnlink("/mq");

    printf("[childFunc pid = %d] exit(0)\n", disastrOS_getpid());
    disastrOS_exit(0);

}

void initFunc(void *argsPtr) {
    printf("[init (pid = %d)] Start! \n", disastrOS_getpid());

    printf("[init (pid = %d)] Spawning childFunc\n", disastrOS_getpid());
    disastrOS_spawn(childFunc, NULL);

    disastrOS_printStatus();

    printf("init va sulla waiting list: attede la termineazione di childFunc\n");
    disastrOS_wait(0, NULL);
    printf("[init] wait(0, NULL) --> legge valore di ritorno di childFunc\n");

    disastrOS_printStatus();

    printf("[init] Shutdown!\n\n");
    disastrOS_shutdown();
}

int main(int argc, char** argv) {
    printf("\n************************** IPC with MESSAGE QUEUE ******************************\n\n");
    char* logfilename = 0;
    if (argc > 1) {
        logfilename = argv[1];
    }
    // we create the init process processes
    // the first is in the running variable
    // the others are in the ready queue
    // spawn an init process
    printf("Start! \n");
    disastrOS_start(initFunc, 0, logfilename);
    return 0;
}
