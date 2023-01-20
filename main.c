#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()
#include <math.h>

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
#define WORDS_FILE_NAME "wordsLarge.txt"
//#define WORDS_FILE_NAME  "./wordsTiny.txt"
#define true 1
#define false 0

typedef struct wordCount{
  int score;
  char word[WORD_LENGTH + 1];
}wordCountStruct;

int countWordsFromFile(char fileName[])
{
    int wordCount = 0;
    FILE *inFilePtr = fopen(fileName, "r");
    assert( inFilePtr != NULL);               // Ensure file open worked correctly
    char inputString[81];
    if (inFilePtr == NULL) {
        printf("Error: could not open %s for reading\n", fileName);
        exit(-1);
    }
    while (fscanf(inFilePtr, "%s", inputString) != EOF) {
        wordCount++;
    }
    fclose(inFilePtr);
    return wordCount;
}

void readWordsFromFile(
            char fileName[],        // Filename we'll read from
            wordCountStruct **words, // Array of words where we'll store words we read from file
            int *wordCount)         // How many words.  Gets updated here and returned
{
    *wordCount = countWordsFromFile(fileName);
    *words = (wordCountStruct *)malloc(sizeof(wordCountStruct) * (*wordCount));

    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert( inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[ 6];
    int i = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( (*words)[i].word, inputString);
        (*words)[i].score = 0;
        i++;
    }

    // Close the file
    fclose( inFilePtr);
} // end readWordsFromFile(..)

void printWords(wordCountStruct *words, int wordCount)
{   
    for(int i = 0; i < wordCount; i++)
    {
        printf("%d. %s %d\n", i + 1, words[i].word, words[i].score);
    }
}

void clearScores(wordCountStruct **words, int wordCount)
{
    for(int i = 0; i < wordCount; i++)
    {
        (*words)[i].score = 0;
    }
}

int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        return strcmp( ((wordCountStruct *)a)->word,  ((wordCountStruct *)b)->word );
    }
} //end compareFunction(..)

void countWords(wordCountStruct *words, int *wordCount)
{
    int count = 0;
    while(words[count].score >= 0)
        count++;
    *wordCount = count;
}


void eliminateWordsWithoutLetterInPosition(
    wordCountStruct *words[],  // Array of all words
    int wordCount,               // How many current words there are
    char letterToMatch,          // Letter that should be there
    int indexPosition,           // Index position of the letter that should be there 
    int mode)
    {
    // For each word in allWords 
    for( int i=0; i<wordCount; i++) {
    // Check if the letter in the specified index position matches
        if( ((*words)[ i].word[ indexPosition] != letterToMatch && mode == 1)
        || ((*words)[ i].word[ indexPosition] == letterToMatch && mode == 2)) {
            // Mark the word to be deleted by setting its score to -1
            (*words)[ i].score = -1;
        }
        else if((*words)[i].score != -1){
            if(mode == 1)
                (*words)[i].score += 3;
            if(mode == 2)
                (*words)[i].score += 1;
        }
        else if(mode == 3 && ((*words)[ i].word[ indexPosition] == letterToMatch))
        {
            (*words)[ i].score = -1;
        }
    }
}

void getWord(char **secretWord, wordCountStruct *words, int wordCount, int *running)
{
    char userInput[ 81];                // Used for menu input of secret word
    // Reset secret Word
    strcpy(*secretWord, "");

    // Display prompt
    printf("-----------------------------------------------------------\n");
    printf("\n");
    printf("Enter a secret word or just r to choose one at random: ");
    scanf(" %s", userInput);
    // Eliminate the return character at end or userInput if it is there
    int length = (int) strlen( userInput);
    if( userInput[ length] != '\0') {
        userInput[ length] = '\0';
    }
    strcpy( *secretWord, userInput);   // Store the secret word from user input
            
    // If exit command issued
    if(strcmp(userInput,"exit") == 0)
    {
        *running = false;
        return;
    }

    // If input was 'r' then choose a word at random.
    if( strlen( *secretWord) <= 4) {
        // Randomly select a secret word to be guessed.
        int randomIndex = rand() % wordCount;
        strcpy( *secretWord, words[ randomIndex].word);
    }
}

