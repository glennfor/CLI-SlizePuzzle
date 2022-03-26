


//slide puzzle

//note
//COORD defined in windows.h is a struct

//functions to improve

//solveboerd and rungame
//also check drawboard
//improve animations
//style application further
//can add a little windows programming ther eg messageboxes to get some input
//add time counter along side

#include "./main.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;

//[GAME CONSTANTS]

//number of distortions to make to the board
const auto RANDOMIZE_MOVES = 20;

//possible moves defined in MACROS.h
//are UP, DOWN, LEFT, RIGHT

//int csn be interpreted as chars and vice versa;
//i will use int(more natural to deal with)


char MOVES[] = {UP, DOWN, LEFT, RIGHT};

//opposite of each move

char RMOVES[] = {DOWN, UP, RIGHT, LEFT};



//GLOBAL VARIABLE YHAT HOLD ALL MOVES MADE BY THE PLAYER AND
//THE RANDOM ONES MADE BY THE COMPUTER


std::vector<int> allMoves;



void show_state(char state[],bool stay = false){
	TEXTCOLOR(VIOLET);
	GOTOXY(8, 3);
	std::cout <<"CURRENT STATE : ";
    TEXTCOLOR(GREEN);
	for(int i =0; i < state[i] != '\0'; ++ i){
		std::cout << state[i];
		Sleep(50);
	}
	std::cout << "                    ";
	
	if(not stay){
		Sleep(2000);
	}
	
}


void writeGameInfoToScreen();

bool checkForWin(int numbers[][4]){
	int nCheck = 1;
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			if(numbers[i][j] != (nCheck++)%16)
		    	return false;
	return true;
}

void showWelcomeScreen(){
    char message[] = "\n\n\n\n\n\tWELCOME TO SLIDE PUZZLE with CLI \n\n\n";
		
	int index = 0;
	clear_screen();
	while (message[index] != '\0'){
        TEXTCOLOR((index % 15)+1);
		std::cout <<message[index++]; Sleep(20);
	}
	Sleep(3000);
}

///main game run here
void runGame();


//function modifies the allmoves variable by appending a random move
//and adjusting the value in the array of numbers accordingly
//also blits and updates the screen
void generateRandomMoves(int nums[][4]);

//used by generateRandomMoves(int nums[][4]) to validate a move before appending and
//in the game to validate a players move

bool isValidMove(int move, int nums[][4]);

//update the array to keep up with the changing values
void updateArrayValues(int move, int nums[][4]);

//updates the values in updateArrayValues visually on the screen
void updateScreen(int, int (*)[4]);

void drawBoard(void);//draw the grid

void writeNumbersToScreen(int first_x_pos, int first_y_pos,int nums[][4],
								 size_t wait_time=50, size_t size = 16);//16 ie 4X4


void showGameWonAnimation(){
	clear_screen();
	char message[100] = "\n\n\n\n\n\n\n\tEXCELLENT\n\n\n"
		"\t\tYOU HAVE WON , CONGRATULATIONS";
	int index = 0;
	clear_screen();
	while (message[index] != '\0'){
        TEXTCOLOR((index % 15)+1);
		std::cout <<message[index++]; Sleep(70);
	}
	Beep(2000, 500);
	std::cout<<" Press any key to continue .... ";
	getch();
}

void showGameExitScreen(){
	clear_screen();
	TEXTCOLOR(SHINYGREEN);
	char message[] = "\n\n\n\n\n\n\n\tTHANKS FOR TRYING OUT THIS SIMPLE GAME  \n\n\n"
		"\t\tCONGRATULATIONS\n\n\n\t\t\tSEE YOU!!!!!!!";
	int index = 0;
	clear_screen();
	while (message[index] != '\0'){
        TEXTCOLOR((index % 15)+1);
		std::cout <<message[index++]; Sleep(70);
	}
	Beep(2000, 500);
	clear_screen();
	TEXTCOLOR(SHINYGREEN);
	GOTOXY(15, 10), std::cout<<"EXITING IN   ";
	for(int i = 5; i ; --i){
        TEXTCOLOR(RED);
        std::cout << "\b\b" << std::right << std::setw(2) << i;//fillchar('0')<<i;
        Sleep(1000);
	}
	exit(EXIT_SUCCESS);
}

