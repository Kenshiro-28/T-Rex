/*
 * Neuron.h
 *
 *  Created on: Sep 9, 2017
 *      Author: kenshiro
 */

#ifndef LOGIC_TIER_NEURON_H_
#define LOGIC_TIER_NEURON_H_

#include "Chromosome.h"

#define NEURON_MINIMUM_NUMBER_OF_INPUTS 2
#define NEURON_MINIMUM_NUMBER_OF_NEURONS 1

typedef struct neuron Neuron;
typedef struct neuronArray NeuronArray;

typedef enum
{
	NEURON_DATA_ZERO,
	NEURON_DATA_ONE
} NeuronData;

typedef enum
{
	NEURON_RETURN_VALUE_OK = 0,
	NEURON_NULL_POINTER_ERROR = -1,
	NEURON_MEMORY_ALLOCATION_ERROR = -2,
	NEURON_NUMBER_OF_INPUTS_ERROR = -3,
	NEURON_NUMBER_OF_NEURONS_ERROR = -4,
	NEURON_DIFFERENT_NEURONS_ERROR = -5,
	NEURON_DIFFERENT_NEURON_ARRAYS_ERROR = -6,
	NEURON_CHROMOSOME_ERROR = -7
} NeuronErrorCode;


//Neuron operations
NeuronErrorCode createNeuron(Neuron **myNeuron, int numberOfInputs);
NeuronErrorCode destroyNeuron(Neuron **myNeuron);
NeuronErrorCode setNeuronInput(Neuron *myNeuron, int inputNumber, NeuronData inputValue);
NeuronErrorCode getNeuronChromosome(Neuron *myNeuron, Chromosome **myChromosome);
NeuronErrorCode getNeuronWeight(Neuron *myNeuron, int inputNumber, Gene *inputWeight);
NeuronErrorCode setNeuronWeight(Neuron *myNeuron, int inputNumber, Gene inputWeight);
NeuronErrorCode computeNeuronOutput(Neuron *myNeuron, NeuronData *neuronOutput);
NeuronErrorCode cloneNeuron(Neuron *myNeuron, Neuron *myNeuronClone);
NeuronErrorCode mutateNeuron(Neuron *myNeuron);

//Neuron array operations
NeuronErrorCode createNeuronArray(NeuronArray **myNeuronArray, int numberOfInputs, int numberOfNeurons);
NeuronErrorCode destroyNeuronArray(NeuronArray **myNeuronArray);
NeuronErrorCode getNumberOfNeurons(NeuronArray* myNeuronArray, int *numberOfNeurons);
NeuronErrorCode getNeuron(NeuronArray *myNeuronArray, int neuronNumber, Neuron **myNeuron);
NeuronErrorCode cloneNeuronArray(NeuronArray *myNeuronArray, NeuronArray *myNeuronArrayClone);
NeuronErrorCode mutateNeuronArray(NeuronArray *myNeuronArray, bool isMassiveMutation);

#endif /* LOGIC_TIER_NEURON_H_ */
