#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BOARD_SIZE 7
#define REQ_DISTANCE 3

typedef int bool;
enum { false, true };

struct players	//array of structs for each player
{
    char name[50];			//variable to store characters name
    char type[50];			//variable to store the character type
    char slotType[50];		//variable to store the characters slot type
    int slotCol;
    int slotRow;			//variable to store the characters slot number
    int numType; 			//variable to limit user input (e.g. Human = 1)
    int lifePoints, smartness, strength, magicSkills, luck, dexterity ;		//player attributes
} players[6];

struct slot{
	//row number
	int row;
	//column number
	int column;

	// adjacent left slot
	struct slot *left;
	// adjacent right slot
	struct slot *right;
	// adjacent up slot
	struct slot *up;
	// adjacent down slot
	struct slot *down;
}slot;

void playerInfo(int i, int numPlayers);
void printPlayers(int i, int numPlayers);
void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight);
struct slot *  reachDesiredElement(int row, int column, struct slot * initialSlot);
int getDesiredElement(int maxsize, int * row, int * col);
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[7][7]);


int main(void){


	srand((unsigned)time(NULL));

	int i = 0, y, x, j = 0, check = 0, numPlayers, numSlots, choice, temp;			//variables for various parts of the program

	//ensures the user inputs a number between 2 and 6
	do{
		printf("\nPlease enter the number of players you wish to play with: ");		//asks for user input
		scanf("%d", &numPlayers);													//records user input

		if (numPlayers == 1)														//if the user wants to play with 1 player
			printf("\nThis game is played with more than 2 players!\n");
		else if (numPlayers <= 1 || numPlayers > 6)									//if the choice is valid prints an error and repeats
			printf("\nMax players is 6, please enter a valid choice!\n");
		}while (numPlayers <= 1 || numPlayers > 6);

    printf("-----------------------------------\n");
    printf("         PLAYER INFORMATION        \n");
    printf("-----------------------------------\n");

    playerInfo(i, numPlayers);				//assigns capabilities to each player

    printf("-----------------------------------\n");
    printf("         PLAYER ATTRIBUTES         \n");
    printf("-----------------------------------\n");

    printPlayers(i, numPlayers);			//prints the data information for each player




	int row, column;

	struct slot* currSlot = NULL;
	struct slot *foundSlots;
	bool explored[BOARD_SIZE][BOARD_SIZE];
	int count =0;

	//pointer to slot (0,0)
	struct slot *upLeft;

	//pointer to slot (0,boardSize -1)
	struct slot *upRight;

	//pointer to slot (boardSize - 1, 0)
	struct slot *downLeft;

	//pointer to slot (boardSize - 1, boardSize -1)
	struct slot *downRight;


	//Creates the board
	createBoard(BOARD_SIZE,&upLeft, &upRight, &downLeft, &downRight);

	for(i=0;i<numPlayers;i++){

		printf("%s is in ", players[i].name);
		getDesiredElement(BOARD_SIZE, &row, &column);
		printf("\n");
	}
	

	/*If the the required slot is closer to the down-right
	 * corner of the board the search starts from downRight,
	 * which points to slot at position (boardSize-1, boarSize -1)*/
	if(row >= BOARD_SIZE/2){
		if(column >= BOARD_SIZE/2)
			currSlot = reachDesiredElement(row,column,downRight);
		else
		/*If the the required slot is closer to the down-left
		* corner of the board the search starts from downLeft,
		* which points to slot at position (boardSize-1, 0)*/
			currSlot = reachDesiredElement(row,column,downLeft);
	} else {
		/*If the the required slot is closer to the up-right
		* corner of the board the search starts from upRight,
		* which points to slot at position (0, boarSize -1)*/
		if(column >= BOARD_SIZE/2)
			currSlot = reachDesiredElement(row,column, upRight);
		/*If the the required slot is closer to the up-left
		* corner of the board the search starts from upLeft,
		* which points to slot at position (0, 0)*/
		else currSlot = reachDesiredElement(row,column,upLeft);
	}

	for(int i=0; i<BOARD_SIZE; i++){
		for(int j=0; j<BOARD_SIZE;j++){
			explored[i][j] = false;
		}
	}

	foundSlots = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(struct slot ));
	printf("\n\nFunction findSlotsinvoked:\n");

	if(currSlot!= NULL){
		//invokes function findSlots. The required distance is a constant
		//However you can reuse this function providing as input any arbitrary distance
		findSlots(REQ_DISTANCE, 0, currSlot, foundSlots, &count, explored);
		for(int i=0; i<count; i++){
			printf("(%d, %d)-> ",foundSlots[i].row, foundSlots[i].column);
		}
	}


	return 0;
}



