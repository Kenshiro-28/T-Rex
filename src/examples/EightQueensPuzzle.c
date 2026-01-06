/*
 * EightQueensPuzzle.c
 *
 *  Created on: Dec 8, 2017
 *      Author: kenshiro
 */

#include "EightQueensPuzzle.h"

#define NUMBER_OF_ROWS 8
#define NUMBER_OF_COLUMNS NUMBER_OF_ROWS
#define NUMBER_OF_SQUARES (NUMBER_OF_ROWS * NUMBER_OF_COLUMNS)

#define LEGAL_NUMBER_OF_QUEENS NUMBER_OF_ROWS

//The number of inputs is the number of squares in the game board
#define NUMBER_OF_INPUTS NUMBER_OF_SQUARES

#define NUMBER_OF_HIDDEN_LAYERS 2

//The output of the neural network is the position of the queens in the game board
#define NUMBER_OF_OUTPUTS NUMBER_OF_SQUARES

/*If the game score does not improve after N generations the current evolutionary
 *branch is abandoned and a new evolutionary branch is created from scratch*/
#define MAXIMUM_NUMBER_OF_GENERATIONS_WITHOUT_IMPROVING_SCORE 1000

#define NEURAL_NETWORK_FILE_NAME "eight_queens_puzzle.json"

typedef enum
{
	SQUARE_QUEEN_MARK = 'Q',
	SQUARE_EMPTY = ' ',
    SQUARE_UNINITIALIZED = 0
} Square;

//The game score is the number of queens not threatened
typedef struct gameBoard
{
	Square gameBoard[NUMBER_OF_COLUMNS][NUMBER_OF_ROWS];
	int numberOfQueens;
	int threatenedQueens;
	int gameScore;
} GameBoard;

static void printGameBoard(GameBoard *myGameBoard)
{
	printf("\n\n");

	for (int y=0; y<NUMBER_OF_ROWS; y++)
	{
		for (int x=0; x<NUMBER_OF_COLUMNS; x++)
			printf("----");

		printf("-\n");

		for (int x=0; x<NUMBER_OF_COLUMNS; x++)
			printf("| %c ", myGameBoard->gameBoard[x][y]);

		printf("|\n");
	}

	for (int x=0; x<NUMBER_OF_COLUMNS; x++)
		printf("----");

	printf("-\n\n\n\n");
}

static bool isQueenMovement(int sourceX, int sourceY, int destinationX, int destinationY)
{
	  bool isQueenMovement = false;

	  int xDifference = abs(sourceX - destinationX);
	  int yDifference = abs(sourceY - destinationY);

	  bool isSameHorizontalLine = sourceX==destinationX;
	  bool isSameVerticalLine = sourceY==destinationY;
	  bool isSameDiagonalLine = xDifference==yDifference;
	  bool isAnotherSquare = (sourceX!=destinationX) || (sourceY!=destinationY);

	  if ((isSameHorizontalLine || isSameVerticalLine || isSameDiagonalLine) && isAnotherSquare)
		  isQueenMovement = true;

	  return isQueenMovement;
}

static void checkQueenThreat(int x1, int y1, GameBoard *myGameBoard)
{
	for (int x2=0; x2<NUMBER_OF_COLUMNS; x2++)
		for (int y2=0; y2<NUMBER_OF_ROWS; y2++)
			if ((myGameBoard->gameBoard[x2][y2]==SQUARE_QUEEN_MARK) &&
				(isQueenMovement(x1, y1, x2, y2)))

				myGameBoard->threatenedQueens++;
}

static void checkQueenDeployment(GameBoard *myGameBoard)
{
	for (int x1=0; x1<NUMBER_OF_COLUMNS; x1++)
		for (int y1=0; y1<NUMBER_OF_ROWS; y1++)
			if (myGameBoard->gameBoard[x1][y1]==SQUARE_QUEEN_MARK)
			{
				myGameBoard->numberOfQueens++;
				checkQueenThreat(x1, y1, myGameBoard);
			}

	myGameBoard->gameScore = myGameBoard->numberOfQueens - myGameBoard->threatenedQueens;
}

static NeuralNetworkErrorCode enableInputNeurons(NeuralNetwork *myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int inputNumber = 0;

	while ((inputNumber<NUMBER_OF_INPUTS) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		returnValue = setNeuralNetworkInput(myNeuralNetwork, inputNumber, NEURON_DATA_ONE);
		inputNumber++;
	}

	return returnValue;
}

static void evaluateNeuralNetworkOutput(NeuronData *neuralNetworkOutput, int numberOfOutputs, GameBoard *myGameBoard)
{
	int neuralNetworkOutputIndex = 0;

	int x=0;
	int y=0;

	while (neuralNetworkOutputIndex < numberOfOutputs)
	{
		if (neuralNetworkOutput[neuralNetworkOutputIndex]==NEURON_DATA_ONE)
			myGameBoard->gameBoard[x][y] = SQUARE_QUEEN_MARK;
		else if (neuralNetworkOutput[neuralNetworkOutputIndex]==NEURON_DATA_ZERO)
			myGameBoard->gameBoard[x][y] = SQUARE_EMPTY;

		neuralNetworkOutputIndex++;

		x++;

		if (x>=NUMBER_OF_COLUMNS)
		{
			y++;
			x=0;
		}
	}

	checkQueenDeployment(myGameBoard);
}

