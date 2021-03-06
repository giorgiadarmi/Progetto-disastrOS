#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pool_allocator.h"
#include "disastrOS.h"
#include "disastrOS_msgqueue.h"
#include "disastrOS_constants.h"
#include "linked_list.h"

// macro associate al Text del messaggio
#define TEXT_SIZE sizeof((MAX_TEXT_LEN + 1) * sizeof(char))
#define TEXT_MEM_SIZE (TEXT_SIZE + sizeof(int))
#define TEXT_BUFFER_SIZE MAX_NUM_MESSAGES * TEXT_MEM_SIZE

// macro associate al Message
#define MESSAGE_SIZE sizeof(Message)
#define MESSAGE_MEM_SIZE (sizeof(Message) + sizeof(int))
#define MESSAGE_BUFFER_SIZE MAX_NUM_MESSAGES * MESSAGE_MEM_SIZE

// macro associate a Subqueue
#define SUBQUEUE_SIZE sizeof(Subqueue)
#define SUBQUEUE_MEM_SIZE (sizeof(Subqueue) + sizeof(int))
#define SUBQUEUE_BUFFER_SIZE MAX_NUM_PRIORITIES * MAX_NUM_RESOURCES * SUBQUEUE_MEM_SIZE

// macro associate alla MsgQueue
#define MSG_QUEUE_SIZE sizeof(MsgQueue)
#define MSG_QUEUE_MEM_SIZE (MSG_QUEUE_SIZE + sizeof(int))
#define MSG_QUEUE_BUFFER_SIZE MAX_NUM_RESOURCES * MSG_QUEUE_MEM_SIZE

// macro associate a MsgQueuePtr
#define MSG_QUEUE_PTR_SIZE sizeof(MsgQueuePtr)
#define MSG_QUEUE_PTR_MEM_SIZE (sizeof(MsgQueuePtr) + sizeof(int))
#define MSG_QUEUE_PTR_BUFFER_SIZE MAX_NUM_RESOURCES * MSG_QUEUE_PTR_MEM_SIZE

// pool allocator allocazione delle risorse da disastrOS_resource.c
static char _texts_buffer[TEXT_BUFFER_SIZE];
static PoolAllocator _texts_allocator;

static char _messages_buffer[MESSAGE_BUFFER_SIZE];
static PoolAllocator _messages_allocator;

static char _subqueues_buffer[SUBQUEUE_BUFFER_SIZE];
static PoolAllocator _subqueues_allocator;

static char _msg_queues_buffer[MSG_QUEUE_BUFFER_SIZE];
static PoolAllocator _msg_queues_allocator;

static char _msg_queue_ptrs_buffer[MSG_QUEUE_PTR_BUFFER_SIZE];
static PoolAllocator _msg_queue_ptrs_allocator;


//inserisco funzioni del pull allocator alla struttura dati TEXT (init, alloc->get Block e free->release Block)

// void Resource_init()
void Text_init() {
    PoolAllocatorResult res = PoolAllocator_init(&_texts_allocator,
                                                TEXT_SIZE,
                                                MAX_NUM_MESSAGES,
                                                _texts_buffer,
                                                TEXT_BUFFER_SIZE);

    if (res != Success) {
        printf("[ERRORE] Fallimento dell'inizializzazione di text allocator!\n");
        assert(res == Success);
    }
}

// Resource* Resource_alloc(int id, int type)
char* Text_alloc() {
    char* s = (char*) PoolAllocator_getBlock(&_texts_allocator);
    memset(s, 0, TEXT_MEM_SIZE);
    return s;
}

// int Resource_free(Resource* r)
int Text_free(char *s) {
    PoolAllocatorResult res = PoolAllocator_releaseBlock(&_texts_allocator, s);
    if (res != Success){
        printf("[ERRORE] Fallimento nella deallocazione di text allocator\n");
        return -1;
    }
    return 0;
}


//strutture del pull allocator alla struttura dati MESSAGE (init, alloc, free, print, printlist)

// void Resource_init()
void Message_init() {
    PoolAllocatorResult res = PoolAllocator_init(&_messages_allocator,
                                                MESSAGE_SIZE,
                                                MAX_NUM_MESSAGES,
                                                _messages_buffer,
                                                MESSAGE_BUFFER_SIZE);

    if (res != Success) {
        printf("[ERRORE] Fallimento dell'inizializzazione di message allocator!\n");
        assert(res == Success);
    }
}

