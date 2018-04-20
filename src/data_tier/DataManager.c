/*
 * DataManager.c
 *
 *  Created on: Dec 29, 2017
 *      Author: kenshiro
 */

#include "DataManager.h"

#define DATA_MANAGER_JSON_NUMBER_OF_INPUTS_KEY "numberOfInputs"
#define DATA_MANAGER_JSON_NUMBER_OF_HIDDEN_LAYERS_KEY "numberOfHiddenLayers"
#define DATA_MANAGER_JSON_NUMBER_OF_OUTPUTS_KEY "numberOfOutputs"
#define DATA_MANAGER_JSON_HIDDEN_LAYER_ARRAY_KEY "hiddenLayerArray"
#define DATA_MANAGER_JSON_OUTPUT_LAYER_KEY "outputLayer"
#define DATA_MANAGER_JSON_MAX_LENGTH (pow(1024, 3))

static DataManagerErrorCode addNeuronWeights(Neuron *myNeuron, int numberOfInputs, JsonBuilder *myJsonBuilder)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	Chromosome *myChromosome;
	Gene myWeight;

	int numberOfGenes;
	int inputNumber = 0;

	NeuronErrorCode result = getNeuronChromosome(myNeuron, &myChromosome);

	if (result!=NEURON_RETURN_VALUE_OK)
		returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		result = getNumberOfGenes(myChromosome, &numberOfGenes);

		if ((result!=NEURON_RETURN_VALUE_OK) || (numberOfGenes!=numberOfInputs))
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;
	}

	//Begin weight array
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_begin_array(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	while ((inputNumber<numberOfInputs) && (returnValue==DATA_MANAGER_RETURN_VALUE_OK))
	{
		result = getNeuronWeight(myNeuron, inputNumber, &myWeight);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

		if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
		{
			myJsonBuilder = json_builder_add_int_value(myJsonBuilder, myWeight);

			if (myJsonBuilder==NULL)
				returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
		}

		inputNumber++;
	}

	//End weight array
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_end_array(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	return returnValue;
}

static DataManagerErrorCode addNeuralLayer(NeuralLayer *myNeuralLayer, int numberOfInputs, JsonBuilder *myJsonBuilder)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	int neuronNumber = 0;
	int numberOfNeurons = 0;

	NeuronErrorCode result = getNumberOfNeurons(myNeuralLayer, &numberOfNeurons);

	if (result!=NEURON_RETURN_VALUE_OK)
		returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

	//Begin neuron array
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_begin_array(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	while ((neuronNumber<numberOfNeurons) && (returnValue==DATA_MANAGER_RETURN_VALUE_OK))
	{
		Neuron *myNeuron;

		result = getNeuron(myNeuralLayer, neuronNumber, &myNeuron);

		if (result==NEURON_RETURN_VALUE_OK)
			returnValue = addNeuronWeights(myNeuron, numberOfInputs, myJsonBuilder);
		else
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

		neuronNumber++;
	}

	//End neuron array
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_end_array(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	return returnValue;
}

static DataManagerErrorCode saveFile(char *filePath, JsonBuilder *myJsonBuilder)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	JsonGenerator *myJsonGenerator = NULL;
	JsonNode *myRootNode = NULL;

	gchar *myJsonString = NULL;

	//Create file
	FILE *myFile = fopen(filePath, "w");

	if (myFile==NULL)
		returnValue = DATA_MANAGER_FILE_SAVE_ERROR;

	//Create json generator
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonGenerator = json_generator_new();

		myRootNode = json_builder_get_root(myJsonBuilder);

		if (myRootNode!=NULL)
			json_generator_set_root(myJsonGenerator, myRootNode);
		else
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Create the json string and write it in the file
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonString = json_generator_to_data(myJsonGenerator, NULL);

		int result = fputs(myJsonString, myFile);

		if (result==EOF)
			returnValue = DATA_MANAGER_FILE_SAVE_ERROR;
	}

	//Close file
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		int result = fclose(myFile);

		if (result==EOF)
			returnValue = DATA_MANAGER_FILE_SAVE_ERROR;
	}

	//Free memory
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		json_node_free(myRootNode);
		g_object_unref(myJsonGenerator);
		g_free(myJsonString);
	}

	return returnValue;
}

static DataManagerErrorCode setNeuronWeights(Neuron *myNeuron, int numberOfInputs, JsonReader *myJsonReader)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	int inputIndex = 0;

	while ((inputIndex < numberOfInputs) && (returnValue==DATA_MANAGER_RETURN_VALUE_OK))
	{
		json_reader_read_element(myJsonReader, inputIndex);
		gint64 inputWeight = json_reader_get_int_value(myJsonReader);
		json_reader_end_element(myJsonReader);

		NeuronErrorCode result = setNeuronWeight(myNeuron, inputIndex, inputWeight);

		if (result!=NEURON_RETURN_VALUE_OK)
			returnValue = NEURAL_NETWORK_RETURN_VALUE_OK;

		inputIndex++;
	}

	return returnValue;
}

