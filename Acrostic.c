/* 	Georgios Krimitzakis
	AM: 202220230082
	dit23082@go.uop.gr

	Giorgos Kritsidimas
	AM:2022202300116
	dit23116@go.uop.gr
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct wList
{
    char *word;                 // the stored word
    int wordLength;             // length of the word in letters
    int uniqLetters;            // number of unique word letters
    int numOfTimes;             // no of times a word appears in the text
    int insertionMode;          //0 from file, 1 from keyboard
    struct tm insertionDate;    /// Insert date time https://cplusplus.com/reference/ctime/tm/
    struct wList *next;
    struct wList *previous;
} wordList;

typedef struct{
    char *verticalWord;         /// Η κατακόρυφη λέξη
    char **horizontalWords;     /// πινακας με τις οριζοντιες λέξεις
    char **displayedAcrostic;     /// Πινακας με την εμφανιζομενη Ακροστιχίδα
    int difficulty;             /// 0 ευκολο, 1 δυσκολο
    int length;                 /// Μηκος κατακορυφης λέξης.
}Acrostic;

/// https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm
///struct tm {
///   int tm_sec;         /* seconds,  range 0 to 59          */
///   int tm_min;         /* minutes, range 0 to 59           */
///   int tm_hour;        /* hours, range 0 to 23             */
///   int tm_mday;        /* day of the month, range 1 to 31  */
///   int tm_mon;         /* month, range 0 to 11             */
///   int tm_year;        /* The number of years since 1900   */
///   int tm_wday;        /* day of the week, range 0 to 6    */
///   int tm_yday;        /* day in the year, range 0 to 365  */
///   int tm_isdst;       /* daylight saving time             */
///};

/// Functions declaration
char* toUpperCase(char*);
int countUniqueLetters(char*);
wordList* createNode(char*, int );
int insertNode(wordList*);
int deleteNode(char*);
wordList* findWord(char*);
void freeList();              // clear memory from list
int isValidWord(char*);       // if word contains only characters not numbers or special characters or white spaces
void trimWhiteSpace(char *);  // trim white spaces from word
int processText(char*, int);   // Text Process tokenize a sentence and inserted in list all words in sentence
void insertCommand(char*);
/// -----------------------------   6/6/25
void deleteCommand(char*);
void showForwardCommand(int);
void showReverseCommand(int);
void findForwardCommand(char*);
void findReverseCommand(char*);
Acrostic* createAcrostic(char*, int);
/// ------------------   10/6/25
void freeAcrostic(Acrostic*);
void displayAcrostic(Acrostic*);
void loadFromFile(char*);
void saveAcrostic(char*, Acrostic*);
void createCommand(char*);


/// Global
wordList *head   = NULL;           /// Head of doubled linked list
wordList *tail   = NULL;           /// tail of doubled linked list
Acrostic *lastAcrostic = NULL;     ///


int main(int argc, char* argv[])
{

    char input[1024];
    while(1){
        printf("$> ");
        if(fgets(input, sizeof(input), stdin) == NULL){
            break;
        }
        input[strcspn(input,"\n")] = '\0';
        if(strcmp(input,"exit")== 0){
            printf("Exit from program by user command!!!\n\n");
            break;
        }
        if(strncmp(input,"insert:",7)== 0) insertCommand(7+ input);
        else if(strncmp(input,"delete:",7)== 0)   deleteCommand(7+ input);
        else if(strncmp(input,"showfwd:",8)== 0)  showForwardCommand(atoi(input+8));
        else if(strncmp(input,"showrev:",8)== 0)  showReverseCommand(atoi(input+8));
        else if(strncmp(input,"findfwd:",8)== 0)  findForwardCommand(input+8);
        else if(strncmp(input,"findrev:",8)== 0)  findReverseCommand(input+8);
        else if(strncmp(input,"create:",7)== 0) printf("Leipei\n");
        else if(strncmp(input,"load:",5)== 0)     loadFromFile(input+5);
        else if(strncmp(input,"save:",5)== 0){
            if(lastAcrostic != NULL)              saveAcrostic(input+5,lastAcrostic);
            else                                  printf("No Acrostic to save!!!\n");
        }
        else printf("Unknown command!!!\n");

    }
    if(lastAcrostic != NULL)
        freeAcrostic(lastAcrostic);
    freeList();
    return 0;
}