int makeGuess(char *guess, char *secretWord)
{
    if(strcmp(guess, secretWord) == 0)
    {
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(
        wordCountStruct *allWords[],    // Array of all the words
        int *wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
    char computerGuess[ 6];  // Allocate space for the computer guess
    int tempWordCount = *wordCount;
    
    printf("Trying to find secret word: \n");
    // Display secret word with a space between letters, to match the guess words below.
    printf("       ");
    for( int i=0; i<WORD_LENGTH; i++) {
        printf("%c ", secretWord[ i]);
    }
    printf("\n");
    printf("\n");
    
    // Loop until the word is found

    // Compare guess with secret word 1st

    // Remove characters that don't show up
    // Score based on comparison s
    // Choose a word based on score
    // Compare guess with secret word
    int guessNumber = 1;
    while( true) {
        if(guessNumber == 1)
        {
            strcpy(computerGuess, "letch");
            //strcpy(computerGuess, "soare");
        }
        else if (guessNumber == 2)
        {
            strcpy(computerGuess, "yarns");
        }
        else
        {
            strcpy(computerGuess, (*allWords)[0].word);
            //printf("%s",(*allWords)[0].word );
        }

        printf("%d. ", guessNumber);

        // copy of words
        char secretWordCopy[6];
        strcpy(secretWordCopy, secretWord);
        char guessCopy[6];
        strcpy(guessCopy, computerGuess); 

        // array indicating any indices that match anything in secretword
        // 0 means the index was not found to have a match
        int correct[] = {0, 0, 0, 0, 0};

        // For each letter index in secretWord see if it matches that letter in computerGuess 
        for( int i=0; i<5; i++) {
            if( secretWordCopy[ i] == guessCopy[ i]) {
                // Display computerGuess[ i] in upper-case
                //correct[i]++;
                printf("%c ", toupper(computerGuess[i]));
                eliminateWordsWithoutLetterInPosition( allWords, *wordCount, computerGuess[ i], i, 1);
                guessCopy[i] = ' ';
                secretWordCopy[i] = ' ';
            }
            else
            {
                printf("%c ", computerGuess[i]);
            }
        }
        printf("\n");
        for(int i = 0; i < floor(log10((double)guessNumber) + 1) + 1; i++)
        {
            printf(" ");
        }

        // For each letter in secretWord see if it matches another letter in computerGuess 
        for( int i=0; i<5; i++) {
            int printed = false;
            for(int j = 0; j < 5; j++)
            {
                if( secretWordCopy[ j] == guessCopy[ i] && i != j && guessCopy[i] != ' ') {
                    //correct[i]++;
                    printed = true;
                    eliminateWordsWithoutLetterInPosition( allWords, *wordCount, computerGuess[ i], i, 2);
                    secretWordCopy[j] = ' ';
                }
            }
            if(printed == false)
            {
                printf("  ");
            }
            else
            {
                printf(" *");
            }
        }
        printf("\n");   

        //Make array that keeps track of letters not in list then eliminate words based on that
        //increase scores of more common words in positions than other ones

        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                if(computerGuess[i] == secretWord[j])
                {
                    correct[i]++;
                }
            }
        }

        //For every letter that didn't appear in secretWord, eliminate them in words
        for(int i = 0; i < 5; i++)
        {
            if(correct[i] == 0)
            {
                for(int j = 0; j < 5; j++)
                {
                    eliminateWordsWithoutLetterInPosition( allWords, *wordCount, computerGuess[ i], j, 2);
                }
            }
        }

        if(makeGuess(computerGuess, secretWord) == true)
        {
            break;
        }
        // Our guess is incorrect
        else if( guessNumber > 2)
        {
            (*allWords)[0].score = -1;
        }

        qsort(*allWords, *wordCount, sizeof(wordCountStruct), compareFunction);
        
        countWords(*allWords, wordCount);

        // If by this point all words in our list don't work, then the entered word is not a word in our list
        if(*wordCount == 0 && (*allWords)[0].score == -1)
        {
          printf("The word you entered is not a word in the list.\n");
          break;
        }
      
        // Update guess number
        guessNumber++;
    } //end for( int i...)
    *wordCount = tempWordCount;
} //end findSecretWord



int main()
{
    // Wordle variables
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    wordCountStruct *words;
    int wordCount = 0;

    char *secretWord = (char *)malloc(sizeof(char) * (WORD_LENGTH + 1));

    readWordsFromFile(wordsFileName, &words, &wordCount);
    //printWords(words, wordCount);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);
    printf("You can type 'exit' to exit the program.\n ");

    int running = true;

    while(running)
    {
        getWord(&secretWord, words, wordCount, &running);
        if(!running)
            break;
        findSecretWord(&words, &wordCount, secretWord);
        //printWords(words, wordCount);
        clearScores(&words, wordCount);
    }
    
    free(secretWord);
    free(words);
    return 0;
}