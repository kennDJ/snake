#include <conio.h>
#include <stdio.h>
#include <BIOS.H>
#include <stdlib.h>
#include <time.h>
#include <dos.h>

#include "snake.h"
/*对食物的监听*/
boolean getTimeFood(int *surplusFood, boolean *eatup, SNAKE *snake, int *point) {
	int i;
	int k;
	int tmp;
	int x;
	int y;

	if(snake->sb == NULL) {
		return  FALSE;
	}
	x = snake->sb[snake->head].row;
	y = snake->sb[snake->head].col;
	tmp = (y - 1) * 80 +  x - 1;
	if(point[tmp] == FOOD) {
		(*surplusFood)--;
		point[tmp] = 0;
		(snake->len)++;
	}if(point[tmp] == BODY) {
		return TRUE;
	}
	point[(y - 1) * 80 +  x - 1] = BODY;
	if(*surplusFood <= 0 ) {
		*eatup = TRUE;
		*surplusFood = FOODNUM;
	}
	return FALSE;
}
/*显示食物*/
void showFood(int *point, boolean *eatup) {
	int i;
	int row;
	int col;

	for(i = 0; i < 2000; i++) {
		if(point[i] == FOOD) {
			row = (i + 1) % 80;
			col = (i + 1) / 80 + 1;
			gotoxy(row, col);
			printf("#");
		}
	}
	*eatup = FALSE;
}
/*产生食物的坐标*/
void getFoodXy(int *point) {
	int randNum;
	int i;
	int count = 0;
	int array[2000] = {0};
	int j = 1999;

	for(i = 0; i < 2000; i++) {
		if(point[i] == 0) {
			array[count] = i;
			count++;
		}
	}

	for(i = 0; i < FOODNUM; i++) {
		srand(time(0) + i);
		randNum = rand() % count;
		point[array[randNum]] = FOOD;
		array[randNum] = array[j--];
		count--;
	}
}
/*根据方向得到蛇头的图形*/
int getSymbol(int DELTA_MOVEIndex){
	switch(DELTA_MOVEIndex) {
		case 0:		return SNAKE_UP; /*上*/
		case 1:     return SNAKE_DOWN; /*下*/
		case 2:		return SNAKE_LEFT; /*左*/
		case 3:     return SNAKE_RIGHT; /*右*/
	}
}
/*控制蛇的移动*/
void move(int *xx, int *yy, DELTA_MOVE *delta, int symbol, SNAKE *snake, int *point) {
	int tail;
	int tailRow;
	int tailCol;
	int x;
	int y;

	if(snake->sb == NULL) {
		return;
	}
	x = snake->sb[snake->head].row;
	y = snake->sb[snake->head].col;
	gotoxy(x, y);
	printf("*");
	x = x + delta->deltRow;
	y = y + delta->deltCol;
	gotoxy(x, y);
	printf("%c", symbol);
	snake->head  = (snake->head + 1) % MAX_LEN;
	snake->sb[snake->head].row = x;
	snake->sb[snake->head].col = y;
	*xx = x;
	*yy = y;
	if(snake->curLen < snake->len) {
		(snake->curLen)++;
		return;
	}else{
		tail = (snake->head - snake->len + MAX_LEN) % MAX_LEN;
		tailRow = snake->sb[tail].row;
		tailCol = snake->sb[tail].col;
		point[(tailCol - 1) * 80 +  tailRow - 1] = 0;
		gotoxy(tailRow, tailCol);
		printf(" ");
	}
	
}
/*通过判断按键的键值来得到不同的响应*/
int judge(int key, long int *tempCount, SNAKE *snake) {
	int tmp;

	tmp = snake->direct;
	switch(key) {
		case RIGHT: return tmp == 2 ? 2 : 3;
		case LEFT:  return tmp == 3 ? 3 : 2;
		case DOWN:  return tmp == 0 ? 0 : 1;
		case UP:    return tmp == 1 ? 1 : 0;
		case D:		return tmp == 6 ? 6 : 7;
		case A:  	return tmp == 7 ? 7 : 6;
		case S:  	return tmp == 4 ? 4 : 5;
		case W:    	return tmp == 5 ? 5 : 4;
		case PGUP:  if(*tempCount > MIN_COUNT) {
						*tempCount = *tempCount / 2;
					}
					return -1;
		case PGDN:  if(*tempCount < MAX_COUNT) {
						*tempCount = *tempCount * 2;
					}
					return -1;
	}
}
/*去掉光标对游戏的干扰*/
void no_text_cursor() {
      union REGS r;
      r.h.ah = 1;
      r.h.ch = 32;
      int86(0x10, &r, &r);
}

