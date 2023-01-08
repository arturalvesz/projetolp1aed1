//
// Created by artur on 02/01/2023.
//


#include <stdio.h>
#include <stdlib.h>
#include "Users.h"


void addUserHead(struct USER **users, char *name, char *email){

    struct USER *newUser = malloc(sizeof(struct USER));
    newUser->name = strdup(name);
    newUser->email = strdup(email);
    newUser->kh = NULL;

    newUser->next = *users;
    *users = newUser;
}
void addUserTail(struct USER **users, char *name, char *email){

    struct USER *newUser = malloc(sizeof(struct USER));
    newUser->name = strdup(name);
    newUser->email = strdup(email);
    newUser->kh = NULL;
    newUser->next = NULL;

    if(*users == NULL){
        *users = newUser;
    }else{
        struct USER *current = *users;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = newUser;
    }
}
void addUserOrdered(struct USER **users, char *name, char *email){

    struct USER *newUser = malloc(sizeof(struct USER));
    newUser->name = strdup(name);
    newUser->email = strdup(email);
    newUser->kh = NULL;

    if(*users == NULL || strcmp((*users)->name, newUser->name) > 0){
        newUser->next = *users;
        *users = newUser;
    } else{
        struct USER *current = *users;
        while(current->next != NULL && strcmp(current->next->name,newUser->name) < 0){
            current = current->next;
        }
        newUser->next = current->next;
        current->next = newUser;
    }
}

struct USER *searchUserByName (struct USER *users, char *nameToSearch){
    struct USER * current = users;
    while(current != NULL){
        if(strstr(current->name, nameToSearch) != NULL){
            return current;
        }
        current = current->next;
    }
    return 0;
}

void addKeys(struct USER *users,int numberKH ,int lines, int columns){

    struct USER *currentUser = users;

    while(currentUser->next != NULL && currentUser->kh != NULL){
        currentUser = currentUser->next;
    }

    for(int i = 0; i < numberKH; i++) {
        struct KEY_HOLDER *newKH;
        newKH = malloc(sizeof(struct KEY_HOLDER));
        newKH = createNode(lines, columns);
        newKH->next = NULL;

        if (currentUser->kh == NULL) {
            currentUser->kh = newKH;
        } else {

            struct KEY_HOLDER *current = currentUser->kh;

            while (current->next != NULL) {
                current = current->next;
            }

            if (current->next == NULL) {
                current->next = newKH;
            }
        }
    }
}
/**
 * Função para testes
void print(struct USER *user, int lines){

    struct KEY_HOLDER *current = user->kh;
    printf("Key holder from %s: \n", user->name);


    for (int i = 0; i < lines; i++) {
        printf("public keys chars: %s\n", current->khChars.matrixPubKeysChars[i]);
    }
    for (int i = 0; i < lines; i++) {
        printf("private keys chars: %s\n", current->khChars.matrixPrivKeysChars[i]);
    }
    for (int i = 0; i < lines; i++) {
        printf("cod keys chars: %s\n", current->khChars.matrixRunLengthChars[i]);
    }

    for (int i = 0; i < lines; i++) {
        printf("pub keys ints: ");
        for (int j = 0; current->khInts.matrixPubKeysInts[i][j] != -1; j++) {
            printf(" %hd", current->khInts.matrixPubKeysInts[i][j]);
        }

        printf("\n");
    }
    for (int i = 0; i < lines; i++) {
        printf("priv keys ints: ");
        for (int j = 0; current->khInts.matrixPrivKeysInts[i][j] != -1; j++) {
            printf(" %hd", current->khInts.matrixPrivKeysInts[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < lines; i++) {
        printf("cod keys ints: ");
        for (int j = 0; current->khInts.matrixCodKeysInts[i][j] != -1; j++) {
            printf(" %hd", current->khInts.matrixCodKeysInts[i][j]);
        }
        printf("\n");
    }
}
*/
void sortUser(struct USER **users){
    struct USER *sorted = NULL;

    struct USER *current = *users;
    while(current != NULL) {
        struct USER *next = current->next;
        struct USER **pos = &sorted;
        while (*pos != NULL && strcmp((*pos)->name, current->name) < 0) {
            pos = &(*pos)->next;
        }
        current->next = *pos;
        *pos = current;

        current = next;
    }
    *users = sorted;
}

void save_userkeys_tofile(struct USER *users, int lines,char* filename){
    struct USER *current = users;
    struct KEY_HOLDER *kh = current->kh;

    while(current != NULL) {
        kh = current->kh;
        while(kh != NULL) {
            save_txt_keys_char(kh->khChars.matrixPubKeysChars, kh->khChars.matrixPrivKeysChars,kh->khChars.matrixRunLengthChars, lines, filename);
            save_txt_keys_int(kh->khInts.matrixPubKeysInts, kh->khInts.matrixPrivKeysInts,kh->khInts.matrixCodKeysInts,lines,filename);
            kh = kh->next;
        }
        current = current->next;
    }
}



int main_user(){
    struct USER *users = NULL;

    int lines = 5;

    addUserHead(&users,"Joao", "joao@gmail.com");
    addKeys(users,5,lines,20);

    addUserHead(&users,"Jorge", "jorge@gmail.com");
    addKeys(users, 5,lines,20);

    addUserTail(&users,"Antonio", "antonio@gmail.com");
    addKeys(users,5,lines,20);

    addUserTail(&users,"Cristiano", "cristiano@gmail.com");
    addKeys(users, 3,lines,20);

    addUserOrdered(&users,"Joaquim", "joaquim@gmail.com");
    addKeys(users, 3,lines,20);

    addUserOrdered(&users,"Francisco", "francisco@gmail.com");
    addKeys(users, 3,lines,20);

    //struct USER *user = searchUserByName(users,"Jorge");
    //print(user, 10);

    //sortUser(&users);
    char* filename = "C:\\Users\\artur\\Desktop\\UFP\\LP12022pubufp\\PROJETOLP1AED1\\keysFile.txt";
    save_userkeys_tofile(users,lines,filename);




    return 0;
};

