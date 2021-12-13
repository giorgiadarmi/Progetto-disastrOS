#pragma once

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "pool_allocator.h"
#include "disastrOS_resource.h"
#include "disastrOS_descriptor.h"

// Funzioni associate a Text del msg
void Text_init(); //inizializzo testo
char* Text_alloc(); //alloco testo
int Text_free(char *s); //libero spazio

// Definisce messaggio
typedef struct Message {
    ListItem list;
    char *msg_ptr; // puntatore al testo del messaggio
    unsigned msg_len; // lunghezza messaggio
} Message;

void Message_init(); //inizializzo messaggio
Message* Message_alloc(const char* msg, unsigned size); //alloco messaggio
int Message_free(Message* msg); //libero memoria messaggio
void Message_print(const Message* msg); //stampo messaggio

// Definisce una lista di messaggi
typedef ListHead MessageList;
void MessageList_print(MessageList *list);



// Definisce sottocoda di messaggi con priorità fissata
typedef struct Subqueue {
    ListItem list;
    MessageList messages;
    unsigned priority;
} Subqueue;

void Subqueue_init(); //inizializzo sottocoda
Subqueue * Subqueue_alloc(unsigned priority); //alloco sottocoda
int Subqueue_free(Subqueue *msg); //dealloco sottocoda
void Subqueue_print(const Subqueue *msg); //stampo sottocoda 



// definisce coda di messaggi
typedef struct MsgQueue {
    Resource resource;
    Subqueue *subqueues[MAX_NUM_PRIORITIES];         //definizione numero di processi prioritari
    PCB* pcb;                                       // identifica processo running che alloca msg queue
    int size;                                       // numero di messaggi che compongono la coda
    ListHead waiting_descriptors;
} MsgQueue;

void MsgQueue_init();  //inizializzazione coda di messaggi 
MsgQueue* MsgQueue_alloc(const char *name, int id, PCB *pcb);    //allocazione coda di messaggi
int MsgQueue_free(MsgQueue *q);    //deallocazione coda di messaggi
void MsgQueue_print(const MsgQueue *q);    //stampa coda



// Definisce un riferimento alla coda di msg, puntatore
typedef struct MsgQueuePtr {
    ListItem list;
    MsgQueue *msgQueuePtr;
} MsgQueuePtr;

void MsgQueuePtr_init();   //inizializzo puntatore
MsgQueuePtr* MsgQueuePtr_alloc(MsgQueue *q);    //alloco puntatore 
int MsgQueuePtr_free(MsgQueuePtr *q);    //dealloco puntatore
void MsgQueuePtr_print(const MsgQueuePtr *q);    //stampo puntatore

// Definisce lista di riferimenti alle code di msg
typedef ListHead MsgQueuePtrList;

// Ricerca cosa di messaggi per nome
MsgQueuePtr * MsgQueuePtrList_findByName(MsgQueuePtrList *l, const char *name);
void MsgQueuePtrList_print(MsgQueuePtrList *l);
