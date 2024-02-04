#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

int **createMatrix(int n, int m);
void freeMatrix(int **matrix, int n);
void copyMatrix(int **matrix1, int **matrix2, int n, int m);
int readFile(int **matrix, int n, int m, char *filename);
void updateFrame(int **matrix, int n, int m);
void drawFrame(int **matrix, int n, int m);
int countNeighbors(int **matrix, int n, int m, int currentI, int currentJ);
int getNext(int current, int neighbors);
char getChar(int x);
int generateConf(int n, int m, char *filename);
int gameOver(int **matrix, int n, int m);

int main() {
    int n = 25, m = 80, speed = 100000;
    char *filename = "position.txt";
    int **matrix = createMatrix(n, m);
    char c;

    if (generateConf(n, m, filename) == 0) {
        printf("Error!");
        freeMatrix(matrix, n);
        return 1;
    }
    if (readFile(matrix, n, m, filename) == 0) {
        printf("Error!");
        freeMatrix(matrix, n);
        return 1;
    }

    initscr();
    raw();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    while (1) {
        c = getch();

        updateFrame(matrix, n, m);
        drawFrame(matrix, n, m);

        if (c == 'q' || c == 'Q' || gameOver(matrix, n, m)) {
            break;
        }
        if ((c == '+' || c == '=') && speed < 300000) {
            speed += 10000;
        }
        if ((c == '-' || c == '_') && speed > 50000) {
            speed -= 10000;
        }
        int temp = rand() % 2;

        usleep(speed);
    }

    endwin();
    freeMatrix(matrix, n);

    return 0;
}

int **createMatrix(int n, int m) {
    int **matrix = (int **)calloc(n, sizeof(int *));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int *)calloc(m, sizeof(int));
    }
    return matrix;
}

void freeMatrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void copyMatrix(int **matrix1, int **matrix2, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix1[i][j] = matrix2[i][j];
        }
    }
}

int readFile(int **matrix, int n, int m, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);
    return 1;
}

void updateFrame(int **matrix, int n, int m) {
    int **temp = createMatrix(n, m);
    int neighbors;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            neighbors = countNeighbors(matrix, n, m, i, j);
            temp[i][j] = getNext(matrix[i][j], neighbors);
        }
    }
    copyMatrix(matrix, temp, n, m);
    freeMatrix(temp, n);
}

void drawFrame(int **matrix, int n, int m) {
    clear();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printw("%c", getChar(matrix[i][j]));
        }
        if (i != n - 1) {
            printw("\n");
        }
    }
    refresh();
}

int countNeighbors(int **matrix, int n, int m, int currentI, int currentJ) {
    int count = 0, newI, newJ;
    for (int i = currentI - 1; i <= currentI + 1; i++) {
        for (int j = currentJ - 1; j <= currentJ + 1; j++) {
            newI = (i + n) % n;
            newJ = (j + m) % m;
            if (matrix[newI][newJ] == 1) {
                count++;
            }
        }
    }
    return count;
}

int getNext(int current, int neighbors) {
    if (current == 1) {
        neighbors -= 1;
        return (neighbors < 2 || neighbors > 3) ? 0 : 1;
    }
    return (neighbors == 3) ? 1 : 0;
}

char getChar(int x) {
    return (x == 1) ? '*' : ' ';
}

int generateConf(int n, int m, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int temp = rand() % 2;
            fprintf(file, "%d ", temp);
        }
        if (i != n - 1) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
    return 1;
}

int gameOver(int **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 1) {
                return 0;
            }
        }
    }
    return 1;
}