char* toUpperCase(char* word){
    int i;
    if(word==NULL || strlen(word)==0){
        return NULL;
    }
    char *upperWord = (char*)malloc((strlen(word)+1)*sizeof(char));
     if(upperWord == NULL){
        printf("Memory out!!!\n");
        return NULL;
    }
    for(i =0; word[i]!='\0';i++) upperWord[i] = toupper(word[i]);
    upperWord[strlen(word)] = '\0';
    return upperWord;

}

int countUniqueLetters(char *word){
    int i;
    if(word==NULL || strlen(word)==0){
        return 0;
    }
    int count =0;
    int letters[26] = {0};
    for(i =0; word[i] != '\0';i++){
        if(isalpha(word[i])){
            int index = toupper(word[i]) - 'A';  /// A = 65
            if(index>=0 && index<26 && letters[index] ==0){
                letters[index] = 1;
                count++;
            }
        }
    }
    return count;
/// 0 1 2 3 4                     25
///{1,0,1,0,0,00000000000000000000,0}

}

wordList* createNode(char* word, int insertionMode){
    if(word==NULL || strlen(word)==0){
        return NULL;
    }
    wordList *newNode = (wordList *) malloc(sizeof(wordList));
    if(newNode==NULL){
        printf("Memory out!!!\n");
        return NULL;
    }
    char *upperWord = toUpperCase(word);
    if(newNode==NULL){
        free(newNode);
        return NULL;
    }
    newNode->word  = upperWord;
    newNode->wordLength  = strlen(upperWord);
    newNode->uniqLetters  = countUniqueLetters(upperWord);
    newNode->numOfTimes = 1;
    newNode->insertionMode = insertionMode;
    time_t now = time(NULL); //clock system time
    newNode->insertionDate = *localtime(&now); ///https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

int insertNode(wordList *newNode){
    if(newNode == NULL){
        return 0;
    }
    if(head==NULL){ // empty list
        head = newNode;
        tail = newNode;
        return 1;
    }
    // if word exist in list
    wordList *current = head;
    while(current!=NULL){
        if(strcmp(current->word, newNode->word)==0){ // https://www.geeksforgeeks.org/strcmp-in-c/
            current->numOfTimes++;
            free(newNode->word);
            free(newNode);
            return 0;
        }
        if(strcmp(newNode->word,current->word)<0){ /// insert first
            newNode->next = current;
            newNode->previous = current->previous;
            if(current->previous!= NULL){
                current->previous->next = newNode;
            }else{  // exo nea kefalida
                head=newNode;
            }
            current->previous = newNode;
            return 1;
        }

        if(current->next == NULL) break;
        current = current->next;
    }
    /// gia na ftaso edo perasa oli tin lista xoris na xporeso kapou oyte na vro ton eafto mou = eimai sto telos tis listas
    newNode->previous = current;
    current->next = newNode;
    tail= newNode;
    return 1;
}

int deleteNode(char *word ){
    if(word == NULL || head == NULL){
        return 0;
    }
    char *upperWord = toUpperCase(word);
    if(upperWord == NULL ){
        return 0;
    }
    /// anazitsi lexis mesa stin lista
    wordList *current = head;
    while(current !=NULL){
        if(strcmp(current->word, upperWord) ==0 ){ ///Vrethike i lexi https://www.geeksforgeeks.org/strcmp-in-c/

            /// elegxos an einai tail
            /// an den einai head or tail
            if(current->previous!=NULL){
                current->previous->next = current->next;
            }else{
                /// current is head
                head = current->next;
            }
            if(current->next != NULL){
                current->next->previous = current->previous;

            }else{
                 /// current is tail
                tail = current->previous;
            }
            free(current->word);
            free(current);
            free(upperWord);
            return 1;
        }
        current = current->next; /// to next Node
    }
    free(upperWord);
    return 0;
}

wordList* findWord(char* word){
    if(word == NULL || head == NULL){
        return NULL;
    }
    char *upperWord = toUpperCase(word);
    if(upperWord == NULL ){
        return NULL;
    }
    /// anazitsi lexis mesa stin lista
    wordList *current = head;
    while(current != NULL){
        if(strcmp(current->word, upperWord) ==0 ){ ///Vrethike i lexi https://www.geeksforgeeks.org/strcmp-in-c/
            free(upperWord);
            return current;
        }
        current = current->next; /// to next Node
    }
    free(upperWord);
    return 0;
}

void freeList(){
    wordList *current = head;
    wordList *next;
    while(current!=NULL){
        next = current->next;
        free(current->word);
        free(current);
        current = next;
    }
    head = NULL;
    tail = NULL;
}

int isValidWord(char *word){
    if(word==NULL || strlen(word)==0) return 0;
    int i;
    for(i =0; word[i] != '\0';i++){  /// '\0' = null terminated string(array of characters)
         if(!isalpha(word[i])) return 0; ///https://www.geeksforgeeks.org/isalpha-isdigit-functions-c-example/
    }
    return 1;
}

void trimWhiteSpace(char *str){
     if(str==NULL) return;
     /// remove spaces from begin
     char *start = str;
     while(isspace(*start)) start++;                    ///https://www.geeksforgeeks.org/isspace-in-c/
     if(start!=str) memmove(str,start,strlen(start)+1); ///https://www.geeksforgeeks.org/memmove-in-cc/

     /// remove spaces from end
     char *end = str+strlen(str)-1;
     while(end > str && isspace(*end)){
        *end='\0';
        end--;
     }
}

int processText(char *text, int insertionMode){
   if(text==NULL) return 0;
   int insertedWords = 0;
   char *copy  = strdup(text); ///https://www.geeksforgeeks.org/strdup-strdndup-functions-c/
   if(copy==NULL){
            printf("Error : Memory is full!!!\n");
            return 0;
   }
   char *token = strtok(copy," \t\n\f\r\v,.;:!?'\"()[]{}" );
   while(token !=NULL){
    trimWhiteSpace(token);
    if(isValidWord(token)){
        wordList *newNode = createNode(token,insertionMode);
        if(newNode !=NULL){
            if(insertNode(newNode)){
                printf("Word <<%s>> added to list\n",token);
                insertedWords++;
            }else{
                printf("Word <<%s>> all ready exist in list\n",token);
            }
        }
    }else{
        printf("Word <<%s>> is not valid and reject\n",token);
    }
    token = strtok(NULL," \t\n\f\r\v,.;:!?'\"()[]{}" );

    }
    free(copy);
    return insertedWords;
}

void insertCommand(char *input){
    if(input == NULL){
        printf("Error invalid input!!\n");
        return;
    }
    while(isspace(*input)) input++;
    int insertedWords = processText(input,1);
    printf("Total inserted new words = %d",insertedWords);
}

//
void deleteCommand(char *input) {
    if (input == NULL) {
        printf("Σφάλμα: Μη έγκυρη είσοδος.\n");
        return;
    }

    // delete all white spaces from begin
    while (isspace(*input)) {
        input++;
    }

    // tokenization
    char *copy = strdup(input);
    if (copy == NULL) {
        printf("Memory Full!!\n");
        return;
    }

    char *token = strtok(copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Delete Word
        if (deleteNode(token)) {
            printf("Word '%s' deleted.\n", token);
        } else {
            printf("Not found '%s' in list\n", token);
        }

        // next word
        token = strtok(NULL, " \t\n\r\f\v");
    }

    free(copy);
}


void showForwardCommand(int count) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    int shown = 0;
    wordList *current = head;

    printf("Words from Begin ):\n");
    while (current != NULL && (count <= 0 || shown < count)) {
        printf("%d. %s (length: %d, unique letters: %d, views: %d, input method: %s, date: %d/%d/%d %d:%d:%d)\n",
               shown + 1,
               current->word,
               current->wordLength,
               current->uniqLetters,
               current->numOfTimes,
               current->insertionMode == 0 ? "file" : "keyboard",
               current->insertionDate.tm_mday,
               current->insertionDate.tm_mon + 1,
               current->insertionDate.tm_year + 1900,
               current->insertionDate.tm_hour,
               current->insertionDate.tm_min,
               current->insertionDate.tm_sec);

        current = current->next;
        shown++;
    }

    printf("Shown %d words.\n", shown);
}

