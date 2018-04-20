/*
 * NeuralNetwork.c
 *
 *  Created on: Sep 9, 2017
 *      Author: kenshiro
 */

#include "NeuralNetwork.h"

typedef struct neuralNetwork
{
	int numberOfInputs;
	NeuronData *inputLayer;
	NeuronData *neuralLayerInputArray;
	NeuronData *neuralLayerOutputArray;
	int numberOfHiddenLayers;
	NeuralLayer **hiddenLayerArray;
	int numberOfOutputs;
	NeuralLayer *outputLayer;
	NeuronData *neuralNetworkOutputArray;
} NeuralNetwork;


static NeuralNetworkErrorCode computeNeuralLayerOutput(NeuralLayer *myNeuralLayer, NeuronData *inputArray, int numberOfInputs, NeuronData *outputArray)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int numberOfNeurons;
	int neuronIndex = 0;

	NeuronErrorCode result = getNumberOfNeurons(myNeuralLayer, &numberOfNeurons);

	if (result!=NEURON_RETURN_VALUE_OK)
		returnValue = NEURAL_NETWORK_NEURON_ERROR;

	while ((neuronIndex<numberOfNeurons) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		Neuron *myNeuron;

		NeuronErrorCode result = getNeuron(myNeuralLayer, neuronIndex, &myNeuron);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_NEURON_ERROR;

		int inputIndex=0;

		//Set the input array as the neuron input
		while ((inputIndex<numberOfInputs) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			result = setNeuronInput(myNeuron, inputIndex, inputArray[inputIndex]);

			if (result!=NEURON_RETURN_VALUE_OK)
				returnValue = NEURAL_NETWORK_NEURON_ERROR;

			inputIndex++;
		}

		//Calculate neuron output
		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			result = computeNeuronOutput(myNeuron, &(outputArray[neuronIndex]));

			if (result!=NEURON_RETURN_VALUE_OK)
				returnValue = NEURAL_NETWORK_NEURON_ERROR;
		}

		neuronIndex++;
	}


	return returnValue;
}