void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight){

	//The board is represented as a pointer of pointer to slots
	//This allocates in memory the space for the pointers to each row of the board
	struct slot ** board = malloc(boardSize * sizeof(struct slot *));


	for(int i =0; i< boardSize; i++){
		//This allocates in memory the space for the slots in each row of the board
		board[i] = malloc(boardSize * sizeof(struct slot));

		//For each slot it sets up the row and column number
		for(int j=0;j < boardSize; j++){
			board[i][j].row = i;
			board[i][j].column = j;
			printf("(%d, %d)", i, j);
		}
		printf("\n");
	}

	//It sets up the adjacent slots for the slots that are
	//not at the borders. These slots have 4 adjacent elements
	for(int i =1; i< boardSize-1; i++){
		for(int j=1;j < boardSize-1; j++){
			board[i][j].up = &board[i-1][j];
			board[i][j].right = &board[i][j+1];
			board[i][j].down = &board[i+1][j];
			board[i][j].left = &board[i][j-1];
		}
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last row, except the slots at the edges.
	//
	for(int j = 1; j < boardSize -1; j++){
		//It sets up the adjacent slots for the slots that are in the first row.
		//These slots don't have an adjacent element on top of them
		// because they are on the first row of the board
		board[0][j].right = &board[0][j+1];
		board[0][j].left = &board[0][j-1];
		board[0][j].down = &board[1][j];
		board[0][j].up = NULL;

		//It sets up the adjacent slots for the slots that are in the last row.
		//These slots don't have an adjacent element on down them
		// because they are on the last row of the board
		board[boardSize - 1][j].right = &board[boardSize - 1][j+1];
		board[boardSize -1][j].left = &board[boardSize - 1][j-1];
		board[boardSize - 1][j].up = &board[boardSize - 2][j];
		board[boardSize - 1][j].down = NULL;
	}

	//It sets up the adjacent slots for the slots that are
	//in the first and the last column, except the slots at the edges.
	//
	for(int i = 1; i < boardSize -1; i++){
		//It sets up the adjacent slots for the slots that are in the first column.
		//These slots don't have an adjacent element on the left
		// because they are on the first column of the board
		board[i][0].right = &board[i][1];
		board[i][0].up = &board[i-1][0];
		board[i][0].down = &board[i+1][0];
		board[i][0].left = NULL;

		//It sets up the adjacent slots for the slots that are in the last column.
		//These slots don't have an adjacent element on the right
		// because they are on the last column of the board
		board[i][boardSize-1].left = &board[i][boardSize-2];
		board[i][boardSize -1].up = &board[i -1][boardSize-1];
		board[i][boardSize -1].down = &board[i+1][boardSize -1];
		board[i][boardSize -1].right = NULL;
	}


		//It sets up the adjacent slots for the slot at position (0,0).
		//This only has only 2 adjacent slots: right and down
		board[0][0].right = &board[0][1];
		board[0][0].down = &board[1][0];

		//It sets up the adjacent slots for the slot at position (0,boardSize -1).
		//This only has only 2 adjacent slots: left and down
		board[0][boardSize-1].left = &board[0][boardSize-2];
		board[0][boardSize -1].down = &board[1][boardSize -1];

		//It sets up the adjacent slots for the slot at position (boarSize -1 ,0).
		//This only has only 2 adjacent slots: up and right
		board[boardSize -1][0].right = &board[boardSize -1][1];
		board[boardSize -1][0].up = &board[boardSize -2][0];

		//It sets up the adjacent slots for the slot at position (boarSize -1 ,boardSize-1).
		//This only has only 2 adjacent slots: up and left
		board[boardSize - 1][boardSize-1].up = &board[boardSize-2][boardSize-1];
		board[boardSize - 1][boardSize -1].left = &board[boardSize -1][boardSize -2];


	//assigns a pointer to slot at position (0, 0)
	*upLeft = &board[0][0];
	//assigns pointer of pointer to slot at position (0, boardSize -1)
	*upRight = &board[0][boardSize -1];
	//assigns pointer of pointer to slot at position ( boardSize -1,)
	*downLeft = &board[boardSize -1][0];
	//assigns pointer of pointer to slot at position (boardSize -1, boardSize -1)
	*downRight = &board[boardSize -1][boardSize -1];
	}

	struct slot * reachDesiredElement(int row, int column, struct slot * initialSlot){

	bool found = false;
	//current slot
	struct slot * currentSlot = initialSlot;

	printf("\nFunction reachDesiredElement invoked\n");

	//prints the column and the row of the initial slot from which the search starts
	printf("Initial slot (%d, %d) -> \n",initialSlot->row,initialSlot->column);


	//while the slot is not found
	while(found == false){


		//if the row of the current slot is > of the row of the desired slot,
		//we move up
		if(currentSlot->row > row){
			//the current slot now points to the slot one row up
			currentSlot = currentSlot->up;
			//prints the column and the row of the current slot
			printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);
		}
		//if the row of the current slot is < of the row of the desired slot,
		//we move down
		if(currentSlot->row < row){
			//the current slot now points to the slot one row down
			currentSlot = currentSlot->down;
			//prints the row and the column of the current slot
			printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);

		}
		//if the column of the current slot is > of the column of the desired slot,
		//we move left
		if(currentSlot->column > column){

			//the current slot now points to the slot one column left
			currentSlot = currentSlot->left;
			//prints the row and the column of the current slot
			printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);
		}

		//if the column of the current slot is < of the column of the desired slot,
		//we move right
		if(currentSlot->column < column){

			//the current slot now points to the slot one column right
			currentSlot = currentSlot->right;
			//prints the row and the column of the current slot
			printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);

		}
		//if the current slot is at a column and a row equal to the desired column and row, respectively
		// we found the slot
		if(currentSlot->column == column && currentSlot->row == row){
			printf("Found\n");
			found = true;

		}

	}
	//returns the found slot
	return currentSlot;
}