//
void showReverseCommand(int count) {
    if (tail == NULL) {
        printf("List is empty.\n");
        return;
    }

    int shown = 0;
    wordList *current = tail;

    printf("Words from the end:\n");
    while (current != NULL && (count <= 0 || shown < count)) {
        printf("%d. %s (length: %d, unique letters: %d, views: %d, input method: %s, date: %d/%d/%d %d:%d:%d)\n",
               shown + 1,
               current->word,
               current->wordLength,
               current->uniqLetters,
               current->numOfTimes,
               current->insertionMode == 0 ? "file" : "keyboard",
               current->insertionDate.tm_mday,
               current->insertionDate.tm_mon + 1,
               current->insertionDate.tm_year + 1900,
               current->insertionDate.tm_hour,
               current->insertionDate.tm_min,
               current->insertionDate.tm_sec);

        current = current->previous;
        shown++;
    }

    printf("Shown %d words.\n", shown);
}


void findForwardCommand(char *input) {
    if (input == NULL || head == NULL) {
        printf("Error List empty or wrong input\n");
        return;
    }


    while (isspace(*input)) {
        input++;
    }


    char *copy = strdup(input);
    if (copy == NULL) {
        printf("Memory Full!!!\n");
        return;
    }

    char *prefix = strtok(copy, " \t\n\r\f\v");
    char *countStr = strtok(NULL, " \t\n\r\f\v");

    if (prefix == NULL || countStr == NULL) {
        printf("Error: Invalid command. Use: findfwd: [acronym] [number]\n");
        free(copy);
        return;
    }

    // Acronim to Upper
    char *upperPrefix = toUpperCase(prefix);
    if (upperPrefix == NULL) {
        free(copy);
        return;
    }

    // str to Integer
    int count = atoi(countStr);
    if (count <= 0) {
        printf("Error only positive numbers\n");
        free(copy);
        free(upperPrefix);
        return;
    }

    // serach for the acornym
    int found = 0;
    wordList *current = head;

    printf("words start ftom '%s':\n", upperPrefix);
    while (current != NULL && found < count) {
        if (strncmp(current->word, upperPrefix, strlen(upperPrefix)) == 0) {
            printf("%d. %s (length: %d, unique letters: %d, views: %d)\n",
                   found + 1,
                   current->word,
                   current->wordLength,
                   current->uniqLetters,
                   current->numOfTimes);

            found++;
        }

        current = current->next;
    }

    if (found == 0) {
        printf("No words start with '%s'.\n", upperPrefix);
    } else {
        printf("Found %d words start from '%s'.\n", found, upperPrefix);
    }

    free(copy);
    free(upperPrefix);
}