NeuralNetworkErrorCode createNeuralNetwork(NeuralNetwork **myNeuralNetwork, int numberOfInputs, int numberOfHiddenLayers, int numberOfOutputs)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	static bool randomSeedInitialized = false;

	int neuronsPerHiddenLayer = numberOfInputs;
	NeuronErrorCode result;
	int i=0;

	//Initialize random seed
	if (!randomSeedInitialized)
	{
		srand(time(NULL));
		randomSeedInitialized = true;
	}

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if ((numberOfInputs<NEURAL_NETWORK_MINIMUM_NUMBER_OF_INPUTS) || (numberOfInputs>INT_MAX))
		returnValue = NEURAL_NETWORK_NUMBER_OF_INPUTS_ERROR;

	if ((numberOfHiddenLayers<NEURAL_NETWORK_MINIMUM_NUMBER_OF_HIDDEN_LAYERS) || (numberOfHiddenLayers>INT_MAX))
		returnValue = NEURAL_NETWORK_NUMBER_OF_HIDDEN_LAYERS_ERROR;

	if ((numberOfOutputs<NEURAL_NETWORK_MINIMUM_NUMBER_OF_NEURONS_PER_LAYER) || (numberOfOutputs>INT_MAX))
		returnValue = NEURAL_NETWORK_NUMBER_OF_NEURONS_PER_LAYER_ERROR;

	//Create neural network
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		*myNeuralNetwork = malloc(sizeof(NeuralNetwork));

		if (myNeuralNetwork==NULL)
			returnValue = NEURAL_NETWORK_MEMORY_ALLOCATION_ERROR;
	}

	//Create input layer
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		(*myNeuralNetwork)->numberOfInputs = numberOfInputs;
		(*myNeuralNetwork)->inputLayer = malloc(sizeof(NeuronData)*numberOfInputs);

		if ((*myNeuralNetwork)->inputLayer==NULL)
			returnValue = NEURAL_NETWORK_MEMORY_ALLOCATION_ERROR;
	}

	//Create auxiliary arrays
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		(*myNeuralNetwork)->neuralLayerInputArray = malloc(sizeof(NeuronData)*numberOfInputs);
		(*myNeuralNetwork)->neuralLayerOutputArray = malloc(sizeof(NeuronData)*numberOfInputs);

		if (((*myNeuralNetwork)->neuralLayerInputArray==NULL) || ((*myNeuralNetwork)->neuralLayerOutputArray==NULL))
			returnValue = NEURAL_NETWORK_MEMORY_ALLOCATION_ERROR;
	}

	//Create hidden layer array
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		(*myNeuralNetwork)->numberOfHiddenLayers = numberOfHiddenLayers;
		(*myNeuralNetwork)->hiddenLayerArray = malloc(sizeof(NeuralLayer*)*numberOfHiddenLayers);

 		if ((*myNeuralNetwork)->hiddenLayerArray==NULL)
			returnValue = NEURAL_NETWORK_MEMORY_ALLOCATION_ERROR;
	}

	//Create hidden layers
 	while ((i<numberOfHiddenLayers) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		NeuralLayer **myHiddenLayer = &((*myNeuralNetwork)->hiddenLayerArray[i]);

		result = createNeuronArray(myHiddenLayer, numberOfInputs, neuronsPerHiddenLayer);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_NEURON_ERROR;

		i++;
	}

 	//Create output layer
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		NeuralLayer **myOutputLayer = &((*myNeuralNetwork)->outputLayer);

		result = createNeuronArray(myOutputLayer, numberOfInputs, numberOfOutputs);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_NEURON_ERROR;
	}

	//Create neural network output array
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		(*myNeuralNetwork)->numberOfOutputs = numberOfOutputs;
		(*myNeuralNetwork)->neuralNetworkOutputArray = malloc(sizeof(NeuronData)*numberOfOutputs);

		if ((*myNeuralNetwork)->neuralNetworkOutputArray==NULL)
			returnValue = NEURAL_NETWORK_MEMORY_ALLOCATION_ERROR;
	}

	return returnValue;
}

