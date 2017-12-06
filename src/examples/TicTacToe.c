/*
 * TicTacToe.c
 *
 *  Created on: Nov 6, 2017
 *      Author: kenshiro
 */

#include "TicTacToe.h"

#define NUMBER_OF_ROWS 3
#define NUMBER_OF_COLUMNS NUMBER_OF_ROWS
#define NUMBER_OF_SQUARES (NUMBER_OF_ROWS * NUMBER_OF_COLUMNS)

/*The number of inputs is the number of squares in two game boards (one for circles
 *and other for crosses) plus an extra input always active (this prevents all entries
 *in the neural network from being zero when the game board is empty.)*/
#define NUMBER_OF_INPUTS (NUMBER_OF_SQUARES*2 + 1)

#define NUMBER_OF_HIDDEN_LAYERS 2

/*The neural network always plays with circles. The output of the neural network
 *is the position of the new circle in the game board*/
#define NUMBER_OF_OUTPUTS (NUMBER_OF_SQUARES)

#define NUMBER_OF_MOVEMENTS_PER_TURN 1

typedef enum
{
	PLAYER_MARK_CIRCLE = 0x4F,
	PLAYER_MARK_CROSS = 0x58,
	PLAYER_MARK_FREE = 0x20
} PlayerMark;

typedef enum
{
	GAME_RESULT_NO_WINNER,
	GAME_RESULT_PLAYER_CIRCLE_WINS,
	GAME_RESULT_PLAYER_CROSS_WINS,
	GAME_RESULT_DRAW,
	GAME_RESULT_ILLEGAL_MOVEMENT
} GameResult;

//The player's score is the highest number of marks in any line that does not contain any mark of the opponent
typedef struct gameBoard
{
	char gameBoard[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS];
	int circlePlayerScore;
	int crossPlayerScore;
	GameResult gameResult;
} GameBoard;

static void printGameBoard(GameBoard *myGameBoard)
{
	printf("\n");

	for (int i=0; i<NUMBER_OF_ROWS; i++)
	{
		for (int j=0; j<NUMBER_OF_COLUMNS; j++)
			printf("----");

		printf("-\n");

		for (int j=0; j<NUMBER_OF_COLUMNS; j++)
			printf("| %c ", myGameBoard->gameBoard [i][j]);

		printf("|\n");
	}

	for (int j=0; j<NUMBER_OF_COLUMNS; j++)
		printf("----");

	printf("-\n\n");
}

//Update the player score with the number of circles and crosses of a new row, column or diagonal
static void updatePlayerScore(GameBoard *myGameBoard, int numberOfCircles, int numberOfCrosses)
{
	if ((numberOfCircles > myGameBoard->circlePlayerScore) && (numberOfCrosses==0))
		myGameBoard->circlePlayerScore = numberOfCircles;

	if ((numberOfCrosses > myGameBoard->crossPlayerScore) && (numberOfCircles==0))
		myGameBoard->crossPlayerScore = numberOfCrosses;

	if (numberOfCircles==NUMBER_OF_COLUMNS)
		myGameBoard->gameResult = GAME_RESULT_PLAYER_CIRCLE_WINS;
	else if (numberOfCrosses==NUMBER_OF_COLUMNS)
		myGameBoard->gameResult = GAME_RESULT_PLAYER_CROSS_WINS;
}

static void checkHorizontalLines(GameBoard *myGameBoard)
{
	int i=0;

	while ((i<NUMBER_OF_ROWS) && (myGameBoard->gameResult==GAME_RESULT_NO_WINNER))
	{
		int numberOfCircles = 0;
		int numberOfCrosses = 0;

		int j=0;

		while (j<NUMBER_OF_COLUMNS)
		{
			if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_CIRCLE)
				numberOfCircles++;
			else if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_CROSS)
				numberOfCrosses++;

			j++;
		}

		updatePlayerScore(myGameBoard, numberOfCircles, numberOfCrosses);

		i++;
	}
}