void findReverseCommand(char *input) {
    int i;
    if (input == NULL || tail == NULL) {
        printf("Error wrong input or empty list\n");
        return;
    }

    //
    while (isspace(*input)) {
        input++;
    }

    //
    char *copy = strdup(input);
    if (copy == NULL) {
        printf("Memory full\n");
        return;
    }

    char *prefix = strtok(copy, " \t\n\r\f\v");
    char *countStr = strtok(NULL, " \t\n\r\f\v");

    if (prefix == NULL || countStr == NULL) {
        printf("Error command. use: findrev: [acronym] [number]\n");
        free(copy);
        return;
    }

    //
    char *upperPrefix = toUpperCase(prefix);
    if (upperPrefix == NULL) {
        free(copy);
        return;
    }

    //
    int count = atoi(countStr);
    if (count <= 0) {
        printf("Error Only Positive Numbers\n");
        free(copy);
        free(upperPrefix);
        return;
    }

    //
    wordList **matchingWords = NULL;
    int matchingCount = 0;
    wordList *current = head;

    //
    while (current != NULL) {
        if (strncmp(current->word, upperPrefix, strlen(upperPrefix)) == 0) {
            matchingCount++;
        }
        current = current->next;
    }

    if (matchingCount == 0) {
        printf("No words start with '%s'.\n", upperPrefix);
        free(copy);
        free(upperPrefix);
        return;
    }

    //
    matchingWords = (wordList **)malloc(matchingCount * sizeof(wordList *));
    if (matchingWords == NULL) {
        printf("Memeory full\n");
        free(copy);
        free(upperPrefix);
        return;
    }

    //
    current = head;
    int index = 0;
    while (current != NULL) {
        if (strncmp(current->word, upperPrefix, strlen(upperPrefix)) == 0) {
            matchingWords[index++] = current;
        }
        current = current->next;
    }

    //
    int start = matchingCount - count;
    if (start < 0) {
        start = 0;
    }

    printf("Words start with  '%s' (from end):\n", upperPrefix);
    for (i = matchingCount - 1; i >= start; i--) {
        printf("%d. %s (length: %d, unique letters: %d, views: %d)\n",
               matchingCount - i,
               matchingWords[i]->word,
               matchingWords[i]->wordLength,
               matchingWords[i]->uniqLetters,
               matchingWords[i]->numOfTimes);
    }

    printf("Found %d words that start with '%s'.\n", matchingCount - start, upperPrefix);

    free(matchingWords);
    free(copy);
    free(upperPrefix);
}