static void initializeGameBoard(GameBoard *myGameBoard)
{
	myGameBoard->numberOfQueens = 0;
	myGameBoard->threatenedQueens = 0;
	myGameBoard->gameScore = 0;
}

static NeuralNetworkErrorCode playEightQueensPuzzle(NeuralNetwork *myNeuralNetwork, GameBoard *myGameBoard)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	//Neural network output
	NeuronData *neuralNetworkOutput = NULL;
	int numberOfOutputs = 0;

	if ((myNeuralNetwork==NULL) || (myGameBoard==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;
	else
		initializeGameBoard(myGameBoard);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		//Enable all input neurons since the output does not depend on the input
		returnValue = enableInputNeurons(myNeuralNetwork);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = computeNeuralNetworkOutput(myNeuralNetwork, &neuralNetworkOutput, &numberOfOutputs);

		if ((numberOfOutputs!=NUMBER_OF_SQUARES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
			returnValue = NEURAL_NETWORK_NUMBER_OF_OUTPUT_NEURONS_ERROR;

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			evaluateNeuralNetworkOutput(neuralNetworkOutput, numberOfOutputs, myGameBoard);
	}

	return returnValue;
}

static NeuralNetworkErrorCode trainNeuralNetwork(NeuralNetwork **myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	NeuralNetwork *myNeuralNetworkClone = NULL;

	GameBoard myGameBoard = {0};

	bool trainingCompleted = false;
	int generationNumber = 0;
	int myNeuralNetworkScore = -INT_MAX;
	int generationsWithoutImprovingScore = 0;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;
	else
		returnValue = createNeuralNetwork(&myNeuralNetworkClone, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	//Play the game until T-Rex wins
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
			returnValue = playEightQueensPuzzle(myNeuralNetworkClone, &myGameBoard);
			printf("Generation: %d - Game score: %d\n", generationNumber, myGameBoard.gameScore);
		}

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			//Select the best neural network as the reference
			if (myGameBoard.gameScore > myNeuralNetworkScore)
			{
				myNeuralNetworkScore = myGameBoard.gameScore;

				NeuralNetwork *auxNeuralNetwork = *myNeuralNetwork;
				*myNeuralNetwork = myNeuralNetworkClone;
				myNeuralNetworkClone = auxNeuralNetwork;

				generationsWithoutImprovingScore = 0;
			}
			else
			{
				generationsWithoutImprovingScore++;

				if (generationsWithoutImprovingScore>MAXIMUM_NUMBER_OF_GENERATIONS_WITHOUT_IMPROVING_SCORE)
				{
					printf("\n%d generations without improving the game score, starting a new evolutionary branch ...\n\n", MAXIMUM_NUMBER_OF_GENERATIONS_WITHOUT_IMPROVING_SCORE);

					generationNumber = 0;
					myNeuralNetworkScore = -INT_MAX;

					returnValue = destroyNeuralNetwork(myNeuralNetwork);

					if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
						returnValue = createNeuralNetwork(myNeuralNetwork, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);
				}
			}

			if (myGameBoard.gameScore==LEGAL_NUMBER_OF_QUEENS)
				trainingCompleted = true;
		}
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetworkClone);

	return returnValue;
}

NeuralNetworkErrorCode runEightQueensPuzzle(void)
{
	printf("\n----- EIGHT QUEENS PUZZLE -----\n\n");

	NeuralNetwork *myNeuralNetwork, *myLoadedNeuralNetwork;
	GameBoard myGameBoard, myLoadedGameBoard;

	NeuralNetworkErrorCode returnValue = createNeuralNetwork(&myNeuralNetwork, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = trainNeuralNetwork(&myNeuralNetwork);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = playEightQueensPuzzle(myNeuralNetwork, &myGameBoard);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\n\n\nShowing the output of the trained neural network\n\n");
		printGameBoard(&myGameBoard);
		printf("Saving the trained neural network in a json file\n");

		returnValue = saveNeuralNetwork(NEURAL_NETWORK_FILE_NAME, myNeuralNetwork);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\nLoading the trained neural network from the json file\n");
		returnValue = loadNeuralNetwork(NEURAL_NETWORK_FILE_NAME, &myLoadedNeuralNetwork);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\nShowing the output of the neural network loaded from the json file\n\n");
		returnValue = playEightQueensPuzzle(myLoadedNeuralNetwork, &myLoadedGameBoard);
		printGameBoard(&myLoadedGameBoard);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetwork);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myLoadedNeuralNetwork);

	return returnValue;
}
