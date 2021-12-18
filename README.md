# project DisastrOS
Progetto Sistemi Operativi (2020/2021)

## Sviluppato da
- **Giorgia D'Armiento**
- **1711864**

## Message queues in disastrOS 
- Implementazione di un sistema IPC basato sulle code di messaggi disastrOS per consentire e bloccare la comunicazione tra processi.
- Il punto di partenza è la lezione presentata a lezione [qui](https://gitlab.com/grisetti/sistemi_operativi_2019_20/-/tree/master/source/08_disastrOS/disastrOS_04_resources) presente.

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
- mq_send() -- Mette un messaggio nella coda.
- mq_receive() -- Riceve (rimuove) il messaggio più vecchio e con la priorità più alta dalla coda.

#### MODIFICHE FILE
- <del>disastrOS.c per l'installazione nuove syscall</del>
- <del>disastrOS.h dichiarazione nuove syscall</del>
- <del>disastrOS_test -- modofico test da fare</del>

