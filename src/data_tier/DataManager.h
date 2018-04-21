/*
 * DataManager.h
 *
 *  Created on: Dec 29, 2017
 *      Author: kenshiro
 */

#ifndef SRC_DATA_TIER_DATAMANAGER_H_
#define SRC_DATA_TIER_DATAMANAGER_H_

#include "../logic_tier/NeuralNetwork.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <json-glib/json-glib.h>
#include <json-glib/json-gobject.h>

NeuralNetworkErrorCode loadNeuralNetwork(char *filePath, NeuralNetwork **myNeuralNetwork);
NeuralNetworkErrorCode saveNeuralNetwork(char *filePath, NeuralNetwork *myNeuralNetwork);

#endif /* SRC_DATA_TIER_DATAMANAGER_H_ */
