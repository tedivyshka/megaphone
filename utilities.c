#include "utilities.h"

void testMalloc(void *ptr){
    if(ptr==NULL){
        perror("Erreur de malloc() ou realloc().\n");
        exit(1);
    }
}

uint16_t get_id_entete(uint16_t ent){
    return ntohs(ent)>>5;
}

uint16_t chars_to_uint16(char a,char b){
    return ((uint16_t)b << 8) | a;
}

void print_8bits(uint8_t n){
    for(int i=7; i>=0; i--){
        uint8_t mask=1<<i;
        uint8_t bit=(n & mask)>>i;

        printf("%u",bit);
    }
    printf(" = %c\n",n);
}

void print_bits(uint16_t n){
    for(int i=0; i<=15; i++){
        uint16_t mask=1<<i;
        uint16_t bit=(n & mask)>>i;

        printf("%u",bit);
    }
    printf("\n");
}

entete *create_entete(uint8_t codereq,uint16_t id){
    entete* entete=malloc(sizeof(struct entete));
    entete->val=id;
    entete->val=entete->val<<5;
    entete->val=entete->val | codereq;
    entete->val=htons(entete->val);

    return entete;
}

void print_inscription_bits(inscription *msg){
    uint8_t *bytes=(uint8_t *) msg;
    for(size_t i=0; i<sizeof(inscription); i++){
        for(int j=7; j>=0; j--){
            printf("%d",(bytes[i]>>j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

client_message *string_to_client_message(const char *buffer) {
    client_message *msg = malloc(sizeof(client_message));

    // Copy entete, numfil, and nb from the buffer
    memcpy(&(msg->entete.val), buffer, sizeof(uint16_t));
    memcpy(&(msg->numfil), buffer + sizeof(uint16_t), sizeof(uint16_t));
    memcpy(&(msg->nb), buffer + sizeof(uint16_t) * 2, sizeof(uint16_t));

    // Extract datalen from the buffer, located right after nb
    uint8_t datalen = buffer[sizeof(uint16_t) * 3];

    // Allocate memory for data, including space for the null-terminator
    msg->data = malloc((size_t)(datalen + 1));

    // Copy the data from the buffer, starting after datalen
    memcpy(msg->data, buffer + sizeof(uint16_t) * 3, (size_t)datalen);

    // Manually set the datalen as the first byte of the data array
    msg->data[0] = datalen;

    // Add a null-terminator at the end of the data
    msg->data[datalen + 1] = '\0';

    return msg;
}

char *client_message_to_string(client_message *msg) {
    size_t buffer_size = sizeof(uint16_t) * 3 + 1 + msg->data[0];
    char *buffer = malloc(buffer_size);

    memcpy(buffer, &(msg->entete.val), sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t), &(msg->numfil), sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t) * 2, &(msg->nb), sizeof(uint16_t));
    memcpy(buffer + sizeof(uint16_t) * 3, msg->data, 1 + msg->data[0]);

    return buffer;
}

client_message *copy_client_message(client_message *msg) {
    client_message *copy = (client_message *)malloc(sizeof(client_message));
    copy->entete = msg->entete;
    copy->numfil = msg->numfil;
    copy->nb = msg->nb;
    uint8_t datalen = msg->data[0];
    copy->data = (uint8_t *)malloc(sizeof(uint8_t) * (datalen + 1));
    memcpy(copy->data, msg->data, sizeof(uint8_t) * (datalen + 1));
    return copy;
}

void add_message_to_fil(fil *fils[], client_message *msg, uint16_t fil_number) {
    fil *current_fil = fils[fil_number];
    if (current_fil == NULL) {
        current_fil = (fil *)malloc(sizeof(fil));
        current_fil->fil_number = fil_number;
        current_fil->head = NULL;
        fils[fil_number] = current_fil;
    }
    message_node *new_node = (message_node *)malloc(sizeof(message_node));
    new_node->msg = msg;
    new_node->next = current_fil->head;
    current_fil->head = new_node;
}


char** retrieve_messages_from_fil(fil *fils[], uint16_t fil_number) {
    fil *current_fil = fils[fil_number];
    message_node *current = current_fil->head;
    char **messages = malloc(sizeof(char *) * 1024);
    int index = 0;
    while (current != NULL) {
        messages[index] = strdup((char *)(current->msg->data) + 1); // Assuming the data starts at msg.data[1]
        index++;
        current = current->next;
    }
    messages[index] = NULL; // Add NULL at the end of the messages array
    return messages;
}
