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

## Dependencies

T-Rex uses the GNOME JSON-Glib library to load and save trained neural networks in json files:

https://wiki.gnome.org/Projects/JsonGlib

To install JSON-Glib in Ubuntu, open a system console and run this command:

```
$ sudo apt install libjson-glib-dev
```

Then, run this command and add the names of the libraries without `-l` to your Eclipse Project:

```
$ pkg-config --libs json-glib-1.0 

-ljson-glib-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
```

Finally, run this command and add the include paths without `-I` to your Eclipse Project:

```
$ pkg-config --cflags json-glib-1.0

-pthread -I/usr/include/json-glib-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
```

## Quick guide

- Download Eclipse IDE for C/C++ Developers: https://www.eclipse.org/cdt/
- Create a new C Project in the root folder of the repository
- Add the JSON-Glib dependencies to your project
- Edit the file **RunExample.c** and set the name of the selected example in the **SELECTED_EXAMPLE** macro
- Compile and run the project
