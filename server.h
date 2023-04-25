#ifndef MEGAPHONE_SERVER_H

#include "utilities.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>


typedef struct list_client{
    unsigned long id;
    char * pseudo;
    struct list_client * suivant;
}list_client;

uint16_t id_dernier_client = 0;

list_client * clients;
// possibilité d'optimisation en utilisant un tableau au lieu d'une liste.

// liste des listes chainees pour les messages postes dans un fil
fil **fils;

int running = 1;
void signal_handler(int signal, siginfo_t *siginfo, void *context) {
    running = 0;
}

#define MEGAPHONE_SERVER_H

#endif //MEGAPHONE_SERVER_H
