#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_msgqueue.h"
#include "disastrOS_descriptor.h"


void internal_msgQueueClose() {+

	//dichiariamo il file descriptor della coda di messaggi che si vuole chiudere 
    int fd = running->syscall_args[0];

	//troviamo il file descriptor corrispondennte alla coda di messaggi e lo assegniamo al puntatore desc
    Descriptor *desc = DescriptorList_byFd(&running->descriptors, fd);
    DescriptorPtr *descPtr = desc->ptr;

    //se la coda di messaggio è già chiusa lo comunichiamo
    if (!desc) {
        printf("[ERRORE] Msg queue (fd = %d) è già chiusa!\n", fd);
        running->syscall_retvalue = 0;
        return;
    }

	//creiamo descrittore per la coda di messaggio 
    MsgQueue *mqdesc = (MsgQueue*) desc->resource;

	//stacchiamo il descrittore (deallochiamo) e il suo puntatore dalla lista dei descrittori e lista dei puntatori
    List_detach(&(mqdesc->resource.descriptors_ptrs), (ListItem *) descPtr);
    List_detach(&(running->descriptors), (ListItem *) desc);
	
    if (DescriptorPtr_free(descPtr) != 0) {
        printf("[ERRORE] Fallimento nella deallocazione del puntatore al descrittore (fd = %d)!\n", fd);
        running->syscall_retvalue = DSOS_EMQ_CLOSE;
        return;
    }
    if (Descriptor_free(desc) != 0) {
        printf("[ERRORE] Fallimento nella deallocazione del descrittore con fd %d!\n", fd);
        running->syscall_retvalue = DSOS_EMQ_CLOSE;
        return;
    }

	//se tutti i descrittori dei processi che hanno aperto la cosa vengono deallocati dalla lista dei descrittori
	//quindi tutti i pricessi che hanno aperto la coda chiudono i descrittori associati ad essa
	//la coda verrà distrutta
    if (mqdesc->resource.descriptors_ptrs.size == 0) {
        disastrOS_debug("Tutti i descrittori sono chiusi: msg queue (fd = %d) unlinked!\n", fd);

        running->syscall_args[0] = (long) mqdesc->resource.name;
        internal_msgQueueUnlink();
        if (running->syscall_retvalue == DSOS_EMQ_CLOSE)
            return;
    }
    disastrOS_debug("Message queue (fd = %d) closed e unlinked\n", fd);
    running->syscall_retvalue = 0;




}
