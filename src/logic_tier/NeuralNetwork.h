/*
 * NeuralNetwork.h
 *
 *  Created on: Sep 9, 2017
 *      Author: kenshiro
 */

#ifndef LOGIC_TIER_NEURALNETWORK_H_
#define LOGIC_TIER_NEURALNETWORK_H_

#include "Neuron.h"

#define NEURAL_NETWORK_MINIMUM_NUMBER_OF_INPUTS NEURON_MINIMUM_NUMBER_OF_INPUTS
#define NEURAL_NETWORK_MINIMUM_NUMBER_OF_HIDDEN_LAYERS 1
#define NEURAL_NETWORK_MINIMUM_NUMBER_OF_NEURONS_PER_LAYER 1

#define NEURAL_NETWORK_PERCENTAGE_OF_MASSIVE_MUTATIONS 10

typedef struct neuralNetwork NeuralNetwork;
typedef NeuronArray NeuralLayer;

typedef enum
{
	NEURAL_NETWORK_RETURN_VALUE_OK = 0,
	NEURAL_NETWORK_NULL_POINTER_ERROR = -1,
	NEURAL_NETWORK_MEMORY_ALLOCATION_ERROR = -2,
	NEURAL_NETWORK_NUMBER_OF_INPUTS_ERROR = -3,
	NEURAL_NETWORK_NUMBER_OF_HIDDEN_LAYERS_ERROR = -4,
	NEURAL_NETWORK_NUMBER_OF_NEURONS_PER_LAYER_ERROR = -5,
	NEURAL_NETWORK_NUMBER_OF_OUTPUT_NEURONS_ERROR = -6,
	NEURAL_NETWORK_DIFFERENT_NEURAL_NETWORKS_ERROR = -7,
	NEURAL_NETWORK_NEURON_ERROR = -8
} NeuralNetworkErrorCode;

NeuralNetworkErrorCode createNeuralNetwork(NeuralNetwork **myNeuralNetwork, int numberOfInputs, int numberOfHiddenLayers, int numberOfOutputs);
NeuralNetworkErrorCode destroyNeuralNetwork(NeuralNetwork **myNeuralNetwork);
NeuralNetworkErrorCode getInputLayer(NeuralNetwork *myNeuralNetwork, NeuronData **myInputLayer, int *numberOfInputs);
NeuralNetworkErrorCode getNumberOfHiddenLayers(NeuralNetwork *myNeuralNetwork, int *numberOfHiddenLayers);
NeuralNetworkErrorCode getHiddenLayer(NeuralNetwork *myNeuralNetwork, int hiddenLayerNumber, NeuralLayer **myHiddenLayer);
NeuralNetworkErrorCode getOutputLayer(NeuralNetwork *myNeuralNetwork, NeuralLayer **myOutputLayer, int *numberOfOutputs);
NeuralNetworkErrorCode setNeuralNetworkInput(NeuralNetwork *myNeuralNetwork, int inputNumber, NeuronData input);
NeuralNetworkErrorCode computeNeuralNetworkOutput(NeuralNetwork *myNeuralNetwork, NeuronData **outputArray, int *numberOfOutputs);
NeuralNetworkErrorCode getNeuralNetworkOutput(NeuralNetwork *myNeuralNetwork, NeuronData **outputArray, int *numberOfOutputs);
NeuralNetworkErrorCode cloneNeuralNetwork(NeuralNetwork *myNeuralNetwork, NeuralNetwork *myNeuralNetworkClone);
NeuralNetworkErrorCode mutateNeuralNetwork(NeuralNetwork *myNeuralNetwork);

#endif /* LOGIC_TIER_NEURALNETWORK_H_ */
