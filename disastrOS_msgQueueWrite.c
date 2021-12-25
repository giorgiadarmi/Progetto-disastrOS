#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_msgqueue.h"
#include "disastrOS_descriptor.h"


void internal_msgQueueWrite() {


    int mqdes = (int) running->syscall_args[0];
    const char *msg_ptr = (const char *) running->syscall_args[1];
    unsigned int msg_len = (unsigned int) running->syscall_args[2];
    unsigned int priority = (unsigned int) running->syscall_args[3];

    Descriptor *desc = DescriptorList_byFd(&running->descriptors, mqdes);
    
    if (!desc) {
        printf("[ERRORE] la coda di messaggi (fd = %d) non è stata aperta per il processo in running!\n", mqdes);
        running->syscall_retvalue = DSOS_EMQ_WRITE;
        return;
    }
    
    MsgQueue *mq = (MsgQueue*)desc->resource;

    if (mq->size == MAX_NUM_MESSAGES_PER_MSG_QUEUE) { // coda piena
        printf("[ERRORE] Non riesce a scrivere msg: la coda di messaggi (fd =% d) è piena!\n", mqdes);
        running->syscall_retvalue = 0;
        return;
    }
    if (msg_len > MAX_TEXT_LEN) {   // messaggio troppo grande
        printf("[ERRORE] La massima lunghezza dei messaggi è %d!\n", MAX_TEXT_LEN);
        running->syscall_retvalue = DSOS_EMQ_WRITE;
        return;
    }


    Subqueue *msgSubqueue = mq->subqueues[priority];
    printf("Allocato un nuovo messaggio nella subqueue con priorità %u\n", priority);
    Message *msg = Message_alloc(msg_ptr, msg_len);

    if (!msg) {
        printf("[ERRORE] Fallimento nell'allocazione di un nuovo messaggio!\n");
        running->syscall_retvalue = DSOS_EMQ_WRITE;
        return;
    }
    // inserisco (scrivo) messaggio nella lista di messaggi della sottocoda con quella priorità
    List_insert(&(msgSubqueue->messages), msgSubqueue->messages.last, (ListItem *) msg);
    ++(mq->size);

    printf(">> Messaggio scritto!\n");
    running->syscall_retvalue = msg_len;



}
