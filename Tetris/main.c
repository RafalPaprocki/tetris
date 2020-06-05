#include<stdio.h>
#include<stdlib.h> 
#include<time.h> 
#include<string.h>
#include<conio.h>
#include<ctype.h>

#define gameFieldHeigh 20
#define gameFieldWidth 12
#define moveLeftKey 'a'
#define moveRighKey 'd'
#define turnFallingElem 'o'
#define refreshingTime 0.15
#define shapeCount 3

enum Shape {
	Block = 1,
	Line = 2,
	Zblock = 3
};

struct Game {
	char** gameField;
	int points;
};

struct Point {
	int positionX;
	int positionY;
};

struct FallingElem {
	struct Point pointsOfFallingElem[4];
	struct Point centre;
	enum Shape shape;
	int pointsCount;
};

char** allockMemory(int height, int width) {
	char** temp = (char**)malloc(height * sizeof(char*));
	for (int k = 0; k < height; k++) {
		temp[k] = (char*)malloc(width * sizeof(char));
	}
	return temp;
}

void freeMemory(char** tab) {
	for (int i = 0; i < gameFieldHeigh; i++) {
		free(tab[i]);
	}
	free(tab);
}

struct Game createGameField() {
	struct Game game;
	
	game.gameField = allockMemory(gameFieldHeigh, gameFieldWidth);
	game.points = 0;

	for (int i = 0; i < gameFieldHeigh; ++i) {
		for (int j = 0; j < gameFieldWidth; ++j) {
			if (i == 0 || i == gameFieldHeigh - 1) {
				game.gameField[i][j] = '-';
			}else if (j == 0 || j == gameFieldWidth - 1) {
				game.gameField[i][j] = '|';
			}
			else {
				game.gameField[i][j] = ' ';
			}
		}
	}
	return game;
}


void printGameField(struct Game game) {
	for (int i = 0; i < gameFieldHeigh; ++i) {
		for (int j = 0; j < gameFieldWidth; ++j) {
			printf("%c", game.gameField[i][j]);
		}
		printf("\n");
	}
	printf("\nPunkty: %d", game.points);
}

int canMoveLeft(struct FallingElem el, struct Game game) {
	for (int i = 0; i < el.pointsCount; ++i) {
		int posX = el.pointsOfFallingElem[i].positionX - 1;
		int posY = el.pointsOfFallingElem[i].positionY;
		if (game.gameField[posY][posX] == '|' || game.gameField[posY][posX] == '*') {
			return 0;
		}
	}
	return 1;
}

int canMoveRight(struct FallingElem el, struct Game game) {
	for (int i = 0; i < el.pointsCount; ++i) {
		int posX = el.pointsOfFallingElem[i].positionX + 1;
		int posY = el.pointsOfFallingElem[i].positionY;
		if (game.gameField[posY][posX] == '|' || game.gameField[posY][posX] == '*') {
			return 0;
		}
	}
	return 1;
}

struct FallingElem move(struct FallingElem el, int step) {
	for (int i = 0; i < el.pointsCount; ++i) {
		el.pointsOfFallingElem[i].positionX += step;
	}
	return el;
}

int collision(struct Point p, struct Game game) {
	if (game.gameField[p.positionY + 1][p.positionX] == '-'
		|| game.gameField[p.positionY + 1][p.positionX] == '*'
		|| game.gameField[p.positionY + 1][p.positionX] == '|') {
		return 1;
	}
	return 0;
}

struct FallingElem rotate(struct FallingElem el, struct Game game) {
	int delX = 0;
	int delY = 0;
	struct FallingElem temp = el;
	for (int i = 0; i < temp.pointsCount; ++i) {
		struct Point p = temp.pointsOfFallingElem[i];
		delX = temp.pointsOfFallingElem[0].positionX - p.positionX;
		delY = temp.pointsOfFallingElem[0].positionY - p.positionY;
		temp.pointsOfFallingElem[i].positionX += delY + delX;
		temp.pointsOfFallingElem[i].positionY += delY - delX;
		if (collision(temp.pointsOfFallingElem[i], game)) {
			return el;
		}
	}
	return temp;
}

struct FallingElem moveFallingElemIfKeyPressed(struct FallingElem el, struct Game game) {
	if (_kbhit()) {
		char pressedKey = tolower((char)_getch());
		switch (pressedKey) {
		case moveLeftKey:
			if (canMoveLeft(el, game)) {
				el = move(el, -1);
			}
			break;
		case moveRighKey:
			if (canMoveRight(el, game)) {
				el = move(el, 1);
			}
			break;
		case turnFallingElem:
			el = rotate(el, game);
			break;
		}
	}
	return el;
}

struct FallingElem createNewBlock(struct FallingElem el, int startPosition) {
	struct Point p;
	el.pointsCount = 4;
	p.positionX = startPosition;
	p.positionY = 1;
	el.centre = p;
	el.pointsOfFallingElem[0] = p;
	p.positionX = startPosition + 1;
	p.positionY = 1;
	el.pointsOfFallingElem[1] = p;
	p.positionX = startPosition + 1;
	p.positionY = 2;
	el.pointsOfFallingElem[2] = p;
	p.positionX = startPosition;
	p.positionY = 2;
	el.pointsOfFallingElem[3] = p;

	return el;
}

struct FallingElem createLine(struct FallingElem el, int startPosition) {
	struct Point p;
	el.pointsCount = 4;
	p.positionX = startPosition;
	p.positionY = 1;
	el.centre = p;
	el.pointsOfFallingElem[0] = p;
	p.positionX = startPosition + 1;
	p.positionY = 1;
	el.pointsOfFallingElem[1] = p;
	p.positionX = startPosition + 2;
	p.positionY = 1;
	el.pointsOfFallingElem[2] = p;
	p.positionX = startPosition + 3;
	p.positionY = 1;
	el.pointsOfFallingElem[3] = p;

