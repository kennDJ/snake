#define RIGHT 0x4d00
#define LEFT  0x4b00
#define UP	  0x4800
#define DOWN  0x5000
#define ESC	  0x11b
#define PGUP  0x4900
#define PGDN  0x5100
#define A     0X1e61
#define D     0x2064
#define S     0x1f73
#define W	  0x1177

#define MAX_COUNT 20000
#define MIN_COUNT 200
#define DEFAULT_COUNT 5000
#define MAX_X 80
#define MAX_Y 25
#define MAX_LEN 100

#define SNAKE_UP 30
#define SNAKE_DOWN 31
#define SNAKE_LEFT 17
#define SNAKE_RIGHT 16

#define FOOD  2
#define BODY  -1
#define WALL  -2

typedef unsigned char boolean;
#define TRUE				1
#define FALSE				0
#define FOODNUM				2

typedef struct DELTA_MOVE{
	int deltRow;
	int deltCol;
}DELTA_MOVE;

typedef struct SNAKE_BODY{
	int row;
	int col;
}SNAKE_BODY;

typedef struct SNAKE{
	int head;
	int len;
	int curLen;
	int direct;
	SNAKE_BODY *sb;
}SNAKE;

boolean finished(int x, int y);
void no_text_cursor();
int getSymbol(int DELTA_MOVEIndex);
void move(int *xx, int *yy, DELTA_MOVE *delta, int symbol, SNAKE *snake, int *point);
int judge(int key, long int *tempCount, SNAKE *snake);
void showFood(int *point, boolean *eatup);
void getFoodXy(int *point);
void excludeSnakeBody(int *array, SNAKE *snake);
void showBorder();
void no_text_cursor();
boolean finished(int x, int y);
void initPoint(int *point);
int getPlayerNum();
boolean getTimeFood(int *surplusFood, boolean *eatup, SNAKE *snake, int *point);
