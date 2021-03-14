#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define LIMIT_CHANCES 5

// define global variables
char secretWord[20];
char kicks[26];
int attempts = 0;

void printWinnerMessage() {
  printf("\n\n");
  printf("Parabéns Você Venceu!\n\n");
  printf("  \\'-=======-'/    \n");
  printf("  _|   .=.   |_     \n");
  printf(" ((|  {{1}}  |))    \n");
  printf("  \\|   /|\\   |/   \n");
  printf("   \\__ '`' __/     \n");
  printf("     _`) (`_        \n");
  printf("   _/_______\\_     \n");
  printf("  _/_________\\_    \n");
  printf("\n\n");
}

void printLooserMessage() {
  printf("\n              YOU LOOSE!                                \n\n");
  printf(" ________,           _       __           _               \n");
  printf(" |/      |          | |     / _|         | |              \n");
  printf(" |      (_)       __| | ___| |_ ___  __ _| |_             \n");
  printf(" |      \\|/      / _` |/ _ \\  _/ _ \\/ _` | __|         \n");
  printf(" |       |      | (_| |  __/ ||  __/ (_| | |_             \n");
  printf(" |      / \\      \\__,_|\\___|_| \\___|\\__,_|\\__|      \n");
  printf("_|___                                                     \n\n");
}

/** 
 * Ask user for add a new word to the database, and if yes, add it
 */
void newWord() {
  char userChoice;

  printf("Voce desejaria adicionar uma palavra ao nosso banco de dados? (S/N)");
  scanf(" %c", &userChoice);

  if (userChoice == 'S' || userChoice == 's') {
    char newWord[20];
    printf("Qual a nova palavra: ");
    scanf("%s", newWord);

    FILE *wordFile;
    wordFile = fopen("words.txt", "r+"); // Open file

    // ensure the database was successfully "conected"
    if (wordFile == 0) {
      printf("Desculpe houve um problema com o banco de dados.");
      exit(1);
    }

    // store the amount of words (first line we store an integer that represents
    // the amout of words registered).
    int amountOfWords;
    fscanf(wordFile, "%d", &amountOfWords);
    amountOfWords++; // add 1 to the count

    // return the file reader head to the start
    fseek(wordFile, 0, SEEK_SET);
    // overwrite the old amount of words to the brand new added more 1
    fprintf(wordFile, "%d", amountOfWords);

    // bring the file reader header to the end of the file
    fseek(wordFile, 0, SEEK_END);
    // add the new word
    fprintf(wordFile, "\n%s", newWord);

    // close file
    fclose(wordFile);
  }
  else {
    printf("Obrigado por utilizar nosso programa! até mais!\n");
  }
}

/**
 * Select a random word from database
 */
void randomWord() {
  FILE *wordsFile;
  wordsFile = fopen("words.txt", "r");

  if (wordsFile == 0) {
    printf("Desculpe banco de dados não disponível!");
    exit(1);
  }

  srand(time(0));

  int totalLinesNumber;
  fscanf(wordsFile, "%d", &totalLinesNumber);

  int randomLine = rand() % totalLinesNumber;

  for (int i = 0; i <= randomLine; i++) {
    fscanf(wordsFile, "%s", secretWord);
  }

  fclose(wordsFile);
}

/**
 * Print the game's banner
 */
void printBan() {
  printf("***************\n");
  printf("* Gibbet Game *\n");
  printf("***************\n\n");
}

/**
 * Verify if a given letter was already kicked and hitted
 * @param letter string secret word letter for comparison
 */
int alreadyKicked(char letter) {
  int found = 0;

  for (int letterIndex = 0; letterIndex < attempts; letterIndex++) {
    if (kicks[letterIndex] == letter) {
      found = 1;
      break;
    }
  }

  return found;
}

int userErrors() {
  int userMissedKicks = 0;

  for (int kickIndex = 0; kickIndex < attempts; kickIndex++) {
    int foundMatch = 0;

    for (int secretLetterIndex = 0; secretLetterIndex < strlen(secretWord); secretLetterIndex++) {
      if (kicks[kickIndex] == secretWord[secretLetterIndex]) {
        foundMatch = 1;
        break;
      }
    }

    if (!foundMatch) userMissedKicks++;
  }

  return userMissedKicks;
}

/**
 * Dinamically draws the gibbet and show the discovered letters, show the current attempts
 */
void drawGibbet() {
  printf("Voce ja deu %d chutes.\n", attempts);

  int errors = userErrors();

  printf("  ________,   \n");
  printf("  |/      |   \n");

  if (errors >= 1) {
    printf("  |      (_)  \n");
  } else {
    printf("  |           \n");
  }

  if (errors >= 2) {
    printf("  |      \\|/  \n");
  } else {
    printf("  |           \n");
  }

  if (errors >= 3) {
    printf("  |       |   \n"); // 3 errors
  } else {
    printf("  |           \n"); // 3 errors
  }

  if (errors == 4) {
    printf("  |      /    \n");
  } else  if (errors >= 5){
    printf("  |      / \\ \n");
  } else {
    printf("  |           \n");
  }

  printf("  |           \n");
  printf(" _|___        \n");
  
  for (int index = 0; index < strlen(secretWord); index++) {
    char currentSecretLetter = secretWord[index];

    int found = alreadyKicked(currentSecretLetter);

    if (found) {
      printf("%c ", currentSecretLetter);
    }
    else {
      printf("_ ");
    }
  }

  printf("\n");
}

/**
 * Scan, store and contabilize new kick.
 */
void kick() {
  char guess; 
  printf("Seu chute: ");
  scanf(" %c", &guess);

  kicks[attempts] = guess;
  attempts++;
}

/**
 * Verify if player has won
 * Searches for secret word letters not kicked yet
 */
int won() {
  for (int index = 0; index < strlen(secretWord); index++) {
    if (!alreadyKicked(secretWord[index])) {
      return 0;
    }
  }

  printWinnerMessage();

  return 1;
}

/**
 * Verify if user is out of chances
 * Search and count for kicks that aren't from secret word
 * if exceds the amount of chances, user is hanged
 */
int hanged() {
  if (userErrors() >= LIMIT_CHANCES) {
    printf("\n\nA PALAVRA ERA %s!\n\n", secretWord);
    printLooserMessage();
    return 1;
  }

  return 0;
}

int main() {
  randomWord(); // generate and save a new random word
  printBan(); // print greet banner

  do {
    drawGibbet();
    kick(); 
  } while (!won() && !hanged());

  newWord();
}