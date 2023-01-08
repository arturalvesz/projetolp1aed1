//
// Created by artur on 06/12/2022.
//

#include <math.h>
#include "aed1_lp1_2223_proj_part1_ints.h"


unsigned long long new_public_key_int(void){

    unsigned long long randomN;

    srand(time(NULL));
    randomN = rand() % 2000;

    return randomN;
}

short* key_long_2_digits_int(unsigned long long key){

    short* digitsArray;
    unsigned long long* auxArray;
    int key_size = 0;
    unsigned long long number = key;

    key_size = keySize(key);

    auxArray = calloc((key_size + 1) , sizeof(unsigned long long));
    digitsArray = calloc((key_size + 1) , sizeof(short));

    int i = 0;
    while(key){
        unsigned long long remainder = key % 10;
        auxArray[i] = remainder;
        i++;

        key /= 10;
    }
    for(int j = 0; j < key_size; j++){
        digitsArray[j] = auxArray[(key_size-1)-j];
    }
    free(auxArray);
    return (short*) digitsArray;
}

unsigned long long key_digits_2_long_int(short* keydigits){

    unsigned long long key = 0;

    //int size = sizeof(keydigits)/sizeof(short);
    for(int i = 0; keydigits[i] != -1; i++){
        key *= 10;
        key += keydigits[i];
    }

    printf("%llu", key);

    return key;
}

unsigned long long calc_private_key_int(unsigned long long pubkey){

    int count = 0;
    unsigned long long mult = pubkey * 2;
    while(!isBipolar_int(mult)){//Enquanto o multiplo não for bipolar, adicionamos o número dado a ele e verificamos novamente
        mult += pubkey;
        count++;
        if(count > 50000){//É feita esta verificação devido ao facto do numero poder não ter bipolar, logo para o programa não dar erro é efetuada esta verificação
            //printf("too large number\n");
            mult = 0;
            return mult;
        }
    }
    //printf("contador vezes: %d\n", count);
    return mult;
}

int isBipolar_int(unsigned long long multiple){

    short *mult_digits = key_long_2_digits_int(multiple);
    int mult_size = 0;
    mult_size = keySize(multiple);

    int count = 1;
    short digit= mult_digits[0];
    for(int i = 0; i < mult_size; i++){
        if(digit != mult_digits[i]){
            digit = mult_digits[i];
            count++;
        }
    }
    if(count == 2){
        return 1;
    }else{
        return 0;
    }
}

int keySize(unsigned long long key){
    int key_size = 0;
    while(key){
        key_size++;
        key /= 10;
    }
    return key_size;
}

unsigned long long calc_runlength_int(unsigned long long privkey){

    int check = isBipolar_int(privkey);//se o numero não for bipolar não é calculada a sua run length
    if(check != 1){
        printf("Impossible to calculate the runlength because the privkey isn't bipolar\n");
        return 0;
    }

    int count = 0;
    unsigned long long auxPrivKey = privkey;

    while (auxPrivKey != 0){
        auxPrivKey /= 10;
        count++;
    }
    unsigned long long aux[count];
    unsigned long long privKeyArr[count];
    unsigned long long runlengthKey = 0;
    unsigned long long digit1 = 0;
    int digit1count = 1;
    unsigned long long digit2 = 0;
    int digit2count = 1;

    for(int i = 0; i < count; i++){
        aux[i] = privkey % 10;
        privkey /= 10;
    }
    for(int j = 0; j < count; j++){
        privKeyArr[j] = aux[(count-1)-j];
    }

    digit1 = privKeyArr[0];

    bool isSecond = false;
    int auxcount = 0;
    for(int i = 0; i < count; i++){
        if(privKeyArr[i] == privKeyArr[i+1]){
            auxcount++;
        }else{
            if(isSecond == false){
                digit1count = auxcount + 1;
                isSecond = true;
            }else{
                digit2count = auxcount + 1;
            }
            auxcount = 0;
        }
    }
    digit2 = privKeyArr[digit1count];

    //formar a runlengthKey
    runlengthKey += digit1count;
    runlengthKey *= 10;
    runlengthKey += digit1;
    runlengthKey *= 10;
    runlengthKey += digit2count;
    runlengthKey *= 10;
    runlengthKey += digit2;

    return runlengthKey;
}