void generateNewPuzzle(){
	runGame();
}

/****************************************************************************
 *                                                                          *
 * Function: main function AYK                                              *
 *                                                                          *
 * Purpose : executes the functions in logical order                        *
 *                                                                          *
 * History : Date      Reason --above                                       *
 *           --/--/--  Created                                              *
 *                                                                          *
 ****************************************************************************/


main(){
    showWelcomeScreen();
    clear_screen();
	runGame();
	getch();
}


//solve board won't check for valid moves
//because if a move is valid then so is the opposite
//uses allMoves to solve
void solveBoard(int board[][4], int wait){
	char msg[40] = "SOLVING . . .";
    show_state(msg,true);
    //reverse moves
    std::vector<int> reverseMoves(allMoves);
    std::reverse(reverseMoves.begin(), reverseMoves.end());    

    for(auto move: reverseMoves){
        auto reverseMove = move;
        switch (move){
            case UP:
            case UPx:
            case UPxl:
                reverseMove = DOWN;
                break;
            case DOWN:
            case DOWNx:
            case DOWNxl:
                reverseMove = UP;
                break;
            case LEFT:
            case LEFTx:
            case LEFTxl:
                reverseMove = RIGHT;
                break;
            case RIGHT:
            case RIGHTx:
            case RIGHTxl:
                reverseMove = LEFT;
        }
        updateScreen(reverseMove, board);
        Sleep(wait);
    }
    
    //swap all the values in the vector
   	strcpy(msg, "Puzzle Solved!!!");
    show_state(msg);
    
    //empty the vector of moves
    allMoves.clear();
}


/****************************************************************************
 *                                                                          *
 * Function: runGame                                                      *
 *                                                                          *
 * Purpose : implements the logic of the game        *
 *                                                                          *
 * History : Date      Reason                                               *
 *           --/--/--  Created                                              *
 *                                                                          *
 ****************************************************************************/


void runGame(){
	clear_screen();
	allMoves.clear();
	char msg[] ="Generating new puzzle...";
	show_state(msg, true);
	TEXTCOLOR(SHINYWHITE);
    drawBoard();
	int playerMove = 0xE7;//lambda
	int board[4][4],//represent boarditions
	y_pos,//represent the x and y axis
	x_pos,
	in = 1;
	
	//generating the main array for the game
	for(int i =0; i< 4; ++i)
	    for(int j = 0; j < 4; ++j)
	        board[i][j] = (in++)%16;
	        
	writeNumbersToScreen(5, 7,board);
	writeGameInfoToScreen();
	Sleep(1000);
	generateRandomMoves(board);
	strcpy(msg, "Solving puzzle......");
	show_state(msg, true);

    bool running = true;
	while(running){

		if(kbhit()){
  
			playerMove = getch();
		
			if(not playerMove){//function or arrow key pressed?
				ungetch(playerMove);
				playerMove = getch();
			}
        //validate and incoporate player move
            if(isValidMove(playerMove,board)){
                updateScreen(playerMove, board);
                allMoves.push_back(playerMove);
            }
            
            //check for win(array sorted as from the beggining)
            if(checkForWin(board)){
                showGameWonAnimation();
                generateNewPuzzle();
	        }
			
		//pARSING commands
  		switch (playerMove){
			case 'G':
			case 'g':
 				solveBoard(board, 100);
                 running = false;
				break;
			case 'E':
			case 'e':
				showGameExitScreen();//Exit_game
				break;
			case '?':
			case 'h':
				//no help available yet
				break;
			case 'N':
			case 'n':
				generateNewPuzzle();
				break;
			case 'Q'://could be a mistake  ; use WINAPI
			case 'q':
				TEXTCOLOR(YELLOW);
				clear_screen();
				GOTOXY(10, 10),std::cout<<"WARNING: Saving not yet available";
				Sleep(2000);
				clear_screen();
				showGameExitScreen();

			}
		}//end if  (kbhit())
		
	}

}


/****************************************************************************
 *                                                                          *
 * Function: drawBoard                                                      *
 *                                                                          *
 * Purpose : draw thegrid to the screen         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           --/--/--  Created                                              *
 *                                                                          *
 ****************************************************************************/