static void checkVerticalLines(GameBoard *myGameBoard)
{
	int i=0;

	while ((i<NUMBER_OF_COLUMNS) && (myGameBoard->gameResult==GAME_RESULT_NO_WINNER))
	{
		int numberOfCircles = 0;
		int numberOfCrosses = 0;

		int j=0;

		while (j<NUMBER_OF_ROWS)
		{
			if (myGameBoard->gameBoard[j][i]==PLAYER_MARK_CIRCLE)
				numberOfCircles++;
			else if (myGameBoard->gameBoard[j][i]==PLAYER_MARK_CROSS)
				numberOfCrosses++;

			j++;
		}

		updatePlayerScore(myGameBoard, numberOfCircles, numberOfCrosses);

		i++;
	}
}

static void checkDiagonals(GameBoard *myGameBoard)
{
	int i=0;
	int j=0;
	int numberOfCircles = 0;
	int numberOfCrosses = 0;

	//Check first diagonal
	while ((i<NUMBER_OF_ROWS) && (j<NUMBER_OF_COLUMNS))
	{
		if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_CIRCLE)
			numberOfCircles++;
		else if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_CROSS)
			numberOfCrosses++;

		i++;
		j++;
	}

	updatePlayerScore(myGameBoard, numberOfCircles, numberOfCrosses);

	i=0;
	j=NUMBER_OF_COLUMNS-1;

	numberOfCircles = 0;
	numberOfCrosses = 0;

	//Check second diagonal
	if (myGameBoard->gameResult==GAME_RESULT_NO_WINNER)
	{
		while ((i<NUMBER_OF_ROWS) && (j>=0))
		{
			if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_CIRCLE)
				numberOfCircles++;
			else if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_CROSS)
				numberOfCrosses++;

			i++;
			j--;
		}
	}

	updatePlayerScore(myGameBoard, numberOfCircles, numberOfCrosses);
}

static void checkStalemate(GameBoard *myGameBoard)
{
	bool stalemate = true;

	int i=0;

	while ((i<NUMBER_OF_ROWS) && (stalemate))
	{
		int j=0;

		while ((j<NUMBER_OF_COLUMNS) && (stalemate))
		{
			if (myGameBoard->gameBoard[i][j]==PLAYER_MARK_FREE)
				stalemate = false;

			j++;
		}

		i++;
	}

	if (stalemate)
		myGameBoard->gameResult = GAME_RESULT_DRAW;
}

static void getPlayerMarkCoordinates(NeuronData *neuralNetworkOutput, int numberOfOutputs, int *rowNumber, int *columnNumber, GameBoard *myGameBoard)
{
	bool coordinatesFound = false;
	int neuralNetworkOutputIndex = 0;

	int i=0;
	int j=0;

	while ((neuralNetworkOutputIndex<numberOfOutputs) && (myGameBoard->gameResult==GAME_RESULT_NO_WINNER))
	{
		if (neuralNetworkOutput[neuralNetworkOutputIndex]==NEURON_DATA_ONE)
		{
			if (!coordinatesFound)
			{
				*rowNumber = i;
				*columnNumber = j;
				coordinatesFound = true;
			}
			else
			{
				myGameBoard->gameResult = GAME_RESULT_ILLEGAL_MOVEMENT;
			}
		}

		neuralNetworkOutputIndex++;

		j++;

		if (j>=NUMBER_OF_COLUMNS)
		{
			i++;
			j=0;
		}
	}
}

static void evaluateMovement(int rowNumber, int columnNumber, PlayerMark myPlayerMark, GameBoard *myGameBoard)
{
	if (myGameBoard->gameBoard[rowNumber][columnNumber]==PLAYER_MARK_FREE)
	{
		myGameBoard->gameBoard[rowNumber][columnNumber] = myPlayerMark;

		checkStalemate(myGameBoard);

		if (myGameBoard->gameResult==GAME_RESULT_NO_WINNER)
			checkHorizontalLines(myGameBoard);

		if (myGameBoard->gameResult==GAME_RESULT_NO_WINNER)
			checkVerticalLines(myGameBoard);

		if (myGameBoard->gameResult==GAME_RESULT_NO_WINNER)
			checkDiagonals(myGameBoard);
	}
	else
		myGameBoard->gameResult = GAME_RESULT_ILLEGAL_MOVEMENT;
}

static void evaluateNeuralNetworkOutput(NeuronData *neuralNetworkOutput, int numberOfOutputs, GameBoard *myGameBoard)
{
	int rowNumber = 0;
	int columnNumber = 0;

	getPlayerMarkCoordinates(neuralNetworkOutput, numberOfOutputs, &rowNumber, &columnNumber, myGameBoard);

	if (myGameBoard->gameResult==GAME_RESULT_NO_WINNER)
		evaluateMovement(rowNumber, columnNumber, PLAYER_MARK_CIRCLE, myGameBoard);
}