unsigned long long private_key_from_runlength_int(unsigned long long runlengthkey){
    unsigned long long key = runlengthkey;
    unsigned long long privKey = 0;
    int arrSize = 0;
    unsigned long long *privKeyArr;
    int runlengthKeySize = keySize(runlengthkey);

    short* runlengthArr = key_long_2_digits_int(runlengthkey);

    if(runlengthKeySize > 4){
        if((runlengthArr[0] * 10) + runlengthArr[1] >= 10 && (runlengthArr[0] * 10) + runlengthArr[1] <= 17){
            arrSize = (runlengthArr[0] * 10) + runlengthArr[1] + runlengthArr[3];
            privKeyArr = malloc(arrSize * sizeof(unsigned long long));
            int n1stOcur = (runlengthArr[0] * 10) + runlengthArr[1]; //number of times the 1st number of the cod key appears in the private key

            for(int j = 0; j < n1stOcur; j++){
                privKeyArr[j] = runlengthArr[2];
            }
            for(int k = n1stOcur; k < n1stOcur + runlengthArr[3]; k++){
                privKeyArr[k] = runlengthArr[4];
            }
            for(int l = 0; l < n1stOcur + runlengthArr[3]; l++){
                privKey *= 10;
                privKey += privKeyArr[l];
            }
            return privKey;
        }
        else if((runlengthArr[2] * 10) + runlengthArr[3] >= 10 && (runlengthArr[2] * 10) + runlengthArr[3] <= 17){
            arrSize = runlengthArr[0] + (runlengthArr[2] * 10) + runlengthArr[3];
            privKeyArr = malloc(arrSize * sizeof(unsigned long long));
            int n2ndOcur = (runlengthArr[2] * 10) + runlengthArr[3]; //number of times the 2nd number of the cod key appears in the private key

            for(int j = 0; j < runlengthArr[0]; j++){
                privKeyArr[j] = runlengthArr[1];
            }
            for(int k = runlengthArr[0]; k < n2ndOcur + runlengthArr[0]; k++){
                privKeyArr[k] = runlengthArr[4];
            }
            for(int l = 0; l < n2ndOcur + runlengthArr[0]; l++){
                privKey *= 10;
                privKey += privKeyArr[l];
            }
            return privKey;
        }
    }else{
        arrSize = runlengthArr[0] + runlengthArr[2];
        privKeyArr = malloc(arrSize * sizeof(unsigned long long));
        for(int j = 0; j < runlengthArr[0]; j++){
            privKeyArr[j] = runlengthArr[1];
        }
        for(int k = runlengthArr[0]; k < runlengthArr[0] + runlengthArr[2]; k++){
            privKeyArr[k] = runlengthArr[3];
        }
        for(int l = 0; l < runlengthArr[0]+runlengthArr[2]; l++){
            privKey *= 10;
            privKey += privKeyArr[l];
        }
        return privKey;
    }
}

short** alloc_matrix_int(int nlines, int ncolumns){

    short** matrix;

    matrix = malloc(nlines * sizeof(short*));
    for(int i = 0; i < nlines; i++){
        *(matrix + i) = malloc(ncolumns * sizeof(short));
    }

    for (int i = 0; i < nlines; i++){
        for (int k = 0; k < ncolumns; k++) {
            matrix[i][k] = -1;
        }
    }

    return (short **) matrix;
}

void store_key_int(short **matrix, int lines, unsigned long long key){

    int key_size = keySize(key);
    int count = 0;
    short *keyDigits = key_long_2_digits_int(key);

    int i = 0;
    for(i = 0; i < lines; i++){
        if(matrix[i][0] == -1){
            for(int j = 0; j < key_size; j++){
                matrix[i][j] = keyDigits[j];
                count++;
            }
        }
        if(count == key_size){
            break;
        }
    }
}

int exists_key_int(short **matrix, int lines, unsigned long long key){

    short* keyDigits = key_long_2_digits_int(key);
    int key_size = keySize(key);
    int count = 0;

    for(int i = 0; i < lines; i++){
        int j = 0;
        while(matrix[i][j] != -1) {
            if (matrix[i][j] == keyDigits[j]) {
                count++;
            }else{
                count = 0;
                break;
            }
            j++;
        }
        if(count == key_size){
            return 1;
        }
    }
    return 0;
}

