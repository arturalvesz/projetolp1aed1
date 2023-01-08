//
// Created by artur on 02/01/2023.
//

#ifndef LP12022PUBUFP_USERS_H
#define LP12022PUBUFP_USERS_H

#include "linkedlist.h"

typedef struct USER{
    char *name;
    char *email;
    KEY_HOLDER *kh;
    struct USER *next;
} USER;

/**
 * adicionar user à cabeça da lista
 * @param users
 * @param name
 * @param email
 */

void addUserHead(struct USER **users, char *name, char* email);

/**
 * adicionar user à cauda da lista
 * @param users
 * @param name
 * @param email
 */
void addUserTail(struct USER **users, char *name, char *email);

/**
 * adicionar user alfabeticamente
 * @param users
 * @param name
 * @param email
 */
void addUserOrdered(struct USER **users, char *name, char *email);


/**
 * procurar utilizador pelo nome ou parte deles
 * @param users
 * @param nameToSearch
 */
struct USER *searchUserByName (struct USER *users, char *nameToSearch);


/**
 * Função para testes
 * @param user
 * @param lines
 */
void print(struct USER *user, int lines);

/**
 * Função para adicionar porta chaves ao utilizador
 * @param users
 * @param numberKH
 * @param lines
 * @param columns
 */
void addKeys(struct USER *users,int numberKH ,int lines, int columns);

/**
 * Função para ordenar os utilizador por ordem alfabetica
 * @param users
 */
void sortUser(struct USER **users);

/**
 * Guardar os porta chaves de cada user em ficheiro de texto
 * @param users
 * @param lines
 * @param filename
 */
void save_userkeys_tofile(struct USER *users, int lines,char* filename);

int main_user();

#endif //LP12022PUBUFP_USERS_H