boolean finished(int x, int y) {
	if(x <= MAX_X && y <= MAX_Y && x >= 3 && y >= 3) {
		return FALSE;
	}else{
		return TRUE;
	}
}
/*设置边框*/
void showBorder() {
	int i;

	clrscr();
	for(i = 2; i <= 79; i++) {
		gotoxy(i, 2);
		putch(223);
	}
	for(i = 2; i <= 24; i++) {
		gotoxy(2, i);
		putch(219);
	}
	for(i = 2; i <= 79; i++) {
		gotoxy(i, 24);
		putch(220);
	}
	for(i = 2; i <= 24; i++) {
		gotoxy(79, i);
		putch(219);
	}
	gotoxy(2, 24);
	putch(219);
}
/*初始化全局一维数组*/
void initPoint(int *point) {
	int i;
	int j;
	int tmp;

	for(i = 1; i <= 80; i++) {
		for(j = 1; j <= 25; j++) {
			if(!(i > 2 && i < 79 && j > 2 && j < 24)) {
				tmp = (j - 1) * 80 +  i - 1;
				point[tmp] = WALL;
			}
		}
	}
}
/*选择单人游戏还是双人游戏*/
int getPlayerNum() {
	int num;

	clrscr();
	gotoxy(10, 12);
	printf("please choose the number of players:(1 is one palyer,other number is two palyer)");
	gotoxy(10, 14);
	scanf("%d", &num);
	return num == 1 ? 1 : 2;
}

void main() {
	long int i = 0;
	int x = 20;
	int y = 12;
	int key;
	int tempIndex1;
	int tempIndex2;
	int palyerNum;
	int newKey;
	int tt;
	long int tempCount = DEFAULT_COUNT;
	int symbol1 = SNAKE_RIGHT;
	int symbol2 = SNAKE_RIGHT;
	int point[2000] = {0};
	boolean finished2 = FALSE;
	boolean finished1 = FALSE;
	boolean eatup = TRUE;
	int surplusFood = FOODNUM;
	SNAKE snake1 = {0, 5, 1, 3, NULL};
	SNAKE snake2 = {0, 5, 1, 3, NULL};

	DELTA_MOVE delta[4] = {{ 0, -1}, /*上*/	
				{ 0, 1},			 /*下*/				
				{ -1, 0},			 /*左*/
				{ 1, 0}};			 /*右*/
	SNAKE_BODY snakeBody1[100] = {{0,0}};
	SNAKE_BODY snakeBody2[100] = {{0,0}};

	no_text_cursor();
	
	initPoint(point);
	palyerNum = getPlayerNum();
	snakeBody1[0].row = x;
	snakeBody1[0].col = y;
	snakeBody2[0].row = x + 20;
	snakeBody2[0].col = y;
	snake1.sb = snakeBody1;
	snake2.sb = snakeBody2;
	if(palyerNum == 1) {
		snake2.sb = NULL;
	}
	clrscr();
	showBorder(point);
	while((!finished(x, y))) {
		i++;
		key = bioskey(1);
		if(key != 0  ){
			newKey = bioskey(0);
			if(newKey == ESC) {
				break;
			}
			tempIndex1 = judge(newKey, &tempCount, &snake1);
			if(tempIndex1 >= 0 && tempIndex1 <= 3) {
				snake1.direct = tempIndex1;
				symbol1 = getSymbol(snake1.direct);
			}
			tempIndex2 = judge(newKey, &tempCount, &snake2);
			if(snake2.sb && tempIndex2 >= 4 && tempIndex2 <= 7) {
				snake2.direct = tempIndex2 % 4;
				symbol2 = getSymbol(snake2.direct);
			}
		}
		if(i > tempCount) {
			finished1 = getTimeFood(&surplusFood, &eatup, &snake1, point);
			if(finished1 == TRUE) {
				break;
			}
			move(&x, &y, delta + snake1.direct, symbol1, &snake1, point);
			if(finished(x, y) == TRUE) {
				break;
			}
			finished2 = getTimeFood(&surplusFood, &eatup, &snake2, point);
			if(finished2 == TRUE) {
				break;
			}
			move(&x, &y, delta + snake2.direct, symbol2, &snake2, point);
			if(eatup == TRUE) {
				getFoodXy(point);
				showFood(point, &eatup);
			}
		i = 0;
		}
	}
		clrscr();
		gotoxy(40, 12);
		printf("Game over!");
		getch();
}