unsigned long long get_private_key_int(short **matrix_kpub, short **matrix_kpriv, int lines, unsigned long long pubkey) {

    short *keyDigits = key_long_2_digits_int(pubkey);
    int key_size = keySize(pubkey);
    unsigned long long privKey = 0;

    int pubKeyCount = 0;

    for (int i = 0; i < lines; i++) {
        pubKeyCount = 0;
        for (int j = 0; j < key_size; j++) {
            if (matrix_kpub[i][j] == keyDigits[j]) {
                pubKeyCount++;
            }
        }
        if (pubKeyCount == key_size) {
            privKey = calc_private_key_int(pubkey);
            break;
        }
    }
    short* privKeyDigits = key_long_2_digits_int(privKey);
    int privKey_size = keySize(privKey);
    int privKeyCount = 0;

    for (int i = 0; i < lines; i++) {
        privKeyCount = 0;
        for (int j = 0; j < privKey_size; j++) {
            if(matrix_kpriv[i][j] == privKeyDigits[j]){
                privKeyCount++;
            }
        }
        if(privKeyCount == privKey_size){
            return privKey;
        }
    }
    return 0;
}

unsigned long long get_runlength_int(short **matrix_kpriv, short **matrix_kcod, int lines, unsigned long long privkey){

    short* privKeyDigits = key_long_2_digits_int(privkey);
    int privKey_size = keySize(privkey);
    int privKeyCount = 0;
    unsigned long long codKey = 0;

    for (int i = 0; i < lines; i++) {
        privKeyCount = 0;
        for (int j = 0; j < privKey_size; j++) {
            if(matrix_kpriv[i][j] == privKeyDigits[j]){
                privKeyCount++;
            }
        }
        if(privKeyCount == privKey_size){
            codKey = calc_runlength_int(privkey);
            break;
        }
    }

    short* codKeyDigits = key_long_2_digits_int(codKey);
    int codKey_size = keySize(codKey);
    int codKeyCount = 0;

    for (int i = 0; i < lines; i++) {
        codKeyCount = 0;
        for (int j = 0; j < codKey_size; j++) {
            if(matrix_kcod[i][j] == codKeyDigits[j]){
                codKeyCount++;
            }
        }
        if(codKeyCount == codKey_size){
            return codKey;
        }
    }
    return 0;
}

