# project DisastrOS

Project Operative System

## Developed by
- **Giorgia D'Armiento**
- **1711864**

## Message queues in disastrOS 
- Implement an IPC system based on message queues in disastrOS
- to allow (blocking) communication between processes;
- the starting point is the version of disastrOS presented in class.

## DA FARE
**MODIFICHE FILE**
- <del>disastrOS_resource.c</del> -- aggiunta campo nome
- <del>disastrOS_resource.h</del> -- aggiunta campo nome 
- disastrOS_descriptor.c e disastrOS_descriptor.h meccanismi di attesa findbyPCB
**AGGIUNTA FILE**
- <del>msgqueue.c -- allocazione strutture dati per coda di messaggi</del>
- <del>msgqueue.h -- allocazione strutture dati per coda di messaggi</del>
**MODIFICHE FILE**
- disastrOS_constants.h -- aggiungo costanti
- disastrOS_syscall.h -- dichiaro implementazione syscall
**AGGIUNTA FILE**
- mq_create() -- crea una subqueue e coda stessa
- mq_open() -- Si connette e facoltativamente crea una coda di messaggi denominata.
- mq_close() -- Termina la connessione a una coda di messaggi aperta.
- mq_unlink() -- Termina la connessione a una coda di messaggi aperta e provoca la rimozione della coda quando l'ultimo processo la chiude.
- mq_send() -- Mette un messaggio nella coda.
- mq_receive() -- Riceve (rimuove) il messaggio più vecchio e con la priorità più alta dalla coda.
**MODIFICHE FILE**
- disastrOS.c per l'installazione nuove syscall
- disastrOS.h dichiarazione nuove syscall
- disastrOS_test -- modofico test da fare