NeuralNetworkErrorCode destroyNeuralNetwork(NeuralNetwork **myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;
	int i=0;

	if ((myNeuralNetwork==NULL) || (*myNeuralNetwork==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		//Free input layer
		free((*myNeuralNetwork)->inputLayer);

		//Free auxiliary arrays
		free((*myNeuralNetwork)->neuralLayerInputArray);
		free((*myNeuralNetwork)->neuralLayerOutputArray);

		//Destroy hidden layers
		while ((i<(*myNeuralNetwork)->numberOfHiddenLayers) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
		{
			NeuronErrorCode result = destroyNeuronArray(&((*myNeuralNetwork)->hiddenLayerArray[i]));

			if (result!=NEURON_RETURN_VALUE_OK)
				returnValue = NEURAL_NETWORK_NEURON_ERROR;

			i++;
		}
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		//Free hidden layer array
		free((*myNeuralNetwork)->hiddenLayerArray);

		//Destroy output layer
		NeuronErrorCode result = destroyNeuronArray(&((*myNeuralNetwork)->outputLayer));

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_NEURON_ERROR;
	}

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		//Free neural network output array
		free((*myNeuralNetwork)->neuralNetworkOutputArray);

		//Free neural network memory
		free(*myNeuralNetwork);
		*myNeuralNetwork = NULL;
	}

	return returnValue;
}

NeuralNetworkErrorCode getInputLayer(NeuralNetwork *myNeuralNetwork, NeuronData **myInputLayer, int *numberOfInputs)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	if ((myNeuralNetwork==NULL) || (myInputLayer==NULL) || (numberOfInputs==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		*myInputLayer = myNeuralNetwork->inputLayer;
		*numberOfInputs = myNeuralNetwork->numberOfInputs;
	}

	return returnValue;
}

NeuralNetworkErrorCode getNumberOfHiddenLayers(NeuralNetwork *myNeuralNetwork, int *numberOfHiddenLayers)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	if ((myNeuralNetwork==NULL) || (numberOfHiddenLayers==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		*numberOfHiddenLayers = myNeuralNetwork->numberOfHiddenLayers;

	return returnValue;
}

NeuralNetworkErrorCode getHiddenLayer(NeuralNetwork *myNeuralNetwork, int hiddenLayerNumber, NeuralLayer **myHiddenLayer)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	if ((myNeuralNetwork==NULL) || (myHiddenLayer==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if ((hiddenLayerNumber<0) || (hiddenLayerNumber>=myNeuralNetwork->numberOfHiddenLayers))
		returnValue = NEURAL_NETWORK_NUMBER_OF_HIDDEN_LAYERS_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		*myHiddenLayer = myNeuralNetwork->hiddenLayerArray[hiddenLayerNumber];

	return returnValue;
}

NeuralNetworkErrorCode getOutputLayer(NeuralNetwork *myNeuralNetwork, NeuralLayer **myOutputLayer, int *numberOfOutputs)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	if ((myNeuralNetwork==NULL) || (myOutputLayer==NULL) || (numberOfOutputs==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		*myOutputLayer = myNeuralNetwork->outputLayer;
		*numberOfOutputs = myNeuralNetwork->numberOfOutputs;
	}

	return returnValue;
}

NeuralNetworkErrorCode setNeuralNetworkInput(NeuralNetwork *myNeuralNetwork, int inputNumber, NeuronData input)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if ((inputNumber<0) || (inputNumber>=myNeuralNetwork->numberOfInputs))
		returnValue = NEURAL_NETWORK_NUMBER_OF_INPUTS_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		myNeuralNetwork->inputLayer[inputNumber] = input;

	return returnValue;
}

NeuralNetworkErrorCode computeNeuralNetworkOutput(NeuralNetwork *myNeuralNetwork, NeuronData **outputArray, int *numberOfOutputs)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int hiddenLayerIndex=0;
	int neuronsPerHiddenLayer;

	if ((myNeuralNetwork==NULL) || (outputArray==NULL) || (numberOfOutputs==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	//The first hidden layer input is the input layer
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		neuronsPerHiddenLayer = myNeuralNetwork->numberOfInputs;

		for (int i=0; i<neuronsPerHiddenLayer; i++)
			myNeuralNetwork->neuralLayerInputArray[i] = myNeuralNetwork->inputLayer[i];
	}

	//Feed hidden layers
	while ((hiddenLayerIndex<myNeuralNetwork->numberOfHiddenLayers) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		returnValue = computeNeuralLayerOutput(myNeuralNetwork->hiddenLayerArray[hiddenLayerIndex], myNeuralNetwork->neuralLayerInputArray, neuronsPerHiddenLayer, myNeuralNetwork->neuralLayerOutputArray);

		//The hidden layer output is the input of the next layer
		for (int i=0; i<neuronsPerHiddenLayer; i++)
			myNeuralNetwork->neuralLayerInputArray[i] = myNeuralNetwork->neuralLayerOutputArray[i];

		hiddenLayerIndex++;
	}

	//Feed output layer
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		returnValue = computeNeuralLayerOutput(myNeuralNetwork->outputLayer, myNeuralNetwork->neuralLayerInputArray, neuronsPerHiddenLayer, myNeuralNetwork->neuralNetworkOutputArray);

	//Get neural network output
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		*outputArray = myNeuralNetwork->neuralNetworkOutputArray;
		*numberOfOutputs = myNeuralNetwork->numberOfOutputs;
	}

	return returnValue;
}

NeuralNetworkErrorCode getNeuralNetworkOutput(NeuralNetwork *myNeuralNetwork, NeuronData **outputArray, int *numberOfOutputs)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	if ((myNeuralNetwork==NULL) || (outputArray==NULL) || (numberOfOutputs==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		*outputArray = myNeuralNetwork->neuralNetworkOutputArray;
		*numberOfOutputs = myNeuralNetwork->numberOfOutputs;
	}

	return returnValue;
}

NeuralNetworkErrorCode cloneNeuralNetwork(NeuralNetwork *myNeuralNetwork, NeuralNetwork *myNeuralNetworkClone)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int i=0;
	int numberOfHiddenLayers;
	NeuronErrorCode result;

	if ((myNeuralNetwork==NULL) || (myNeuralNetworkClone==NULL))
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	//Check if neural networks are different
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		if ((myNeuralNetwork->numberOfInputs!=myNeuralNetworkClone->numberOfInputs) ||
			(myNeuralNetwork->numberOfHiddenLayers!=myNeuralNetworkClone->numberOfHiddenLayers) ||
			(myNeuralNetwork->numberOfOutputs!=myNeuralNetworkClone->numberOfOutputs))

			returnValue = NEURAL_NETWORK_DIFFERENT_NEURAL_NETWORKS_ERROR;
		else
			numberOfHiddenLayers = myNeuralNetwork->numberOfHiddenLayers;
	}

	//Clone hidden layers
	while ((i<numberOfHiddenLayers) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		NeuralLayer *myHiddenLayer = myNeuralNetwork->hiddenLayerArray[i];
		NeuralLayer *myHiddenLayerClone = myNeuralNetworkClone->hiddenLayerArray[i];

		result = cloneNeuronArray(myHiddenLayer, myHiddenLayerClone);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_NEURON_ERROR;

		i++;
	}

	//Clone output layer
	if (returnValue == NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		result = cloneNeuronArray(myNeuralNetwork->outputLayer, myNeuralNetworkClone->outputLayer);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_NEURON_ERROR;
	}

	return returnValue;
}