unsigned long long delete_key_int(short **matrix_kpub, short **matrix_kpriv, short **matrix_kcod, int lines, short pubkey) {

    unsigned long long pubKeyInt = (unsigned long long int) pubkey;
    unsigned long long privKey = calc_private_key_int(pubKeyInt);
    unsigned long long codKey = calc_runlength_int(privKey);

    short *pubKeyDigits = key_long_2_digits_int(pubKeyInt);
    short *privKeyDigits = key_long_2_digits_int(privKey);
    short *codKeyDigits = key_long_2_digits_int(codKey);

    int pubKey_size = keySize(pubKeyInt);
    int privKey_size = keySize(privKey);
    int codKey_size = keySize(codKey);

    int pubKeyCount = 0;
    int privKeyCount = 0;
    int codKeyCount = 0;

    for (int i = 0; i < lines; i++) {
        int pubC = 0;//public key column iterator
        while (matrix_kpub[i][pubC] != -1) {
            if (matrix_kpub[i][pubC] == pubKeyDigits[pubC]){
                pubKeyCount++;
            } else {
                pubKeyCount = 0;
                break;
            }
            pubC++;
        }
        if (pubKeyCount == pubKey_size) {

            for(int j = i; j < lines; j++) {
                for(int k = 0; k < pubKey_size; k++) {
                    matrix_kpub[j][k] = -1;
                }
                for (int k = 0; matrix_kpub[j + 1][k] != -1; k++) {
                    matrix_kpub[j][k] = matrix_kpub[j + 1][k];
                }
            }
        }
        int privC = 0;
        while (matrix_kpriv[i][privC] != -1) {
            if (matrix_kpriv[i][privC] == privKeyDigits[privC]) {
                privKeyCount++;
            } else {
                privKeyCount = 0;
                break;
            }
            privC++;
        }
        if (privKeyCount == privKey_size) {

            for(int j = i; j < lines; j++) {
                for(int k = 0; k < privKey_size; k++) {
                    matrix_kpriv[j][k] = -1;
                }
                for (int k = 0; matrix_kpriv[j + 1][k] != -1; k++) {
                    matrix_kpriv[j][k] = matrix_kpriv[j + 1][k];
                }
            }
        }
        int codC = 0;
        while (matrix_kcod[i][codC] != -1) {
            if (matrix_kcod[i][codC] == codKeyDigits[codC]) {
                codKeyCount++;
            } else {
                codKeyCount = 0;
                break;
            }
            codC++;
        }
        if (codKeyCount == codKey_size) {

            for(int j = i; j < lines; j++) {
                for(int k = 0; k < privKey_size; k++) {
                    matrix_kcod[j][k] = -1;
                }
                for (int k = 0; matrix_kcod[i + 1][k] != -1; k++) {
                    matrix_kcod[i][k] = matrix_kcod[i + 1][k];
                }
            }
        }

        if (i == lines - 1) {
            for (int k = 0; matrix_kpub[i][k] != -1; k++) {
                matrix_kpub[i][k] = -1;
            }
            for (int k = 0; matrix_kpriv[i][k] != -1; k++) {
                matrix_kpriv[i][k] = -1;
            }
            for (int k = 0; matrix_kcod[i][k] != -1; k++) {
                matrix_kcod[i][k] = -1;
            }
        }
    }
    /**
     * Testes da função
    printf("pubkeys:");
    for(int i = 0; i < lines; i++){
        for(int k = 0; matrix_kpub[i][k] != -1; k++){
            printf("%hd ", matrix_kpub[i][k]);
        }
        printf("\n");
    }
    printf("privkeys:");
    for(int i = 0; i < lines; i++){
        for(int k = 0; matrix_kpriv[i][k] != -1; k++){
            printf("%hd ", matrix_kpriv[i][k]);
        }
        printf("\n");
    }
    printf("codkeys:");
    for(int i = 0; i < lines; i++){
        for(int k = 0; matrix_kcod[i][k] != -1; k++){
            printf("%hd ", matrix_kcod[i][k]);
        }
        printf("\n");
    }
    */
    return pubKeyInt;
}

void bulk_populate_public_keys_int(short **matrix_kpub, int lines){
    unsigned long long newKey;

    for(int i = 0; i < lines; i++){
        newKey = rand() % 1000;
        int newKey_size = keySize(newKey);
        short* newKeyDigits = key_long_2_digits_int(newKey);
        for(int k = 0; k < newKey_size ; k++){
            matrix_kpub[i][k] = newKeyDigits[k];
        }
    }
}

void bulk_compute_private_keys_int(short **matrix_kpub, short **matrix_kpriv, int lines){

    unsigned long long *pubKeyInt = malloc(lines * sizeof(unsigned long long));
    unsigned long long aux = 0;
    unsigned long long privKey = 0;

    for(int i = 0; i < lines; i++) {
        aux = 0;
        for (int k = 0; matrix_kpub[i][k] != -1; k++) {
            aux *= 10;
            aux += matrix_kpub[i][k];
        }
        pubKeyInt[i] = aux;
    }
    for(int i = 0; i <lines; i++){
        privKey = calc_private_key_int(pubKeyInt[i]);
        int privKeySize = keySize(privKey);
        short* privKeyDigits = key_long_2_digits_int(privKey);
        for(int k = 0; k < privKeySize; k++){
            matrix_kpriv[i][k] = privKeyDigits[k];
        }
    }
}

void bulk_compute_runlengths_int(short **matrix_kpriv, short **matrix_kcod, int lines){

    unsigned long long *privKeyInt = malloc(lines * sizeof(unsigned long long));

    unsigned long long aux = 0;
    unsigned long long codKey = 0;


    for(int i = 0; i < lines; i++) {
        aux = 0;
        for (int k = 0; matrix_kpriv[i][k] != -1; k++) {
            aux *= 10;
            aux += matrix_kpriv[i][k];
        }
        privKeyInt[i] = aux;
    }
    for(int i = 0; i <lines; i++){
        codKey = calc_runlength_int(privKeyInt[i]);
        int codKeySize = keySize(codKey);
        short* codKeyDigits = key_long_2_digits_int(codKey);
        for(int k = 0; k < codKeySize; k++){
            matrix_kcod[i][k] = codKeyDigits[k];
        }
    }
}

