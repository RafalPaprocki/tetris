#include<stdio.h>
#include<stdlib.h> 
#include<time.h> 
#include<string.h>
#include<conio.h>
#include<ctype.h>

#define gameFieldHeigh 21
#define gameFieldWidth 20
#define moveLeftKey 'a'
#define moveRighKey 'd'
#define turnFallingElem 'o'
#define refreshingTime 0.25
#define shapeCount 2

enum Shape {
	Block = 1,
	Line = 2
};

struct Game {
	char gameField[gameFieldHeigh][gameFieldWidth];
};

struct FallingElem {
	int positionX;
	int positionY;
	enum Shape shape;
};

struct Game createGameField() {
	struct Game e;
	for (int i = 0; i < gameFieldHeigh; ++i) {
		for (int j = 0; j < gameFieldWidth; ++j) {
			if (i == 0 || i == gameFieldHeigh - 1) {
				e.gameField[i][j] = '-';
			}else if (j == 0 || j == gameFieldWidth - 1) {
				e.gameField[i][j] = '|';
			}
			else {
				e.gameField[i][j] = ' ';
			}
		}
	}
	e.gameField[1][3] = '@';
	return e;
}

void printGameField(struct Game e) {
	for (int i = 0; i < gameFieldHeigh; ++i) {
		for (int j = 0; j < gameFieldWidth; ++j) {
			printf("%c", e.gameField[i][j]);
		}
		printf("\n");
	}
}

struct FallingElem moveFallingElemIfKeyPressed(struct FallingElem el) {
	if (_kbhit()) {
		char pressedKey = tolower((char)_getch());
		switch (pressedKey) {
		case moveLeftKey:
			if (el.positionX > 1) {
				el.positionX -= 1;
			}
			break;
		case moveRighKey:
			if (el.positionX < gameFieldWidth - 2) {
				el.positionX += 1;
			}
			break;
		case turnFallingElem:
			printf("Turned");
			break;
		}
	}
	return el;
}

struct FallingElem createFalling() {
	struct FallingElem el;
	el.positionX = rand() % (gameFieldWidth - 3) + 1;
	el.positionY = 1;
	el.shape = rand() % 2 + 1;
	return el;
}

struct Game draw(struct FallingElem el, struct Game game, char drawingSign) {
	if (el.shape == Block) {
		game.gameField[el.positionY][el.positionX] = drawingSign;
		game.gameField[el.positionY + 1][el.positionX] = drawingSign;
		game.gameField[el.positionY + 1][el.positionX + 1] = drawingSign;
		game.gameField[el.positionY][el.positionX + 1] = drawingSign;
	}
	else if (el.shape == Line) {
		game.gameField[el.positionY][el.positionX] = drawingSign;
		game.gameField[el.positionY + 1][el.positionX] = drawingSign;
		game.gameField[el.positionY + 2][el.positionX] = drawingSign;
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
	el.positionY += 1;
	return el;
}

int stopedFalling(struct FallingElem el, struct Game game) {
	if (el.positionX) {
	}
}

void main() {
	srand(time(NULL));
	struct Game gameField = createGameField();
	struct FallingElem falling = createFalling();
	clock_t start, end;

	while (1) {
		start = clock();
		gameField = setFallingElemenInGame(falling, gameField);
		printGameField(gameField);
		gameField = clearFalling(falling, gameField);
		double timeTaken = 0;

		do {
			falling = moveFallingElemIfKeyPressed(falling);
			end = clock();
			timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
		} while (timeTaken < refreshingTime);
		
		
			falling = moveElemDown(falling);
		
		
		system("cls");
	}

	return 0;
}
