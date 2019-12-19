#pragma once

/*
Net.h
Will Massey
Feed Forward Neural Network
*/

#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <random>

using namespace std;

class Layer;

class Neuron
{
	public:
	
	double input = 0;
	double output = 0;
	vector<double> weightsForPreviousLayer;

	void createRandomNeuron(int numNeuronsInPreviousLayer);

	void activate_sigmoid();
	void activate_tanh();
	void getInputFromPreviousLayer(Layer prevLayer);
};

class Layer
{
	public:

	vector<Neuron> neurons;
	double bias = 1;

	void createRandomLayer(int numNeurons, int numNeuronsInPreviousLayer);

	void getInputFromPreviousLayer(Layer prevLayer);
	int getSize();
	void activate_sigmoid();
	void activate_tanh();
};


class Net
{
	public:

	vector<Layer> layers;
	vector<double> DNA_weights;
	vector<double> DNA_bias;

	void createRandomNet(int numInputNeurons, int numHiddenLayers,
		int numHiddenLayerNeurons, int numOutputNeurons);

	void setInputs(vector<double> inputs);
	void feedForward();
	vector<double> getDNA_weights();
	vector<double> getDNA_bias();
	void setDNA_weights(vector<double> newDNA);
	void setDNA_bias(vector<double> newDNA);
};