short** search_private_keys_int(short **matrix_kpub, short **matrix_kpriv, int lines, unsigned long long partialpubkey){

    short* partialpubkeyDigits = key_long_2_digits_int(partialpubkey);
    unsigned long long* pubKeysMatching = calloc(sizeof(unsigned long long), lines);
    unsigned long long privKey;


    int partialKeySize = keySize(partialpubkey);
    int j, m = 0;
    int count;
    int auxPubKey = 0;

    for(int i = 0; i < lines; i++){
        j = 0;
        count = 0;

        for(int k = 0; matrix_kpub[i][k] != -1; k++){
            if(matrix_kpub[i][k] == partialpubkeyDigits[j]){
                count++;
                j++;
            }else{
                j = 0;
            }
        }
        if(count == partialKeySize){
            for(int k = 0; matrix_kpub[i][k]  != -1; k++){
                auxPubKey *= 10;
                auxPubKey += matrix_kpub[i][k];
            }
            pubKeysMatching[m] = auxPubKey;
            auxPubKey = 0;
            m++;
        }
    }
    int matchCount = 0;
    for(int i = 0; i < lines; i++){
        if(pubKeysMatching[i] != 0){
            matchCount++;
        }
    }
    short** privKeysMatching = alloc_matrix_int(lines,20);;

    int biggerCount = 0;
    int privDigitsCount = 0;
    int pubKeyRun = 0;
    int newMrow = 0; //iterador das linhas da matriz que vai receber as privKeys que coincidem com a parcial.
    for(int i = 0; i < lines; i++){
        privKey = calc_private_key_int(pubKeysMatching[pubKeyRun]);
        int privKeySize = keySize(privKey);
        short* privKeyDigits = key_long_2_digits_int(privKey);
        for(int l = 0; l < lines; l++) {
            for (int k = 0; k < privKeySize; k++) {
                if (privKeyDigits[k] == matrix_kpriv[l][k]) {
                    privDigitsCount++;
                } else {
                    privDigitsCount = 0;
                    break;
                }
            }
            if (privDigitsCount == privKeySize) {
                break;
            }
        }
        if(privDigitsCount == privKeySize){
            pubKeyRun++;
            for(int l = 0; l < privKeySize; l++){
                privKeysMatching[newMrow][l] = privKeyDigits[l];
            }
            newMrow++;
            privDigitsCount = 0;
        }
        if(pubKeyRun >= matchCount){
            break;
        }
    }

    for(int i = 0; i < lines; i++){
        for(int k = 0; privKeysMatching[i][k] != -1; k++){
            printf("%hd ", privKeysMatching[i][k]);
        }
        printf("\n");
    }

    return privKeysMatching;
}

void sort_matrix_int(short **matrix, int lines, int order){
    unsigned long long* auxToOrder = calloc(lines, sizeof(unsigned long long));
    unsigned long long aux;

    int countMatrixColumns = 0;
    int biggerCount = 0;

    for(int i = 0; i < lines; i++){
        aux = 0;
        for(int j = 0; matrix[i][j] != -1; j++){
            aux *= 10;
            aux += matrix[i][j];
            countMatrixColumns++;
        }

        if(countMatrixColumns > biggerCount){
            biggerCount = countMatrixColumns;
        }
        auxToOrder[i] = aux;
    }

    //order = 1, ascending order
    if(order == 1){
        for (int i = 0; i < lines; i++) {
            for (int j = i + 1; j < lines; j++) {
                if (auxToOrder[i] > auxToOrder[j]) {
                    unsigned long long temp = auxToOrder[i];
                    auxToOrder[i] = auxToOrder[j];
                    auxToOrder[j] = temp;
                }
            }
        }
    }
    //order = 0, descending order
    if(order == 0){
        for (int i = 0; i < lines; i++) {
            for (int j = i + 1; j < lines; j++) {
                if (auxToOrder[i] < auxToOrder[j]) {
                    unsigned long long temp = auxToOrder[i];
                    auxToOrder[i] = auxToOrder[j];
                    auxToOrder[j] = temp;
                }
            }
        }
    }

    for(int i = 0; i < lines; i++){
        for(int k = 0; k < biggerCount; k++){
            matrix[i][k] = -1;
        }
    }

    for(int i = 0; i < lines; i++){
        int key_size = keySize(auxToOrder[i]);
        short* auxDigits = key_long_2_digits_int(auxToOrder[i]);

        for(int j = 0; j < key_size; j++){
            matrix[i][j] = auxDigits[j];
            printf("%hd",matrix[i][j]);
        }
        printf("\n");
    }

}