void drawBoard(void){//draw the grid

	char ch[12] ;
    int codes[12]= {179, 196,197,194,193,218,191,192,217,195,180};
    for(int i = 0; i < 12; i++)ch[i] =int(codes[i]);
	GOTOXY(2,1+4), std::cout <<ch[5];//top left corner
	GOTOXY(30, 1+4), std::cout << ch[6]; //top right corner
	GOTOXY(2,9+4+8), std::cout <<ch[7];//bottom left corner
	GOTOXY(30, 9+4+8), std::cout <<ch[8];//bottom right corner
	//middle vertical lines
	for(int i = 2; i <=30; i += 7)
		for(int j = 2; j <= 8+8; ++j){
        	GOTOXY(i, j+4), std::cout <<ch[0];
		}
 //joining of the box

    for(int i = 1; i <=27; i++)
		for(int j = 1; j <= 19; j +=4){//c
        	GOTOXY(i+2, j+4), std::cout <<ch[1];
		}

	//the junction
    for(int i = 2; i <=21; i+=7)//fixed
		for(int j = 1; j <= 7+6; j +=4)//c
        	GOTOXY(i+7, j+2+4-2), std::cout <<ch[2];
	//first line
    for(int i = 2; i <=22; i+=7)
        	GOTOXY(i+7, 1+4), std::cout <<ch[3];
	//second line
   for(int i = 2; i <=22; i+=7)
        	GOTOXY(i+7, 9+4+8), std::cout <<ch[4];
	//third line
   for(int i = 2; i <=10; i+=4)
        	GOTOXY(2, i+1+4+2), std::cout <<ch[9];
       //fourth line
   for(int i = 2; i <=10; i+=4)
        	GOTOXY(30, i+1+4+2), std::cout <<ch[10];

}
/****************************************************************************
 *                                                                          *
 * Function: writeNumbersToScreen                                                     *
 *                                                                          *
 * Purpose : updates the screen pn playermoves and computer choices         *
 *                                                                          *
 * History : Date      Reason                                               *
 *           --/--/--  Created                                              *
 *                                                                          *
 ****************************************************************************/

void writeNumbersToScreen(int first_x_pos, int first_y_pos,int nums[][4],
						 size_t sleep_time, size_t size){//16 ie 4X4
    int index = 0;
	for(int i= 0; i < 4; ++i)
    	for(int j = 0; j < 4; j++){
			TEXTCOLOR((index%15) + 1);
			GOTOXY(first_x_pos + j*7, first_y_pos + i*4);
			if(nums[i][j])
				std::cout <<std::setw(2)<<std::left<< nums[i][j];
			else
				std::cout << "  ";
			Sleep(sleep_time);
			++index;
		}
}

void writeGameInfoToScreen(){
	TEXTCOLOR(AQUA);
	GOTOXY(36, 6), std::cout << "COMMANDS:";
	GOTOXY(39, 8),std::cout<<"[N] = Start A New Game";
	GOTOXY(39, 9),std::cout<<"[E] = Exit";
	GOTOXY(39, 10),std::cout<<"[Q] = Save and Exit" "(Currently Unavailable)";
	GOTOXY(39, 11),std::cout<<"[G] = Solve current game";
	GOTOXY(39, 12),std::cout<<"[?] = Help(Not yet Available)";
	//a reset to cancel all users moves
	
	GOTOXY(36, 14),std::cout <<"NAVIGATION";
	
	GOTOXY(39, 16),std::cout<<"[\x1E] or [W] = UP   (Move next box in the same column)";
	GOTOXY(39, 17),std::cout<<"[\x11] or [A] = LEFT (Move previous box in the same row)" ;
	GOTOXY(39, 18),std::cout<<"[\x10] or [D] = RIGHT(Move next box in the same row)";
	GOTOXY(39, 19),std::cout<<"[\x1F] or [S] = DOWN (Move previous box in the same column)";
}



//function modifies the allmoves variable by appending a random move
//and adjusting the value in the array of numbers accordingly
//also blits and updates the screen
void generateRandomMoves(int nums[][4]){
	getch();
	//seeding the random number generator
	srand(time(NULL));
	
	int nValidMoves = 0;
	
	//reseeding the random number generator
	srand(time(NULL));

	while(nValidMoves < RANDOMIZE_MOVES){
		
		//get a random move
		int move = MOVES[rand()%4];
		if(isValidMove(move, nums)){
		    allMoves.push_back(move);
		    updateScreen(move, nums);
		    ++nValidMoves;
		    //wait for effect
			Sleep(100);
		}
        
	}
}




