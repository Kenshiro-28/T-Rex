/*
 * NeuralLayer.c
 *
 *  Created on: Mar 10, 2020
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

NeuronErrorCode createNeuron(Neuron **myNeuron, int numberOfInputs)
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

		if (*myNeuron==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create weight array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuron)->weightArray = malloc(sizeof(NeuronWeight) * numberOfInputs);

		if ((*myNeuron)->weightArray==NULL)
		{
            free(*myNeuron);
            *myNeuron = NULL;
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	    }
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

NeuronErrorCode destroyNeuron(Neuron **myNeuron)
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

NeuronErrorCode cloneNeuron(Neuron *myNeuron, Neuron *myNeuronClone)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (myNeuronClone==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if (myNeuron->numberOfWeights != myNeuronClone->numberOfWeights)
		returnValue = NEURON_DIFFERENT_NEURONS_ERROR;

	//Copy weights
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		int size = sizeof(NeuronWeight) * myNeuron->numberOfWeights;
		
		memcpy(myNeuronClone->weightArray, myNeuron->weightArray, size);
	}

	return returnValue;
}

NeuronErrorCode mutateNeuron(Neuron *myNeuron)
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

//Neural layer operations

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

	//Create neural layer structure
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
	while ((returnValue==NEURON_RETURN_VALUE_OK) && (i<numberOfNeurons))
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

	if ((myNeuralLayer==NULL) || (*myNeuralLayer==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	//Destroy neurons
	while ((returnValue==NEURON_RETURN_VALUE_OK) && (i < (*myNeuralLayer)->numberOfNeurons))
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

	while ((returnValue==NEURON_RETURN_VALUE_OK) && (neuronIndex < myNeuralLayer->numberOfNeurons))
	{
		Neuron *myNeuron = myNeuralLayer->neuronArray[neuronIndex];

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
		returnValue = NEURON_NUMBER_OF_NEURONS_ERROR;

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

	while ((returnValue==NEURON_RETURN_VALUE_OK) && (i < myNeuralLayer->numberOfNeurons))
	{
		Neuron *myNeuron = myNeuralLayer->neuronArray[i];
		Neuron *myNeuronClone = myNeuralLayerClone->neuronArray[i];

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