static NeuralNetworkErrorCode setGameBoardAsNeuralNetworkInput(GameBoard *myGameBoard, NeuralNetwork *myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	//Create input arrays from the game board
	NeuronData circleInputArray[NUMBER_OF_SQUARES];
	NeuronData crossInputArray[NUMBER_OF_SQUARES];

	int inputArrayIndex = 0;

	for (int i=0; i<NUMBER_OF_ROWS; i++)
		for (int j=0; j<NUMBER_OF_COLUMNS; j++)
		{
			PlayerMark myPlayerMark = myGameBoard->gameBoard[i][j];

			if (myPlayerMark==PLAYER_MARK_CIRCLE)
			{
				circleInputArray[inputArrayIndex] = NEURON_DATA_ONE;
				crossInputArray[inputArrayIndex] = NEURON_DATA_ZERO;
			}
			else if (myPlayerMark==PLAYER_MARK_CROSS)
			{
				circleInputArray[inputArrayIndex] = NEURON_DATA_ZERO;
				crossInputArray[inputArrayIndex] = NEURON_DATA_ONE;
			}
			else if (myPlayerMark==PLAYER_MARK_FREE)
			{
				circleInputArray[inputArrayIndex] = NEURON_DATA_ZERO;
				crossInputArray[inputArrayIndex] = NEURON_DATA_ZERO;
			}

			inputArrayIndex++;
		}

	inputArrayIndex = 0;

	//Set the input arrays as the neural network input
	while ((inputArrayIndex<NUMBER_OF_SQUARES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		returnValue = setNeuralNetworkInput(myNeuralNetwork, inputArrayIndex, circleInputArray[inputArrayIndex]);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = setNeuralNetworkInput(myNeuralNetwork, inputArrayIndex + NUMBER_OF_SQUARES, crossInputArray[inputArrayIndex]);

		inputArrayIndex++;
	}

	//Set the extra input as active
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = setNeuralNetworkInput(myNeuralNetwork, NUMBER_OF_INPUTS - 1, NEURON_DATA_ONE);

	return returnValue;
}

static void initializeGameBoard(GameBoard *myGameBoard)
{
	for (int i=0; i<NUMBER_OF_ROWS; i++)
		for (int j=0; j<NUMBER_OF_COLUMNS; j++)
			myGameBoard->gameBoard[i][j] = PLAYER_MARK_FREE;

	myGameBoard->circlePlayerScore = 0;
	myGameBoard->crossPlayerScore = 0;
	myGameBoard->gameResult = GAME_RESULT_NO_WINNER;
}

static void computeClassicAIoutput(GameBoard *myGameBoard)
{
	GameBoard bestMovementGameBoard = *myGameBoard;
	bool movementFound = false;

	for (int i=0; i<NUMBER_OF_ROWS; i++)
		for (int j=0; j<NUMBER_OF_COLUMNS; j++)
		{
			GameBoard myGameBoardCopy = *myGameBoard;

			evaluateMovement(i, j, PLAYER_MARK_CROSS, &myGameBoardCopy);

			if ((myGameBoardCopy.gameResult!=GAME_RESULT_ILLEGAL_MOVEMENT) &&
				(myGameBoardCopy.crossPlayerScore>=bestMovementGameBoard.crossPlayerScore))
			{
				bestMovementGameBoard = myGameBoardCopy;
				movementFound = true;
			}
		}

	if (movementFound)
		*myGameBoard = bestMovementGameBoard;
	else
		printf("\nCLASSIC AI WAS UNABLE TO FIND A MOVEMENT\n");
}

static void printNewGameMessage()
{
	printf("\n---------- NEW GAME ----------\n");
	printf("-                            -\n");
	printf("- T-Rex mark: O              -\n");
	printf("- Classic AI mark: X         -\n");
	printf("-                            -\n");
	printf("------------------------------\n");
}

static NeuralNetworkErrorCode playGameAgainstClassicAI(NeuralNetwork *myNeuralNetwork, GameBoard *myGameBoard)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	//Neural network output
	NeuronData *neuralNetworkOutput;
	int numberOfOutputs;

	if ((myNeuralNetwork==NULL) || (myGameBoard==NULL))
	{
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;
	}
	else
	{
		initializeGameBoard(myGameBoard);
		printNewGameMessage();
	}

	while ((myGameBoard->gameResult==GAME_RESULT_NO_WINNER) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		//T-Rex's movement
		returnValue = setGameBoardAsNeuralNetworkInput(myGameBoard, myNeuralNetwork);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = computeNeuralNetworkOutput(myNeuralNetwork, &neuralNetworkOutput, &numberOfOutputs);

		if ((numberOfOutputs!=NUMBER_OF_SQUARES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
			returnValue = NEURAL_NETWORK_NUMBER_OF_OUTPUT_NEURONS_ERROR;

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			evaluateNeuralNetworkOutput(neuralNetworkOutput, numberOfOutputs, myGameBoard);

		if ((myGameBoard->gameResult!=GAME_RESULT_ILLEGAL_MOVEMENT) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			printf("\nT-Rex moves - Score: %d\n", myGameBoard->circlePlayerScore);
			printGameBoard(myGameBoard);
		}
		else
			printf("\nGAME OVER: T-Rex has tried an illegal movement\n\n");

		//Classic AI's movement
		if ((myGameBoard->gameResult==GAME_RESULT_NO_WINNER) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			computeClassicAIoutput(myGameBoard);

			if (myGameBoard->gameResult!=GAME_RESULT_ILLEGAL_MOVEMENT)
			{
				printf("\nClassic AI moves - Score: %d\n", myGameBoard->crossPlayerScore);
				printGameBoard(myGameBoard);
			}
		}

		if (myGameBoard->gameResult==GAME_RESULT_PLAYER_CIRCLE_WINS)
			printf("\nT-Rex wins the game\n\n");
		else if (myGameBoard->gameResult==GAME_RESULT_PLAYER_CROSS_WINS)
			printf("\nClassic AI wins the game\n\n");
		else if (myGameBoard->gameResult==GAME_RESULT_DRAW)
			printf("\nDraw\n\n");
	}

	return returnValue;
}

static bool isTRexVictoryOrDraw(GameBoard *myGameBoard)
{
	bool isTRexVictoryOrDraw = (myGameBoard->gameResult==GAME_RESULT_PLAYER_CIRCLE_WINS) ||
			   	   	   	       (myGameBoard->gameResult==GAME_RESULT_DRAW);

	return isTRexVictoryOrDraw;
}

static NeuralNetworkErrorCode trainNeuralNetwork(NeuralNetwork **myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	NeuralNetwork *myNeuralNetworkClone;

	GameBoard myGameBoard;

	bool trainingCompleted = false;
	int generationNumber = 0;
	int myNeuralNetworkScore = 0;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;
	else
		returnValue = createNeuralNetwork(&myNeuralNetworkClone, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	//Play the game until T-Rex wins or the game ends in a draw
	while ((!trainingCompleted) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		generationNumber++;

		//Clone the reference neural network
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = cloneNeuralNetwork(*myNeuralNetwork, myNeuralNetworkClone);

		//Mutate the neural network clone
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = mutateNeuralNetwork(myNeuralNetworkClone);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			returnValue = playGameAgainstClassicAI(myNeuralNetworkClone, &myGameBoard);
			printf("Generation: %d\n\n\n", generationNumber);
		}

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			//Select the best neural network as the reference
			if (myGameBoard.circlePlayerScore > myNeuralNetworkScore)
			{
				myNeuralNetworkScore = myGameBoard.circlePlayerScore;

				NeuralNetwork *auxNeuralNetwork = *myNeuralNetwork;
				*myNeuralNetwork = myNeuralNetworkClone;
				myNeuralNetworkClone = auxNeuralNetwork;
			}

			if (isTRexVictoryOrDraw(&myGameBoard))
				trainingCompleted = true;
		}
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetworkClone);

	return returnValue;
}

NeuralNetworkErrorCode runTicTacToe()
{
	printf("\n----- TIC-TAC-TOE -----\n");

	NeuralNetwork *myNeuralNetwork;

	NeuralNetworkErrorCode returnValue = createNeuralNetwork(&myNeuralNetwork, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = trainNeuralNetwork(&myNeuralNetwork);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetwork);

	return returnValue;
}
