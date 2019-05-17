#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "hangman.c" 

int main(void){
	struct player player;
	srand(time(NULL));
	printIntro(&player);
START: 	;
	int dif = 0;
	int *ptrDif = &dif;
	selectDifficulty(ptrDif, &player);
	char secret[15];
	int upper = 3;
	int lower = 1;
	int score = 0;
	int magic = 3;
	int *ptrMagic = &magic;
	int wordsGuessed = 0;
	int counter = 0;
	int *ptrScore = &score;
	int attempts = 9;
	int *ptrAttempt = &attempts;
	do{
		get_word(secret);
		//printf("%s\n", secret);
		hangman(secret, ptrScore, ptrAttempt, ptrMagic, ptrDif);
		if(attempts != 0){
			int bonus = (int) strlen(secret) / 2;
			counter++;
			if(attempts <= 15){
				printf("You achieved %d more attempts!\n", bonus);
				attempts += bonus;
			}
			else{
				printf("You have too many free guesses...\n");
			}
			//checks whether the magic will be present
			if(dif != 3){
				int r = rand();    
				if(r % 3 == 0){
					int growth = (rand() % (upper - lower + 1)) + lower;
					printf("You gained %d more magics!\n", growth);
					magic += growth;
				}
			}
			printf("Hmm... You won't guess the next word.\n");
			wordsGuessed++;
			printf("-------------\n");
		}
	} while(attempts > 0);
	player.numGuessWords = wordsGuessed;
	player.score = score * dif;
	writeRecords(player);
	if(ending(player) == true){
		goto START;
	}
	return 0;
} 