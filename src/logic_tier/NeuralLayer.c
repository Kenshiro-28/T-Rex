/*
 * Neuron.c
 *
 *  Created on: Sep 9, 2017
 *      Author: kenshiro
 */

#include "NeuralLayer.h"

typedef struct neuron
{
	int numberOfWeights;
	NeuronWeight *weightArray;
} Neuron;

typedef struct neuralLayer
{
	int numberOfInputs;
	int numberOfNeurons;
	Neuron **neuronArray;
} NeuralLayer;

//Neuron operations

static NeuronErrorCode createNeuron(Neuron **myNeuron, int numberOfInputs)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;
	
	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((numberOfInputs<NEURAL_LAYER_MINIMUM_NUMBER_OF_INPUTS) || (numberOfInputs>INT_MAX))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	//Create neuron
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		*myNeuron = malloc(sizeof(Neuron));

		if (myNeuron==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create weight array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuron)->weightArray = malloc(sizeof(NeuronWeight) * numberOfInputs);

		if ((*myNeuron)->weightArray==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Initialize
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuron)->numberOfWeights = numberOfInputs;

		for (int i=0; i<numberOfInputs; i++)
		{
			int randomWeight = rand() % 2;

			if (randomWeight)
				(*myNeuron)->weightArray[i] = NEURON_WEIGHT_POSITIVE;
			else
				(*myNeuron)->weightArray[i] = NEURON_WEIGHT_NEGATIVE;
		}
	}

	return returnValue;
}

static NeuronErrorCode destroyNeuron(Neuron **myNeuron)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (*myNeuron==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		free((*myNeuron)->weightArray);
		free(*myNeuron);
		*myNeuron = NULL;
	}

	return returnValue;
}

static NeuronErrorCode cloneNeuron(Neuron *myNeuron, Neuron *myNeuronClone)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (myNeuronClone==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if (myNeuron->numberOfWeights != myNeuronClone->numberOfWeights)
		returnValue = NEURON_DIFFERENT_NEURONS_ERROR;

	//Copy weights
	for (int i=0; i < myNeuron->numberOfWeights; i++)
		myNeuronClone->weightArray[i] = myNeuron->weightArray[i];

	return returnValue;
}

static NeuronErrorCode mutateNeuron(Neuron *myNeuron)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		//Calculate a random number of mutations
		int numberOfMutations = (rand() % myNeuron->numberOfWeights) + 1;

		for (int i=0; i < numberOfMutations; i++)
		{
			int randomWeight = rand() % myNeuron->numberOfWeights;

			if (myNeuron->weightArray[randomWeight] == NEURON_WEIGHT_NEGATIVE)
				myNeuron->weightArray[randomWeight] = NEURON_WEIGHT_POSITIVE;
			else
				myNeuron->weightArray[randomWeight] = NEURON_WEIGHT_NEGATIVE;
		}
	}

	return returnValue;
}

NeuronErrorCode getNumberOfInputs(Neuron *myNeuron, int *numberOfInputs)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (numberOfInputs==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*numberOfInputs = myNeuron->numberOfWeights;

	return returnValue;
}

NeuronErrorCode getNeuronWeight(Neuron *myNeuron, int inputNumber, NeuronWeight *inputWeight)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (inputWeight==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if ((inputNumber < 0) || (inputNumber >= myNeuron->numberOfWeights))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;
	
	if (returnValue==NEURON_RETURN_VALUE_OK)
		*inputWeight = myNeuron->weightArray[inputNumber];

	return returnValue;
}

NeuronErrorCode setNeuronWeight(Neuron *myNeuron, int inputNumber, NeuronWeight inputWeight)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if ((inputNumber < 0) || (inputNumber >= myNeuron->numberOfWeights))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		myNeuron->weightArray[inputNumber] = inputWeight;

	return returnValue;
}

NeuronErrorCode computeNeuronOutput(Neuron *myNeuron, NeuronData *inputArray, NeuronData *neuronOutput)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (inputArray==NULL) || (neuronOutput==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		int inputSum = 0;

		for (int i=0; i < myNeuron->numberOfWeights; i++)
			inputSum = inputSum + inputArray[i] * myNeuron->weightArray[i];
		
		if (inputSum>0)
			*neuronOutput = NEURON_DATA_ONE;
		else
			*neuronOutput = NEURON_DATA_ZERO;
	}

	return returnValue;
}

//Neuron array operations

