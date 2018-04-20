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

typedef enum
{
	DATA_MANAGER_RETURN_VALUE_OK = 0,
	DATA_MANAGER_NULL_POINTER_ERROR = -1,
	DATA_MANAGER_MEMORY_ALLOCATION_ERROR = -2,
	DATA_MANAGER_JSON_GLIB_ERROR = -3,
	DATA_MANAGER_FILE_LOAD_ERROR = -4,
	DATA_MANAGER_FILE_SAVE_ERROR = -5,
	DATA_MANAGER_NEURAL_NETWORK_ERROR = -6
} DataManagerErrorCode;

DataManagerErrorCode loadNeuralNetwork(char *filePath, NeuralNetwork **myNeuralNetwork);
DataManagerErrorCode saveNeuralNetwork(char *filePath, NeuralNetwork *myNeuralNetwork);

#endif /* SRC_DATA_TIER_DATAMANAGER_H_ */