	return el;
}


struct FallingElem createZBlock(struct FallingElem el, int startPosition) {
	struct Point p;
	el.pointsCount = 4;
	p.positionX = startPosition;
	p.positionY = 1;
	el.centre = p;
	el.pointsOfFallingElem[0] = p;
	p.positionX = startPosition + 1;
	p.positionY = 1;
	el.pointsOfFallingElem[1] = p;
	p.positionX = startPosition + 1;
	p.positionY = 2;
	el.pointsOfFallingElem[2] = p;
	p.positionX = startPosition + 2;
	p.positionY = 2;
	el.pointsOfFallingElem[3] = p;

	return el;
}
struct FallingElem createFalling() {
	struct FallingElem el;
	int startPosition = rand() % (gameFieldWidth - 5) + 1;
	el.shape = rand() % shapeCount + 1;
	
	if (el.shape == Block) {
		el = createNewBlock(el, startPosition);
	}
	else if (el.shape == Line) {
		el = createLine(el, startPosition);
	}
	else if (el.shape == Zblock) {
		el = createZBlock(el, startPosition);
	}
	return el;
}


struct Game draw(struct FallingElem el, struct Game game, char drawingSign) {
	for (int i = 0; i < el.pointsCount; ++i) {
		int xPos = el.pointsOfFallingElem[i].positionX;
		int yPos = el.pointsOfFallingElem[i].positionY;
		game.gameField[yPos][xPos] = drawingSign;
	}
	return game;
}

struct Game setFallingElemenInGame(struct FallingElem el, struct Game game) {
	game = draw(el, game, '*');
	return game;
}

struct Game clearFalling(struct FallingElem el, struct Game game) {
	game = draw(el, game, ' ');
	return game;
}

struct FallingElem moveElemDown(struct FallingElem el) {
	for (int i = 0; i < el.pointsCount; ++i) {
		el.pointsOfFallingElem[i].positionY += 1;
	}
	return el;
}

int elementCollision(struct FallingElem el, struct Game game) {
	for (int i = 0; i < el.pointsCount; ++i) {
		struct Point p = el.pointsOfFallingElem[i];
		if (collision(p, game)) {
			return 1;
		}
	}
	return 0;
}

int stoppedFalling(struct FallingElem el, struct Game game) {
	return elementCollision(el, game);
}

int* findFullLines(struct Game game) {
	int fullLines[gameFieldHeigh];
	for (int i = 0; i < gameFieldHeigh; ++i) {
		fullLines[i] = 0;
	}
	for (int i = 1; i < gameFieldHeigh - 1; ++i) {
		for (int j = 1; j < gameFieldWidth - 1; ++j) {
			char g = game.gameField[i][j];
			if (game.gameField[i][j] != '*') {
				break;
			}
			if (j == (gameFieldWidth - 2)) {
				fullLines[i] = 1;
			}
		}
	}
	return fullLines;
}

struct Game clearFullLine(struct Game game) {
	int* fullLines = findFullLines(game);
	int counter = gameFieldHeigh - 1;
	for (int i = gameFieldHeigh - 1; i >= 1; --i) {
		if (fullLines[i] == 0) {
			if (counter != i) { 
				for (int j = 1; j < gameFieldWidth - 1; ++j) {
					game.gameField[counter][j] = game.gameField[i][j];
				}
			} 
			--counter;
		}
		else {
			game.points++;
		}
	}
	for (counter; counter >= 1; --counter) {
		for (int j = 1; j < gameFieldWidth - 1; ++j) {
			game.gameField[counter][j] = ' ';
		}
	}
	return game;
}

int checkIfEnd(struct FallingElem elem, struct Game gameField) {
	return elementCollision(elem, gameField);
}

void showResult(int result) {
	system("cls");
	printf("Game over. \n\n");
	printf("Your result: %d \n\n", result);
	printf("Press Any Key and Enter to back to MENU\n");  
	getchar();
	getchar();
}

int menu() {
	int action;
	system("cls");
	printf("Hello in tetris\n\n");
	printf("Choose proper action:\n");
	printf("\t1.New game.\n");
	printf("\t2.Exit.\n");
	scanf_s("%d", &action);
	return action;
}

void game() {
	struct Game gameField = createGameField();
	struct FallingElem falling = createFalling();

	printf("%c", gameField.gameField[0][0]);

	clock_t start, end;
	while (1) {
		start = clock();
		gameField = setFallingElemenInGame(falling, gameField);
		printGameField(gameField);
		gameField = clearFalling(falling, gameField);
		double timeTaken = 0;

		do {
			falling = moveFallingElemIfKeyPressed(falling, gameField);
			end = clock();
			timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
		} while (timeTaken < refreshingTime);

		if (stoppedFalling(falling, gameField)) {
			gameField = setFallingElemenInGame(falling, gameField);
			falling = createFalling();
			gameField = clearFullLine(gameField);
			if (checkIfEnd(falling, gameField)) {
				system("cls");
				gameField = setFallingElemenInGame(falling, gameField);
				printGameField(gameField);
				showResult(gameField.points);
				freeMemory(gameField.gameField);
				break;
			}
		}
		else {
			falling = moveElemDown(falling);
		}
		system("cls");
	}
}

void main() {
	srand(time(NULL));
	while (1) {
		int action = menu();
		printf("%d", action);
		switch (action)
		{
		case 1:
			game();
			break;
		case 2:
			exit(0);
		default:
			break;
		}
	}
}
