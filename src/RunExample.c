/*
 ============================================================================
 Name        : T-Rex
 Author      : Kenshiro
 Version     : 2.00
 Copyright   : GNU General Public License (GPLv3)
 Description : T-Rex is an evolutionary neural network
 ============================================================================
 */

#include "examples/XorExample.h"
#include "examples/TicTacToe.h"
#include "examples/EightQueensPuzzle.h"

#define SELECTED_EXAMPLE EXAMPLE_EIGHT_QUEENS_PUZZLE

typedef enum
{
	EXAMPLE_XOR,
	EXAMPLE_TIC_TAC_TOE,
	EXAMPLE_EIGHT_QUEENS_PUZZLE
} Example;

int main()
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	switch (SELECTED_EXAMPLE)
	{
		case EXAMPLE_XOR:
			returnValue = runXorExample();
			break;

		case EXAMPLE_TIC_TAC_TOE:
			returnValue = runTicTacToe();
			break;

		case EXAMPLE_EIGHT_QUEENS_PUZZLE:
			returnValue = runEightQueensPuzzle();
			break;
	}

	if (returnValue!=NEURAL_NETWORK_RETURN_VALUE_OK)
		printf("\n\nNEURAL NETWORK ERROR CODE: %d\n\n", returnValue);

	return returnValue;
}