//used by generateRandomMoves(int nums[][4]) to validate a move before appending and
//in the game to validate a players move

bool isValidMove(int move, int nums[][4]){
	//first find the null position in the array
	//MUST be found or programme is brokken
	//no need to validate that here .....
	//4X4 is assumed the array size
		int dx = -1, dy = -1;//weird values are for debugging
	for(int i = 0; i < 4; ++i )
		for(int j = 0; j < 4; ++j)
		    if(not nums[i][j]){//if 0 spot found set dx and dy
		        dx = i;
		        dy = j;
		    }
 	//there are 4 scenarios  that can invalidate a move
 	bool
 	//moving up when the upper bound is already reached
	 movingUpCondition((move==UP or move==UPx or move==UPxl)and (dx == 0)),
	 //moving down when the lower bound is already reached
 	movingDownCondition((move==DOWN or move==DOWNx or move==DOWNxl) and (dx == 3)),//remember runs from 0 to 3
 	//moving left when the left bound is already reached
 	movingLeftCondition((move==LEFT or move==LEFTx or move==LEFTxl)and (dy == 0)),
 	//moving right when the right bound is already reached
 	movingRightCondition((move==RIGHT or move==RIGHTx or move==RIGHTxl) and (dy == 3));
 	
 	//any of the above scenarios true?
 	//the move is invalid.....
	if(movingUpCondition   or   movingDownCondition
		or   movingLeftCondition   or   movingRightCondition)
		return false;
		
	return true;
}


//function modifies the array variable by swapping values in response to a
// valid move (randomly or by the player)

//used exclusively(as of now) by generateRandomMoves(int nums[][4]){

void updateArrayValues(int move, int nums[][4]){
	//get the null position(ie containing the 0)
		//NOTE THAT THE MOVE IS VALIDATED BEFORE BEING PASSED TO THIS FUNCTION
	//no need to validate that here ...
	//4X4 is assumed the array size
	
	//these positions are defined for the null value(0 in this case)
	int curXPos = -1, curYPos = -1, newXPos = -1, newYPos = -1;
	//all indices are negative for the sake of debugging
		for(int i = 0; i < 4; ++i )
			for(int j = 0; j < 4; ++j)
		    	if(nums[i][j]==0){//if 0 spot found set dx and dy
		        	curXPos = i;
		     	    curYPos = j;
		  	  	}


	//switch statement to handle all cases
	//note that the default is not necesrry since the move can only be one of the the
	//4 in the global MOVES[] array
	if(isValidMove(move, nums)){
        //parsing navigational entries
        //commands are parsed in the rungame function
		switch(move) {
			case UP:
			case UPx:
			case UPxl:
				//set new positions
				//the column coordinate remains the same
				newYPos = curYPos;//moving toward the top ie towards 0
				newXPos = curXPos - 1;
				break;
			case DOWN:
			case DOWNx:
			case DOWNxl:
				//set new positions
				//the column coordinate remains the same
	            newYPos = curYPos;//moving toward the bottom ie towards 3
	            newXPos = curXPos + 1;
	            break;
			case LEFT:
			case LEFTx:
			case LEFTxl:
				//set new positions
				//the row coordinate is invariant
				
				newXPos = curXPos;//moving toward the left ie towards 0
				newYPos = curYPos - 1;
				break;
			case RIGHT:
			case RIGHTx:
			case RIGHTxl:
                //set new positions
				//the y coordinate is invariant

	            newXPos = curXPos;//moving toward the right ie towards 3
	            newYPos = curYPos + 1;
	            break;
			default:
				/*do nothing actually????@@@*/
				;
		}
		
		nums[curXPos][curYPos] = nums[newXPos][newYPos];
		nums[newXPos][newYPos] = 0;
		
	}
}

void updateScreen(int move, int nums[][4]){
    updateArrayValues(move, nums);
	writeNumbersToScreen(5, 7 ,nums, 0);
}

