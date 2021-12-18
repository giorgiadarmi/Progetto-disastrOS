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
    
    //creiamo descrittore per la risorsa allocata in questo processo e lo allochiamo
    Descriptor *desc = Descriptor_alloc(running->last_fd, (Resource*) mq, running);
    //errore in caso di file descriptor non valido
    if (!desc) {
        running->syscall_retvalue = DSOS_EMQ_NOFD;
        return;
    }

    running->last_fd++; //incrementiamo il valore dell'ultimo file descriptor
    //allochiamo il puntatore al descrittore 
    DescriptorPtr *descPtr = DescriptorPtr_alloc(desc);
    //aggiungere alla risorsa, nella lista dei descrittori, il descrittore appena creato
    List_insert(&running->descriptors, running->descriptors.last, (ListItem *) desc);

    // aggiungere alla risorsa, nella lista dei descrittori ptr, un puntatore al descrittore appena creato
    desc->ptr = descPtr;
    List_insert(&(mq->resource.descriptors_ptrs), mq->resource.descriptors_ptrs.last, (ListItem *) descPtr);

    disastrOS_debug("Message queue con nome '%s' aperto!\n", name);

    //ritorna il file descriptor associato al descrittore del processo
    running->syscall_retvalue = desc->fd;



}
