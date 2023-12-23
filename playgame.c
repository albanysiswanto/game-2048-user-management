#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#ifdef _WIN32
    #include <conio.h>
#else
    #include <curses.h>
#endif

#define SIZE 4
#define MAX_USERS 10

struct User {
    char username[20];
    char password[20];
    int score;
};

struct User users[MAX_USERS];
struct User currentUser;
int currentUserIndex = -1;

int board[SIZE][SIZE]; // Definisi board

void clearScreen() {
    printf("\033[H\033[J");
}

void initialize() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

void printBoard() {
    clearScreen();
    printf("Score: %d\n", currentUser.score);
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            printf("%6d", board[i][j]);
        }
        printf("\n");
    }
}

void generateRandom() {
    int i, j;
    do {
        i = rand() % SIZE;
        j = rand() % SIZE;
    } while (board[i][j] != 0);
    board[i][j] = (rand() % 2 + 1) * 2;
}

#ifdef _WIN32
    // Pada sistem Windows, gunakan _getch() daripada getch()
    #define getch _getch
#endif

int isGameOver() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return 0;
            }
            if ((i < SIZE - 1 && board[i][j] == board[i + 1][j]) ||
                (j < SIZE - 1 && board[i][j] == board[i][j + 1])) {
                return 0;
            }
        }
    }
    return 1;
}

void shiftLeft() {
    int i, j, k;
    for (i = 0; i < SIZE; i++) {
        k = 0;
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] != 0) {
                board[i][k] = board[i][j];
                if (k != j) {
                    board[i][j] = 0;
                }
                k++;
            }
        }
    }
}

void mergeLeft() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE - 1; j++) {
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0) {
                board[i][j] *= 2;
                currentUser.score += board[i][j];
                board[i][j + 1] = 0;
            }
        }
    }
}

void moveLeft() {
    shiftLeft();
    mergeLeft();
    shiftLeft();
}

void rotateBoardClockwise() {
    int i, j;
    int tempBoard[SIZE][SIZE];

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            tempBoard[i][j] = board[SIZE - 1 - j][i];
        }
    }

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = tempBoard[i][j];
        }
    }
}

void moveRight() {
    rotateBoardClockwise();
    rotateBoardClockwise();
    moveLeft();
    rotateBoardClockwise();
    rotateBoardClockwise();
}

void moveUp() {
    rotateBoardClockwise();
    rotateBoardClockwise();
    rotateBoardClockwise();
    moveLeft();
    rotateBoardClockwise();
}

void moveDown() {
    rotateBoardClockwise();
    moveLeft();
    rotateBoardClockwise();
    rotateBoardClockwise();
    rotateBoardClockwise();
}

void registerUser() {
    printf("Enter your username: ");
    scanf("%s", currentUser.username);

    printf("Enter your password: ");
    scanf("%s", currentUser.password);

    currentUser.score = 0;

    // Tambahkan pengguna ke array pengguna
    if (currentUserIndex < MAX_USERS - 1) {
        currentUserIndex++;
        users[currentUserIndex] = currentUser;
        printf("Registration successful!\n");
    } else {
        printf("Maximum number of users reached.\n");
    }
}

int loginUser() {
    char username[20];
    char password[20];

    printf("Enter your username: ");
    scanf("%s", username);

    printf("Enter your password: ");
    scanf("%s", password);

    // Cari pengguna dalam array pengguna
    for (int i = 0; i <= currentUserIndex; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            currentUser = users[i];
            return 1; // Login berhasil
        }
    }

    printf("Invalid username or password.\n");
    return 0; // Login gagal
}

void displayLeaderboard() {
    printf("Leaderboard:\n");
    for (int i = 0; i <= currentUserIndex; i++) {
        printf("%s - Score: %d\n", users[i].username, users[i].score);
    }
}

int main() {
    srand(time(NULL));

    int choice;

    while (1) {
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    // Jika login berhasil, mulai permainan
                    initialize();
                    generateRandom();
                    generateRandom();
                    printBoard();

                    while (!isGameOver()) {
                        char move = getch();
                        switch (move) {
                            case 'a':
                                moveLeft();
                                break;
                            case 'd':
                                moveRight();
                                break;
                            case 'w':
                                moveUp();
                                break;
                            case 's':
                                moveDown();
                                break;
                            default:
                                printf("Invalid move! Use 'a', 'd', 'w', or 's'.\n");
                                break;
                        }

                        generateRandom();
                        printBoard();
                    }

                    printf("Game Over! Your score: %d\n", currentUser.score);
                }
                break;
            case 3:
                displayLeaderboard();
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                break;
        }
    }

    return 0;
}
