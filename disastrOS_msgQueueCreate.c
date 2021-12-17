#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_msgqueue.h"
#include "disastrOS_descriptor.h"

//Crea la coda di messaggi con il nome preso in input
void internal_msgQueueCreate() {
	//input coda di messaggi da creare
    const char *name = (const char*)running->syscall_args[0];

	if (!name) {
        printf("[ERRORE] 'name' deve essere diverso da NULL!\n");
        running->syscall_retvalue = DSOS_EMQ_CREATE;
        return;
    }
    
	//allocazione subqueue e coda tramite pull alloctor
    MsgQueue *mqdes = MsgQueue_alloc(name, last_rid++, running);
    if(!mqdes){
        printf("[ERRORE] Non è possibile allocare una nuova coda di messaggio!\n");
        running->syscall_retvalue = DSOS_EMQ_CREATE;
        return;
    }
    
    MsgQueuePtr *mqPtr = MsgQueuePtr_alloc(mqdes);
    if(!mqPtr){
        printf("[ERRORE] Non è possibile allocare un puntatore alla coda di messaggio con nome!\n");
        running->syscall_retvalue = DSOS_EMQ_CREATE;
        return;
    }
    
    
	//inserimento delle strutture allocate nella resources_list e msg_queue_list
	printf(">> Inserita coda di messaggio in (rid = %d) in resources list\n", rid);
    List_insert(&resources_list, resources_list.last, (ListItem *)mqdes);
    printf(">> Inserito il puntatore della coda di messaggio in msg queues list\n");
    List_insert(&msg_queues_list, msg_queues_list.last, (ListItem *)mqPtr);

	//Messaggio con nome 'name' è stato creato

    running->syscall_retvalue = 0;
}
