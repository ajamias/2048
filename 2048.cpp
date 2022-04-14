#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

#define row 4
#define col 4


class board {
    private:
        int board[row][col];
        int i, j, k, able;
    public:
        int score;
        void initializeBoard() {
            for (i = 0; i < row; i++) {
                for (j = 0; j < col; j++) {
                    board[i][j] = 0;
                }
            }
            score = 0;
        }
        void printBoard() {
            system("clear");
            cout << "+--------------------------------------------------+\n\n";
            for (i = 0; i < row; i++) {
                cout << '|';
                for (j = 0; j < col; j++) {
                    //cout << "   " << board[i][j] << "   ";
                    if (!board[i][j]) {
                        printf("    %4i    ", board[i][j]);
                    } else if (board[i][j] <= 64){
                        printf("    \033[1;%.0fm%4i\033[0m    ",  97-log2(board[i][j]), board[i][j]);
                    } else if (board[i][j] > 64) {
                        printf("    \033[1;%.0fm%4i\033[0m    ",  43-log2(board[i][j]), board[i][j]);
                    } else {
                        printf("    %4i    ", board[i][j]);
                    }
                }
                cout << "  |\n\n";
            }
            cout << "+--------------------------------------------------+\n";
            cout << "q to quit\n\n";
        }
        int insertNum() {
            int* rzeros = (int*) malloc(14 * sizeof(int));
            int* czeros = (int*) malloc(14 * sizeof(int));
            int* count = (int*) malloc(14 * sizeof(int));
            *count = 0;
            for (i = 0; i < row; i++) {
                for (j = 0; j < col; j++) {
                    if (board[i][j] == 0) {
                        rzeros[*count] = i;
                        czeros[*count] = j;
                        (*count)++;
                    }
                }
            }
            if (*count == 0) {
                free(rzeros);
                free(czeros);
                free(count);
                return 0;
            }
            int index = rand() % *count;
            if (rand() % 2) {
                board[rzeros[index]][czeros[index]] = 2;
                score += 200;
            } else {
                board[rzeros[index]][czeros[index]] = 4;
                score += 400;
            }
            free(rzeros);
            free(czeros);
            free(count);
            return 1;
        }
        int shiftUp() {
            able = 0;
            for (j = 0; j < col; j++) {
                for (i = 1; i < row; i++) {
                    if (!board[i-1][j] && board[i][j] && i != 0) {
                        board[i-1][j] = board[i][j];
                        board[i][j] = 0;
                        i -= 2;
                        able = 1;
                    }
                }
                for (i = 0; i < row-1; i++) {
                    if (board[i][j] == board[i+1][j] && board[i][j] != 0) {
                        board[i][j] *= 2;
                        board[i+1][j] = 0;
                        able = 1;
                    }
                }
                for (i = 1; i < row; i++) {
                    if (!board[i-1][j] && board[i][j] && i != 0) {
                        board[i-1][j] = board[i][j];
                        board[i][j] = 0;
                        i -= 2;
                    }
                }
            }
            return able;
        }
        int shiftDown() {
            int temp;
            for (i = 0; i < row/2; i++) {
                for (j = 0; j < col; j++) {
                    temp = board[i][j];
                    board[i][j] = board[row-1-i][j];
                    board[row-1-i][j] = temp;
                }
            }
            able = this->shiftUp();
            for (i = 0; i < row/2; i++) {
                for (j = 0; j < col; j++) {
                    temp = board[i][j];
                    board[i][j] = board[row-1-i][j];
                    board[row-1-i][j] = temp;
                }
            }
            return able;
        }
        int shiftLeft() {
            int temp;
            for (i = 1; i < row; i++) {
                for (j = 0; j < i; j++) {
                    temp = board[i][j];
                    board[i][j] = board[j][i];
                    board[j][i] = temp;
                }
            }
            able = this->shiftUp();
            for (i = 1; i < row; i++) {
                for (j = 0; j < i; j++) {
                    temp = board[i][j];
                    board[i][j] = board[j][i];
                    board[j][i] = temp;
                }
            }
            return able;
        }
        int shiftRight() {
            int temp;
            for (i = 1; i < row; i++) {
                for (j = 0; j < i; j++) {
                    temp = board[i][j];
                    board[i][j] = board[j][i];
                    board[j][i] = temp;
                }
            }
            able = this->shiftDown();
            for (i = 1; i < row; i++) {
                for (j = 0; j < i; j++) {
                    temp = board[i][j];
                    board[i][j] = board[j][i];
                    board[j][i] = temp;
                }
            }
            return able;
        }
        int checkLose() {
            for (i = 0; i < row; i++) {
                for (j = 0; j < col-1; j++) {
                    if (!board[i][j] || !board[i][3] || board[i][j] == board[i][j+1] || board[j][i] == board[j+1][i]) {
                        return 1;
                    }
                }
            }
            return 0;
        }
};


int main() {

    srand(time(NULL));
    board b;
    b.initializeBoard();

    char c;
    int run, canShift = 1;
    run = b.insertNum();
    b.score = 0;

    do {
        if (canShift) {
            run = b.insertNum();
        }
        b.printBoard();
        system("stty raw");
        do {
            c = getchar();
            if (c == '\033'){
                getchar();
                c = getchar();
                switch(c) { // the real value
                    case 'A':
                        // code for arrow up
                        canShift = b.shiftUp();
                        break;
                    case 'B':
                        // code for arrow down
                        canShift = b.shiftDown();
                        break;
                    case 'C':
                        // code for arrow right
                        canShift = b.shiftRight();
                        break;
                    case 'D':
                        // code for arrow left
                        canShift = b.shiftLeft();
                        break;
                    default:
                        c = ' ';
                }
            } else if (c == 'q') {
                run = 0;
            } else {
                system("stty cooked");
                cout << "invalid input\n";
                system("stty raw");
            }
        } while ((c != 'A') && (c != 'B') && (c != 'C') && (c != 'D') && run);
        system("stty cooked");
    } while (run && b.checkLose());
    printf("   ____    _    __  __ _____    _____     _______ ____   \n");
    printf("  / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / / ____|  _ \\  \n");
    printf(" | |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) | \n");
    printf(" | |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <  \n");
    printf("  \\____/_/   \\_\\_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\ \n\n");
    printf("                     Score: %8i\n\n", b.score);
                                                        

    return 0;
}