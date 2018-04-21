/*
 * XorExample.c
 *
 *  Created on: Nov 5, 2017
 *      Author: kenshiro
 */

#include "XorExample.h"

#define NUMBER_OF_INPUTS 2
#define NUMBER_OF_HIDDEN_LAYERS 1
#define NUMBER_OF_OUTPUTS 1

#define NUMBER_OF_TEST_CASES 4
#define TARGET_FITNESS_SCORE NUMBER_OF_TEST_CASES

#define NEURAL_NETWORK_FILE_NAME "xor.json"

static const NeuronData xorInput[NUMBER_OF_TEST_CASES][NUMBER_OF_INPUTS] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
static const NeuronData xorOutput[NUMBER_OF_TEST_CASES] = {0, 1, 1, 0};


static NeuralNetworkErrorCode evaluateFitness(NeuralNetwork *myNeuralNetwork, int *fitnessScore)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int xorInputIndex = 0;
	int score = 0;

	NeuronData *neuralNetworkOutput;
	int numberOfOutputs;

	if ((myNeuralNetwork==NULL) || (fitnessScore==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	while ((xorInputIndex<NUMBER_OF_TEST_CASES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		int neuralNetworkInputIndex = 0;

		while ((neuralNetworkInputIndex<NUMBER_OF_INPUTS) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			NeuronData neuralNetworkInput = xorInput[xorInputIndex][neuralNetworkInputIndex];

			returnValue = setNeuralNetworkInput(myNeuralNetwork, neuralNetworkInputIndex, neuralNetworkInput);

			neuralNetworkInputIndex++;
		}

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = computeNeuralNetworkOutput(myNeuralNetwork, &neuralNetworkOutput, &numberOfOutputs);

		//Evaluate output
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			if (neuralNetworkOutput[0]==xorOutput[xorInputIndex])
				score++;
		}

		xorInputIndex++;
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		*fitnessScore = score;

	return returnValue;
}

static NeuralNetworkErrorCode trainNeuralNetwork(NeuralNetwork **myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	NeuralNetwork *myNeuralNetworkClone;

	int generationNumber = 1;
	int myNeuralNetworkScore = 0;
	int myNeuralNetworkCloneScore = 0;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = createNeuralNetwork(&myNeuralNetworkClone, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	while ((myNeuralNetworkScore<TARGET_FITNESS_SCORE) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		returnValue = evaluateFitness(*myNeuralNetwork, &myNeuralNetworkScore);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = evaluateFitness(myNeuralNetworkClone, &myNeuralNetworkCloneScore);

		//Set the best scoring neural network as the reference
		if (myNeuralNetworkCloneScore>myNeuralNetworkScore)
		{
			NeuralNetwork *auxNeuralNetwork = *myNeuralNetwork;
			*myNeuralNetwork = myNeuralNetworkClone;
			myNeuralNetworkClone = auxNeuralNetwork;

			myNeuralNetworkScore = myNeuralNetworkCloneScore;
		}

		printf("\nCurrent generation: %d, score: %d", generationNumber, myNeuralNetworkScore);

		//Clone the best neural network
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = cloneNeuralNetwork(*myNeuralNetwork, myNeuralNetworkClone);

		//Mutate the neural network clone
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = mutateNeuralNetwork(myNeuralNetworkClone);

		generationNumber++;
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetworkClone);

	return returnValue;
}

static NeuralNetworkErrorCode printNeuralNetworkResults(NeuralNetwork *myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	NeuronData *neuralNetworkOutput;

	int numberOfOutputs = 0;
	int xorInputIndex = 0;

	//Print trained network results
	while ((xorInputIndex<NUMBER_OF_TEST_CASES) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		int neuralNetworkInputIndex = 0;

		while ((neuralNetworkInputIndex<NUMBER_OF_INPUTS) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			NeuronData neuralNetworkInput = xorInput[xorInputIndex][neuralNetworkInputIndex];

			returnValue = setNeuralNetworkInput(myNeuralNetwork, neuralNetworkInputIndex, neuralNetworkInput);

			neuralNetworkInputIndex++;
		}

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = computeNeuralNetworkOutput(myNeuralNetwork, &neuralNetworkOutput, &numberOfOutputs);

		printNeuralNetwork(myNeuralNetwork);

		xorInputIndex++;
	}

	return returnValue;
}

NeuralNetworkErrorCode runXorExample()
{
	printf("\n----- XOR EXAMPLE -----\n");

	NeuralNetwork *myNeuralNetwork;
	NeuralNetwork *myLoadedNeuralNetwork;

	NeuralNetworkErrorCode returnValue = createNeuralNetwork(&myNeuralNetwork, NUMBER_OF_INPUTS, NUMBER_OF_HIDDEN_LAYERS, NUMBER_OF_OUTPUTS);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = trainNeuralNetwork(&myNeuralNetwork);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\n\n\nShowing the output of the trained neural network\n\n");
		returnValue = printNeuralNetworkResults(myNeuralNetwork);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\n\nSaving the trained neural network in a json file\n");
		returnValue = saveNeuralNetwork(NEURAL_NETWORK_FILE_NAME, myNeuralNetwork);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\nLoading the trained neural network from the json file\n");
		returnValue = loadNeuralNetwork(NEURAL_NETWORK_FILE_NAME, &myLoadedNeuralNetwork);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\nShowing the output of the neural network loaded from the json file\n\n\n");
		returnValue = printNeuralNetworkResults(myLoadedNeuralNetwork);
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myNeuralNetwork);

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = destroyNeuralNetwork(&myLoadedNeuralNetwork);

	return returnValue;
}
