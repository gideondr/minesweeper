//***PROJECT 2 - MINESWEEPER***
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define Max_Lines 22
#define Max_Cols 22

typedef struct gameSize {
	int Lines;
	int Cols;
} GameSize;

//Declaring the global variables
typedef struct cellProperties {
	bool visible;
	int value;
	bool flag;
} CellProperties;

//initialazing the game board


//this function checks the validation of the coordinates entered
bool coordinate_validation(int x, int y, GameSize boardSize)
{
	if (x == -1 && y > 0)
		return true;

	if (y >= boardSize.Cols || y < 0 || x >= boardSize.Lines || x < 0)
		return false;
	else return true;
}
//this function prints out the game board
void OpenNextXCells(CellProperties board[][Max_Cols], GameSize boardSize, int NumOfCellsToOpen)
{
	int cnt = 0;
	int i, j;
	for (i = 0; i < boardSize.Lines; i++)
		for (j = 0; j < boardSize.Cols; j++)
			if (board[i][j].visible != true && board[i][j].value >= 0)
			{
				board[i][j].visible = true;
				cnt++;
				if (cnt == NumOfCellsToOpen)
					return;
			}

}

void print_board(CellProperties board[][Max_Cols], GameSize boardSize)
{
	int i, j;


	printf("     ");
	for (i = 0; i < boardSize.Cols; i++)
		printf("  %2d", i);
	printf(" \n");

	printf("   ");
	for (i = 0; i < boardSize.Cols + 1; i++)
		printf("____");
	printf("\n");

	for (int i = 0; i < boardSize.Lines; i++)
	{
		printf("%5d ", i);
		for (j = 0; j < boardSize.Cols; j++)
		{
			if (board[i][j].visible == false)
				printf("| X ");
			//printf("| %d ", board[i][j].value);
			else
			{
				if (board[i][j].flag)
				{
					printf("| F ");
				}
				else if ((board[i][j].value == -1))
				{
					printf("| * ");
				}
				else if (board[i][j].value == 0)
				{
					printf("|   ");
				}
				else if (board[i][j].value != -1)
				{
					printf("| %d ", board[i][j].value);
				}

			}

		}
		printf("|\n");
	}
	printf("   ");
	for (i = 0; i < boardSize.Cols + 1; i++)
		printf("____");
	printf("\n");

}

void print_Open_board(CellProperties board[][Max_Cols], GameSize boardSize)
{
	int i, j;


	printf("     ");
	for (i = 0; i < boardSize.Cols; i++)
		printf("  %2d", i);
	printf(" \n");

	printf("   ");
	for (i = 0; i < boardSize.Cols + 1; i++)
		printf("____");
	printf("\n");

	for (int i = 0; i < boardSize.Lines; i++)
	{
		printf("%5d ", i);
		for (j = 0; j < boardSize.Cols; j++)
		{
			if ((board[i][j].value == -1))
			{
				printf("| * ");
			}
			else if (board[i][j].value == 0)
			{
				printf("|   ");
			}
			else if (board[i][j].value != -1)
			{
				printf("| %d ", board[i][j].value);
			}
		}
		printf("|\n");
	}
	printf("   ");
	for (i = 0; i < boardSize.Cols + 1; i++)
		printf("____");
	printf("\n");

}

//this function initiate the size of the game board, in accordance to the choise the user enters
void setBoardSize(GameSize* boardSize, int gameType)
{
	if (gameType == 1)
	{
		(*boardSize).Lines = 8;
		(*boardSize).Cols = 8;
	}
	else if (gameType == 2)
	{
		(*boardSize).Lines = 12;
		(*boardSize).Cols = 12;
	}

	else if (gameType == 3)
	{
		(*boardSize).Lines = 15;
		(*boardSize).Cols = 15;
	}
	else if (gameType == 4) // builds a board in the size of the user's choice (up to 25)
	{
		printf("Please enter the size of the board (Lines <= 22 and Cols <= 40  ): ");
		scanf("%d%d", &(*boardSize).Lines, &(*boardSize).Cols);
		if ((*boardSize).Lines < 1 || (*boardSize).Lines > 22)
		{
			printf("Out Of Range , Set to maximum Size : 25): ");
			(*boardSize).Lines = 22;
		}
		if ((*boardSize).Cols < 1 || (*boardSize).Lines > 22)
		{
			printf("Out Of Range , Set to maximum Size : 25): ");
			(*boardSize).Cols = 22;
		}

	}
	else
	{
		(*boardSize).Lines = 8;
		(*boardSize).Cols = 8;
	}
}

//this function builds the numbers surrounding the bombs, so when you unveil it's tile, it shows how many bombs are around this tile
void InitSurrondingBombsCells(CellProperties board[][Max_Cols], int LineNum, int ColNum, GameSize boardSize)
{
	int pos_x_diff, pos_y_diff;
	for (pos_x_diff = -1; pos_x_diff < 2; pos_x_diff++)
	{
		for (pos_y_diff = -1; pos_y_diff < 2; pos_y_diff++)
		{
			if (coordinate_validation(LineNum + pos_x_diff, ColNum + pos_y_diff, boardSize))
				if (board[LineNum + pos_x_diff][ColNum + pos_y_diff].value != -1)
				{
					board[LineNum + pos_x_diff][ColNum + pos_y_diff].value += 1;
				}
		}
	}

}