void sort_all_matrices_int(short **matrix_kpub, short **matrix_kpriv, short **matrix_kcod, int lines, int order) {

    sort_matrix_int(matrix_kpub, lines, order);
    sort_matrix_int(matrix_kpriv,lines,order);
    sort_matrix_int(matrix_kcod,lines,order);


}

void list_keys_int(short **matrix_kpub, short **matrix_kpriv, short **matrix_kcod, int lines, int order){

    unsigned long long pubKey = 0;
    unsigned long long privKey = 0;
    unsigned long long codKey = 0;
    int privKeySize = 0;
    int codKeySize = 0;

    int biggerPrivSize = 0;
    int biggerCodSize = 0;


    short* privKeys = malloc(sizeof(short) * lines);
    short* codKeys = malloc(sizeof(short) * lines);


    short** privKeyDigits = alloc_matrix_int(lines,20);
    short** codKeyDigits = alloc_matrix_int(lines,20);


    if(order == 1){
        sort_matrix_int(matrix_kpub,lines,order);
        for(int i = 0; i < lines; i++){
            pubKey = 0;
            for(int j = 0; matrix_kpub[i][j] != -1; j++) {
                pubKey *= 10;
                pubKey += matrix_kpub[i][j];
            }
            privKey = get_private_key_int(matrix_kpub, matrix_kpriv,lines,pubKey);
            privKeySize = keySize(privKey);
            if(privKeySize > biggerPrivSize){
                biggerPrivSize = privKeySize;
            }
            for (int j = 0; j < privKeySize; j++) {
                privKeys = key_long_2_digits_int(privKey);
                privKeyDigits[i][j] = privKeys[j];
            }
            codKey = get_runlength_int(matrix_kpriv, matrix_kcod,lines,privKey);
            codKeySize = keySize(codKey);
            if(codKeySize > biggerCodSize){
                biggerCodSize = codKeySize;
            }
            for(int j = 0;j < codKeySize; j++){
                codKeys = key_long_2_digits_int(codKey);
                codKeyDigits[i][j] = codKeys[j];
            }
        }
    }

    if(order == 0){
        sort_matrix_int(matrix_kpub,lines,order);
        for(int i = 0; i < lines; i++){
            pubKey = 0;
            for(int j = 0; matrix_kpub[i][j] != -1; j++) {
                pubKey *= 10;
                pubKey += matrix_kpub[i][j];
            }
            privKey = get_private_key_int(matrix_kpub, matrix_kpriv,lines,pubKey);
            privKeySize = keySize(privKey);
            if(privKeySize > biggerPrivSize){
                biggerPrivSize = privKeySize;
            }
            for (int j = 0; j < privKeySize; j++) {
                privKeys = key_long_2_digits_int(privKey);
                privKeyDigits[i][j] = privKeys[j];
            }
            codKey = get_runlength_int(matrix_kpriv, matrix_kcod,lines,privKey);
            codKeySize = keySize(codKey);
            if(codKeySize > biggerCodSize){
                biggerCodSize = codKeySize;
            }
            for(int j = 0;j < codKeySize; j++){
                codKeys = key_long_2_digits_int(codKey);
                codKeyDigits[i][j] = codKeys[j];
            }
        }
    }



    for(int i = 0; i < lines; i++){
        for(int k = 0; k < biggerPrivSize; k++){
            matrix_kpriv[i][k] = -1;
        }
        for(int k = 0; k < biggerCodSize; k++){
            matrix_kcod[i][k] = -1;
        }
    }

    for(int i = 0; i < lines; i++){
        for(int k = 0; k < biggerPrivSize; k++){
            matrix_kpriv[i][k] = privKeyDigits[i][k];
        }
        for(int k = 0; k < biggerCodSize; k++){
            matrix_kcod[i][k] = codKeyDigits[i][k];
        }
    }

    /**
     * Testes da função
    for(int i = 0; i < lines; i++){
        printf("ints: ");
        for(int k = 0; matrix_kpub[i][k] != -1; k++){
            printf("%hd", matrix_kpub[i][k]);
        }
        printf(",");
        for(int k = 0; matrix_kpriv[i][k] != -1; k++){
            printf("%hd", matrix_kpriv[i][k]);
        }
        printf(",");
        for(int k = 0; matrix_kcod[i][k] != -1; k++){
            printf("%hd", matrix_kcod[i][k]);
        }
        printf("\n");
    }
     */

}