/* */
Acrostic* createAcrostic(char *verticalWord, int difficulty) {
    int i,j;
    if (verticalWord == NULL || head == NULL) {
        return NULL;
    }

    //Converting the vertical word to uppercase
    char *upperVertical = toUpperCase(verticalWord);
    if (upperVertical == NULL) {
        return NULL;
    }

    int length = strlen(upperVertical);

    //
    Acrostic *acrostic = (Acrostic *)malloc(sizeof(Acrostic));
    if (acrostic == NULL) {
        printf("Σφάλμα: Αδυναμία δέσμευσης μνήμης για την ακροστιχίδα.\n");
        free(upperVertical);
        return NULL;
    }

    // Initializing the acrostic fields
    acrostic->verticalWord = upperVertical;
    acrostic->length = length;
    acrostic->difficulty = difficulty;

    // Memory allocation for horizontal words
    acrostic->horizontalWords = (char **)malloc(length * sizeof(char *));
    if (acrostic->horizontalWords == NULL) {
        printf("Σφάλμα: Αδυναμία δέσμευσης μνήμης για τις οριζόντιες λέξεις.\n");
        free(upperVertical);
        free(acrostic);
        return NULL;
    }

    // Memory allocation for acrostic

    acrostic->displayedAcrostic = (char **)malloc(length * sizeof(char *));
    if (acrostic->displayedAcrostic == NULL) {
        printf("Memory is full\n");
        free(upperVertical);
        free(acrostic->horizontalWords);
        free(acrostic);
        return NULL;
    }

    // Initializing arrays
    for (i = 0; i < length; i++) {
        acrostic->horizontalWords[i] = NULL;
        acrostic->displayedAcrostic[i] = NULL;
    }

    // Finding horizontal words for each letter of the vertical word
    for (i = 0; i < length; i++) {
        char prefix[2] = {upperVertical[i], '\0'};

        // Collection of words starting with the current letter
        wordList **matchingWords = NULL;
        int matchingCount = 0;
        wordList *current = head;

        // First we count how many words match
        while (current != NULL) {
            if (current->word[0] == upperVertical[i]) {
                matchingCount++;
            }
            current = current->next;
        }

        if (matchingCount == 0) {
            printf("No words found that start with '%c'.\n", upperVertical[i]);
            // Using the same letter as a horizontal word
            acrostic->horizontalWords[i] = strdup(prefix);
            continue;
        }

        //
        matchingWords = (wordList **)malloc(matchingCount * sizeof(wordList *));
        if (matchingWords == NULL) {
            printf("Σφάλμα: Αδυναμία δέσμευσης μνήμης για τον πίνακα ταιριαστών λέξεων.\n");
            continue;
        }

        // Collection of matching words
        current = head;
        int index = 0;
        while (current != NULL) {
            if (current->word[0] == upperVertical[i]) {
                matchingWords[index++] = current;
            }
            current = current->next;
        }

        // Word selection based on difficulty level
        int selectedIndex;
        if (difficulty == 0) { // easy
            // Choose a frequent and easy word (short length, large numOfTimes)
            int maxScore = -1;
            selectedIndex = 0;

            for (j = 0; j < matchingCount; j++) {
                int score = matchingWords[j]->numOfTimes - matchingWords[j]->wordLength;
                if (score > maxScore) {
                    maxScore = score;
                    selectedIndex = j;
                }
            }
        } else { // hard
            //Selecting a rare and difficult word (long length, small numOfTimes)
            int maxScore = -1;
            selectedIndex = 0;

            for (j = 0; j < matchingCount; j++) {
                int score = matchingWords[j]->wordLength - matchingWords[j]->numOfTimes;
                if (score > maxScore) {
                    maxScore = score;
                    selectedIndex = j;
                }
            }
        }

        // copy current word
        acrostic->horizontalWords[i] = strdup(matchingWords[selectedIndex]->word);


        free(matchingWords);
    }

    // Create acrostic
    for (i = 0; i < length; i++) {
        if (acrostic->horizontalWords[i] == NULL) {
            continue;
        }

        int wordLength = strlen(acrostic->horizontalWords[i]);

        //
        acrostic->displayedAcrostic[i] = (char *)malloc((wordLength + 1) * sizeof(char));
        if (acrostic->displayedAcrostic[i] == NULL) {
            printf("Σφάλμα: Αδυναμία δέσμευσης μνήμης για την εμφανιζόμενη λέξη.\n");
            continue;
        }

        //
        strcpy(acrostic->displayedAcrostic[i], acrostic->horizontalWords[i]);

        // Decide which letters will be displayed
        int lettersToShow = wordLength / 3;
        if (difficulty == 0) { // easy
            lettersToShow = wordLength / 2;
        }

        if (lettersToShow == 0) {
            lettersToShow = 1;
        }

        // // Create a table to keep track of the letters that will appear
        int *showLetter = (int *)calloc(26, sizeof(int));
        if (showLetter == NULL) {
            printf("Σφάλμα: Αδυναμία δέσμευσης μνήμης για τον πίνακα εμφάνισης γραμμάτων.\n");
            continue;
        }

        // select random letters
        srand(time(NULL) + i);
        for (j = 0; j < lettersToShow; j++) {
            int letterIndex = rand() % 26;
            showLetter[letterIndex] = 1;
        }

        //hide or display letters
        for (j = 0; j < wordLength; j++) {
            int letterIndex = acrostic->horizontalWords[i][j] - 'A';
            if (letterIndex >= 0 && letterIndex < 26) {
                if (showLetter[letterIndex] == 0) {
                    acrostic->displayedAcrostic[i][j] = '_';
                }
            }
        }

        free(showLetter);
    }

    return acrostic;
}