NeuronErrorCode createNeuralLayer(NeuralLayer **myNeuralLayer, int numberOfInputs, int numberOfNeurons)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;

	if (myNeuralLayer==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((numberOfInputs<NEURAL_LAYER_MINIMUM_NUMBER_OF_INPUTS) || (numberOfInputs<numberOfNeurons) || (numberOfInputs>INT_MAX))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if ((numberOfNeurons<NEURAL_LAYER_MINIMUM_NUMBER_OF_NEURONS) || (numberOfNeurons>INT_MAX))
		returnValue = NEURON_NUMBER_OF_NEURONS_ERROR;

	//Create neuron array structure
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		*myNeuralLayer = malloc(sizeof(NeuralLayer));

		if (*myNeuralLayer==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create neuron array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuralLayer)->neuronArray = malloc(sizeof(Neuron*) * numberOfNeurons);

		if ((*myNeuralLayer)->neuronArray==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create neurons
	while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron;

		returnValue = createNeuron(&myNeuron, numberOfInputs);

		if (returnValue==NEURON_RETURN_VALUE_OK)
			(*myNeuralLayer)->neuronArray[i] = myNeuron;

		i++;
	}

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuralLayer)->numberOfInputs = numberOfInputs;
		(*myNeuralLayer)->numberOfNeurons = numberOfNeurons;
	}

	return returnValue;
}

NeuronErrorCode destroyNeuralLayer(NeuralLayer **myNeuralLayer)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;
	int numberOfNeurons;

	if ((myNeuralLayer==NULL) || (*myNeuralLayer==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else
		numberOfNeurons = (*myNeuralLayer)->numberOfNeurons;

	//Destroy neurons
	while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron = (*myNeuralLayer)->neuronArray[i];
		returnValue = destroyNeuron(&myNeuron);
		i++;
	}

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		free((*myNeuralLayer)->neuronArray);
		free(*myNeuralLayer);
		*myNeuralLayer = NULL;
	}

	return returnValue;
}

NeuronErrorCode computeNeuralLayerOutput(NeuralLayer *myNeuralLayer, NeuronData *inputArray, NeuronData *outputArray)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int neuronIndex = 0;

	if ((myNeuralLayer==NULL) || (inputArray==NULL) || (outputArray==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	//Check input array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		float inputArrayLength = sizeof(inputArray) / (float) sizeof(NeuronData);

		if (inputArrayLength!=myNeuralLayer->numberOfInputs)
			returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;
	}

	//Check output array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		float outputArrayLength = sizeof(outputArray) / (float) sizeof(NeuronData);

		if (outputArrayLength!=myNeuralLayer->numberOfNeurons)
			returnValue = NEURON_NUMBER_OF_OUTPUTS_ERROR;
	}

	//Compute the output of the neurons
	while ((neuronIndex < myNeuralLayer->numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron = myNeuralLayer->neuronArray[neuronIndex];

		if (returnValue==NEURON_RETURN_VALUE_OK)
			returnValue = computeNeuronOutput(myNeuron, inputArray, &(outputArray[neuronIndex]));

		neuronIndex++;
	}

	return returnValue;
}

NeuronErrorCode getNumberOfNeurons(NeuralLayer* myNeuralLayer, int *numberOfNeurons)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuralLayer==NULL) || (numberOfNeurons==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*numberOfNeurons = myNeuralLayer->numberOfNeurons;

	return returnValue;
}

NeuronErrorCode getNeuron(NeuralLayer *myNeuralLayer, int neuronNumber, Neuron **myNeuron)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuralLayer==NULL) || (myNeuron==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if ((neuronNumber<0) || (neuronNumber>=myNeuralLayer->numberOfNeurons))
		returnValue = NEURON_NUMBER_OF_NEURONS_ERROR ;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*myNeuron = myNeuralLayer->neuronArray[neuronNumber];

	return returnValue;
}

NeuronErrorCode cloneNeuralLayer(NeuralLayer *myNeuralLayer, NeuralLayer *myNeuralLayerClone)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;

	if ((myNeuralLayer==NULL) || (myNeuralLayerClone==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if (myNeuralLayer->numberOfNeurons!=myNeuralLayerClone->numberOfNeurons)
		returnValue = NEURON_DIFFERENT_NEURAL_LAYERS_ERROR;

	while ((i < myNeuralLayer->numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron = myNeuralLayer->neuronArray[i];
		Neuron *myNeuronClone = myNeuralLayerClone->neuronArray[i];

		if (returnValue==NEURON_RETURN_VALUE_OK)
			returnValue = cloneNeuron(myNeuron, myNeuronClone);

		i++;
	}

	return returnValue;
}

NeuronErrorCode mutateNeuralLayer(NeuralLayer *myNeuralLayer, bool isMassiveMutation)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if (myNeuralLayer==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;
	
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		if (isMassiveMutation)
		{
			int i=0;

			while ((i < myNeuralLayer->numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
			{
				Neuron *mutantNeuron = myNeuralLayer->neuronArray[i];
				returnValue = mutateNeuron(mutantNeuron);

				i++;
			}
		}
		else
		{
			int mutantNeuronIndex = rand() % myNeuralLayer->numberOfNeurons;
			Neuron *mutantNeuron = myNeuralLayer->neuronArray[mutantNeuronIndex];
			returnValue = mutateNeuron(mutantNeuron);
		}
	}
	
	return returnValue;
}