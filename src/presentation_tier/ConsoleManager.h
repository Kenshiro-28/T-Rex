/*
 * ConsoleManager.h
 *
 *  Created on: Oct 28, 2017
 *      Author: kenshiro
 */

#ifndef SRC_PRESENTATION_TIER_CONSOLEMANAGER_H_
#define SRC_PRESENTATION_TIER_CONSOLEMANAGER_H_

#include "../logic_tier/NeuralNetwork.h"
#include <stdio.h>

NeuralNetworkErrorCode printNeuralNetwork(NeuralNetwork *myNeuralNetwork);
NeuronErrorCode printNeuralLayer(NeuralLayer *myNeuralLayer);
ChromosomeErrorCode printChromosome(Chromosome *myChromosome);

#endif /* SRC_PRESENTATION_TIER_CONSOLEMANAGER_H_ */
