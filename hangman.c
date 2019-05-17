#include "hangman.h"

int get_word(char secret[]){
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if( fp == NULL ){
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do{
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if( result != EOF )
            break;
    }while(1);

    fclose(fp);

    return 0;
}

void hangman(const char secret[], int *ptrScore, int *ptrAttempt, int *ptrMagic, const int *dif){
    printf("I am thinking of a word that is %ld letters long.\n", strlen(secret));
    char guessed_word[strlen(secret)];
    guessed_word[0] = '\0';
    char copy_gw[strlen(secret)];
    copy_gw[0] = '\0';
    char list_of_letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0' };
    char available_letters[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '\0'};
    available_letters[strlen(available_letters)] = '\0';
    char letters_guessed[256];
    letters_guessed[0] = '\0';
    char buffer[20];
    buffer[0] = '\0';
    int c_c_l = 0;
    for (int i = 0; i < strlen(secret); ++i){
        guessed_word[i] = '_';
        copy_gw[i] = '_';
    }
    guessed_word[strlen(secret)] = '\0';
    copy_gw[strlen(secret)] = '\0';
    // the part below containt the code which is neccesary for difficulty 1
    bool help = true;
    if(*dif == 1){
        lowestDifficulty(secret, letters_guessed, guessed_word);
        get_guessed_word(secret, letters_guessed, guessed_word);
        get_available_letters(letters_guessed, available_letters);
    }

    do{
BEGIN:  ;
        int common_l = 0;
        buffer[0] = '\0';
        printf("-------------\n");
        if(is_word_guessed(secret, letters_guessed) == 1){
            (*ptrScore)++;
            printf("Congratulations, you guessed the word!\n");
            printf("-------------\n");
            return;
        }
        if(*dif != 3){
            printf("You have %d guesses and %d magics left.\n", *ptrAttempt, *ptrMagic);
        }
        else{
            printf("You have %d guesses left.\n", *ptrAttempt);
        }
        printf("Available letters: %s\n", available_letters); 
        if(*dif == 1 && help == true){
            printf("The known letters are: " );
            for(int i = 0; i < strlen(guessed_word); i++){
                printf(" %c", guessed_word[i]);
            }
            printf("\n");
            help = false;
        }
        printf("Please guess a letter: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer)-1] = '\0';
        if(buffer[0] == '/'){
            switch (buffer[1]){
                case 'm': {
                    if(*ptrMagic == 0){
                        printf("Sorry. Think by yourself.\n");
                        goto BEGIN;
                    }
                    useMagic(secret, copy_gw, letters_guessed);
                    (*ptrMagic)--;
                    get_guessed_word(secret, letters_guessed, guessed_word);
                    get_available_letters(letters_guessed, available_letters);
                    printf("A little bit of magic:");
                    for(int i = 0; i < strlen(guessed_word); i++){
                        printf(" %c", guessed_word[i]);
                    }
                    printf("\n");            
                    goto BEGIN;
                }
                case 'q': {
                    const char goodbye[] = {"Good Bye!"};
                    int lengthGB = (int)strlen(goodbye);
                    for(int i = 0; i < lengthGB; i++){
                        printf("%c ", goodbye[i]);
                    }
                    printf("\n");
                    //usleep(2 * 100000);
                    exit(0);
                }
                default :{
                    break;
                }
            }
        }
        if(strlen(buffer) != 1 && strlen(buffer) != strlen(secret)){
            printf("Follow game rules and try again.\n");
            goto BEGIN;
        }
        for(int i = 0; i < strlen(buffer); i++){
            buffer[i] = tolower(buffer[i]);
        }
        for (int i = 0; i < strlen(letters_guessed); i++){
            for(int q = 0; q < strlen(buffer); q++){
                if((letters_guessed[i] == buffer[q]) && strlen(buffer) != strlen(secret)){
                    printf("Oops! You've already guessed that letter:");
                    for(int i = 0; i < strlen(guessed_word); i++){
                        printf(" %c", guessed_word[i]);
                    }
                    buffer[0] = '\0';
                    printf("\n");
                    goto A;
                }
            }
        }
        
        strcat(letters_guessed, buffer);
        bool result = get_guessed_word(secret, letters_guessed, guessed_word);
        get_available_letters(letters_guessed, available_letters);
        for(int i = 0; i < strlen(letters_guessed); i++){
            c_c_l = common_l;
            for(int q = 0; q < strlen(list_of_letters); q++){
                if((letters_guessed[i] == '\0') || letters_guessed[i] == list_of_letters[q]){
                    common_l++;
                }
            }
            if(common_l == c_c_l){
                printf("Oops! '%c' is not a valid letter:", letters_guessed[i]);
                letters_guessed[i] = '\0';
                buffer[0] = '\0';
                for(int i = 0; i < strlen(guessed_word); i++){
                    printf(" %c", guessed_word[i]);
                }
                printf("\n");
                goto A;
            }

        }
        if(result == false){
            printf("Oops! That letter is not in my word:");
             for(int i = 0; i < strlen(guessed_word); i++){
                    printf(" %c", guessed_word[i]);
                    buffer[0] = '\0';
            }
            printf("\n");
            (*ptrAttempt)--;
            goto A;
        }
        else{
            if((is_word_guessed(secret, letters_guessed) == 0) && (strlen(secret) == strlen(buffer)) && strcmp(secret, buffer) == 0){
                (*ptrScore)++;
                printf("Congratulations, you guessed it!\n");
                goto END;
            }
            else if((is_word_guessed(secret, letters_guessed) == 0) && (strlen(secret) == strlen(buffer)) && strcmp(secret, buffer) != 0){
                printf("Sorry, bad guess. The word was %s.\n", secret);
                goto END;
            }
            (void *)memcpy(copy_gw, guessed_word, strlen(secret));
            printf("Good guess:");
            (*ptrScore)++;
            for(int i = 0; i < strlen(guessed_word); i++){
                    printf(" %c", guessed_word[i]);
            }
            printf("\n");            
        }

A:      ;
    }while(*ptrAttempt > 0);
    printf("-------------\n");
    printf("Sorry, you ran out of guesses. The word was %s.\n", secret);
END: ;
}

int is_word_guessed(const char secret[], const char letters_guessed[]){
    int n_c_letters = 0;
    for(int i = 0; i < strlen(secret); i++){
        for(int q = 0; q < strlen(letters_guessed); q++){
            if(secret[i] == letters_guessed[q]){
                n_c_letters++;
            }
        }
    }
    if(n_c_letters == strlen(secret))
        return 1;
    return 0;
}

void get_available_letters(const char letters_guessed[], char available_letters[]){

    int len_al = strlen(available_letters);
    int len_lg = strlen(letters_guessed);
    for(int q = 0; q < len_lg; q++){
        for (int i = 0; i < len_al; ++i){
            if(available_letters[i] == letters_guessed[q] && letters_guessed[q] != '\0' && available_letters[i] != '\0'){
                for(int j = i; j < len_al; j++){
                    available_letters[j] = available_letters[j+1];
                }
                len_al--;
                i--;
            }
        }
        available_letters[strlen(available_letters)] = '\0';
    }
} 

bool get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]){
    if(letters_guessed[strlen(letters_guessed)-1] == '\0'){
    	for (int i = 0; i < strlen(secret); ++i){
        	guessed_word[i] = '_';
  		}
        guessed_word[strlen(secret)] = '\0';
        return false;
    }
    int coef = 0;
    int length_s = strlen(secret);
    int length_l = strlen(letters_guessed);
    for(int i = 0; i < length_s; i++){
        for(int q = 0; q < length_l; q++){
            if(secret[i] == letters_guessed[q]){
                guessed_word[i] = secret[i];
                break;
            }
        }
    }
    for(int j = 0; j < length_s; j++){
        if(letters_guessed[length_l-1] == secret[j]){
            coef++;
            break;
        }
    }
    guessed_word[strlen(secret)] = '\0';
    return (coef != 0) ? true : false;

}