void freeAcrostic(Acrostic* acrostic){
    if(acrostic == NULL){
        return;
    }
    free(acrostic->verticalWord);
    int i = 0;
    for(i=0; i<acrostic->length;i++){
        if(acrostic->horizontalWords[i] != NULL){
            free(acrostic->horizontalWords[i]);
        }
        if(acrostic->displayedAcrostic[i] != NULL){
            free(acrostic->displayedAcrostic[i]);
        }
    }
    free(acrostic->horizontalWords);
    free(acrostic->displayedAcrostic);
    free(acrostic);
}

void displayAcrostic(Acrostic *acrostic){
    if(acrostic == NULL){
        printf("No Acrostic!!!\n");
        return;
    }
    printf("Vertical Acrostic %s\n\n",acrostic->verticalWord);
    int i,j;
    for (i=0; i< acrostic->length; i++){
        if(acrostic->displayedAcrostic[i]==NULL){
            printf("%c _ _ _\n",acrostic->verticalWord[i]);
        }else{
            printf("%c ",acrostic->verticalWord[i]);
            for(j=0;acrostic->displayedAcrostic[i][j] != '\0';j++){
                printf("%c ",acrostic->displayedAcrostic[i][j]);
            }
            printf("\n");
        }
    }
    printf("\nResult\n\n");

    for (i=0; i< acrostic->length; i++){
        if(acrostic->horizontalWords[i]==NULL){
            printf("%c ?\n",acrostic->verticalWord[i]);
        }else{
            printf("%c ",acrostic->verticalWord[i]);
            for(j=0;acrostic->horizontalWords[i][j] != '\0';j++){
                printf("%c \n",acrostic->horizontalWords[i][j]);
            }
            printf("\n");
        }
    }
}

