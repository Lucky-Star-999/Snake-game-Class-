#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>


//Set up a condition of movement
enum State {UP, DOWN, LEFT, RIGHT} tt;

void clrscr()
{
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
	HANDLE	hConsoleOut;
	COORD	Home = {0,0};
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut,csbiInfo.dwCursorPosition);
}

void gotoxy();
void table();
void movement(int *ptrx, int *ptry);
void control();
void body(int *ptrx, int *ptry, int currentscore);
void touchwall(int *ptrx, int *ptry, int *count);
void touchbody(int *ptrx, int *ptry, int *count, int currentscore);
void food(int *x, int *y, int *ptrx, int *ptry, int currentscore, int currentfood);
void screenplay(char *currentname, int currentscore);
int menu();
void name();

int main() 
{
	int reverse = 0;
	AGAIN: reverse = 1;
	system("color 0a");
	if (menu()==1){
	clrscr();
	system("color 5b");
	//Declare the user's name
	name();
	system("color 1a");
	char name[100] = "Linh";
	
	//Declare the score + Gameover
	int score=0;			int countover = 0;
	//Declare the safe food
	int countfood=0;
	//Declare the whole parts of snake
	int snakex[22]={0};   	int snakey[22]={0};
	//Making the table
	table();
	//Declare the food
	int a=0, b=0;
	//Declare the head
	snakex[0]=40; 			snakey[0]=17;
	gotoxy(snakex[0],snakey[0]);
	tt = LEFT;

	food(&a, &b, &snakex[0], &snakey[0],score,countfood);
	
		
	
	while(1){
		
		Sleep(50);
		
		screenplay(name, score);
				
		control();
		
		if (snakex[0]==a && snakey[0]==b)
			{
				score++;
				food(&a, &b, &snakex[0], &snakey[0],score,countfood);
			}
		
		movement(&snakex[0], &snakey[0]);
		body(&snakex[0], &snakey[0],score);
		
		touchwall(&snakex[0], &snakey[0], &countover);
		touchbody(&snakex[0], &snakey[0], &countover, score);
		
		if (countover!=0){
			gotoxy(25,6);
			printf("* Press ENTER to return menu!");
			gotoxy(25,8);
			printf("* Press ESC to exit the game!");
			break;
		}
			
		if (score == 21){
			gotoxy(35,2);
			printf("You win!!!!!");
			gotoxy(25,6);
			printf("* Press ENTER to return menu!");
			gotoxy(25,8);
			printf("* Press ESC to exit the game!");
			break;
		}
	}
	while (1){
		
	if (kbhit())
        {
            char key = _getch();
            if (key == 13)
            	goto AGAIN;
            else if (key == 27)
            	goto END;
        }
    }
}
	END: reverse = 0;
	return 0;
}

void gotoxy (int x, int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}

void table()
{
	//Set up borders
	gotoxy(0,10);
	printf("%c", 218);
	gotoxy(79,10);
	printf("%c", 191);
	gotoxy(0,24);
	printf("%c", 192);
	gotoxy(79,24);
	printf("%c", 217);
	
	//Set up horizontal lines
	int i=0;
	for (i=1; i<=78; i++){
		gotoxy(i,10);
		printf("%c", 196);
		gotoxy(i,24);
		printf("%c", 196);
	}
	
	//Set up vertical lines
	for (i=11; i<=23; i++){
		gotoxy(0,i);
		printf("%c", 179);
		gotoxy(79,i);
		printf("%c", 179);
	}
	printf("\n\n");
}

void movement(int *ptrx, int *ptry)
{
	/*
	  tt: Condition of the snake
	  		ex: tt = UP -> The snake is going upwards
	*/

  	if (tt == UP)
    {
		//The snake is going up, it means that the coordinate "y" will decrease
		(*ptry)--;
		//After decreasing the coorinate "y", move the cursor (teleport the head of snake) to new position
        gotoxy(*ptrx,*ptry);
        //Output the head of snake
      	printf("*");
    }
    else if (tt == DOWN)
    {
        (*ptry)++;
        gotoxy(*ptrx,*ptry);
      	printf("*");
    }
    else if (tt == LEFT)
    {
        (*ptrx)--;
        gotoxy(*ptrx,*ptry);
      	printf("*");
    }
    else if (tt == RIGHT)
    {
        (*ptrx)++;
        gotoxy(*ptrx,*ptry);
      	printf("*");
    }

}

