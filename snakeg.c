#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // For usleep()
#include <time.h>

#define WIDTH 20
#define HEIGHT 10

// Snake Node Structure
typedef struct Snake {
    int x, y;
    struct Snake* next;
} Snake;

Snake* head = NULL;
Snake* tail = NULL;

int foodX, foodY;
int score = 0;
char direction = 'd'; // Initial direction: right

// Add new head
void addHead(int x, int y) {
    Snake* newNode = (Snake*)malloc(sizeof(Snake));
    newNode->x = x;
    newNode->y = y;
    newNode->next = head;
    head = newNode;
    if (tail == NULL) tail = newNode;
}

// Remove tail
void removeTail() {
    if (head == tail) {
        free(tail);
        head = tail = NULL;
        return;
    }
    Snake* temp = head;
    while (temp->next != tail) temp = temp->next;
    free(tail);
    tail = temp;
    tail->next = NULL;
}

// Generate food at random position
void generateFood() {
    foodX = rand() % WIDTH;
    foodY = rand() % HEIGHT;
}

// Check collision with wall or self
int checkCollision(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 1;
    Snake* temp = head;
    while (temp != NULL) {
        if (temp->x == x && temp->y == y) return 1;
        temp = temp->next;
    }
    return 0;
}

// Draw the game board
void drawBoard() {
    system("clear"); // use "clear" for Linux/Android
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int printed = 0;
            if (i == foodY && j == foodX) {
                printf("F"); printed = 1;
            } else {
                Snake* temp = head;
                while (temp != NULL) {
                    if (temp->x == j && temp->y == i) {
                        printf("O");
                        printed = 1;
                        break;
                    }
                    temp = temp->next;
                }
            }
            if (!printed) printf(".");
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("Controls: w=up, s=down, a=left, d=right\n");
}

// Update direction based on user input
void updateDirection() {
    char input;
    printf("Enter direction (w/a/s/d): ");
    scanf(" %c", &input);
    if ((input == 'w' && direction != 's') ||
        (input == 's' && direction != 'w') ||
        (input == 'a' && direction != 'd') ||
        (input == 'd' && direction != 'a')) {
        direction = input;
    }
}

int main() {
    srand(time(0));
    addHead(WIDTH/2, HEIGHT/2);
    generateFood();

    while (1) {
        drawBoard();
        updateDirection();

        int newX = head->x;
        int newY = head->y;

        if (direction == 'w') newY--;
        else if (direction == 's') newY++;
        else if (direction == 'a') newX--;
        else if (direction == 'd') newX++;

        if (checkCollision(newX, newY)) {
            drawBoard();
            printf("Game Over! Final Score: %d\n", score);
            break;
        }

        addHead(newX, newY);

        if (newX == foodX && newY == foodY) {
            score++;
            generateFood();
        } else {
            removeTail();
        }

        usleep(150000); // 150 ms delay
    }

    return 0;
}