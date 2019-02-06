# T-Rex

T-Rex is an evolutionary neural network. It learns by adjusting the strength of the connection weights by mutation and selection. The programmer must define the problem to solve with a scoring system so that T-Rex can evolve gradually until finding the optimal solution.

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

## Building

T-Rex uses the GNOME JSON-Glib library to load trained neural networks from json files:

https://wiki.gnome.org/Projects/JsonGlib

To install JSON-Glib in Ubuntu, open a system console and run this command:

```
$ sudo apt install libjson-glib-dev
```

### Compiling the included examples

Edit the file **RunExample.c** and set the name of the selected example in the **SELECTED_EXAMPLE** macro

Run this command to compile the binary file:

```
$ make
```

Run this command to start the selected example:

```
$ ./runExample
```

Run this command to delete the generated files:

```
$ make clean
```

### Compiling a shared library

Run this command to compile a shared library in the current folder:

```
$ make library=true
```

Run this command to delete the generated files:

```
$ make library=true clean
```