void control()
{
	if (kbhit())
        {
    /*
	The format of the catching keyborad of the system is:
		if (kbhit())
		{
			char key = _getch();
			
			...
		}
	*/
			
			char key = _getch();

            if ((key == 'a' || key == 75) && tt!=RIGHT)
            	tt = LEFT;
			else if ((key == 's' || key == 80) && tt!=UP)
            	tt = DOWN;
			else if ((key == 'd' || key == 77) && tt!=LEFT)
            	tt = RIGHT;
			else if ((key == 'w' || key == 72) && tt!=DOWN)
            	tt = UP;
        }
}

void body(int *ptrx, int *ptry, int currentscore)
{
	int i=0;

	gotoxy(ptrx[currentscore + 1] , ptry[currentscore + 1]);
	printf(" ");
	for (i=currentscore+1; i>=1; i--){
		ptrx[i] = ptrx[i-1];
		ptry[i] = ptry[i-1];
	}
	for (i=currentscore+1; i>=0; i--){
		gotoxy(ptrx[i] , ptry[i]);
		printf("*");
	}
	
}

void touchwall(int *ptrx, int *ptry, int *count)
{
	if (*ptrx == 0 || *ptrx == 79 || *ptry == 10 || *ptry == 24){
		gotoxy (35,2);
		printf("Game over!");
		(*count)++;
	}
		
}

void touchbody(int *ptrx, int *ptry, int *count, int currentscore)
{
	int i=0;
	
	for(i=2; i<=currentscore; i++){
		if (ptrx[0] == ptrx[i] && ptry[0] == ptry[i]){
			gotoxy (35,2);
			printf("Game over!");
			(*count)++;
		}
	}
	
	
}

void food(int *x, int *y, int *ptrx, int *ptry, int currentscore, int currentfood)
{
	srand(time(NULL));
	while (1)
	{
		int i=0;
		currentfood = 0;
		*x = 1 + rand() % 78;
		*y = 11 + rand() % 13;
		
		//Checking if the food "touch" the body of the snake
		for(i=0; i<=currentscore; i++){
		if (*x == ptrx[i] && (*y == ptry[i]) )
			currentfood++;
		}
		
		//If the food is "safe" (not touch the body of the snake), create the food on the screen
		if (currentfood==0)
		{
			gotoxy (*x,*y);
			printf("O");
			break;
		}			
			
	}	
	
	
}

void screenplay(char *currentname, int currentscore)
{
	gotoxy(50,4);
	printf("Score: %d", currentscore);
}

int menu()
{
	int y=13;
	clrscr();
	
	gotoxy(21,5);
	printf("*****  *   *      *      *    *  *****");
	gotoxy(21,6);
	printf("*      **  *     * *     *   *   *");
	gotoxy(21,7);
	printf("*****  * * *    *****    ***     ***");
	gotoxy(21,8);
	printf("    *  *  **   *     *   *   *   *");
	gotoxy(21,9);
	printf("*****  *   *  *       *  *    *  *****");
	
	gotoxy(35,13);
	printf("Start game");
	gotoxy(35,15);
	printf("Exit");
	
	//Set up an triangle on the screen
	gotoxy(33,13);
	putchar(16);
	
	
	gotoxy(18,20);
	printf("* Note: Press WASD or arrow keys to control!");
	gotoxy(26,22);
	printf("Press ENTER to choose!");
	while(1){

	if (kbhit())
        {
            char key = _getch();

            	
			if (key == 's' || key == 80){
				
				//Delete the old triangle
				gotoxy(33,13);
				printf(" ");
				
				//Create new triangle
				gotoxy(33,15);
				putchar(16);
				
				//Record the new position (just y coordinate) to variable y
				y = 15;
			}
            	

            	
			if (key == 'w' || key == 72){
				gotoxy(33,15);
				printf(" ");
				gotoxy(33,13);
				putchar(16);
				y = 13;
			}
			
			//If the triangle points to the line "Start game", then play game
			if (key == 13 && y==13)
				return 1;
				
			//If the triangle points to the line "Exit", then exit the game
			if (key == 13 && y==15)
				return 2;	   
	}
}
}

void name()
{
	clrscr();
	char n[30];
	gotoxy(4, 18);
	printf("*Instruction: After typing your name, press ENTER to play.");
	gotoxy(30, 22);
	printf("Good luck to you ^^");
	
	gotoxy(25,12);
	printf("Enter your name: ");
	fgets(n, sizeof(n), stdin);
	clrscr();
	gotoxy(15,4);
	printf("Your name: %s", n);
}
	
