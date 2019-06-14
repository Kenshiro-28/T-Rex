/*
 * Neuron.c
 *
 *  Created on: Sep 9, 2017
 *      Author: kenshiro
 */

#include "Neuron.h"

typedef struct neuron
{
	int numberOfInputs;
	NeuronData *inputArray;
	NeuronWeight *weightArray;
} Neuron;

typedef struct neuronArray
{
	int numberOfNeurons;
	Neuron **neuronArray;
} NeuronArray;

//Neuron operations

NeuronErrorCode createNeuron(Neuron **myNeuron, int numberOfInputs)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;
	
	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((numberOfInputs<NEURON_MINIMUM_NUMBER_OF_INPUTS) || (numberOfInputs>INT_MAX))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	//Create neuron
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		*myNeuron = malloc(sizeof(Neuron));

		if (myNeuron==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create input array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuron)->inputArray = malloc(sizeof(NeuronData) * numberOfInputs);

		if ((*myNeuron)->inputArray==NULL)
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
		(*myNeuron)->numberOfInputs = numberOfInputs;

		for (int i=0; i<numberOfInputs; i++)
		{
			int randomWeight = rand() % 2;

			(*myNeuron)->inputArray[i] = NEURON_DATA_ZERO;

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
		free((*myNeuron)->inputArray);
		free((*myNeuron)->weightArray);
		free(*myNeuron);
		*myNeuron = NULL;
	}

	return returnValue;
}

NeuronErrorCode getNumberOfInputs(Neuron *myNeuron, int *numberOfInputs)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (numberOfInputs==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*numberOfInputs = myNeuron->numberOfInputs;

	return returnValue;
}

NeuronErrorCode setNeuronInput(Neuron *myNeuron, int inputNumber, NeuronData inputValue)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if ((inputNumber < 0) || (inputNumber >= myNeuron->numberOfInputs))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;
	
	if (returnValue==NEURON_RETURN_VALUE_OK)
		myNeuron->inputArray[inputNumber] = inputValue;

	return returnValue;
}

NeuronErrorCode getNeuronWeight(Neuron *myNeuron, int inputNumber, NeuronWeight *inputWeight)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (inputWeight==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if ((inputNumber < 0) || (inputNumber >= myNeuron->numberOfInputs))
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
	else if ((inputNumber < 0) || (inputNumber >= myNeuron->numberOfInputs))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		myNeuron->weightArray[inputNumber] = inputWeight;

	return returnValue;
}

NeuronErrorCode computeNeuronOutput(Neuron *myNeuron, NeuronData *neuronOutput)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (neuronOutput==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		int inputSum = 0;

		for (int i=0; i < myNeuron->numberOfInputs; i++)
			inputSum = inputSum + myNeuron->inputArray[i] * myNeuron->weightArray[i];
		
		if (inputSum>0)
			*neuronOutput = NEURON_DATA_ONE;
		else
			*neuronOutput = NEURON_DATA_ZERO;
	}

	return returnValue;
}

