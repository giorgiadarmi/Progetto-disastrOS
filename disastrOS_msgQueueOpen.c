#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_msgqueue.h"
#include "disastrOS_descriptor.h"


void internal_msgQueueOpen() {

    //specifichiamo il nome della coda di messaggi che si vuole aprire
    const char *name = (const char*) running->syscall_args[0];

	//trova il puntatore della coda di messaggi con il nome richiesto, tramite la funzione findbyname e lo assegna ad un puntatore
    MsgQueuePtr *mqdesc = MsgQueuePtrList_findByName(&msg_queues_list, name);
    MsgQueue *mq = mqdesc->msgQueuePtr;

	//errore in caso di coda di messaggi non trovata
    if (!mq) {
        running->syscall_retvalue = DSOS_EMQ_NOEXIST;
        return;
    }

}
