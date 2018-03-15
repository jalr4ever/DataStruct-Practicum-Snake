#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

const int Width = 50;
const int Height = 25;
int Speed = 200;;//全局-记录速度 
int Point;//全局-记录分数 
int Level = 1;
int Count = 1;
int Number = 1;

typedef struct Snake {
	int x, y;
	Snake *prev;
	Snake *next;
}Snake, *Snakee;

typedef struct Food {
	int x, y;
	bool isEat;
}Food;
//获取蛇坐标 
void gotoxySnake(Snakee snake) {
	COORD position = { snake->x,snake->y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

//获取实体坐标
void gotoxyEntity(int x, int y) {
	COORD position = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

/*-----Main Logic-----*/

//Logic-Snake
Snakee snake_move(Snakee snake, int dir,Food &food);
void snake_grow(Snakee snake, int x, int y);
void free_snake(Snakee snake);
bool snake_die_itself(Snakee snake);
bool snake_die_edge(int x, int y);
bool snake_eat(int s_x,int s_y,int f_x,int f_y);
int snake_get_dir(int old_dir);
Snakee init_snake();

//Logic-food
void generate_food(Food &food, Snakee *snake);//50% big food with double point
bool food_is_eat(Food &food, Snakee *snake);


//Logic-level
void level_add();

//Logic-speed
void speed_add();

void init_game(Snakee snake, Food &food);



int main() {

	Snakee snake;
	Food food;
	snake = init_snake();
	int new_dir = 75;//向左
	init_game(snake, food);

	while (true) {
		if (kbhit()) {
			new_dir = snake_get_dir(new_dir);
		}
		if (snake) {
			snake = snake_move(snake, new_dir,food);
		}
		if (food.isEat==true) {
			generate_food(food, &snake);
		}
	}
}

void init_game(Snakee snake, Food &food) {

	Snakee p = snake;
	do {
		gotoxySnake(p), printf("*");
		p = p->next;
	} while (p != snake);



	/*--Init For Wall Panel--*/
	gotoxyEntity(Width, Height / 4), printf("  Point:  %d",Point);
	gotoxyEntity(Width, Height / 4 + Height / 8), printf("  Speed:  %d",Speed);
	gotoxyEntity(Width, (Height / 4) - Height / 8), printf("  Level:  %d",Level);
	gotoxyEntity(Width, (Height * 3 / 4)), printf("    By Setihex  ");
	for (int i = Width; i<Width + Width / 3; i++) {
		gotoxyEntity(i, Height / 2), printf("+");
		gotoxyEntity(i, 1), printf("+");
		gotoxyEntity(i, Height), printf("+");
	}
	for (int i = 1; i <= Width; i++) {
		gotoxyEntity(i, 1), printf("+");
		gotoxyEntity(i, Height), printf("+");
	}
	for (int i = 1; i <= Height; i++) {
		gotoxyEntity(1, i), printf("+");
		gotoxyEntity(Width, i), printf("+");
		gotoxyEntity(Width + Width / 3, i), printf("+");
	}
	food.isEat = false;
	food.x = 10;
	food.y = 10;
	gotoxyEntity(food.x, food.y), printf("#");


}

void generate_food(Food &food, Snakee *snake) {
	srand(time(NULL));
	bool generate = true;
	if (generate == true) {
		generate = false;
		food.x = (rand() % (Width-2 - 5 + 1)) + 5;
		food.y = (rand() % (Height-2 - 5 + 1)) + 5;
		if (food_is_eat(food, snake)) {
			generate = true;
		}
	}
	food.isEat = false;
	gotoxyEntity(food.x, food.y);
	printf("#");
}

bool food_is_eat(Food &food, Snakee *snake) {
	Snakee p = *snake;//这里获得了一级指针
	food.isEat = true;
	if (food.x == p->x&&food.y == p->y) {
		return true;
	}
	else
		return false;
}

void snake_grow(Snakee snake, int x, int y) {
	/*在待插入节点之前插入*/
	Snakee head = (Snakee)malloc(sizeof(Snake));//获取一节待插入节点
	head->prev = snake->prev;//待插的前驱后续
	head->next = snake;
	head->x = x;
	head->y = y;
	snake->prev->next = head;//自身前驱的后续 此时已经隔开一个空格了！ 
	snake->prev = head;//自身的前驱 此时已经把待插放到空格 	 	
}

Snakee init_snake() {
	Snakee head = (Snakee)malloc(sizeof(Snake));
	head->x = 36;
	head->y = 5;
	head->next = head;//双向循环链表初始化 
	head->prev = head;
	for (int i = 36; i<40; i++) {
		snake_grow(head, i, 5);
	}
	return head;
}

int snake_get_dir(int old_dir) {
	int new_dir = old_dir;
	if (kbhit()) {
		getch();
		new_dir = _getch();//第二次getch才是实际字符
						   //L-R&U-D
		if (abs(new_dir - old_dir == 2) || abs(new_dir - old_dir) == 8) {
			new_dir = old_dir;
		}
	}
	return new_dir;
}

Snakee snake_move(Snakee snake, int dir,Food &food) {
	Snakee head;
	head = (Snakee)malloc(sizeof(Snake));//在头节点前插入一节
	head->prev = snake->prev;//待插的前驱
	head->next = snake;//待插的后续
	head->x = snake->x;
	head->y = snake->y;
	snake->prev->next = head;//前驱的后续
	snake->prev = head;//后续的前驱

	Snakee p = head->prev;//头之前 这同指向尾巴节点

	//存下尾巴坐标
	int tail_x = p->x;
	int tail_y = p->y;
	gotoxySnake(p);
	printf(" ");//前进距离
	p->prev->next = p->next;//删除尾巴节点的前一节
	p->next->prev = p->prev;
	free(p);

	int new_dir = snake_get_dir(dir);
	if (dir) {//检测移动
		switch (new_dir)
		{
			case 72://上
			head->y--;
			break;
			case 77://右 
			head->x++;
			break;			
			case 75://左
			head->x--; 
			break;						
			case 80://下 
			head->y++;
			break;		
			default:
			break;
		}
	}
	
	if (snake_die_edge(head->x, head->y) == true|| snake_die_itself(snake) == true ) {//两个死亡逻辑
		system("cls");
		gotoxyEntity((Width+10)/2,(Height)/2);
		printf("Game Over\n");
		free_snake(snake);
		return 0;
	}
	if (snake_eat(head->x, head->y, food.x, food.y) == true) {
		food.isEat = true;
		Snakee tail= (Snakee)malloc(sizeof(Snake));
		tail->prev = head->prev;
		tail->next = head;
		tail->x = tail_x;
		tail->y = tail_y;
		head->prev->next = tail;//在头的之前就是尾 插入了新的节点
		head->prev = tail;
	}

	gotoxySnake(head), printf("*");
	Sleep(Speed);
	return head;
}
bool snake_die_edge(int x, int y) {
	if (x == 1 || x == Width || y == 1 || y == Height) {
		return true;
	}
	else
		return false;
}
bool snake_die_itself(Snakee snake) {
	Snakee t;
	t = snake->next->next;
	do {
		if (snake->x == t->x&&snake->y == t->y) {
			return true;
		}
		t = t->next;
	} while (t != snake);
	return false;
}
bool snake_eat(int s_x, int s_y, int f_x, int f_y) {

	if (Number % 3 == 0) {
		Level++;
		Number=1;
		gotoxyEntity(Width, (Height / 4) - Height / 8), printf("  Level:  %d",Level);
		speed_add();//加速		
	}
	if (s_x == f_x&&s_y == f_y) {
		Count++;
		Number++;
		Point += 10;
		gotoxyEntity(Width, Height / 4), printf("  Point:  %d",Point);	
		return true;
	}
	else
		return false;
}
void free_snake(Snakee snake) {
	Snakee p = snake;
	do {
		Snakee t = p;
		p = p->next;
		free(t);
	} while (p != snake);
}
void speed_add() {//速度逻辑
		Speed -= 20;
		gotoxyEntity(Width, Height / 4 + Height / 8), printf("  Speed:  %d",Speed);
}