NeuronErrorCode cloneNeuron(Neuron *myNeuron, Neuron *myNeuronClone)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;

	if ((myNeuron==NULL) || (myNeuronClone==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if (myNeuron->numberOfInputs != myNeuronClone->numberOfInputs)
		returnValue = NEURON_DIFFERENT_NEURONS_ERROR;

	//Copy weights
	while ((i < myNeuron->numberOfInputs) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		NeuronWeight neuronWeight;

		returnValue = getNeuronWeight(myNeuron, i, &neuronWeight);

		if (returnValue==NEURON_RETURN_VALUE_OK)
			returnValue = setNeuronWeight(myNeuronClone, i, neuronWeight);

		i++;
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
		int numberOfMutations = (rand() % myNeuron->numberOfInputs) + 1;

		for (int i=0; i < numberOfMutations; i++)
		{
			int randomWeight = rand() % myNeuron->numberOfInputs;

			if (myNeuron->weightArray[randomWeight] == NEURON_WEIGHT_NEGATIVE)
				myNeuron->weightArray[randomWeight] = NEURON_WEIGHT_POSITIVE;
			else
				myNeuron->weightArray[randomWeight] = NEURON_WEIGHT_NEGATIVE;
		}
	}

	return returnValue;
}

//Neuron array operations

NeuronErrorCode createNeuronArray(NeuronArray **myNeuronArray, int numberOfInputs, int numberOfNeurons)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;

	if (myNeuronArray==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((numberOfInputs<NEURON_MINIMUM_NUMBER_OF_INPUTS) || (numberOfInputs>INT_MAX))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if ((numberOfNeurons<NEURON_MINIMUM_NUMBER_OF_NEURONS) || (numberOfNeurons>INT_MAX))
		returnValue = NEURON_NUMBER_OF_NEURONS_ERROR;

	//Create neuron array structure
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		*myNeuronArray = malloc(sizeof(NeuronArray));

		if (*myNeuronArray==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create neuron array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuronArray)->neuronArray = malloc(sizeof(Neuron*)*numberOfNeurons);

		if ((*myNeuronArray)->neuronArray==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create neurons
	while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron;

		returnValue = createNeuron(&myNeuron, numberOfInputs);

		if (returnValue==NEURON_RETURN_VALUE_OK)
			(*myNeuronArray)->neuronArray[i] = myNeuron;

		i++;
	}

	if (returnValue==NEURON_RETURN_VALUE_OK)
		(*myNeuronArray)->numberOfNeurons = numberOfNeurons;

	return returnValue;
}

NeuronErrorCode destroyNeuronArray(NeuronArray **myNeuronArray)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;
	int numberOfNeurons;

	if ((myNeuronArray==NULL) || (*myNeuronArray==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else
		numberOfNeurons = (*myNeuronArray)->numberOfNeurons;

	//Destroy neurons
	while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron = (*myNeuronArray)->neuronArray[i];
		returnValue = destroyNeuron(&myNeuron);
		i++;
	}

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		free((*myNeuronArray)->neuronArray);
		free(*myNeuronArray);
		*myNeuronArray = NULL;
	}

	return returnValue;
}

NeuronErrorCode getNumberOfNeurons(NeuronArray* myNeuronArray, int *numberOfNeurons)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuronArray==NULL) || (numberOfNeurons==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*numberOfNeurons = myNeuronArray->numberOfNeurons;

	return returnValue;
}

NeuronErrorCode getNeuron(NeuronArray *myNeuronArray, int neuronNumber, Neuron **myNeuron)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuronArray==NULL) || (myNeuron==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if ((neuronNumber<0) || (neuronNumber>=myNeuronArray->numberOfNeurons))
		returnValue = NEURON_NUMBER_OF_NEURONS_ERROR ;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*myNeuron = myNeuronArray->neuronArray[neuronNumber];

	return returnValue;
}

NeuronErrorCode cloneNeuronArray(NeuronArray *myNeuronArray, NeuronArray *myNeuronArrayClone)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;
	int numberOfNeurons;

	if ((myNeuronArray==NULL) || (myNeuronArrayClone==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;
	else if (myNeuronArray->numberOfNeurons!=myNeuronArrayClone->numberOfNeurons)
		returnValue = NEURON_DIFFERENT_NEURON_ARRAYS_ERROR;
	else
		numberOfNeurons = myNeuronArray->numberOfNeurons;

	while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron, *myNeuronClone;

		returnValue = getNeuron(myNeuronArray, i, &myNeuron);

		if (returnValue==NEURON_RETURN_VALUE_OK)
			returnValue = getNeuron(myNeuronArrayClone, i, &myNeuronClone);

		if (returnValue==NEURON_RETURN_VALUE_OK)
			returnValue = cloneNeuron(myNeuron, myNeuronClone);

		i++;
	}

	return returnValue;
}

NeuronErrorCode mutateNeuronArray(NeuronArray *myNeuronArray, bool isMassiveMutation)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	Neuron *mutantNeuron;

	int numberOfNeurons;

	if (myNeuronArray==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;
	else
		numberOfNeurons = myNeuronArray->numberOfNeurons;
	
	if ((isMassiveMutation) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		int i=0;

		while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
		{
			mutantNeuron = myNeuronArray->neuronArray[i];
			returnValue = mutateNeuron(mutantNeuron);

			i++;
		}
	}
	else if ((!isMassiveMutation) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		int mutantNeuronIndex = rand() % numberOfNeurons;
		mutantNeuron = myNeuronArray->neuronArray[mutantNeuronIndex];

		returnValue = mutateNeuron(mutantNeuron);
	}
	
	return returnValue;
}
