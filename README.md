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
**AGGIUNTA FILE**
<del> - msgqueue.c -- allocazione strutture dati per coda di messaggi </del>
<del> - msgqueue.h -- allocazione strutture dati per coda di messaggi </del>
- mq_create() -- crea una subqueue e coda stessa
- mq_open() -- Si connette e facoltativamente crea una coda di messaggi denominata.
- mq_close() -- Termina la connessione a una coda di messaggi aperta.
- mq_unlink() -- Termina la connessione a una coda di messaggi aperta e provoca la rimozione della coda quando l'ultimo processo la chiude.
- mq_send() -- Mette un messaggio nella coda.
- mq_receive() -- Riceve (rimuove) il messaggio più vecchio e con la priorità più alta dalla coda.
