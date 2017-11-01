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
	Chromosome *weightData;
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
	Chromosome *myChromosome = NULL;

	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((numberOfInputs<NEURON_MINIMUM_NUMBER_OF_INPUTS) || (numberOfInputs>INT_MAX))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	//Create chromosome
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		ChromosomeErrorCode result = createChromosome(&myChromosome, numberOfInputs);

		if (result!=CHROMOSOME_RETURN_VALUE_OK)
			returnValue = NEURON_CHROMOSOME_ERROR;
	}

	//Create neuron
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		*myNeuron = malloc(sizeof(Neuron));

		if (*myNeuron==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Create input array
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuron)->inputArray = malloc(sizeof(NeuronData)*numberOfInputs);

		if ((*myNeuron)->inputArray==NULL)
			returnValue = NEURON_MEMORY_ALLOCATION_ERROR;
	}

	//Initialize
	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		(*myNeuron)->numberOfInputs = numberOfInputs;

		for (int i=0; i<numberOfInputs; i++)
			(*myNeuron)->inputArray[i] = NEURON_DATA_ZERO;

		(*myNeuron)->weightData = myChromosome;
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
		free((*myNeuron)->weightData);
		free(*myNeuron);
		*myNeuron = NULL;
	}

	return returnValue;
}

NeuronErrorCode setNeuronInput(Neuron *myNeuron, int inputNumber, NeuronData inputValue)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((inputNumber<0) || (inputNumber>=myNeuron->numberOfInputs))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		myNeuron->inputArray[inputNumber] = inputValue;

	return returnValue;
}

NeuronErrorCode getNeuronChromosome(Neuron *myNeuron, Chromosome **myChromosome)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (myChromosome==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
		*myChromosome = myNeuron->weightData;

	return returnValue;
}

NeuronErrorCode getNeuronWeight(Neuron *myNeuron, int inputNumber, Gene *inputWeight)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (inputWeight==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((inputNumber<0) || (inputNumber>=myNeuron->numberOfInputs))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		ChromosomeErrorCode result = getGene(myNeuron->weightData, inputNumber, inputWeight);

		if (result!=CHROMOSOME_RETURN_VALUE_OK)
			returnValue = NEURON_CHROMOSOME_ERROR;
	}

	return returnValue;
}

NeuronErrorCode setNeuronWeight(Neuron *myNeuron, int inputNumber, Gene inputWeight)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if (myNeuron==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	if ((inputNumber<0) || (inputNumber>=myNeuron->numberOfInputs))
		returnValue = NEURON_NUMBER_OF_INPUTS_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		ChromosomeErrorCode result = setGene(myNeuron->weightData, inputNumber, inputWeight);

		if (result!=CHROMOSOME_RETURN_VALUE_OK)
			returnValue = NEURON_CHROMOSOME_ERROR;
	}

	return returnValue;
}

NeuronErrorCode computeNeuronOutput(Neuron *myNeuron, NeuronData *neuronOutput)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	if ((myNeuron==NULL) || (neuronOutput==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		ChromosomeErrorCode result = CHROMOSOME_RETURN_VALUE_OK;
		int inputSum = 0;
		Gene inputWeight;
		int i=0;

		while ((i<myNeuron->numberOfInputs) && (result==CHROMOSOME_RETURN_VALUE_OK))
		{
			result = getGene(myNeuron->weightData, i, &inputWeight);

			if (result==CHROMOSOME_RETURN_VALUE_OK)
			{
				NeuronData neuronInput = myNeuron->inputArray[i];
				inputSum = inputSum + neuronInput*inputWeight;
			}

			i++;
		}

		if (result==CHROMOSOME_RETURN_VALUE_OK)
		{
			if (inputSum>0)
				*neuronOutput = NEURON_DATA_ONE;
			else
				*neuronOutput = NEURON_DATA_ZERO;
		}
		else
			returnValue = NEURON_CHROMOSOME_ERROR;
	}

	return returnValue;
}

NeuronErrorCode cloneNeuron(Neuron *myNeuron, Neuron *myNeuronClone)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;

	int i=0;
	int numberOfWeights;

	if ((myNeuron==NULL) || (myNeuronClone==NULL))
		returnValue = NEURON_NULL_POINTER_ERROR;

	if (myNeuron->numberOfInputs!=myNeuronClone->numberOfInputs)
		returnValue = NEURON_DIFFERENT_NEURONS_ERROR;
	else
		numberOfWeights = myNeuron->numberOfInputs;

	//Copy weights
	while ((i<numberOfWeights) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Gene neuronWeight;

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
		ChromosomeErrorCode result = mutateChromosome(myNeuron->weightData);

		if (result!=CHROMOSOME_RETURN_VALUE_OK)
			returnValue = NEURON_CHROMOSOME_ERROR;
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

	if ((neuronNumber<0) || (neuronNumber>=myNeuronArray->numberOfNeurons))
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

	if (myNeuronArray->numberOfNeurons!=myNeuronArrayClone->numberOfNeurons)
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

	if (returnValue==NEURON_RETURN_VALUE_OK)
	{
		numberOfNeurons = myNeuronArray->numberOfNeurons;

		if (isMassiveMutation)
		{
			int i=0;

			while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
			{
				mutantNeuron = myNeuronArray->neuronArray[i];
				returnValue = mutateNeuron(mutantNeuron);

				i++;
			}
		}
		else
		{
			int mutantNeuronIndex = rand() % numberOfNeurons;
			mutantNeuron = myNeuronArray->neuronArray[mutantNeuronIndex];

			returnValue = mutateNeuron(mutantNeuron);
		}
	}

	return returnValue;
}