// Resource* Resource_alloc(int id, int type)
Message* Message_alloc(const char *msg, unsigned size) {
    char *txt = Text_alloc();
    Message *new_msg = (Message*) PoolAllocator_getBlock(&_messages_allocator);

    if (!new_msg) {
        printf("[ERRORE] Fallimento nell'allocazione di un messaggio %s!\n", txt);
        Text_free(txt);
        return NULL;
    }

    new_msg->list.prev = new_msg->list.next = NULL;    // da disastOS_resource.c r->list.prev = r->list.next = 0;

    strcpy(txt, msg); //copiamo il testo nel messaggio

    new_msg->msg_ptr = txt;                     // da disastOS_resource.c r->id=id;
    new_msg->msg_len = size;                    // da disastOS_resource.c r->type=type;
    printf(">> Messaggio '%s' allocato correttamente!\n", txt);
    return new_msg;
}

// int Resource_free(Resource* r)
int Message_free(Message *msg) {
    PoolAllocatorResult res = PoolAllocator_releaseBlock(&_messages_allocator, msg);
    if (res != Success) {
        printf("[ERRORE] Fallimento nella deallocazione di un messaggio %s!\n", msg->msg_ptr);
        return -1;
    }
    printf(">> Messaggio '%s' deallocato correttamente!\n", msg->msg_ptr);
    return 0;
}

// void Resource_print(Resource* r)
void Message_print(const Message *msg) {
    if (msg == NULL) printf("[ ]");
    printf("(messaggio: '%s', size: %d) ", msg->msg_ptr, msg->msg_len);
}

// ResourceList_print(ListHead* l)
void MessageList_print(MessageList *list) {
    printf("{");
    ListItem* aux = list->first;
    while(aux) {
        Message *msg = (Message*)aux;
        Message_print(msg);
        if (aux->next != NULL)
            printf(",");
        aux = aux->next;
    }
    printf("}");
}


//inserisco funzioni del pull allocator alla struttura dati SUBQUEUE (init, alloc, free, print)

// void Resource_init()
void Subqueue_init() {
    PoolAllocatorResult res = PoolAllocator_init(&_subqueues_allocator,
                                                SUBQUEUE_SIZE,
                                                MAX_NUM_PRIORITIES * MAX_NUM_RESOURCES, // numero massimo di risorse priority
                                                _subqueues_buffer,
                                                SUBQUEUE_BUFFER_SIZE);

    if (res != Success) {
        printf("[ERRORE] Fallimento inizializzazione messaggio subqueue allocator!\n");
        assert(res == Success);
    }
}

// Resource* Resource_alloc(int id, int type)
Subqueue* Subqueue_alloc(unsigned priority) {
    Subqueue *m = (Subqueue *)PoolAllocator_getBlock(&_subqueues_allocator);
    m->list.prev = m->list.next = NULL;
    List_init(&(m->messages));              // da desastrOS_ List_init(&r->descriptors_ptrs);
    m->priority = priority;
    printf(">> Messaggio subqueue con priorit?? %d allocato correttamente!\n", priority);
    return m;
}

// int Resource_free(Resource* r)
int Subqueue_free(Subqueue* msg) {
    for (ListItem *currMsg = msg->messages.first; currMsg != NULL;) {
        Message *oldMsg = (Message*)currMsg;
        currMsg = currMsg->next;
        List_detach(&msg->messages, (ListItem*)oldMsg);
        Message_free(oldMsg);
    }

    PoolAllocatorResult res = PoolAllocator_releaseBlock(&_subqueues_allocator, msg);
    if (res != Success) {
        printf("[ERRORE] Fallimento nella deallocazione del messaggio subqueue!\n");
        return -1;
    }
    return 0;
}

// void Resource_print(Resource* r)
void Subqueue_print(const Subqueue* msg) {
    if (msg == NULL)
        printf("[NULL]");
    else {
        printf("\n\t\t {priority: %d, messages: ", msg->priority);
        MessageList_print((ListHead*) & (msg->messages));
        printf(", count: %d}", msg->messages.size);
    }
}

//inserisco funzioni del pull allocator alla struttura dati MSG QUEUE PTR (init, alloc, free, print)

// void Resource_init()
void MsgQueue_init() {
    PoolAllocatorResult res = PoolAllocator_init(&_msg_queues_allocator,
                                                MSG_QUEUE_SIZE,
                                                MAX_NUM_RESOURCES,
                                                _msg_queues_buffer,
                                                MSG_QUEUE_BUFFER_SIZE);

    if (res != Success) {
        printf("[ERRORE] Faillimento inizializzazione message queues allocator!\n");
        assert(res == Success);
    }
}


