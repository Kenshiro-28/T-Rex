/*
 ============================================================================
 Name        : RunExample.c
 Author      : Kenshiro
 Version     :
 Copyright   :
 Description : Basic examples to learn how to use the T-Rex neural network
 ============================================================================
 */

#include "examples/XorExample.h"
#include "examples/TicTacToe.h"

#define SELECTED_EXAMPLE EXAMPLE_TIC_TAC_TOE

typedef enum
{
	EXAMPLE_XOR,
	EXAMPLE_TIC_TAC_TOE
} Example;

const Example selectedExample = SELECTED_EXAMPLE;

int main()
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	switch (selectedExample)
	{
		case EXAMPLE_XOR:
			returnValue = runXorExample();

			break;

		case EXAMPLE_TIC_TAC_TOE:
			returnValue = runTicTacToe();

			break;
	}

	if (returnValue!=NEURAL_NETWORK_RETURN_VALUE_OK)
		printf("\n\nERROR RUNNING EXAMPLE: %d\n\n", returnValue);

	return returnValue;
}