static DataManagerErrorCode setNeuralLayerWeights(NeuralLayer *myNeuralLayer, int numberOfInputs, int numberOfNeurons, JsonReader *myJsonReader)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	int neuronIndex = 0;

	while ((neuronIndex < numberOfNeurons) && (returnValue==DATA_MANAGER_RETURN_VALUE_OK))
	{
		Neuron *myNeuron;

		NeuronErrorCode result = getNeuron(myNeuralLayer, neuronIndex, &myNeuron);

		if (result==NEURON_RETURN_VALUE_OK)
		{
			json_reader_read_element(myJsonReader, neuronIndex);
			returnValue = setNeuronWeights(myNeuron, numberOfInputs, myJsonReader);
			json_reader_end_element(myJsonReader);
		}
		else
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

		neuronIndex++;
	}

	return returnValue;
}

DataManagerErrorCode loadNeuralNetwork(char *filePath, NeuralNetwork **myNeuralNetwork)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	FILE *myFile;

	gchar *myJsonString;

	JsonParser *myJsonParser = NULL;
	JsonReader *myJsonReader = NULL;

	int numberOfInputs = 0;
	int numberOfHiddenLayers = 0;
	int numberOfOutputs = 0;
	int hiddenLayerIndex = 0;

	if ((filePath==NULL) || (myNeuralNetwork==NULL))
		returnValue = DATA_MANAGER_NULL_POINTER_ERROR;

	//Allocate memory
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonString = malloc(DATA_MANAGER_JSON_MAX_LENGTH);

		if (myJsonString==NULL)
			returnValue = DATA_MANAGER_MEMORY_ALLOCATION_ERROR;
	}

	//Open file
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myFile = fopen(filePath, "r");

		if (myFile==NULL)
			returnValue = DATA_MANAGER_FILE_LOAD_ERROR;
	}

	//Read json string
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		char* result = fgets(myJsonString, DATA_MANAGER_JSON_MAX_LENGTH - 1, myFile);

		if (result==NULL)
			returnValue = DATA_MANAGER_FILE_LOAD_ERROR;
	}

	//Parse json string
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
	    myJsonParser = json_parser_new();
		gboolean result = json_parser_load_from_data(myJsonParser, myJsonString, -1, NULL);

		if (!result)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Create Json Reader
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonReader = json_reader_new(json_parser_get_root(myJsonParser));

		if (myJsonReader==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Get number of inputs
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		json_reader_read_member(myJsonReader, DATA_MANAGER_JSON_NUMBER_OF_INPUTS_KEY);
		numberOfInputs = json_reader_get_int_value(myJsonReader);
		json_reader_end_member(myJsonReader);
	}

	//Get number of hidden layers
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		json_reader_read_member(myJsonReader, DATA_MANAGER_JSON_NUMBER_OF_HIDDEN_LAYERS_KEY);
		numberOfHiddenLayers = json_reader_get_int_value(myJsonReader);
		json_reader_end_member(myJsonReader);
	}

	//Get number of outputs
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		json_reader_read_member(myJsonReader, DATA_MANAGER_JSON_NUMBER_OF_OUTPUTS_KEY);
		numberOfOutputs = json_reader_get_int_value(myJsonReader);
		json_reader_end_member(myJsonReader);
	}

	//Create neural network
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		NeuralNetworkErrorCode result = createNeuralNetwork(myNeuralNetwork, numberOfInputs, numberOfHiddenLayers, numberOfOutputs);

		if (result!=NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;
	}

	json_reader_read_member(myJsonReader, DATA_MANAGER_JSON_HIDDEN_LAYER_ARRAY_KEY);

	//Set weights of hidden layers
	while ((hiddenLayerIndex<numberOfHiddenLayers) && (returnValue==DATA_MANAGER_RETURN_VALUE_OK))
	{
		NeuralLayer *myHiddenLayer;

		NeuralNetworkErrorCode result = getHiddenLayer(*myNeuralNetwork, hiddenLayerIndex, &myHiddenLayer);

		if (result==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			json_reader_read_element(myJsonReader, hiddenLayerIndex);
			returnValue = setNeuralLayerWeights(myHiddenLayer, numberOfInputs, numberOfInputs, myJsonReader);
			json_reader_end_element(myJsonReader);
		}
		else
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

		hiddenLayerIndex++;
	}

	json_reader_end_member(myJsonReader);

	//Set weights of output layer
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		NeuralLayer *myOutputLayer;

		int dummy;

		NeuralNetworkErrorCode result = getOutputLayer(*myNeuralNetwork, &myOutputLayer, &dummy);

		if (result==NEURAL_NETWORK_RETURN_VALUE_OK)
		{
			json_reader_read_member(myJsonReader, DATA_MANAGER_JSON_OUTPUT_LAYER_KEY);
			returnValue = setNeuralLayerWeights(myOutputLayer, numberOfInputs, numberOfOutputs, myJsonReader);
			json_reader_end_member(myJsonReader);
		}
		else
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;
	}

	//Free memory
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		free(myJsonString);
		g_object_unref(myJsonReader);
	    g_object_unref(myJsonParser);
	}

	return returnValue;
}

