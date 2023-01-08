//
// Created by artur on 30/12/2022.
//

#ifndef LP12022PUBUFP_LINKEDLIST_H
#define LP12022PUBUFP_LINKEDLIST_H
#include "aed1_lp1_2223_proj_part1_ints.h"
#include "aed1_lp1_2223_proj_part1_chars.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct DateTime{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
}DATE_TIME;


typedef struct KEY_HOLDER {

    Key_HolderChars khChars;
    Key_HolderInts khInts;

    DATE_TIME createdDate;
    DATE_TIME lastUpdateDate;

    struct KEY_HOLDER* next;

}KEY_HOLDER;

/**
 * Criar porta chaves e popular
 * @param lines
 * @param columns
 * @return
 */
KEY_HOLDER* createNode(int lines, int columns);

/**
 * Adicionar os porta chaves criados à estrutura
 * @param head
 * @param lines
 * @param columns
 */
void push(struct KEY_HOLDER** head,int lines,int columns);

/**
 * Inserir porta chaves na posição desejada
 * @param head
 * @param position
 * @param lines
 * @param columns
 */
void insertAtPosition(struct KEY_HOLDER** head,int position,int lines,int columns);

/**
 * Apagar porta-chaves em determinada posição
 * @param head
 * @param position
 */
void deleteKeyHolderOnPos(struct KEY_HOLDER **head, int position);
/**
 * Função de testes para verificar o que tem a lista
 * @param head
 * @param lines
 */
void printList(struct KEY_HOLDER* head,int lines);

int main_linkedlist();

#endif //LP12022PUBUFP_LINKEDLIST_H