int getDesiredElement(int boardSize, int * row, int * col){


		//random number generation
		srand((unsigned)time(NULL));

		int x=0;
		int y=0;
		//it will cycle asking the user to insert the row
		//until the value of the desired row is >= 0 or < of the
		// size of the board
			x = rand()%6;
			*row = x;

		//it will cycle asking the user to insert the column
		//until the value of the desired row is >= 0 or < of the
		// size of the board

			y = rand()%6;
			*col = y;
			printf("(%d,%d)\n", *row, *col);
}



void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[BOARD_SIZE][BOARD_SIZE]){



	//The base case: the current slot is at the required distance from the starting slot
	if(currDist == reqDist){
		//the current slot was not explored
		if(explored[currSlot->row][currSlot->column] == false){
			//The next availbale position (indicated by count) at foundSlots points to the current slot
			*(foundSlots + *count) = *currSlot;
			//the counter is incremented
			(*count)++;
			//the matrix of the explored slots set to true the element at the row and column of the current slot
			explored[currSlot->row][currSlot->column] = true;
		}
	}
	//The recursive call: the current slot is at a distance that is less than the required distance (more slots still have to be traversed)
	else{
		//if the current slot has the up slot != NULL (i.e. the slot is not in the first row)
		if(currSlot->up != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the up slot
			findSlots(reqDist, currDist +1,  currSlot->up, foundSlots, count, explored);
		}
		//if the current slot has the right slot != NULL (i.e. the slot is not in the last column)
		if(currSlot->right != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the right slot
			findSlots(reqDist, currDist +1,  currSlot->right, foundSlots, count, explored);
		}
		//if the current slot has the down slot != NULL (i.e. the slot is not in the last row)
		if(currSlot->down != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the down slot
			findSlots(reqDist, currDist +1,  currSlot->down, foundSlots, count, explored);
		}
		//if the current slot has the left slot != NULL (i.e. the slot is not in the first column)
		if(currSlot->left != NULL){
			//invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the left slot
			findSlots(reqDist, currDist +1,  currSlot->left, foundSlots, count, explored);
		}

	}


}