NeuralNetworkErrorCode mutateNeuralNetwork(NeuralNetwork *myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	bool isMassiveMutation = false;
	int numberOfHiddenLayers;
	NeuronErrorCode result;
	int i=0;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue == NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		numberOfHiddenLayers = myNeuralNetwork->numberOfHiddenLayers;

		int randomPercent = (rand() % 100) + 1;

		if (randomPercent<=NEURAL_NETWORK_PERCENTAGE_OF_MASSIVE_MUTATIONS)
			isMassiveMutation = true;

		if (isMassiveMutation)
		{
			//Mutate hidden layers
			while ((i<numberOfHiddenLayers) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
			{
				result = mutateNeuronArray(myNeuralNetwork->hiddenLayerArray[i], isMassiveMutation);

				if (result!=NEURON_RETURN_VALUE_OK)
					returnValue = NEURAL_NETWORK_NEURON_ERROR;

				i++;
			}

			//Mutate output layer
			if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
			{
				result = mutateNeuronArray(myNeuralNetwork->outputLayer, isMassiveMutation);

				if (result!=NEURON_RETURN_VALUE_OK)
					returnValue = NEURAL_NETWORK_NEURON_ERROR;
			}
		}
		else
		{
			//The number of neural layers is the number of hidden layers plus the output layer
			int numberOfNeuralLayers = numberOfHiddenLayers + 1;
			int mutantNeuronLayer = rand() % numberOfNeuralLayers;

			if (mutantNeuronLayer<numberOfHiddenLayers)
				result = mutateNeuronArray(myNeuralNetwork->hiddenLayerArray[mutantNeuronLayer], isMassiveMutation);
			else
				result = mutateNeuronArray(myNeuralNetwork->outputLayer, isMassiveMutation);

			if (result!=NEURON_RETURN_VALUE_OK)
				returnValue = NEURAL_NETWORK_NEURON_ERROR;
		}
	}

	return returnValue;
}

