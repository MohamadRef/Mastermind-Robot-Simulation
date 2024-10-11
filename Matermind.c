#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define CODE_LENGTH 6
#define MAX_GUESSES 100

// Function to check if the input string represents a valid integer
bool isInteger(const char *inputStr) {
    while (*inputStr) {
        if (!isdigit(*inputStr)) {
            return false;
        }
        inputStr++;
    }
    return true;
}

// Function to evaluate the user's guess against the target number
void evaluate_guess(const int *userGuess, const int *targetNumber, int *exactMatches, int *nearMatches) {
    *exactMatches = 0;
    *nearMatches = 0;

    bool guessChecked[CODE_LENGTH] = {false};
    bool answerChecked[CODE_LENGTH] = {false};

    // First pass: Check for exact matches
    for (int i = 0; i < CODE_LENGTH; ++i) {
        if (userGuess[i] == targetNumber[i]) {
            (*exactMatches)++;
            guessChecked[i] = true;
            answerChecked[i] = true;
        }
    }

    // Second pass: Check for near matches
    for (int i = 0; i < CODE_LENGTH; ++i) {
        if (!guessChecked[i]) {
            for (int j = 0; j < CODE_LENGTH; ++j) {
                if (!answerChecked[j] && userGuess[i] == targetNumber[j]) {
                    (*nearMatches)++;
                    guessChecked[i] = true;
                    answerChecked[j] = true;
                    break;
                }
            }
        }
    }
}

// Function to print the history of guesses and results
void print_guess_history(const int **historyGuesses, const int *matchesExact, const int *matchesNear, int guessesCount) {
    for (int i = 0; i < guessesCount; ++i) {
        printf("Guess %d: ", i + 1);
        for (int j = 0; j < CODE_LENGTH; ++j) {
            printf("%d ", historyGuesses[i][j]);
        }
        printf("- %d correct, %d partially correct\n", matchesExact[i], matchesNear[i]);
    }
}

// Function to read a user's guess input
void read_guess(int *guessInput) {
    char ch;
    int countDigit = 0;

    printf("\nEnter your guess (6 digits, 0-9): ");
    while (countDigit < CODE_LENGTH) {
        ch = getchar();
        if (isdigit(ch)) {
            guessInput[countDigit++] = ch - '0'; // Convert char to int
        } else if (isspace(ch)) {
            continue; // Ignore spaces and tabs
        } else if (ch == '\n') {
            printf("You need to enter %d more digits to complete your guess\n", CODE_LENGTH - countDigit);
        } else {
            printf("ERROR: Input contains non-digit characters.\n");
            // Clear remaining characters in the stream
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Please enter your guess (6 digits, 0-9): ");
            countDigit = 0; // Reset digit count
        }
    }
    // Clear any remaining characters in the input buffer
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main() {
    char userInput[100];
    int gameSeed;

    // Prompt user for a seed value
    printf("Enter the integer value of the seed for the game: ");
    while (true) {
        scanf("%s", userInput);

        // Validate seed input
        if (!isInteger(userInput)) {
            printf("Invalid input. Please enter an integer as the seed: ");
            continue; // Restart the loop to prompt for input again
        }

        // Convert input to integer
        gameSeed = atoi(userInput);
        break; // Exit the loop if input is valid
    }

    // Clear input buffer
    while (getchar() != '\n');

    // Allocate memory for secret code
    int *secretCode = malloc(CODE_LENGTH * sizeof(int));
    if (secretCode == NULL) {
        fprintf(stderr, "Memory allocation failed. Exiting program.\n");
        return 1;
    }

    // Generate secret code using the provided seed
    srand(gameSeed);
    for (int i = 0; i < CODE_LENGTH; ++i) {
        secretCode[i] = rand() % 10;
    }

    printf("For each turn, enter 6 digits (0-9).\n");
    printf("Spaces or tabs in your response will be ignored.\n");

    // Allocate memory for user guesses and results
    int **userGuesses = malloc(MAX_GUESSES * sizeof(int *));
    int *correctAnswers = malloc(MAX_GUESSES * sizeof(int));
    int *partialCorrectAnswers = malloc(MAX_GUESSES * sizeof(int));
    int totalGuesses = 0;

    while (totalGuesses < MAX_GUESSES) {
        int *currentGuess = malloc(CODE_LENGTH * sizeof(int));
        read_guess(currentGuess);

        userGuesses[totalGuesses] = currentGuess;
        evaluate_guess(currentGuess, secretCode, &correctAnswers[totalGuesses], &partialCorrectAnswers[totalGuesses]);

        // Check for a win
        if (correctAnswers[totalGuesses] == CODE_LENGTH) {
            printf("YOU DID IT!!\n");
            print_guess_history((const int **)userGuesses, correctAnswers, partialCorrectAnswers, totalGuesses + 1);
            break;
        }

        // Print guess history
        print_guess_history((const int **)userGuesses, correctAnswers, partialCorrectAnswers, totalGuesses + 1);
        totalGuesses++;
    }

    // Check if maximum guesses reached
    if (totalGuesses >= MAX_GUESSES) {
        printf("Maximum number of guesses reached. You lose.\n");
    }

    // Free allocated memory
    free(secretCode);
    for (int i = 0; i < totalGuesses; ++i) {
        free(userGuesses[i]);
    }
    free(userGuesses);
    free(correctAnswers);
    free(partialCorrectAnswers);

    return 0;
}
