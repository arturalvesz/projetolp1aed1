//
// Created by artur on 03/12/2022.
//

#include "aed1_lp1_2223_proj_part1_chars.h"


char* key_long_2_digits_char(unsigned long long key){
    char* digitsArray;
    int count = 0;
    unsigned long long n = key;

    while (n){

        count++;
        n /= 10;
    }

    digitsArray = (char*)calloc(count+1, sizeof(char));

    ulltoa(key, digitsArray, 10);

    return (char*) digitsArray;
}

unsigned long long key_digits_2_long_char(char* keydigits){

    unsigned long long  key = 0;
    key = atoll(keydigits);
    return key;
}

unsigned long long calc_private_key_char(unsigned long long pubkey){
    int count = 0;
    unsigned long long mult = pubkey * 2;
    while (!isBipolar(mult)) { //Enquanto o multiplo não for bipolar, adicionamos o número dado a ele e verificamos novamente
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

int isBipolar(unsigned long long multiple){

    char* mult_char = key_long_2_digits_char(multiple);
    int mult_size = strlen(mult_char);
    char digit;
    int count = 1;
    digit = mult_char[0];
    for(int i = 0; i < mult_size; i++){
        if(digit != mult_char[i]){
            digit = mult_char[i];
            count++;
        }
    }
    if(count == 2){
        return 1;
    }else{
        return 0;
    }
}

unsigned long long calc_runlength_char(unsigned long long privkey){
    int count = 0;
    unsigned long long auxPrivKey = privkey;

    int check = isBipolar(privkey); //se o numero não for bipolar não é calculada a sua run length
    if(check != 1){
        printf("Impossible to calculate the runlength because the privkey isn't bipolar\n");
        return 0;
    }

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

unsigned long long private_key_from_runlength_char(unsigned long long runlengthkey){
    unsigned long long key = runlengthkey;
    unsigned long long privKey = 0;
    int arrSize = 0;
    unsigned long long *privKeyArr;


    int runlengthKeySize = 0;
    while(runlengthkey){
        runlengthKeySize++;
        runlengthkey /= 10;
    }
    unsigned long long aux[runlengthKeySize];
    unsigned long long runlengthArr[runlengthKeySize];

    for(int i = 0; i < runlengthKeySize; i++){
        aux[i] = key % 10;
        key /= 10;
    }
    for(int i = 0; i < runlengthKeySize; i++){
        runlengthArr[i] = aux[(runlengthKeySize-1)-i];
    }


    if(runlengthKeySize > 4){
        if((runlengthArr[0] * 10) + runlengthArr[1] > 10 && (runlengthArr[0] * 10) + runlengthArr[1] < 19){
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
        else if((runlengthArr[2] * 10) + runlengthArr[3] > 10 && (runlengthArr[2] * 10) + runlengthArr[3] < 19){
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

    return privKey;
}

char** alloc_matrix_char(int nlines, int ncolumns){

    char** matrix = malloc(nlines * sizeof(char*));
    for(int i = 0; i < nlines; i++){
        *(matrix + i) = malloc(ncolumns * sizeof(char));
        *(matrix + i) = "0";

    }
    return (char **) matrix;
}

void store_key_char(char **matrix, int lines, unsigned long long key){
    char *keyDigits = key_long_2_digits_char(key);

    for(int i = 0; i < lines; i++){;
        if(strcmp(*(matrix + i), "0") == 0) {
            *(matrix + i) = malloc(sizeof(char) * (strlen(keyDigits) + 1));
            strcpy(*(matrix + i), keyDigits);
            break;
        }
    }
}

int exists_key_char(char **matrix, int lines, unsigned long long key){
    char *keyDigits = key_long_2_digits_char(key);
    for(int i = 0; i < lines; i++){
        if(strcmp(matrix[i], keyDigits) == 0){
            return 1;
        }
    }
    return 0;
}

unsigned long long get_private_key_char(char **matrix_kpub, char **matrix_kpriv, int lines, unsigned long long pubkey){

    char *keyDigits = key_long_2_digits_char(pubkey);
    unsigned long long priv_Key = 0;
    char *privKeyDigits;

    for(int i = 0; i < lines; i++){
        if(strcmp(matrix_kpub[i], keyDigits) == 0){
            priv_Key = calc_private_key_char(pubkey);
        }
    }
    privKeyDigits = key_long_2_digits_char(priv_Key);

    for(int i = 0; i < lines; i++){
        if(strcmp(matrix_kpriv[i],privKeyDigits) == 0){
            return priv_Key;
        }
    }
    return 0;
}

unsigned long long get_runlength_char(char **matrix_kpriv, char **matrix_kcod, int lines, unsigned long long privkey){
    char *privKeyDigits = key_long_2_digits_char(privkey);
    unsigned long long runlengthKey = 0;
    char *runlengthKeyDigits;

    for(int i = 0; i < lines; i++){
        if(strcmp(matrix_kpriv[i], privKeyDigits) == 0){
            runlengthKey = calc_runlength_char(privkey);
        }
    }
    runlengthKeyDigits = key_long_2_digits_char(runlengthKey);

    for(int i = 0; i < lines; i++){
        if(strcmp(matrix_kcod[i],runlengthKeyDigits) == 0){
            return runlengthKey;
        }
    }
    return 0;
}


unsigned long long delete_key_char(char **matrix_kpub, char **matrix_kpriv, char **matrix_kcod, int lines, char* pubkey) {

    unsigned long long pubKeyINT = key_digits_2_long_char(pubkey);
    unsigned long long privKey = calc_private_key_char(pubKeyINT);
    unsigned long long runlengthKey = calc_runlength_char(privKey);

    char *privKeyDigits = key_long_2_digits_char(privKey);
    char *runlengthDigits = key_long_2_digits_char(runlengthKey);

    for (int i = 0; i < lines; i++){
        if(strcmp(matrix_kpub[i], pubkey) == 0){
            int pos = i;
            for(int j = pos; j < lines; j++){
                matrix_kpub[j] = matrix_kpub[j+1];
            }
        }
        if(strcmp(matrix_kpriv[i], privKeyDigits) == 0){
            int pos = i;
            for(int j = pos; j < lines; j++){
                matrix_kpriv[j] = matrix_kpriv[j+1];
            }
        }
        if(strcmp(matrix_kcod[i], runlengthDigits) == 0){
            int pos = i;
            for(int j = pos; j < lines; j++){
                matrix_kcod[j] = matrix_kcod[j+1];
            }
        }

        if(i == lines - 1) {
            matrix_kpub[lines] = "0";
            matrix_kpriv[i] = "0";
            matrix_kcod[i] = "0";
        }
    }
    return pubKeyINT;
}


void bulk_populate_public_keys_char(char **matrix_kpub, int lines){

    unsigned long long* randomN = malloc(lines * sizeof(unsigned long long));

    for(int i = 0; i < lines; i++){
        randomN[i]= rand() % 2000; //limite superior do random
    }
    for(int i = 0; i < lines; i++){
        matrix_kpub[i] = key_long_2_digits_char(randomN[i]);
    }

    for(int i = 0; i < lines; i++){
        printf("char pub key: %s\n", matrix_kpub[i]);
    }
}

void bulk_compute_private_keys_char(char **matrix_kpub, char **matrix_kpriv, int lines){
    unsigned long long * pubKeyInt = malloc(lines * sizeof(unsigned long long));
    unsigned long long * privKeyInt = malloc(lines * sizeof(unsigned long long));

    for(int i = 0; i < lines; i++){
        pubKeyInt[i] = key_digits_2_long_char(matrix_kpub[i]);
    }

    for(int i = 0; i < lines; i++){
       privKeyInt[i] = calc_private_key_char(pubKeyInt[i]);
    }

    for(int i = 0; i < lines; i++){
        matrix_kpriv[i] = key_long_2_digits_char(privKeyInt[i]);
    }
    for(int i = 0; i < lines; i++){
        printf("private keys : %s\n", matrix_kpriv[i]);
    }

}

void bulk_compute_runlengths_char(char **matrix_kpriv, char **matrix_kcod, int lines){

    for(int i = 0; i < lines; i++){
        matrix_kcod[i] = key_long_2_digits_char(calc_runlength_char(key_digits_2_long_char(matrix_kpriv[i])));
    }

    for(int i = 0; i < lines; i++){
        printf("cod keys : %s\n", matrix_kcod[i]);
    }
}


char** search_private_keys_char(char **matrix_kpub, char **matrix_kpriv, int lines, unsigned long long partialpubkey){

    char *partialpubkeyDigits = key_long_2_digits_char(partialpubkey);
    char** privKeysMatching = malloc(lines * sizeof(char*));
    int* partialPubKeysint = malloc(sizeof(int) * lines);
    int count = 0;



    for(int i = 0; i < lines; i++){
        char* res = strstr(matrix_kpub[i], partialpubkeyDigits); //procura-se a partialpubkeyDigits na matriz e a função strstr retorna a substring se for encontrada

        if(res != NULL) {
            partialPubKeysint[i] = i;
            count++;
        }
    }

    for(int i = 0; i < count; i++){

        privKeysMatching[i] = matrix_kpriv[partialPubKeysint[i]];
    }

    for(int i = 0; i < count; i++){
        printf("cod keys : %s\n", privKeysMatching[i]);
    }

    return privKeysMatching;
}

void sort_matrix_char(char **matrix, int lines, int order) {

    unsigned long long* matrixInt = malloc(sizeof(unsigned long long) * lines);

    for(int i = 0; i < lines; i++) {
        matrixInt[i] = key_digits_2_long_char(matrix[i]);
    }
    //order = 1, ascending order
    if(order == 1) {
        for (int i = 0; i < lines; i++) {
            for (int j = i + 1; j < lines; j++) {
                if (matrixInt[i] > matrixInt[j]) {
                    unsigned long long temp = matrixInt[i];
                    matrixInt[i] = matrixInt[j];
                    matrixInt[j] = temp;
                }
            }
        }
    }
    //order = 0, descending order
    if(order == 0) {
        for (int i = 0; i < lines; i++) {
            for (int j = i + 1; j < lines; j++) {
                if (matrixInt[i] < matrixInt[j]) {
                    unsigned long long temp = matrixInt[i];
                    matrixInt[i] = matrixInt[j];
                    matrixInt[j] = temp;
                }
            }
        }
    }

    for(int i = 0; i < lines; i++){
        matrix[i] = key_long_2_digits_char(matrixInt[i]);
    }


}

void sort_all_matrices_char(char **matrix_kpub, char **matrix_kpriv, char **matrix_kcod, int lines, int order){

    sort_matrix_char(matrix_kpub,lines,order);
    sort_matrix_char(matrix_kpriv,lines,order);
    sort_matrix_char(matrix_kcod,lines,order);

    /**
     * Testes da função
    for(int i = 0; i < lines; i++){
        printf("pub: %s ", matrix_kpub[i]);
        printf("priv: %s ", matrix_kpriv[i]);
        printf("cod: %s\n", matrix_kcod[i]);
    }
     */
}

int compareAsc(const void *a, const void *b) {
    // Return a positive value if the length of *a > the length of *b,
    // a negative value if the length of *a < the length of *b,
    // and 0 if the length of *a == the length of *b
    return strlen(*(const char **)a) - strlen(*(const char **)b);
}

int compareDesc(const void *a, const void *b) {
    // Return a positive value if the length of *a > the length of *b,
    // a negative value if the length of *a < the length of *b,
    // and 0 if the length of *a == the length of *b
    return strlen(*(const char **)b) - strlen(*(const char **)a);
}

void list_keys_char(char **matrix_kpub, char **matrix_kpriv, char **matrix_kcod, int lines, int order){


    char** privKeyDigits = malloc(sizeof(char*) * lines);
    char** runlengthKeyDigits = malloc(sizeof(char*) * lines);


    unsigned long long pubKey = 0;
    unsigned long long privKey = 0;
    unsigned long long runlengthKey = 0;


    //order = 1, ascending order
    if(order == 1) {
        qsort(matrix_kpub, lines, sizeof (char*), compareAsc); //comparação da length das strings
        for (int i = 0; i < lines; i++) {
            pubKey = key_digits_2_long_char(matrix_kpub[i]);
            privKey = get_private_key_char(matrix_kpub, matrix_kpriv, lines,pubKey);
            privKeyDigits[i] = key_long_2_digits_char(privKey);
            runlengthKey = get_runlength_char(matrix_kpriv,matrix_kcod,lines,privKey);
            runlengthKeyDigits[i] = key_long_2_digits_char(runlengthKey);
        }
    }
    unsigned long long tempPubKey = 0;
    unsigned long long tempPrivKey = 0;
    unsigned long long tempRunlengthKey = 0;

    //order = 0, descending order
    if(order == 0) {
        qsort(matrix_kpub, lines, sizeof(char *), compareDesc);//comparação da length das strings
        for (int i = 0; i < lines; i++) {
            pubKey = key_digits_2_long_char(matrix_kpub[i]);
            privKey = get_private_key_char(matrix_kpub, matrix_kpriv, lines, pubKey);
            privKeyDigits[i] = key_long_2_digits_char(privKey);
            runlengthKey = get_runlength_char(matrix_kpriv, matrix_kcod, lines, privKey);
            runlengthKeyDigits[i] = key_long_2_digits_char(runlengthKey);
        }
    }

    for(int i = 0; i < lines; i++){
        strcpy(matrix_kpriv[i], privKeyDigits[i]);
        strcpy(matrix_kcod[i], runlengthKeyDigits[i]);
   }

    /**
     * testes da função
     *for(int i = 0; i < lines; i++){
     * printf("pub: %s ", matrix_kpub[i]);
     * printf("priv: %s ", matrix_kpriv[i]);
     * printf("cod: %s\n", matrix_kcod[i]);
     * }
     * */
}


void save_txt_keys_char(char **matrix_kpub, char **matrix_kpriv, char **matrix_kcod, int lines, char filename[]){
    unsigned long long pubKey = 0;
    unsigned long long privKey = 0;
    unsigned long long runlengthKey = 0;

    char** pubKeyDigits = malloc(sizeof(char*) * lines);
    char** privKeyDigits = malloc(sizeof(char*) * lines);
    char** runlengthKeyDigits = malloc(sizeof(char*) * lines);

    FILE *file;


    file = fopen(filename, "a");

    if(file == NULL){
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < lines; i++) {
        pubKey = key_digits_2_long_char(matrix_kpub[i]);
        pubKeyDigits[i] = key_long_2_digits_char(pubKey);
        privKey = get_private_key_char(matrix_kpub, matrix_kpriv, lines,pubKey); //verificar se existe a privKey na matriz de chaves publicas, se existir é passada a privKey para a variavel, se não 0
        privKeyDigits[i] = key_long_2_digits_char(privKey);
        runlengthKey = get_runlength_char(matrix_kpriv,matrix_kcod,lines,privKey);//verificar se existe a codKey na matriz de chaves privadas, se existir é passada a codKey para a variavel, se não 0
        runlengthKeyDigits[i] = key_long_2_digits_char(runlengthKey);
    }

    fprintf(file,"char keys: \n");
    for(int i = 0; i < lines; i++){
        fprintf(file, "pubKey: %s | privKey: %s | codKey: %s\n", pubKeyDigits[i],privKeyDigits[i],runlengthKeyDigits[i]);//print para o file

        if(ferror(file)){
            printf("Error writing to file.\n");
            return;
        }
    }
}
void load_txt_keys_char(char **matrix_kpub, char **matrix_kpriv, char **matrix_kcod, int lines, char filename[]){

    FILE *file;
    file = fopen(filename, "r");

    if(file == NULL){
        printf("Error opening file\n");
        return;
    }

    matrix_kpub = malloc(sizeof(char*) * lines);
    matrix_kpriv = malloc(sizeof(char*) * lines);
    matrix_kcod = malloc(sizeof(char*) * lines);


    char buffer[255];
    int i = 0;


    while(fgets(buffer,255,file) != NULL){
        char *token1 = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        char *token3 = strtok(NULL, "\n");
        if(token1 != NULL){
            matrix_kpub[i] = malloc(strlen(token1) + 1);
            strcpy(matrix_kpub[i], token1);
        }if(token2 != NULL){
            matrix_kpriv[i] = malloc(strlen(token2) + 1);
            strcpy(matrix_kpriv[i], token2);
        }if(token3 != NULL){
            matrix_kcod[i] = malloc(strlen(token3) + 3);
            strcpy(matrix_kcod[i], token3);
        }
        i++;
    }
    fclose(file);
}


int main_aedlp1_char(){
    //TESTE DAS FUNÇÕES CRIADAS

    //unsigned long long key = 12933339939;
    //printf("%s" , key_long_2_digits_char(key));
    //const char *keys= {"11112255555"};
    //printf("%lld", key_digits_2_long_char(keys));
    //printf("%lld", private_key_from_runlength_char(13430));
    //printf("%lld", calc_runlength_char(555557777));

    //printf("%lld", calc_private_key_char(3096));
    Key_HolderChars kh1 = {NULL,NULL,NULL,10,20};
    kh1.matrixPubKeysChars = alloc_matrix_char(kh1.nlines,kh1.ncolumns);
    kh1.matrixPrivKeysChars = alloc_matrix_char(kh1.nlines,kh1.ncolumns);
    kh1.matrixRunLengthChars = alloc_matrix_char(kh1.nlines,kh1.ncolumns);
    int lines = 3;

    //char *matrix_priv[] = {"1112222", "33336666", "33335555"};
    //char *matrix_runlength[] = {"3142", "4346","4345"};

    //testar insercao na struct de pubKeys
    store_key_char(kh1.matrixPubKeysChars, lines, 2013);
    store_key_char(kh1.matrixPubKeysChars, lines, 251);
    store_key_char(kh1.matrixPubKeysChars, lines, 18);
    //testar inserção na struct de privkeys.
    store_key_char(kh1.matrixPrivKeysChars, lines, 36);
    store_key_char(kh1.matrixPrivKeysChars, lines, 5522);
    store_key_char(kh1.matrixPrivKeysChars, lines, 555588);
    //testar inserção na struct de keys codificadas(runlength).
    store_key_char(kh1.matrixRunLengthChars, lines, 1316);
    store_key_char(kh1.matrixRunLengthChars, lines, 2522);
    store_key_char(kh1.matrixRunLengthChars, lines, 4528);

    //exists_key_char(kh1.matrixPrivKeysChars,lines,222444);

    //get_runlength_char(kh1.matrixPrivKeysChars,kh1.matrixRunLengthChars,lines,33355555);

    //delete_key_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars,kh1.matrixRunLengthChars,lines,"251");

    //printf("%lld", get_runlength_char(kh1.matrixPrivKeysChars, matrix_runlength, lines, 33336666));

    //bulk_populate_public_keys_char(kh1.matrixPubKeysChars, lines);
    //bulk_compute_private_keys_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars, lines);
    //bulk_compute_runlengths_char(kh1.matrixPrivKeysChars,kh1.matrixPrivKeysChars,lines);
    //search_private_keys_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars,lines,18);
    //sort_matrix_char(kh1.matrixPubKeysChars, lines,1);
    //sort_all_matrices_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars,kh1.matrixRunLengthChars,lines,0);
    //list_keys_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars,kh1.matrixRunLengthChars,lines,0);

    /*char* filename = "C:\\Users\\artur\\Desktop\\UFP\\LP12022pubufp\\PROJETOLP1AED1\\keysFile.txt";
    save_txt_keys_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars,kh1.matrixRunLengthChars,2,filename);
    save_txt_keys_char(kh2.matrixPubKeysChars,kh2.matrixPrivKeysChars,kh2.matrixRunLengthChars,1,filename);
*/
    /*char* fileToRead = "C:\\Users\\artur\\Desktop\\UFP\\LP12022pubufp\\PROJETOLP1AED1\\keysToRead.txt";
     *
    load_txt_keys_char(kh1.matrixPubKeysChars,kh1.matrixPrivKeysChars,kh1.matrixRunLengthChars,lines,fileToRead);
*/
}