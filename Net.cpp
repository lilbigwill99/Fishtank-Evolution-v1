/*
Net.cpp
Will Massey
*/

#include "Net.h"
#include "Tools.h"

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

void Neuron::createRandomNeuron(int numNeuronsInPreviousLayer)
{
	/*
		creates a new randomly generated neuron
	*/
	Tools toolBox;
	weightsForPreviousLayer.clear();
	
	for(int i = 0; i < numNeuronsInPreviousLayer; i++)
	{
		double newWeight = toolBox.generateRandomNumber();
		weightsForPreviousLayer.push_back(newWeight);
	}
}

void Neuron::activate_sigmoid()
{
	/*
		sigmoid nonlinear activation function
	*/
	output = 1 / (1 + exp(input * -1));
}

void Neuron::activate_tanh()
{
	/*
		tanh nonlinear activation function
	*/
	output = tanh(input);
}

void Neuron::getInputFromPreviousLayer(Layer prevLayer)
{
	/*
		sets the input based on the previous layer's output
	*/
	double newInput = prevLayer.bias;
	
	for (int i = 0; i < prevLayer.neurons.size(); i++)
	{
		newInput += prevLayer.neurons[i].output * weightsForPreviousLayer[i];
	}

	input = newInput;
}

void Layer::createRandomLayer(int numNeurons, int numNeuronsInPreviousLayer)
{
	/*
		creating a new layer with randomly generated neurons
	*/
	neurons.clear();
	Tools toolbox;
	bias = toolbox.generateRandomNumber();

	for (int i = 0; i < numNeurons; i++)
	{
		Neuron n;
		n.createRandomNeuron(numNeuronsInPreviousLayer);
		neurons.push_back(n);
	}
}

void Layer::getInputFromPreviousLayer(Layer prevLayer)
{
	/*
		gathers the outputs from the previous layer and sets the inputs
		for this layer
	*/
	for (int i = 0; i < neurons.size(); i++)
	{
		neurons[i].getInputFromPreviousLayer(prevLayer);
	}
}

int Layer::getSize()
{
	return neurons.size();
}

void Layer::activate_sigmoid()
{
	/*
		activates the entire layer
	*/
	for (int i = 0; i < neurons.size(); i++)
	{
		neurons[i].activate_sigmoid();
	}
}

void Layer::activate_tanh()
{
	/*
		activates the entire layer
	*/
	for (int i = 0; i < neurons.size(); i++)
	{
		neurons[i].activate_tanh();
	}
}

void Net::createRandomNet(int numInputNeurons, int numHiddenLayers, 
	int numHiddenLayerNeurons, int numOutputNeurons)
{
	/*
		creates a randomly generated feed forward neural network
	*/
	layers.clear();

	//input layer
	
	Layer inputLayer;
	inputLayer.createRandomLayer(numInputNeurons, 0);
	layers.push_back(inputLayer);

	//hidden layers

	for (int i = 0; i < numHiddenLayers; i++)
	{
		Layer newHiddenLayer;
		newHiddenLayer.createRandomLayer(numHiddenLayerNeurons, layers.back().getSize());
		layers.push_back(newHiddenLayer);
	}

	//output layers
	
	Layer outputLayer;

	for (int i = 0; i < numOutputNeurons; i++)
	{
		Neuron n;
		n.createRandomNeuron(layers.back().getSize());
		outputLayer.neurons.push_back(n);
	}

	layers.push_back(outputLayer);
}

void Net::setInputs(vector<double> inputs)
{
	/*
		sets custom inputs for the first layer
	*/
	if (inputs.size() != layers.front().getSize())
	{
		cout << "error in Net::setInputs; probably wrong amount of inputs." << endl;
	}
	else
	{
		for (int i = 0; i < inputs.size(); i++)
		{
			layers.front().neurons[i].input = inputs[i];
		}
	}
}

void Net::feedForward()
{
	/*
		feed forward the net using the first layer's inputs
	*/
	
	//using sigmoid activation for all the layers except output, will use tanh for that
	//gather first layer inputs first before calling

	for (int i = 0; i < layers.size() - 1; i++)
	{
		layers[i].activate_sigmoid();
		layers[i + 1].getInputFromPreviousLayer(layers[i]);
	}

	layers.back().activate_tanh();
}

vector<double> Net::getDNA_weights()
{
	/*
		gathering the net's weights to use for a genetic aglorithm
	*/
	DNA_weights.clear();

	//skip first index cause first layer doesnt have weights
	for (int l = 1; l < layers.size(); l++)
	{
		for (int n = 0; n < layers[l].neurons.size(); n++)
		{
			for (int w = 0; w < layers[l].neurons[n].weightsForPreviousLayer.size(); w++)
			{
				DNA_weights.push_back(layers[l].neurons[n].weightsForPreviousLayer[w]);
			}
		}
	}
	
	return DNA_weights;
}

vector<double> Net::getDNA_bias()
{
	/*
		gathering each layer's bais to use for a genetic algorithm
	*/
	DNA_bias.clear();

	for (int i = 0; i < layers.size(); i++)
	{
		DNA_bias.push_back(layers[i].bias);
	}
	
	return DNA_bias;
}

void Net::setDNA_weights(vector<double> newDNA)
{
	/*
		sets new weights to each layer
	*/
	int counter = 0;

	//skip first index cause first layer doesnt have weights
	for (int l = 1; l < layers.size(); l++)
	{
		for (int n = 0; n < layers[l].neurons.size(); n++)
		{
			for (int w = 0; w < layers[l].neurons[n].weightsForPreviousLayer.size(); w++)
			{
				layers[l].neurons[n].weightsForPreviousLayer[w] = newDNA[counter];
				counter++;
			}
		}
	}
}

void Net::setDNA_bias(vector<double> newDNA)
{
	/*
		sets new bias for each layer
	*/
	int counter = 0;

	for (int i = 0; i < layers.size(); i++)
	{
		layers[i].bias = newDNA[counter];
		counter++;
	}
}
