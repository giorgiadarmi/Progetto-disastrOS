# project DisastrOS
Progetto Sistemi Operativi (2020/2021)

## Sviluppato da
- **Giorgia D'Armiento**
- **1711864**

## Message queues in disastrOS 
- Implementazione di un sistema IPC basato sulle code di messaggi disastrOS per consentire e bloccare la comunicazione tra processi.

- Il punto di partenza è la lezione presentata a lezione [qui](https://gitlab.com/grisetti/sistemi_operativi_2019_20/-/tree/master/source/08_disastrOS/disastrOS_04_resources) presente.

- Abbiamo aggiunto un campo 'name' alla struct Resource, come puntatore alla coda di messaggio e modifichiamo i file disastrOS_resource.c, disastrOS_resource.h e disastrOS_open_resource.c

- Aggiungiamo il disastrOS_msgqueue.h che contiene tutte le strutture dati necessarie per implementare le code di messaggi: text (testo del messaggio), message (messaggio), sottocoda di priorità (subqueue), coda di messaggi (MsgQueue) e riferimento alla coda di messaggi (MsgQueuePtr). 
Tutte queste strutture vengono allocate tramite SLAB allocator, la struttura dati viene definita, inizializzata, allocata e deallocata. Aggiungiamo la funzione MsgQueuePtr * MsgQueuePtrList_findByName per la ricerca di coda tramite il nome. 

- Aggiungiamo anche disastrOS_msgqueue.c in cui definiamo la costante DSOS_MSG_QUEUE_RESOURCE per impostare il tipo di risorsa, inseriamo le macro relative alle 5 strutture dati allocate e allochiamo le strutture dati come il pool allocator per ognuno init, alloc, free, print, printlist

- In disastrOS_descriptor.c e disastrOS_descriptor.h aggiungiamo la coda ListHead e i meccanismi di FindbyPCB

- In disastrOS_constants.h aggiungiamo le costanti relative ai messaggi per il massimo e minimo, che abbiamo già utilizzato al momento dell’allocazione, aggiungiamo anche gli errori relativi ad esse e i numeri delle syscall relative
MAX_NUM_MESSAGES_PER_MSG_QUEUE --> che rappresenta il numero massim di messaggi contenuti nella message queue, se il valore è < 3 è possibile tesatre l'errore causato dalla coda piena
MAX_NUM_PRIORITIES --> indica il numero di sottocode che verranno create nella coda di messaggi
MAX_TEXT_LEN --> rappresenta la dimensione del messaggio scritto, se il valore è < 6 testiamo errori di scrittura
MAX_NUM_MESSAGES --> indica numero massimo di messaggi che si possono scrivere se < 9 causerà segmentation fault nel momento in cui cerca di allocare l'ultimo messaggio

- In disastrOS_global.h (tiene traccia delle variabili globali, visibili a tutti i processi) aggiungiamo la variabile globale last_rid, ovvero il secondo processo per la comunicazione e la variabile lista coda di messaggi

- In disastrOS_syscall.h -- dichiaro implementazione syscall 
void internal_msgQueueCreate();
void internal_msgQueueOpen();
void internal_msgQueueClose();
void internal_msgQueueUnlink();
void internal_msgQueueRead();
void internal_msgQueueWrite();

- In disastrOS.c installiamo le nuove syscall, vengono aggiunte al vettore delle syscall del sistema operativo, il syscall_vector, e viene specificato il numero di argomenti e l'ordine nel vettore degli argomenti, il syscall_numarg. Viene dichiarata, inizializzata e stampata una lista di code di messaggi, la msg_queues_list 

- In disastrOS.h vengono dichiarate le nuove syscall aggiunte:
Implementiamo la dichiarazione delle syscall appena inserite nei relativi file:
* msgQueueCreate → crea la coda di messaggi con il nome preso in input
* msgQueueOpen → apre una coda di messaggi con il nome preso in input
* msgQueueClose → chiude il descrittore mqdes corrispondente alla coda di messaggi
* msgQueueUnlink →rimuove la coda con il nome preso in input, la coda verrà rimossa/distrutta una volta che tutti i processi che l’hanno aperta la chiudono, chiudendo i propri descrittori mqdes associati alla coda
* msgQueueRead → rimuove il messaggio con priorità più alta (più vecchio) dalla coda a cui si riferisce mqdes e lo inserisce nel buffer puntato da msg_ptr di cui specifichiamo la dimensione msg_len
* msgQueueWrite → aggiunge il messaggio puntato da msg_ptr alla coda a cui si riferisce mqdes e si specificano dimensione msg_len e priorità del messaggio priority

- Modifichiamo il Makefile aggiungendo HEADER disastrOS_msgqueue.h e i file oggetto relativi alle sistemcall in OBJ  

- Modifichiamo il file disastrOS_test.c, in cui viene implementata la comunicazione tra un producer ed un consumer. In esso è definita una costante ITERATIONS per settare il numero di iterazioni che verranno eseguite

## ESECUZIONE
Per testare il funzionamento del progetto, eseguire:
- git clone https://github.com/giorgiadarmi/Progetto-disastrOS.git
- cd ProgettoDisastrOS
- make
- ./disastrOS_test per l'esecuzione 

## DA FARE

#### MODIFICHE FILE
- <del>disastrOS_resource.c -- aggiunta campo nome</del>
- <del>disastrOS_resource.h -- aggiunta campo nome</del> 

#### AGGIUNTA FILE
- <del>msgqueue.c -- allocazione strutture dati per coda di messaggi</del>
- <del>msgqueue.h -- allocazione strutture dati per coda di messaggi</del>

#### MODIFICHE FILE
- <del>disastrOS_descriptor.c e disastrOS_descriptor.h meccanismi di attesa findbyPCB</del>
- <del>disastrOS_constants.h -- aggiungo costanti</del>
- <del>disastrOS_syscall.h -- dichiaro implementazione syscall</del>

#### AGGIUNTA FILE
- <del>mq_create() -- crea una subqueue e coda stessa</del>
- <del>mq_open() -- Si connette e facoltativamente crea una coda di messaggi denominata.</del>
- <del>mq_close() -- Termina la connessione a una coda di messaggi aperta.</del>
- <del>mq_unlink() -- Termina la connessione a una coda di messaggi aperta e provoca la rimozione della coda quando l'ultimo processo la chiude.</del>
- <del>mq_send() -- Mette un messaggio nella coda.</del>
- <del>mq_receive() -- Riceve (rimuove) il messaggio più vecchio e con la priorità più alta dalla coda.</del>

#### MODIFICHE FILE
- <del>disastrOS.c per l'installazione nuove syscall</del>
- <del>disastrOS.h dichiarazione nuove syscall</del>
- <del>disastrOS_test -- modofico test da fare</del>