void save_txt_keys_int(short **matrix_kpub, short **matrix_kpriv, short **matrix_kcod, int lines, char filename[]){
    unsigned long long pubKey = 0;
    unsigned long long privKey = 0;
    unsigned long long codKey = 0;

    FILE *file;

    file = fopen(filename, "a");

    if(file == NULL){
        printf("Error opening file\n");
        return;
    }

    fprintf(file,"int keys: \n");
    for(int i = 0; i < lines; i++){
        pubKey = 0;
        for(int j = 0; matrix_kpub[i][j] != -1; j++){
            pubKey *= 10;
            pubKey += matrix_kpub[i][j];
        }
        privKey = get_private_key_int(matrix_kpub, matrix_kpriv, lines, pubKey);//verificar se existe a privKey na matriz de chaves publicas, se existir é passada a privKey para a variavel, se não é passado 0
        codKey = get_runlength_int(matrix_kpriv,matrix_kcod,lines,privKey);//verificar se existe a codKey na matriz de chaves privadas, se existir é passada a codKey para a variavel, se não é passado 0

        fprintf(file, "pubKey: %llu | privKey: %llu | codKey: %llu\n", pubKey, privKey, codKey);//print para o file

        if(ferror(file)){
            printf("Error writing to file.\n");
            return;
        }
    }
}

void load_txt_keys_int(short **matrix_kpub, short **matrix_kpriv, short **matrix_kcod, int lines, char filename[]){

    FILE *file;

    file = fopen(filename, "r");

    short* pubkeyDigits;
    short* privkeyDigits;
    short* codkeyDigits;

    if(file == NULL){
        printf("Error opening file\n");
        return;
    }
    char buffer[255];
    int i = 0;
    while(fgets(buffer,255,file) != NULL) {
        char *token1 = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        char *token3 = strtok(NULL, "\n");

        int pubkey = atoi(token1);
        int privkey = atoi(token2);
        int codkey = atoi(token3);
        if(token1 != NULL){
            pubkeyDigits = key_long_2_digits_int(pubkey);
            for(int j = 0; j < keySize(pubkey); j++) {
                matrix_kpub[i][j] = pubkeyDigits[j];
            }
        }
        if(token1 != NULL){
            privkeyDigits = key_long_2_digits_int(privkey);
            for(int j = 0; j < keySize(privkey); j++) {
                matrix_kpriv[i][j] = privkeyDigits[j];
            }
        }
        if(token1 != NULL){
            codkeyDigits = key_long_2_digits_int(codkey);
            for(int j = 0; j < keySize(codkey); j++) {
                matrix_kcod[i][j] = codkeyDigits[j];
            }
        }
        i++;
    }
    fclose(file);
}


