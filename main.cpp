#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "matrixMul.cpp"
#include <algorithm>
#include <random>
#include <math.h>
#include <map>
#include "gnuplot_i.hpp"

using namespace std;

void shuffle_data(vector<vector<double> > &data, vector<vector<double> > &labels) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(begin(data), end(data), default_random_engine(seed));
	shuffle(begin(labels), end(labels), default_random_engine(seed));
}

void train_test_split(vector<vector<double> > &data, vector<vector<double> > &labels, vector<vector<double> > &X_train, vector<vector<double> > &X_test, vector<vector<double> > &y_train, vector<vector<double> > &y_test, double test_size){
	shuffle_data(data, labels); // Shuffle data before spliting
	int testSamples = data.size() * test_size;

	for (int i = 0; i < data.size(); i++){
		if (i < testSamples){
			X_test.push_back(data[i]);
			y_test.push_back(labels[i]);
		} else {
			X_train.push_back(data[i]);
			y_train.push_back(labels[i]);
		}
	}
}

void load_data(string path, vector<vector<double> > &data, vector<string> &labels, const string& outOfData) {
	string line;
	ifstream myfile(path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			string delimiter = ",";
			size_t pos = 0;
			string token;

			vector<double> temp;
			while ((pos = line.find(delimiter)) != string::npos) {
				token = line.substr(0, pos);
				temp.push_back(stod(token));
				line.erase(0, pos + delimiter.length());
			}
			if (line == outOfData) continue; // Perceptron is binary classfier so we need to took out one class
			labels.push_back(line);			 // parameter outOfData is used for that.
			data.push_back(temp);
		}
		myfile.close();
	} else cout << "Unable to open file";
}

vector<vector<double> > convertLabels(vector<string> labels) {
	int iter_ = 1;
	map<string, int> LookupTable;
	for (string x: labels) {
		if (LookupTable.find(x) == LookupTable.end()) {
			LookupTable[x] = iter_;
			iter_ *= -1;
		}
	}

	int iter2_ = 0;
	vector<vector<double> > result(labels.size());
	for (string x: labels) {
		result[iter2_].push_back(LookupTable[x]);
		iter2_++;
	}
	return result;
}


class Perceptron {
private:
	double eta;
	int epochs;
	vector<vector<double> > weights;

	void addBias(vector<vector<double> > &X);

	vector<vector<double> > activation(vector<vector<double> > X);

public:
	vector<double> errors;

	Perceptron(double et, int ep);

	void fit(vector<vector<double> > X, vector<vector<double> > y);

	vector<vector<double> > decision(vector<vector<double> > X);

	vector<vector<double> > predict(vector<vector<double> > X);

	double score(vector<vector<double> > X, vector<vector<double> > y);

};

Perceptron::Perceptron(double et, int ep) {
	eta = et;
	epochs = ep;
	srand(time(NULL));
}

void Perceptron::fit(vector<vector<double> > X, vector<vector<double> > y) {
	addBias(X);

	// Set random weights
	for (int i = 0; i < X[0].size(); i++) {
		vector<double> temp = {(double) rand() / (double) RAND_MAX};
		weights.push_back(temp);
	}

	// Training
	for (int i = 0; i < epochs; i++) {
		vector<vector<double> > output = activation(MatrixMul(X, weights));
		vector<vector<double> > error = MatrixSub(y, output);
		weights = MatrixAdd(weights, MatrixMul(MatrixMul(MatrixTranspose(X), error), eta));
		errors.push_back(MatrixSum(MatrixNotEqual(output, y)));
	}
}

vector<vector<double> > Perceptron::decision(vector<vector<double> > X) {
	addBias(X);
	return MatrixMul(X, weights);
}

vector<vector<double> > Perceptron::activation(vector<vector<double> > X) {
	vector<vector<double> > result;
	for (int i = 0; i < X.size(); i++) {
		for (int k = 0; k < X[0].size(); k++) {
			vector<double> temp;
			if (X[i][k] > 0) temp.push_back(1);
			else temp.push_back(-1);
			result.push_back(temp);
		}
	}
	return result;
}

vector<vector<double> > Perceptron::predict(vector<vector<double> > X) {
	vector<vector<double> > result;
	vector<vector<double> > decisionPred;

	decisionPred = decision(X);
	for (int i = 0; i < decisionPred.size(); i++) {
		for (int k = 0; k < decisionPred[0].size(); k++) {
			vector<double> temp;
			if (decisionPred[i][k] > 0) temp.push_back(1);
			else temp.push_back(-1);
			result.push_back(temp);
		}
	}
	return result;
}

void Perceptron::addBias(vector<vector<double> > &X) {
	for (int i = 0; i < X.size(); i++) {
		X[i].push_back(1);
	}
}

double Perceptron::score(vector<vector<double> > X, vector<vector<double> > y) {
	return MatrixSum(MatrixEqual(X, y)) / y.size();
}

int main() {
//	vector<vector<double> > A = {{1,2,3,4}};
//	vector<vector<double> > B = {{5,3,4},{1,1,3},{3,1,3},{6,2,1}};
//
//	vector<vector<double> > temp;
//	temp = MatrixTranspose(A);


	vector<vector<double> > data;
	vector<string> labels;
	int epochs = 100;
	double eta = 0.0001;

//	Loading data
	load_data("../data/iris.txt", data, labels, "Iris-virginica");

//	Converting string labels into -1 and 1
	vector<vector<double> > convertedLabels = convertLabels(labels);

	vector<vector<double> > X_train;
	vector<vector<double> > X_test;
	vector<vector<double> > y_train;
	vector<vector<double> > y_test;

//	Split data into training and testing dataset
	train_test_split(data, convertedLabels, X_train, X_test, y_train, y_test, 0.2);

//	Train perceptron
	Perceptron perceptron(eta, epochs);
	perceptron.fit(X_train, y_train);

	vector<vector<double> > predictions;
	predictions = perceptron.predict(X_test);

//	for (int i = 0; i < predictions.size(); i++) {
//		for (int k = 0; k < predictions[0].size(); k++) {
//			cout << predictions[i][k] << " ";
//		}
//		cout << y_test[i][0] << endl;
//	}
//	for (double x : perceptron.errors){
//		cout << x << "   ";
//	}
	cout << endl;
	cout << "Dokładność perceptronu to: " << perceptron.score(predictions, y_test) * 100 << " %"<< endl;


	Gnuplot gp("lines");
	gp.set_terminal_std("pngcairo");
	gp.plot_x(perceptron.errors, "Error");
	gp.set_xrange(1, epochs);
	gp.set_xlabel("Epochs");
	gp.set_ylabel("Number of errors");


	return 0;
}