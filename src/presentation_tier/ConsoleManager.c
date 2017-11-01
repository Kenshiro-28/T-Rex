/*
 * ConsoleManager.c
 *
 *  Created on: Oct 28, 2017
 *      Author: kenshiro
 */

#include "ConsoleManager.h"

NeuralNetworkErrorCode printNeuralNetwork(NeuralNetwork *myNeuralNetwork)
{
	NeuralNetworkErrorCode returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

	int numberOfHiddenLayers;
	int hiddenLayerIndex=0;

	if (myNeuralNetwork==NULL)
		returnValue = NEURAL_NETWORK_NULL_POINTER_ERROR;

	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		printf("\n----- NEURAL NETWORK -----\n\n");
		returnValue = getNumberOfHiddenLayers(myNeuralNetwork, &numberOfHiddenLayers);
	}

	//Print input layer
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		NeuronData *myInputLayer;
		int numberOfInputs;

		returnValue = getInputLayer(myNeuralNetwork, &myInputLayer, &numberOfInputs);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			printf("Input Layer: ");

			for (int i=0; i<numberOfInputs; i++)
			{
				printf("%d", myInputLayer[i]);

				if (i<numberOfInputs-1)
					printf(", ");
			}

			printf("\n");
		}
	}

	//Print hidden layers
	while ((hiddenLayerIndex<numberOfHiddenLayers) && (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK))
	{
		NeuralLayer *myHiddenLayer;
		returnValue = getHiddenLayer(myNeuralNetwork, hiddenLayerIndex, &myHiddenLayer);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			printf("Hidden Layer %d: ", hiddenLayerIndex);
			printNeuralLayer(myHiddenLayer);
		}

		hiddenLayerIndex++;
	}

	//Print output layer
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		NeuralLayer *myOutputLayer;

		returnValue = getOutputLayer(myNeuralNetwork, &myOutputLayer);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			printf("Output Layer: ");
			printNeuralLayer(myOutputLayer);
		}
	}

	//Print neural network output
	if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
	{
		NeuronData *neuralLayerOutput;
		int numberOfOutputs;

		returnValue = getNeuralNetworkOutput(myNeuralNetwork, &neuralLayerOutput, &numberOfOutputs);

		if (returnValue==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			printf("Neural Network Output: ");

			for (int i=0; i<numberOfOutputs; i++)
			{
				printf("%d", neuralLayerOutput[i]);

				if (i<numberOfOutputs-1)
					printf(", ");
			}

			printf("\n\n");
		}
	}

	return returnValue;
}

NeuronErrorCode printNeuralLayer(NeuralLayer *myNeuralLayer)
{
	NeuronErrorCode returnValue = NEURON_RETURN_VALUE_OK;
	int numberOfNeurons;
	int i=0;

	if (myNeuralLayer==NULL)
		returnValue = NEURON_NULL_POINTER_ERROR;

	returnValue = getNumberOfNeurons(myNeuralLayer, &numberOfNeurons);

	while ((i<numberOfNeurons) && (returnValue==NEURON_RETURN_VALUE_OK))
	{
		Neuron *myNeuron;
		Chromosome *myChromosome;

		ChromosomeErrorCode result;

		returnValue = getNeuron(myNeuralLayer, i, &myNeuron);

		if (returnValue==NEURON_RETURN_VALUE_OK)
			returnValue = getNeuronChromosome(myNeuron, &myChromosome);

		if (returnValue==NEURON_RETURN_VALUE_OK)
		{
			printf("N%d: ", i);

			result = printChromosome(myChromosome);

			if (i<numberOfNeurons-1)
				printf(", ");

			if (result!=CHROMOSOME_RETURN_VALUE_OK)
				returnValue = NEURON_CHROMOSOME_ERROR;
		}

		i++;
	}

	printf("\n");

	return returnValue;
}

ChromosomeErrorCode printChromosome(Chromosome *myChromosome)
{
	ChromosomeErrorCode returnValue = CHROMOSOME_RETURN_VALUE_OK;
	Gene myGene;
	int numberOfGenes;
	int i=0;

	if (myChromosome==NULL)
		returnValue = CHROMOSOME_NULL_POINTER_ERROR;
	else
		returnValue = getNumberOfGenes(myChromosome, &numberOfGenes);

	printf("|");

	while ((i<numberOfGenes) && (returnValue==CHROMOSOME_RETURN_VALUE_OK))
	{
		returnValue = getGene(myChromosome, i, &myGene);

		if (returnValue==CHROMOSOME_RETURN_VALUE_OK)
		{
			if (myGene==GENE_STATE_NEGATIVE)
				printf("%d |", myGene);
			else
				printf(" %d |", myGene);
		}

		i++;
	}

	return returnValue;
}