int main_aedlp1_int() {

    Key_HolderInts khInts = {NULL, NULL, NULL, 10, 20};
    khInts.matrixPubKeysInts = alloc_matrix_int(khInts.nlines, khInts.ncolumns);
    khInts.matrixPrivKeysInts = alloc_matrix_int(khInts.nlines, khInts.ncolumns);
    khInts.matrixCodKeysInts = alloc_matrix_int(khInts.nlines, khInts.ncolumns);
    int lines = 5;

    //key_long_2_digits_int(123456789);

    /*int n = 5;
    short* keydigits = malloc(n * sizeof(short));
    keydigits[0] = 1;
    keydigits[1] = 2;
    keydigits[2] = 3;
    keydigits[3] = 4;
    keydigits[4] = 5;
    key_digits_2_long_int(keydigits);
*/
    //printf("%llu", calc_private_key_int(91));
    //printf("%llu", calc_runlength_int(22555557));
    //printf("%llu", private_key_from_runlength_int(26115));

    //testar insercao na struct de pubKeys
/*
    store_key_int(khInts.matrixPubKeysInts, lines, 18);
    store_key_int(khInts.matrixPubKeysInts, lines, 2013);
    store_key_int(khInts.matrixPubKeysInts, lines, 251);
    store_key_int(khInts.matrixPubKeysInts, lines, 285);
    store_key_int(khInts.matrixPubKeysInts, lines, 138);

    //testar insercao na struct de privKeys
    store_key_int(khInts.matrixPrivKeysInts, lines, 555588);
    store_key_int(khInts.matrixPrivKeysInts, lines, 5522);
    store_key_int(khInts.matrixPrivKeysInts, lines, 36);
    store_key_int(khInts.matrixPrivKeysInts, lines, 552);
    store_key_int(khInts.matrixPrivKeysInts, lines, 855);

    //testar insercao na struct de codKeys
    store_key_int(khInts.matrixCodKeysInts, lines, 1316);
    store_key_int(khInts.matrixCodKeysInts, lines, 2522);
    store_key_int(khInts.matrixCodKeysInts, lines, 4528);
    store_key_int(khInts.matrixCodKeysInts, lines, 1825);
    store_key_int(khInts.matrixCodKeysInts, lines, 2512);

*/

    //exists_key_int(khInts.matrixPubKeysInts, lines,18);
    //printf("%d" ,exists_key_int(khInts.matrixPubKeysInts, lines,2013));
    //delete_key_int(khInts.matrixPubKeysInts,khInts.matrixPrivKeysInts,khInts.matrixCodKeysInts, lines,285);

    //bulk_populate_public_keys_int(khInts.matrixPubKeysInts, lines);
    //bulk_compute_private_keys_int(khInts.matrixPubKeysInts,khInts.matrixPrivKeysInts, lines);
    //bulk_compute_runlengths_int(khInts.matrixPrivKeysInts,khInts.matrixCodKeysInts, lines);

    //search_private_keys_int(khInts.matrixPubKeysInts,khInts.matrixPrivKeysInts, lines, 1);
    //sort_matrix_int(khInts.matrixPubKeysInts, lines, 1);
    //sort_matrix_int(khInts.matrixPubKeysInts, lines, 1);
    //sort_all_matrices_int(khInts.matrixPubKeysInts, khInts.matrixPrivKeysInts, khInts.matrixCodKeysInts, lines, 1);




    //int size = keySize(123456789123456789);

    //char *filename = "C:\\Users\\artur\\Desktop\\UFP\\LP12022pubufp\\PROJETOLP1AED1\\keysFileInts.txt";
    //save_txt_keys_int(khInts.matrixPubKeysInts,khInts.matrixPrivKeysInts,khInts.matrixCodKeysInts, lines,filename);
    char* fileToRead = "C:\\Users\\artur\\Desktop\\UFP\\LP12022pubufp\\PROJETOLP1AED1\\keysToRead.txt";
    load_txt_keys_int(khInts.matrixPubKeysInts,khInts.matrixPrivKeysInts,khInts.matrixCodKeysInts, lines,fileToRead);

    printf("pubkeys: \n");
    for (int j = 0; j < lines; j++) {
        for (int i = 0; khInts.matrixPubKeysInts[j][i] != -1; i++) {
            printf("%hd ", khInts.matrixPubKeysInts[j][i]);
        }
        printf("\n");
    }
    printf("privkeys: \n");
    for (int j = 0; j < lines; j++) {
        for (int i = 0; khInts.matrixPrivKeysInts[j][i] != -1; i++) {
            printf("%hd ", khInts.matrixPrivKeysInts[j][i]);
        }
        printf("\n");
    }
    printf("codkeys: \n");
    for (int j = 0; j < lines; j++) {
        for (int i = 0; khInts.matrixCodKeysInts[j][i] != -1; i++) {
            printf("%hd ", khInts.matrixCodKeysInts[j][i]);
        }
        printf("\n");
    }
    //key_long_2_digits_int(4969686898566);

    //list_keys_int(khInts.matrixPubKeysInts,khInts.matrixPrivKeysInts,khInts.matrixCodKeysInts,lines,0);

    return 0;
}