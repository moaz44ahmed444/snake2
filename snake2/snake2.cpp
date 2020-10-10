
#include <windows.h>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <conio.h>


#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
using namespace std;


enum DIRECTION {
	UP, RIGHT, LEFT, DOWN
};



#pragma region Global Variables
COORD screen_size, pos, food_Pos, wall_pos; // console screen size
HANDLE screen;
COORD snake[1000];
int snakeLength , level;
bool  is_food_exist, gameOver;;
DIRECTION dir;
int score , high_score=0;
string name;
#pragma endregion


void printSnake();
void levels();



void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}



void nameLevel() {
	system("cls");
	cout << "\nplayer name : " ;
	cin >> name ;
	cout << "\n\nlevel from 1 to 10 : " ;
	cin >> level;
	if (level < 1) {
		level = 1;
	}
	else if (level > 10) {
		level = 10;
	}
	//levels();
	system("cls");

}



/// <summary>
/// 
/// </summary>
void getScreenSize() {
	screen = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	screen_size.X = csbi.srWindow.Right - csbi.srWindow.Left -20;
	screen_size.Y = csbi.srWindow.Bottom - csbi.srWindow.Top - 5;
}

void moveCursor(int x, int y) {
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(screen, pos);
}

/// <summary>
/// draw the borader of game
/// </summary>
void drawBorder() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);

	//upper border
	for (int i = 0; i < screen_size.X; i++) {
		cout << "-";
	}

	//left & right border
	for (int i = 1; i < screen_size.Y; i++) {
		moveCursor(0, i);
		cout << "|";

		moveCursor(screen_size.X - 1, i);
		cout << "|";
	}

	//down border
	moveCursor(0, screen_size.Y - 1);
	for (int i = 0; i < screen_size.X; i++) {
		cout << "-";
	}
}

/// <summary>
/// 1. inisialize snake array
/// 2. draw snake in middle of screen
/// 3. insialize movment variables
/// </summary>
void initSnak() {
	// insialization snake first posistions
	snakeLength = 5;
	dir = RIGHT;
	is_food_exist = false;
	for (int i = 0; i < snakeLength; i++)
	{
		if (i == 0) {
			snake[0].X = screen_size.X / 2;
			snake[0].Y = screen_size.Y / 2;
			continue;
		}
		snake[i] = { snake[i - 1].X - 1 , snake[i - 1].Y };
	}

}


/// <summary>
/// print snake
/// </summary>
void printSnake() {

	for (int i = 0; i < snakeLength; i++)

	{
		moveCursor(snake[i].X, snake[i].Y);
		if (i == 0) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);

			cout << "@";
			continue;

		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),4);
		cout << "*";
	}
}

/// <summary>
/// check about food
/// random position food
/// add food 
/// check food on snake or no
/// </summary>

int random(int from, int to) {
	return rand() % (to - from + 1) + from;
}
void addFood()
{
	if (is_food_exist == true) {
		return;
	}
	bool valid_food = false;
	while (valid_food == false) {


		srand(time(0));
		food_Pos.X = random(2, screen_size.X - 4);
		food_Pos.Y = random(2, screen_size.Y - 3);

		for (int i = 0; i < snakeLength; i++) {
			if (food_Pos.X == snake[i].X && food_Pos.Y == snake[i].Y) {
				valid_food = false;
				return;
			}
			else {
				valid_food = true;
			}
		}
	}
	moveCursor(food_Pos.X, food_Pos.Y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	cout << "F";
	is_food_exist = true;

}




void check_food_eaten() {
	if (food_Pos.X == snake[0].X && food_Pos.Y == snake[0].Y) {
		is_food_exist = false;
		snakeLength += 4;
		score += 10;
	}
	moveCursor(2, screen_size.Y + 2);
	cout << "\n score = " << score;
}





/// <summary>
/// get input frim user
/// </summary>
void getUserInput() {
	
	if (_kbhit())
	{
		_getch();
		
		int input = _getch();

		switch (input)
		{
		case -32: //This value is returned by all arrow key. So, we don't want to do something.
			break;
		case KEY_DOWN:
			if (dir != DOWN) {

				dir = UP;
			}
			break;

		case KEY_LEFT:
			if (dir != RIGHT) {
				dir = LEFT;

			}
			break;

		case KEY_RIGHT:
			if (dir != LEFT) {
				dir = RIGHT;

			}
			break;

		case KEY_UP:
			if (dir != UP) {
				dir = DOWN;

			}
			break;

		}
	}
}



/// <summary>
/// move to right
/// move to up
/// move to down
/// move to left  
/// </summary>
void snakeNextMove() {

	for (int i = 0; i < snakeLength; i++) {
		moveCursor(snake[i].X, snake[i].Y);
		cout << " ";
	}
	COORD prev_point, temp;
	prev_point = snake[0];
	switch (dir)
	{

	case UP:
		snake[0] = { snake[0].X , snake[0].Y + 1 };
		break;
	case DOWN:
		snake[0] = { snake[0].X , snake[0].Y - 1 };
		break;
	case LEFT:
		snake[0] = { snake[0].X - 1 , snake[0].Y };
		break;
	case RIGHT:
		snake[0] = { snake[0].X + 1 , snake[0].Y };
		break;
	}

	for (int i = 1; i < snakeLength; i++) {
		temp = snake[i];
		snake[i] = prev_point;
		prev_point = temp;
	}
}

void checkBorderCollision() {
//right
	if (snake[0].X >= screen_size.X - 1) {
		snake[0].X = 1;
	}
	//left
	else if (snake[0].X < 1) {
		snake[0].X = screen_size.X - 2;
	}
	//down
	else if (snake[0].Y > screen_size.Y -2) {
		snake[0].Y = 1;
	}
	//up
	else if (snake[0].Y <= 0) {
		snake[0].Y = screen_size.Y - 2;
	}
}


void checkSelfCollesion() {
	for (int i = 1; i < snakeLength; i++) {
		if (snake[0].X == snake[i].X && snake[0].Y == snake[i].Y) {
			gameOver = true;
			system("cls");
			moveCursor(screen_size.X / 2, 3);
			cout << "game over\n\n";
		}
		
	}
		
	
}


bool game_cont() {
	if (gameOver == true) {
		if (score > high_score) { high_score = score; }
		cout << "\nHigh score\n" << name << " :\t" << high_score;
		cout << "\n\nenter y to continue playing : \n";
		
		if (_getch() == 'y') {
			gameOver = false;
			score = 0;
			for (int i = 0; i < snakeLength; i++) {
				snake[i] = { NULL };
			}

			return true;

		}
		else {
			gameOver = true;
			return false;
		}
		
	}
}
//
//void levels() {
//	/*for (int i = 0; i < level; i++) {
//		srand(time(0));
//		wall_pos.X = random(2, screen_size.X - 4);
//		wall_pos.Y = random(2, screen_size.Y - 3);
//		moveCursor(wall_pos.X, wall_pos.Y);
//		cout << "+++";
//	}*/
//	for (int i = 0; i < 2; i++) {
//		for (int j = 0; j < 2; j++) {
//			cout << wall[i][j];
//		}
//	}
//}

int main()
{
	do {
	nameLevel();
	hidecursor();
	getScreenSize();
	drawBorder();
	initSnak();
	printSnake();
	
		while (gameOver == false) {

			getUserInput();

			addFood();

			snakeNextMove();

			check_food_eaten();


			checkBorderCollision();

			printSnake();

			checkSelfCollesion();

			Sleep(500 / level);

			
		}
	} while (game_cont()==true);
	return 0;
}