void playerInfo(int i, int numPlayers)
{
	for(i = 0; i< numPlayers; ++i)	//loop around each player to input their details
	{
		printf("\n-----Player number %d-----\n", i + 1);	//heading for player number

	    printf("Enter name: ");
	    scanf("%s",players[i].name);						//stores users name in the struct

	    //ensure the user prints a number between 1 and 4 for player type
	    do{
	    	printf("Enter type (1 = Human || 2 = Ogre || 3 = Elf || 4 = Wizard): ");
	        scanf("%d",&players[i].numType);				//records user input for player type

	        if( players[i].numType > 4 || players[i].numType <= 0)
	        	printf("Please enter a valid choice\n");	//prints error if not a valid option
	        }while( players[i].numType > 4 || players[i].numType <= 0);

	    if( players[i].numType == 1)			//if this player is a human
	    {
	    	strcpy(players[i].type, "Human");	//copies the string human to the player type
	    	players[i].lifePoints = 100;			//initializes life points at 100

	    	//loops until the sum of all capabilities are less than 300
	    	do{
	    		players[i].smartness = rand () % 100;
	    		players[i].strength = rand () % 100;
	    		players[i].magicSkills = rand () % 100;
	    		players[i].luck = rand () % 100;
	    		players[i].dexterity = rand () % 100;
	    	   } while ((players[i].smartness + players[i].strength + players[i].magicSkills +
	    	    		 players[i].luck + players[i].dexterity) >= 300);
	    }
	    else if(players[i].numType == 2)		//if this player is an ogre
	    {
	    	strcpy(players[i].type, "Ogre");		//copies the string ogre to the player type
	    	players[i].lifePoints = 100;			//initializes life points at 100

	    	players[i].magicSkills = 0;
	    	players[i].dexterity = (rand() % (100 - 80)) + 80;
	    	players[i].strength = (rand() % (100 - 80)) + 80;

	    	//loops until sum of smartness and luck is less than 50
	    	do{
	    		players[i].smartness = rand () % 20;
	    	    players[i].luck = rand () % 100;
	    	  } while ((players[i].smartness + players[i].luck) > 50);
	    }
	    else if(players[i].numType == 3)		//if this player is an elf
	    {
	    	strcpy(players[i].type, "Elf");		//copies the string elf to the player type
	        players[i].lifePoints = 100;			//initializes life points at 100

	        //assigns values to each of the players capabilities
	        players[i].luck = (rand() % (100 - 60)) + 60;
	        players[i].smartness = (rand() % (100 - 70)) + 70;
	        players[i].strength = rand() % 50;
	        players[i].magicSkills = (rand() % (80 - 50)) + 50;
	        players[i].dexterity = rand () % 100;
	    }
	    else if(players[i].numType == 4)		//if this player is a wizard
	    {
	        strcpy(players[i].type, "Wizard");	//copies the string wizard to the player type
	        players[i].lifePoints = 100;			//initializes life points at 100

	        //assigns values to each of the players capabilities
	        players[i].luck = (rand() % (100 - 50)) + 50;
	        players[i].smartness = (rand() % (100 - 90)) + 90;
	        players[i].strength = rand() % 20;
	        players[i].magicSkills = (rand() % (100 - 80)) + 80;
	        players[i].dexterity = rand () % 100;
	    }

	}
}
void printPlayers(int i, int numPlayers)
{
	for(i=0; i < numPlayers; ++i)		//loops around each player to print their capabilities
	{
		printf("\n------------Player Number %d-----------\n",i+1);
	    printf("Name:\t\t\t%s\n", players[i].name);
	    printf("Type:\t\t\t%s\n", players[i].type);
	    printf("Life Points:\t\t%d\n", players[i].lifePoints);
	    printf("Smartness:\t\t%d\n", players[i].smartness);
	    printf("Strength:\t\t%d\n", players[i].strength);
	    printf("Magic Skills:\t\t%d\n", players[i].magicSkills);
	    printf("Luck:\t\t\t%d\n", players[i].luck);
	    printf("Dexterity:\t\t%d\n", players[i].dexterity);
	 }
}