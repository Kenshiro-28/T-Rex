/*
 * NeuralLayer.h
 *
 *  Created on: Mar 10, 2020
 *      Author: kenshiro
 */

#ifndef LOGIC_TIER_NEURAL_LAYER_H_
#define LOGIC_TIER_NEURAL_LAYER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define NEURAL_LAYER_MINIMUM_NUMBER_OF_INPUTS 2
#define NEURAL_LAYER_MINIMUM_NUMBER_OF_NEURONS 1

typedef enum
{
	NEURON_DATA_ZERO,
	NEURON_DATA_ONE
} NeuronData;

typedef enum
{
	NEURON_WEIGHT_POSITIVE = 1,
	NEURON_WEIGHT_NEGATIVE = -1
} NeuronWeight;

typedef enum
{
	NEURON_RETURN_VALUE_OK = 0,
	NEURON_NULL_POINTER_ERROR = -1,
	NEURON_MEMORY_ALLOCATION_ERROR = -2,
	NEURON_NUMBER_OF_INPUTS_ERROR = -3,
	NEURON_NUMBER_OF_NEURONS_ERROR = -4,
	NEURON_DIFFERENT_NEURONS_ERROR = -5,
	NEURON_DIFFERENT_NEURAL_LAYERS_ERROR = -6
} NeuronErrorCode;

typedef struct neuron Neuron;
typedef struct neuralLayer NeuralLayer;

//Neuron operations
NeuronErrorCode createNeuron(Neuron **myNeuron, int numberOfInputs);
NeuronErrorCode destroyNeuron(Neuron **myNeuron);
NeuronErrorCode cloneNeuron(Neuron *myNeuron, Neuron *myNeuronClone);
NeuronErrorCode mutateNeuron(Neuron *myNeuron);
NeuronErrorCode getNumberOfInputs(Neuron *myNeuron, int *numberOfInputs);
NeuronErrorCode getNeuronWeight(Neuron *myNeuron, int inputNumber, NeuronWeight *inputWeight);
NeuronErrorCode setNeuronWeight(Neuron *myNeuron, int inputNumber, NeuronWeight inputWeight);
NeuronErrorCode computeNeuronOutput(Neuron *myNeuron, NeuronData *inputArray, NeuronData *neuronOutput);

//Neural layer operations
NeuronErrorCode createNeuralLayer(NeuralLayer **myNeuralLayer, int numberOfInputs, int numberOfNeurons);
NeuronErrorCode destroyNeuralLayer(NeuralLayer **myNeuralLayer);
NeuronErrorCode computeNeuralLayerOutput(NeuralLayer *myNeuralLayer, NeuronData *inputArray, NeuronData *outputArray);
NeuronErrorCode getNumberOfNeurons(NeuralLayer* myNeuralLayer, int *numberOfNeurons);
NeuronErrorCode getNeuron(NeuralLayer *myNeuralLayer, int neuronNumber, Neuron **myNeuron);
NeuronErrorCode cloneNeuralLayer(NeuralLayer *myNeuralLayer, NeuralLayer *myNeuralLayerClone);
NeuronErrorCode mutateNeuralLayer(NeuralLayer *myNeuralLayer, bool isMassiveMutation);

#endif /* LOGIC_TIER_NEURAL_LAYER_H_ */