// Resource* Resource_alloc(int id, int type)
MsgQueue* MsgQueue_alloc(const char *name, int id, PCB *pcb) {
    MsgQueue *q = (MsgQueue*)PoolAllocator_getBlock(&_msg_queues_allocator);
    q->resource.list.prev = q->resource.list.next = NULL;
    q->resource.name = name;
    q->resource.rid = id;
    q->resource.type = DSOS_MSG_QUEUE_RESOURCE;

    List_init(&(q->resource.descriptors_ptrs));

    for (unsigned priority = 0; priority < MAX_NUM_PRIORITIES; ++priority) {
        q->subqueues[priority] = Subqueue_alloc(priority);
    }

    q->pcb = pcb;
    q->size = 0;
    printf(">> Message queue con nome %s allocata correttamente!\n", name);
    return q;
}

// int Resource_free(Resource* resource)
int MsgQueue_free(MsgQueue *q) {
    for (unsigned priority = 0; priority < MAX_NUM_PRIORITIES; ++priority) {
        Subqueue_free(q->subqueues[priority]);
        printf(">> Subqueue con priorit?? %d deallocato correttamente!\n", priority);
    }
    PoolAllocatorResult res = PoolAllocator_releaseBlock(&_msg_queues_allocator, q);
    if (res != Success) {
        printf("[ERRORE] Fallimento deallocazione message queue!\n");
        return -1;
    }
    return 0;
}

// void Resource_print(Resource* r)
void MsgQueue_print(const MsgQueue *q) {
    if (q == NULL)
        printf("[NULL]");
    else {
        printf("[name: '%s', \n\t resource id: %d, \n\t descriptors: ", q->resource.name, q->resource.rid);
        DescriptorPtrList_print((ListHead *) &(q->resource.descriptors_ptrs));
        printf(",  \n\t subqueues: ");
        for (unsigned priority = 0; priority < MAX_NUM_PRIORITIES; ++priority) {
            Subqueue_print(q->subqueues[priority]);
            if (priority + 1 < MAX_NUM_PRIORITIES)
                printf(",\n");
        }
        //printf("\n\t]");
    }


}


//inserisco funzioni del pull allocator alla struttura dati MSG QUEUE PTR (init, alloc, free, print)

// void Resource_init()
void MsgQueuePtr_init() {
    PoolAllocatorResult res = PoolAllocator_init(&_msg_queue_ptrs_allocator,
                                                MSG_QUEUE_PTR_SIZE,
                                                MAX_NUM_RESOURCES,
                                                _msg_queue_ptrs_buffer,
                                                MSG_QUEUE_PTR_BUFFER_SIZE);

    if (res != Success) {
        printf("[ERRORE] Fallimento inizializzazione message queue ptr allocator!\n");
        assert(res == Success);
    }
}

// Resource* Resource_alloc(int id, int type);
MsgQueuePtr* MsgQueuePtr_alloc(MsgQueue *q) {
    MsgQueuePtr *mq = (MsgQueuePtr*)PoolAllocator_getBlock(&_msg_queue_ptrs_allocator);
    mq->msgQueuePtr = q;
    return mq;
}

// int Resource_free(Resource* resource)
int MsgQueuePtr_free(MsgQueuePtr *q) {
    PoolAllocatorResult res = PoolAllocator_releaseBlock(&_msg_queue_ptrs_allocator, q);
    if (res != Success) {
        printf("[ERRORE] Faillimento deallocazione message queue ptr!\n");
        return -1;
    }
    return 0;
}

// void Resource_print(Resource* r)
void MsgQueuePtr_print(const MsgQueuePtr *q) {
    MsgQueue_print(q->msgQueuePtr);
}

// Resource* ResourceList_byId(ResourceList* l, int id)
MsgQueuePtr* MsgQueuePtrList_findByName(MsgQueuePtrList *l, const char *name) {
    if (name == NULL) return NULL;

    for (ListItem *current = l->first; current != NULL; current = current->next) {
        if (strcmp(name, ((MsgQueuePtr *)current)->msgQueuePtr->resource.name) == 0) {
            return (MsgQueuePtr *)current;
        }
    }
    return NULL;
}

// void ResourceList_print(ListHead* l)
void MsgQueuePtrList_print(MsgQueuePtrList *l) {
    printf("{\n");
    for (ListItem *currNode = l->first; currNode != NULL; currNode = currNode->next) {
        MsgQueuePtr *msgQueue = (MsgQueuePtr *)currNode;
        printf("\t");
        MsgQueuePtr_print(msgQueue);
        if (currNode->next != NULL)
            printf(",");
        printf("\n");
    }
    printf("}\n");
}
