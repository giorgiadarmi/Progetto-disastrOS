#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_msgqueue.h"
#include "disastrOS_descriptor.h"


void internal_msgQueueUnlink() {
	
	//specifichiamo il nome della coda di messaggi che si vuole aprire
    const char *name = (const char *) running->syscall_args[0];

	//trova il puntatore della coda di messaggi con il nome richiesto, tramite la funzione findbyname e assegnana a NULL al descrittore
    MsgQueuePtr *mqPtr = MsgQueuePtrList_findByName(&msg_queues_list, name);
    MsgQueue *mqdesc = NULL;

	//se il puntatore è diverso da NULL, il descrittore della coda di messaggi mette tutti i puntatori dei suoi descrittori a NULL
    if (mqPtr != NULL) {
        mqdesc = mqPtr->msgQueuePtr;
    }
    
    //se il descrittore della cosa di messaggio è diverso da zero, non esiste la coda di messaggio con quel nome e ritorna l'errore
    if (!mqdesc) {
        printf("Non esiste message queue '%s'!\n", name);
        running->syscall_retvalue = DSOS_EMQ_UNLINK;
        return;
    }

	//se il descrittore della coda di messaggi ha dimensione maggiore di 0, ci sono descrittori aperti nella coda di messaggi
    if (mqdesc->resource.descriptors_ptrs.size > 0) {
        printf("[ERRORE] Il message queue'%s' ha descrittori aperti!\n", name);
        running->syscall_retvalue = 0;
        return;
    }
    
    //stacchiamo e deallochiamo il puntatore alla coda di messaggi e il suo descrittore con eventuali errori 
    List_detach(&msg_queues_list, (ListItem *) mqPtr);
    List_detach(&resources_list, (ListItem *) mqdesc);

    if (MsgQueuePtr_free(mqPtr) != 0) {
        printf("[ERRORE] Fallimento della deallocazione del message queue ptr per il message queue '%s'\n!", name);
        running->syscall_retvalue = DSOS_EMQ_UNLINK;
        return;
    }

    if (MsgQueue_free(mqdesc) != 0) {
        printf("[ERRORE] Fallimento della deallocazione del  message queue '%s'!\n", name);
        running->syscall_retvalue = DSOS_EMQ_UNLINK;
        return;
    }

    disastrOS_debug("Message queue con name '%s' distrutto!\n", name);
    running->syscall_retvalue = 0;


}