void printIntro(struct player *plptr){
    printf("###################################\n");
    printf("#---------------------------------#\n");
    char logo[] = {"#--Welcome to the game, Hangman!--#\n"};
    printf("%s", logo);
    printf("#---------------------------------#\n");
    printf("###################################\n\n");
    printf("Please, enter your name: ");
    scanf("%s", plptr->name);
    time_t now;
    time (&now);
    strcpy(plptr->time, ctime(&now));

    //clear input buffer
    char c = getchar();

    printf("------------\n");
}

int useMagic(const char secret[], char copy_gw[], char letters_guessed[]){
    printf("Enter the number you'd like to reveal: ");
    int letter;
    int numLetters = strlen(secret);
E:  scanf("%d", &letter);
    if(letter > numLetters || letter < 1){
        printf("Invalid number! Try again: ");
        goto E;
    }
    if(secret[letter-1] == copy_gw[letter-1]){
        printf("You already know that letter. Try again: ");
        goto E;
    }
    char answer[1];
    answer[0] = secret[letter-1];
    strcat(letters_guessed, answer);
    //clear input buffer
    char c = getchar();
}

bool ending(struct player player){
    printf("-------------\n");
    printf("Your score was %d and you guessed %d words.\n", player.score, player.numGuessWords);
    printf("Would you like to play again? [Y/N]: ");
    char shortAnswer;
CHOISE:    scanf("%c", &shortAnswer);
    switch (shortAnswer) {
        case 'y': case 'Y': {
            printf("Hmm... %s, you are the brave person.\n", player.name);
            printf("-------------\n");
            return true;
        }
        case 'n': case 'N': {
            printf("Loser.\n");
            return false;
        }
        default: {
            printf("Please, answer the question. Answer is: ");
            goto CHOISE;
        }
    }
 }

void selectDifficulty(int *ptrDif, struct player *ptrPlayer){
    printf("Select the difficulty [1/2/3]: ");
    char answer = getchar();
    printf("-------------\n");
    if(answer != '1' && answer != '2' && answer != '3'){
        printf("Invalid input! ");
        char c = getchar();
        selectDifficulty(ptrDif, ptrPlayer);
    }
    else{
        *ptrDif = answer -'0';
        char c = getchar();
        printf("So, %s, good luck! It won't help you.\n", ptrPlayer->name);
    }

}

void lowestDifficulty(const char secret[], char letters_guessed[], char guessed_word[]){
    int upper = strlen(secret);
    int lower = 1;
    int loop;
    if(upper >= 8)
        loop = 2;
    else
        loop = 1;
    for(int i = 0; i < loop; i++){
        int randInt = (rand() % (upper - lower + 1)) + lower;
        char letter[2];
        letter[0] = secret[randInt-1];
        strcat(letters_guessed, letter);
    }
}

void writeRecords(struct player player){
    FILE *ph = fopen(PLAYER_HIGHLIGHTS, "a+");
    fprintf(ph, "Name: %s;\nScore: %d;\nNumber of guessed words: %d;\nGame session: %s\n", player.name, player.score, player.numGuessWords, player.time);
    fclose(ph);
}