DataManagerErrorCode saveNeuralNetwork(char *filePath, NeuralNetwork *myNeuralNetwork)
{
	DataManagerErrorCode returnValue = DATA_MANAGER_RETURN_VALUE_OK;

	JsonBuilder *myJsonBuilder = NULL;

	NeuralLayer *myHiddenLayer;
	NeuralLayer *myOutputLayer;

	int numberOfInputs = 0;
	int numberOfHiddenLayers = 0;
	int numberOfOutputs = 0;
	int hiddenLayerIndex = 0;

	if ((filePath==NULL) || (myNeuralNetwork==NULL))
		returnValue = DATA_MANAGER_NULL_POINTER_ERROR;

	//Get number of inputs
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		NeuronData *myInputLayer;

		NeuralNetworkErrorCode result = getInputLayer(myNeuralNetwork, &myInputLayer, &numberOfInputs);

		if (result!=NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;
	}

	//Get number of hidden layers
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		NeuralNetworkErrorCode result = getNumberOfHiddenLayers(myNeuralNetwork, &numberOfHiddenLayers);

		if (result!=NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;
	}

	//Get output layer
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		NeuralNetworkErrorCode myNeuralNetworkErrorCode = getOutputLayer(myNeuralNetwork, &myOutputLayer, &numberOfOutputs);

		if (myNeuralNetworkErrorCode!=NEURAL_NETWORK_RETURN_VALUE_OK)
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;
	}

	//Create json builder
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_new();
		myJsonBuilder = json_builder_begin_object(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add number of inputs key
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_set_member_name(myJsonBuilder, DATA_MANAGER_JSON_NUMBER_OF_INPUTS_KEY);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add number of inputs value
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_add_int_value(myJsonBuilder, numberOfInputs);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add number of hidden layers key
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_set_member_name(myJsonBuilder, DATA_MANAGER_JSON_NUMBER_OF_HIDDEN_LAYERS_KEY);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add number of hidden layers value
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_add_int_value(myJsonBuilder, numberOfHiddenLayers);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add number of outputs key
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_set_member_name(myJsonBuilder, DATA_MANAGER_JSON_NUMBER_OF_OUTPUTS_KEY);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add number of outputs value
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_add_int_value(myJsonBuilder, numberOfOutputs);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add hidden layer array key
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_set_member_name(myJsonBuilder, DATA_MANAGER_JSON_HIDDEN_LAYER_ARRAY_KEY);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Begin hidden layer array
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_begin_array(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add hidden layers
	while ((hiddenLayerIndex<numberOfHiddenLayers) && (returnValue==DATA_MANAGER_RETURN_VALUE_OK))
	{
		NeuralNetworkErrorCode result = getHiddenLayer(myNeuralNetwork, hiddenLayerIndex, &myHiddenLayer);

		if (result==NEURAL_NETWORK_RETURN_VALUE_OK)
		    returnValue = addNeuralLayer(myHiddenLayer, numberOfInputs, myJsonBuilder);
		else
			returnValue = DATA_MANAGER_NEURAL_NETWORK_ERROR;

		hiddenLayerIndex++;
	}

	//End hidden layer array
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_end_array(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Set output layer key
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_set_member_name(myJsonBuilder, DATA_MANAGER_JSON_OUTPUT_LAYER_KEY);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Add output layer
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
		returnValue = addNeuralLayer(myOutputLayer, numberOfInputs, myJsonBuilder);

	//End json builder
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		myJsonBuilder = json_builder_end_object(myJsonBuilder);

		if (myJsonBuilder==NULL)
			returnValue = DATA_MANAGER_JSON_GLIB_ERROR;
	}

	//Save the json in the file
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
		returnValue = saveFile(filePath, myJsonBuilder);

	//Free memory
	if (returnValue==DATA_MANAGER_RETURN_VALUE_OK)
	{
		g_object_unref(myJsonBuilder);
	}

	return returnValue;
}

