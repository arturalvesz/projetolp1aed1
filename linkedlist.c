//
// Created by artur on 30/12/2022.
//

#include "linkedlist.h"


KEY_HOLDER* createNode(int lines, int columns){

    KEY_HOLDER *newNode = malloc(sizeof(struct KEY_HOLDER));

    time_t currentTime;
    struct tm *localTime;

    time(&currentTime); //get the current time
    localTime = localtime(&currentTime); //convert the current to time local time

    newNode->createdDate.day = localTime->tm_mday;
    newNode->createdDate.month = localTime->tm_mon + 1;  // tm_mon is 0-11
    newNode->createdDate.year = localTime->tm_year + 1900;  // tm_year is the number of years since 1900
    newNode->createdDate.hour = localTime->tm_hour;
    newNode->createdDate.minute = localTime->tm_min;
    newNode->createdDate.second = localTime->tm_sec;

    newNode->lastUpdateDate.day = localTime->tm_mday;
    newNode->lastUpdateDate.month = localTime->tm_mon + 1;  // tm_mon is 0-11
    newNode->lastUpdateDate.year = localTime->tm_year + 1900;  // tm_year is the number of years since 1900
    newNode->lastUpdateDate.hour = localTime->tm_hour;
    newNode->lastUpdateDate.minute = localTime->tm_min;
    newNode->lastUpdateDate.second = localTime->tm_sec;

    newNode->khInts.matrixPubKeysInts = alloc_matrix_int(lines, columns);
    newNode->khInts.matrixPrivKeysInts = alloc_matrix_int(lines, columns);
    newNode->khInts.matrixCodKeysInts = alloc_matrix_int(lines, columns);

    newNode->khChars.matrixPubKeysChars = alloc_matrix_char(lines, columns);
    newNode->khChars.matrixPrivKeysChars = alloc_matrix_char(lines, columns);
    newNode->khChars.matrixRunLengthChars = alloc_matrix_char(lines, columns);

    bulk_populate_public_keys_char(newNode->khChars.matrixPubKeysChars, lines);
    bulk_compute_private_keys_char(newNode->khChars.matrixPubKeysChars, newNode->khChars.matrixPrivKeysChars, lines);
    bulk_compute_runlengths_char(newNode->khChars.matrixPrivKeysChars, newNode->khChars.matrixRunLengthChars, lines);

    bulk_populate_public_keys_int(newNode->khInts.matrixPubKeysInts, lines);
    bulk_compute_private_keys_int(newNode->khInts.matrixPubKeysInts, newNode->khInts.matrixPrivKeysInts, lines);
    bulk_compute_runlengths_int(newNode->khInts.matrixPrivKeysInts, newNode->khInts.matrixCodKeysInts, lines);

    newNode->next = NULL;
    return newNode;
}

void push(struct KEY_HOLDER** head,int lines,int columns){

    KEY_HOLDER *newNode = createNode(lines,columns);
    newNode->next = *head;
    *head = newNode;
}

void insertAtPosition(struct KEY_HOLDER** head,int position,int lines,int columns){

    if(*head == NULL){
        push(head,lines,columns);
        return;
    }
    if(position == 0){
        push(head,lines,columns);
        return;
    }
    KEY_HOLDER* current = *head;
    for(int i = 0; i < position - 1 && current->next != NULL;i++){
        current = current->next;
    }
    KEY_HOLDER* newNode = createNode(lines,columns);
    newNode->next = current->next;
    current->next = newNode;
}
void deleteKeyHolderOnPos(struct KEY_HOLDER **head, int position) {

    if(*head == NULL){
        return;
    }
    if(position == 0){
        KEY_HOLDER* current = *head;
        *head = current->next;
        free(current);
        return;
    }
    KEY_HOLDER* current = *head;
    for(int i = 0; i < position - 1 && current->next != NULL; i++){
        current = current->next;
    }
    if(current->next == NULL){
        return;
    }
    KEY_HOLDER* temp = current->next;
    current->next = temp->next;
    free(temp);
}

/**
 * Função de testes
void printList(struct KEY_HOLDER* head, int lines){
    struct KEY_HOLDER* listHead = head;

    printf("-------------------------------------");
    printf("-----WANTED NAME STRUCT KEYS-----");
    printf("-------------------------------------");
    printf("\n");
    for (int i = 0; i < lines; i++) {
        printf("public keys chars: %s\n", listHead->khChars.matrixPubKeysChars[i]);
           }
    for (int i = 0; i < lines; i++) {
        printf("private keys chars: %s\n", listHead->khChars.matrixPrivKeysChars[i]);
    }
    for (int i = 0; i < lines; i++) {
        printf("cod keys chars: %s\n", listHead->khChars.matrixRunLengthChars[i]);
    }

    for (int i = 0; i < lines; i++) {
        printf("pub keys ints: ");
        for (int j = 0; listHead->khInts.matrixPubKeysInts[i][j] != -1; j++) {
            printf(" %hd", listHead->khInts.matrixPubKeysInts[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < lines; i++) {
        printf("priv keys ints: ");
        for (int j = 0; listHead->khInts.matrixPrivKeysInts[i][j] != -1; j++) {
            printf(" %hd", listHead->khInts.matrixPrivKeysInts[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < lines; i++) {
        printf("cod keys ints: ");
        for (int j = 0; listHead->khInts.matrixCodKeysInts[i][j] != -1; j++) {
            printf(" %hd", listHead->khInts.matrixCodKeysInts[i][j]);
        }
        printf("\n");
    }
}
 */



int main_linkedlist(){
    struct KEY_HOLDER *head = NULL;
    int lines = 3;
    int columns = 20;

    push(&head, lines,columns);
    push(&head, lines,columns);
    push(&head, lines,columns);

    //insertAtPosition(&head,2,lines,columns);
    deleteKeyHolderOnPos(&head, 1);
    deleteKeyHolderOnPos(&head,0);
    printList(head,lines);

    return 0;
}