//this function initiates randomly the bombs in the game board
void init(CellProperties board[][Max_Cols], GameSize boardSize)
{
	int LineNum;
	int ColNum;
	int bombcounter;
	int bombsNum = sqrt(boardSize.Lines * boardSize.Cols);
	for (bombcounter = 0; bombcounter < bombsNum; bombcounter++)
	{
		srand(time(NULL));
		LineNum = rand() % boardSize.Cols;
		ColNum = rand() % boardSize.Lines;
		while (board[LineNum][ColNum].value == -1)
		{
			LineNum = rand() % boardSize.Cols;
			ColNum = rand() % boardSize.Lines;
		}
		board[LineNum][ColNum].value = -1;
		InitSurrondingBombsCells(board, LineNum, ColNum, boardSize);
	}
}

//This function validates the input (coordinates[using the other function] AND commands)
bool user_input_validation(int x, int y, char z, GameSize boardSize)
{
	bool coordinate;
	coordinate = coordinate_validation(x, y, boardSize);
	if (coordinate)
	{
		if (toupper(z) != 'F' && toupper(z) != 'O')
			return false;
		return true;
	}
	return false;
}

//this function fills the board if the tile that was revelead was blank and its surroundings are blank
void RevealBlankTiles(CellProperties board[][Max_Cols], int x, int y, GameSize boardSize)
{
	if (coordinate_validation(x, y, boardSize) && board[x][y].flag == false && (board[x][y].visible == false))
	{
		//if (board[x][y].value == 0)
		if (board[x][y].value == -1000) // will be chambged in second level
		{
			board[x][y].visible = true;
			RevealBlankTiles(board, x, y + 1, boardSize);
			RevealBlankTiles(board, x + 1, y + 1, boardSize);
			RevealBlankTiles(board, x - 1, y - 1, boardSize);
			RevealBlankTiles(board, x - 1, y + 1, boardSize);
			RevealBlankTiles(board, x + 1, y - 1, boardSize);
			RevealBlankTiles(board, x, y - 1, boardSize);
			RevealBlankTiles(board, x + 1, y, boardSize);
			RevealBlankTiles(board, x - 1, y, boardSize);
		}
		if (board[x][y].value > 0)
		{
			board[x][y].visible = true;
		}
	}
}
//this function unviels or marks the tile, according to the user's command
bool mark_tile(CellProperties board[][Max_Cols], int x, int y, char z, GameSize boardSize)
{
	if (board[x][y].flag == true)
	{
		board[x][y].flag = false;
		board[x][y].visible = false;
	}
	if (board[x][y].visible)
	{
		return false;
	}
	else
	{

		if (z == 'F')
			board[x][y].flag = true;
		else
		{
			board[x][y].flag = false;
			RevealBlankTiles(board, x, y, boardSize);
		}
		board[x][y].visible = true;
		return true;
	}
}
//this function checks whether the game was won
bool checkwin(CellProperties board[][Max_Cols], GameSize boardSize)
{
	for (int x = 0; x < boardSize.Cols; x++)
	{
		for (int y = 0; y < boardSize.Lines; y++)
		{
			if ((board[x][y].value != -1 && board[x][y].visible == false) || ((board[x][y].flag == true) && board[x][y].value != -1))
				return false;//didn't win yet
		}
	}
	return true; // user won
}

int main()
{
	int gameType = 0;
	GameSize boardSize = { 8,8 };
	CellProperties board[Max_Lines][Max_Cols] = { 0 };

	int x, y; // coordinates
	char command; // command

	//Main menu
	do {
		printf("Welcome to Minesweeper!\n\n");

		printf("Please choose one of the following options and enter it's number: \n\n");
		printf("1 - for size 8X8\n\n");
		printf("2 - for size 12X12\n\n");
		printf("3 - for size 15X15\n\n");
		printf("4 - for custom size \n\n");
		printf("0 - Exit\n\n");
		scanf("%d", &gameType);
		if (gameType == 0)
		{
			return 1;
		}
		setBoardSize(&boardSize, gameType);
	} while (gameType < 0 || gameType > 1);
	init(board, boardSize); // initiates the board
	//print_Open_board(board, boardSize);
	bool gameover = false;
	printf(" \n");
	print_board(board, boardSize);
	while (!gameover)
	{
		//printf("Please enter your move (O for opening the tile, F for flag), row and column : ");
		printf("Please enter your move, row and column : ");

		scanf("%d%d", &x, &y);
		//command = getchar();
		while (user_input_validation(x, y, 'O', boardSize) == false)
		{
			printf("Please enter a valid choice!\n");
			scanf("%d%d", &x, &y);
			//command = getchar();
		}
		//command = toupper(command);
		if (x == -1)
			OpenNextXCells(board, boardSize, y);
		else
		{
			bool mark_tile_succeeded = mark_tile(board, x, y, 'O', boardSize);
			if (mark_tile_succeeded)
			{
				printf(" \n");
			}
			else
			{
				printf("Invalid move, please enter valid choice!\n");
			}
		}
		print_board(board, boardSize);
		if (board[x][y].value == -1 && board[x][y].flag == false) // hitting a bomb
		{
			gameover = !gameover;
			printf("\nYou've hit a bomb! Game over!\n");
			print_Open_board(board, boardSize);
		}
		else if (checkwin(board, boardSize) == true) // winning the game
		{
			gameover = true;
			printf("\n You won!! congragulations! CHAMPION!\n");
		}
	}
	system("PAUSE");
}