void loadFromFile(char* filename){
    if(filename==NULL){
        printf("File name Error !!!\n");
        return;
    }
    while(isspace(*filename)) filename++;  /// vgazo ta kena apo tin arxi metaxy tou load kai tou onomatos tou arxeioy

    char *fullFileName = (char*)malloc((strlen(filename)+5) *sizeof(char)); ///+5 = ".txt\0"
    if(fullFileName == NULL){
        printf("Memory is full\n");
        return;
    }
    strcpy(fullFileName,filename);
    strcat(fullFileName,".txt");

    FILE *file = fopen(fullFileName,"r");
    if(file == NULL){
        printf("Read file Error!!!\n");
        free(fullFileName);
        return;
    }
    char buffer[1024];
    int totalInserted = 0;
    while(fgets(buffer, sizeof(buffer), file)!=NULL){
        buffer[strcspn(buffer,"\n")] = '\0';
        totalInserted += processText(buffer,0);
    }
    printf("From file %s inserted %d new words\n",fullFileName,totalInserted);
    fclose(file);
    free(fullFileName);
}

void saveAcrostic(char* filename, Acrostic* acrostic){
    if(filename==NULL){
        printf("File name Error !!!\n");
        return;
    }
    if(acrostic==NULL){
        printf("Error no Acrostic!!!\n");
        return;
    }
    int i,j;
    while(isspace(*filename)) filename++;  /// vgazo ta kena apo tin arxi metaxy tou save: kai tou onomatos tou arxeioy

    char *fullFileName = (char*)malloc((strlen(filename)+5) *sizeof(char)); ///+5 = ".txt\0"
    if(fullFileName == NULL){
        printf("Memory is full\n");
        return;
    }
    strcpy(fullFileName,filename);
    strcat(fullFileName,".txt");

    FILE *file = fopen(fullFileName,"w");
    if(file == NULL){
        printf("Write file Error!!!\n");
        free(fullFileName);
        return;
    }

    fprintf(file,"Vertical word Acrostic '%s' : \n\n",acrostic->verticalWord);
    for (i=0; i< acrostic->length; i++){
        if(acrostic->displayedAcrostic[i]==NULL){
            fprintf(file,"%c _ _ _\n",acrostic->verticalWord[i]);
        }else{
            fprintf(file,"%c ",acrostic->verticalWord[i]);
            for(j=0;acrostic->displayedAcrostic[i][j] != '\0';j++){
                fprintf(file,"%c ",acrostic->displayedAcrostic[i][j]);
            }
            fprintf(file,"\n");
        }
    }
    fprintf(file,"\nResult\n\n");

    for (i=0; i< acrostic->length; i++){
        if(acrostic->horizontalWords[i]==NULL){
            fprintf(file,"%c ?\n",acrostic->verticalWord[i]);
        }else{
            fprintf(file,"%c ",acrostic->verticalWord[i]);
            for(j=0;acrostic->horizontalWords[i][j] != '\0';j++){
                fprintf(file,"%c \n",acrostic->horizontalWords[i][j]);
            }
            fprintf(file,"\n");
        }
    }
    printf("Acrostic saved successfully!!!\n");

    fclose(file);
    free(fullFileName);
}


void createCommand(char* input){
    if(input==NULL){
        printf("Input Error !!!\n");
        return;
    }
    while(isspace(*input)) input++;  /// vgazo ta kena apo tin arxi metaxy tou load kai tou onomatos tou arxeioy

    char *copy = strdup(input);
    if(copy==NULL){
        printf("Memory full!!!\n");
        return;
    }
    char *word = strtok(copy," \t\r\n\f\v");
    char *difficultyStr = strtok(NULL," \t\r\n\f\v");
    if(word==NULL || difficultyStr==NULL){
        printf("Invalid command!!! Use : create: [word] [difficulty] \n");
        free(copy);
        return;
    }
    int difficulty;
    if(strcmp(difficultyStr, "easy")==0) difficulty =0;
    else if(strcmp(difficultyStr, "hard")==0) difficulty =1;
    else{
        printf("Invalid difficulty level!!! Use : easy or hard \n");
        free(copy);
        return;
    }
    if(lastAcrostic != NULL){
        free(lastAcrostic);
        lastAcrostic = NULL;
    }
    lastAcrostic = createAcrostic(word,difficulty);
    if(lastAcrostic == NULL){
        printf("Creation Error!!! Invalid Acrostic\n");
        free(copy);
        return;
    }
    displayAcrostic(lastAcrostic);
    free(copy);
}
