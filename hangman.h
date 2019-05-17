#define WORDLIST_FILENAME "words.txt"
#define PLAYER_HIGHLIGHTS "records.txt"

struct player {
    char name[15];
    int score;
    int numGuessWords;
    char time[26];
};

void printIntro(struct player *);
bool ending(struct player);
void selectDifficulty(int *ptrDif, struct player *);
void lowestDifficulty(const char secret[], char letters_guessed[], char guessed_word[]);
void writeRecords(struct player);
int useMagic(const char secret[], char copy_gw[], char letters_guessed[]);

/**
 * Function detects, whether player guessed the secret word
 * Based on the letters player used for guessing, this function detects,
 * if it is possible to construct (and guess) the secret word from this set.
 * If it is possible, function returns 1. If not, returns 0.
 * @param secret the secret word lowercase
 * @param letters_guessed the lowercase letters player already used in his guessing
 * @return 1, if word is guess; 0 otherwise.
 */
int is_word_guessed(const char secret[], const char letters_guessed[]);


/**
 * Function returns string representing the guessed word
 * This string contains visible letters, which were guessed successfuly
 * by player at their correct positions. If there are still some hidden
 * letters, the character '_' will be used for their representation.
 * @param secret the secret word lowercase
 * @param letters_guessed the lowercase letters player already used in his guessing
 * @param guessed_word the constructed string as result of this function (output parameter)
 */
bool get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]);


/**
 * Returns all letters, which were not used for guessing
 * Function returns set of lowercase letters sorted in alphabetical order. This
 * set will contain all letters from the alphabet, but it ommits those, which
 * were already guessed.
 * @param letters_guessed the lowercase letters player already used in his guessing
 * @param available_letters set of lowercase not yet used letters
 */
void get_available_letters(const char letters_guessed[], char available_letters[]); 


/**
 * Starts interactive hangman game
 *
 * @param secret the secret word lowercase
 */
void hangman(const char secret[], int *ptrScore, int *ptrAttempt, int *ptrMagic, const int *dif);


/**
 * Returns the random word
 * Function opens the file with all the words and reads randomly one of them
 * into the buffer pointed by secret. Word consists of lowercase letters.
 * If there is some error with reading the file, 1 is returned.
 * Don't forget to initialize the random numbers in your main() function will srand() call!
 * Otherwise (everything is ok), 0 is returned.
 * @param secret buffer, where random word will be read
 * @return status code
 */
int get_word(char secret[]);

