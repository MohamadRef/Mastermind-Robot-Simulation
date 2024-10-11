#include <stdio.h>
#include <stdlib.h>

#define MAX_ATTEMPTS 5
#define MIN_ROWS 20
#define MAX_ROWS 300
#define MIN_COLUMNS 20
#define MAX_COLUMNS 300
#define MIN_BOTS 1
#define MAX_BOTS 10
#define MIN_SEED_VAL 10
#define MAX_SEED_VAL 32767
#define MIN_TURNS_COUNT 10
#define MAX_TURNS_COUNT 5000
#define NUM_DIRECTIONS 4
#define NUM_COLOURS 4

enum InitType { RANDOM = 1, CHECKERBOARD, ALL_WHITE };

struct Robot {
    int x;
    int y;
    int direction;  // 1: North, 2: South, 3: East, 4: West
    int paintColour; // 1: White, 2: Red, 3: Green, 4: Blue
};

// Function to display error messages
void showError(const char *errMsg) {
    fprintf(stderr, "ERROR: %s\n", errMsg);
}

// Function to read an integer from the file and handle errors
int readIntFromFile(FILE *file, const char *errMsg) {
    int val;
    if (fscanf(file, "%d", &val) != 1) {
        showError(errMsg);
        exit(EXIT_FAILURE);
    }
    return val;
}

// Function to allocate a 2D game board
int** allocateGameBoard(int rows, int columns) {
    int **board = (int **)malloc(rows * sizeof(int *));
    if (board == NULL) {
        showError("Failed to allocate memory for game board.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        board[i] = (int *)malloc(columns * sizeof(int));
        if (board[i] == NULL) {
            showError("Failed to allocate memory for game board row.");
            exit(EXIT_FAILURE);
        }
    }
    return board;
}

// Function to initialize the game board randomly
void initBoardRand(int **board, int numRows, int numCols, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            board[i][j] = rand() % NUM_COLOURS + 1; // Randomly choose color from 1 to 4
        }
    }
}

// Function to initialize the game board in a checkerboard pattern
void initBoardChecker(int **board, int numRows, int numCols) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            board[i][j] = (i + j) % 2 == 0 ? 1 : 2; // 1 for white, 2 for red
        }
    }
}

// Function to initialize the game board to all white
void initBoardAllWhite(int **board, int numRows, int numCols) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            board[i][j] = 1; // White
        }
    }
}

// Function to allocate memory for robots
struct Robot* allocateRobots(int numRobots) {
    struct Robot *robots = (struct Robot *)malloc(numRobots * sizeof(struct Robot));
    if (robots == NULL) {
        showError("Failed to allocate memory for robots.");
        exit(EXIT_FAILURE);
    }
    return robots;
}

// Function to place robots randomly on the board
void placeRobots(struct Robot *robots, int numRobots, int numRows, int numCols, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < numRobots; i++) {
        robots[i].x = rand() % numCols;
        robots[i].y = rand() % numRows;
        robots[i].direction = rand() % NUM_DIRECTIONS + 1; // Random direction
        robots[i].paintColour = rand() % NUM_COLOURS + 1; // Random color
    }
}

// Function to print the game board
void printBoard(int **board, int numRows, int numCols) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to run the game for the specified number of turns
void runGame(int **board, struct Robot *robots, int numBots, int numTurns, int printInterval, int numRows, int numCols) {
    for (int turn = 0; turn < numTurns; turn++) {
        for (int i = 0; i < numBots; i++) {
            // Move robot based on its direction
            switch (robots[i].direction) {
                case 1: // Move north
                    robots[i].y = (robots[i].y - 1 + numRows) % numRows;
                    break;
                case 2: // Move south
                    robots[i].y = (robots[i].y + 1) % numRows;
                    break;
                case 3: // Move east
                    robots[i].x = (robots[i].x + 1) % numCols;
                    break;
                case 4: // Move west
                    robots[i].x = (robots[i].x - 1 + numCols) % numCols;
                    break;
                default:
                    break;
            }

            // Determine rotation based on the color of the tile
            int rotation = 0;
            switch (board[robots[i].y][robots[i].x]) {
                case 1: rotation = 1; break; // Rotate clockwise 90 degrees
                case 2: rotation = 2; break; // Rotate clockwise 180 degrees
                case 3: rotation = 3; break; // Rotate clockwise 270 degrees
                case 4: rotation = 0; break; // No rotation
                default: break; // Handle invalid color
            }

            // Update robot's direction after rotation
            robots[i].direction = (robots[i].direction + rotation) % NUM_DIRECTIONS;
            // Paint the tile with the robot's paint color
            board[robots[i].y][robots[i].x] = robots[i].paintColour;
        }

        // Print the game board at specified intervals
        if (turn % printInterval == 0 || turn == numTurns - 1) {
            printf("Turn %d:\n", turn);
            printBoard(board, numRows, numCols);
            printf("\n");
        }
    }
}

// Main function
int main() {
    char filename[100];
    FILE *file;
    int attempts = 0;

    // Step 1: Read Input File Name
    do {
        printf("Enter the name of the input file: ");
        scanf("%s", filename);
        file = fopen(filename, "r");
        if (file == NULL) {
            showError("Failed to open the input file.");
            attempts++;
        }
    } while (file == NULL && attempts < MAX_ATTEMPTS);

    if (file == NULL) {
        showError("ERROR: Failed to open the input file 5 times. Terminating the program.");
        exit(EXIT_FAILURE);
    }

    // Step 2: Read Parameters from Input File
    int numRows = readIntFromFile(file, "Number of rows not found in input file.");
    if (numRows < MIN_ROWS || numRows > MAX_ROWS) {
        showError("Number of rows is outside the specified range (20 to 300 inclusive).");
        exit(EXIT_FAILURE);
    }

    int numCols = readIntFromFile(file, "Number of columns not found in input file.");
    if (numCols < MIN_COLUMNS || numCols > MAX_COLUMNS) {
        showError("Number of columns is outside the specified range (20 to 300 inclusive).");
        exit(EXIT_FAILURE);
    }

    int numBots = readIntFromFile(file, "Number of robots not found in input file.");
    if (numBots < MIN_BOTS || numBots > MAX_BOTS) {
        showError("Number of robots is outside the specified range (1 to 10 inclusive).");
        exit(EXIT_FAILURE);
    }

    enum InitType initType = (enum InitType)readIntFromFile(file, "Initialization type not found in input file.");
    if (initType < RANDOM || initType > ALL_WHITE) {
        showError("Initialization type is outside the specified range (1 to 3 inclusive).");
        exit(EXIT_FAILURE);
    }

    unsigned int seedVal = (unsigned int)readIntFromFile(file, "Seed value not found in input file.");
    if (seedVal < MIN_SEED_VAL || seedVal > MAX_SEED_VAL)
