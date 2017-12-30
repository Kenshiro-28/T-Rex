# T-Rex

T-Rex is an Evolutionary Neural Network. It learns by adjusting the strength of the connection weights by mutation and selection. The programmer must define the problem to solve with a scoring system so that T-Rex can evolve gradually until finding the optimal solution.

Main features:

- Binary feedforward neural network
- Configurable number of inputs, hidden layers and outputs
- Developed using object-oriented programming
- Fast, robust and portable

## Network topology

- Feedforward neural network: each neuron in one layer has directed connections to the neurons of the subsequent layer
- Binary neural network: the input and output values can be 0 or 1 and the connection weights can be -1 or 1
- Configurable number of inputs
- Configurable number of hidden layers 
- The number of neurons in each hidden layer is set to be the number of input neurons
- Configurable number of outputs

## Quick guide

- Download Eclipse IDE for C/C++ Developers: https://www.eclipse.org/cdt/
- Create a new C Project in the project root folder
- Edit the file **RunExample.c** and save the name of the selected example in the **SELECTED_EXAMPLE** macro
- Compile and run the project
