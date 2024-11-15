#include <iostream>
#include <conio.h>
#include <windows.h>
#include <locale>

using namespace std;

struct Snake {
    int x, y;
    Snake* next;
};

void input();
void generateFood(Snake* head);
void drawField(Snake* head);
bool gameOver(Snake* head);
void moveSnake(Snake*& head, Snake*& tail);
bool foodTaken(Snake* head);
void showMenu();
void startGame();

const int width = 20;
const int height = 10;

int foodX;
int foodY;
int score;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

int main() {
    setlocale(LC_ALL, "RU"); // Установка поддержки русского языка
    showMenu();
    return 0;
}

void showMenu() {
    char choice;
    do {
        system("cls");
        cout << "=== Игра \"Змейка\" ===" << endl;
        cout << "1. Играть" << endl;
        cout << "2. Выйти" << endl;
        cout << "Выберите: ";
        cin >> choice;

        switch (choice) {
        case '1':
            startGame();
            break;
        case '2':
            cout << "Выход из игры..." << endl;
            exit(0);
        default:
            cout << "Неверный выбор! Попробуйте снова." << endl;
            Sleep(1000);
        }
    } while (true);
}

void startGame() {
    Snake* head = new Snake();
    head->x = width / 2;
    head->y = height / 2;
    head->next = nullptr;
    Snake* tail = head;

    score = 0;
    dir = RIGHT;
    generateFood(head);

    while (dir != STOP) {
        drawField(head);
        input();
        moveSnake(head, tail);
        if (gameOver(head)) {
            cout << "Игра окончена!" << endl;
            Sleep(1000);

            cout << "Хотите начать заново? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                delete head; // Удаление старой змейки
                startGame();
            }
            else {
                delete head;
                showMenu();
            }
        }
        Sleep(300);
    }
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': dir = LEFT; break;
        case 'd': dir = RIGHT; break;
        case 'w': dir = UP; break;
        case 's': dir = DOWN; break;
        case '0': dir = STOP; break;
        }
    }
}

void generateFood(Snake* head) {
    bool foodOnSnake;
    do {
        foodX = rand() % width;
        foodY = rand() % height;
        foodOnSnake = false;
        Snake* temp = head;
        while (temp != nullptr) {
            if (temp->x == foodX && temp->y == foodY) {
                foodOnSnake = true;
                break;
            }
            temp = temp->next;
        }
    } while (foodOnSnake);
}

void drawField(Snake* head) {
    system("cls");
    for (int i = 0; i < width + 2; i++) cout << "Z"; // Верхняя граница
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "Z"; // Левая граница
            bool isBodyPart = false;
            Snake* temp = head;

            while (temp != nullptr) {
                if (temp->x == j && temp->y == i) {
                    cout << "O";
                    isBodyPart = true;
                }
                temp = temp->next;
            }

            if (!isBodyPart) {
                if (foodX == j && foodY == i) cout << "*"; // Значок еды
                else cout << " ";
            }

            if (j == width - 1) cout << "Z"; // Правая граница
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++) cout << "Z"; // Нижняя граница
    cout << endl;

    cout << "Счёт: " << score << endl;
}

bool gameOver(Snake* head) {
    if (head->x < 0 || head->x >= width || head->y < 0 || head->y >= height)
        return true;

    Snake* temp = head->next;
    while (temp != nullptr) {
        if (temp->x == head->x && temp->y == head->y)
            return true;
        temp = temp->next;
    }

    return false;
}

void moveSnake(Snake*& head, Snake*& tail) {
    int prevX = head->x;
    int prevY = head->y;
    int tempX, tempY;

    switch (dir) {
    case LEFT: head->x--; break;
    case RIGHT: head->x++; break;
    case UP: head->y--; break;
    case DOWN: head->y++; break;
    default: break;
    }

    if (foodTaken(head)) {
        Snake* newSnake = new Snake();
        newSnake->x = prevX;
        newSnake->y = prevY;
        newSnake->next = nullptr;
        tail->next = newSnake;
        tail = newSnake;
        generateFood(head);
    }
    else {
        Snake* temp = head->next;
        while (temp != nullptr) {
            tempX = temp->x;
            tempY = temp->y;

            temp->x = prevX;
            temp->y = prevY;

            prevX = tempX;
            prevY = tempY;
            temp = temp->next;
        }
    }
}

bool foodTaken(Snake* head) {
    if (head->x == foodX && head->y == foodY) {
        score += 10;
        return true;
    }
    return false;
}
