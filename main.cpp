#include <iostream>
#include <vector>
#include <string>
#include "matrixMul.cpp"
#include <algorithm>
#include <random>
#include <math.h>
#include <map>
#include "gnuplot_i.hpp"

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
	cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
	std::cout << std::endl << "Press ENTER to continue..." << std::endl;

	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
#endif
	return;
}


void shuffle_data(std::vector<std::vector<double> > &data, std::vector<std::vector<double> > &labels) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(begin(data), end(data), std::default_random_engine(seed));
	shuffle(begin(labels), end(labels), std::default_random_engine(seed));
}

void train_test_split(std::vector<std::vector<double> > &data, std::vector<std::vector<double> > &labels, std::vector<std::vector<double> > &X_train, std::vector<std::vector<double> > &X_test, std::vector<std::vector<double> > &y_train, std::vector<std::vector<double> > &y_test, double test_size){
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

void load_data(std::string path, std::vector<std::vector<double> > &data, std::vector<std::string> &labels, const std::string& outOfData) {
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			std::string delimiter = ",";
			size_t pos = 0;
			std::string token;

			std::vector<double> temp;
			while ((pos = line.find(delimiter)) != std::string::npos) {
				token = line.substr(0, pos);
				temp.push_back(stod(token));
				line.erase(0, pos + delimiter.length());
			}
			if (line == outOfData) continue; // Perceptron is binary classfier so we need to took out one class
			labels.push_back(line);			 // parameter outOfData is used for that.
			data.push_back(temp);
		}
		myfile.close();
	} else std::cout << "Unable to open file";
}

std::vector<std::vector<double> > convertLabels(std::vector<std::string> labels) {
	int iter_ = 1;
	std::map<std::string, int> LookupTable;
	for (std::string x: labels) {
		if (LookupTable.find(x) == LookupTable.end()) {
			LookupTable[x] = iter_;
			iter_ *= -1;
		}
	}

	int iter2_ = 0;
	std::vector<std::vector<double> > result(labels.size());
	for (std::string x: labels) {
		result[iter2_].push_back(LookupTable[x]);
		iter2_++;
	}
	return result;
}


class Perceptron {
private:
	double eta;
	int epochs;
	std::vector<std::vector<double> > weights;

	void addBias(std::vector<std::vector<double> > &X);

	std::vector<std::vector<double> > activation(std::vector<std::vector<double> > X);

public:
	std::vector<double> errors;

	Perceptron(double et, int ep);

	void fit(std::vector<std::vector<double> > X, std::vector<std::vector<double> > y);

	std::vector<std::vector<double> > decision(std::vector<std::vector<double> > X);

	std::vector<std::vector<double> > predict(std::vector<std::vector<double> > X);

	double score(std::vector<std::vector<double> > X, std::vector<std::vector<double> > y);

};

Perceptron::Perceptron(double et, int ep) {
	eta = et;
	epochs = ep;
	srand(time(NULL));
}

void Perceptron::fit(std::vector<std::vector<double> > X, std::vector<std::vector<double> > y) {
	addBias(X);

	// Set random weights
	for (int i = 0; i < X[0].size(); i++) {
		std::vector<double> temp = {(double) rand() / (double) RAND_MAX};
		weights.push_back(temp);
	}

	// Training
	for (int i = 0; i < epochs; i++) {
		std::vector<std::vector<double> > output = activation(MatrixMul(X, weights));
		std::vector<std::vector<double> > error = MatrixSub(y, output);
		weights = MatrixAdd(weights, MatrixMul(MatrixMul(MatrixTranspose(X), error), eta));
		errors.push_back(MatrixSum(MatrixNotEqual(output, y)));
	}
}

std::vector<std::vector<double> > Perceptron::decision(std::vector<std::vector<double> > X) {
	addBias(X);
	return MatrixMul(X, weights);
}

std::vector<std::vector<double> > Perceptron::activation(std::vector<std::vector<double> > X) {
	std::vector<std::vector<double> > result;
	for (int i = 0; i < X.size(); i++) {
		for (int k = 0; k < X[0].size(); k++) {
			std::vector<double> temp;
			if (X[i][k] > 0) temp.push_back(1);
			else temp.push_back(-1);
			result.push_back(temp);
		}
	}
	return result;
}

std::vector<std::vector<double> > Perceptron::predict(std::vector<std::vector<double> > X) {
	std::vector<std::vector<double> > result;
	std::vector<std::vector<double> > decisionPred;

	decisionPred = decision(X);
	for (int i = 0; i < decisionPred.size(); i++) {
		for (int k = 0; k < decisionPred[0].size(); k++) {
			std::vector<double> temp;
			if (decisionPred[i][k] > 0) temp.push_back(1);
			else temp.push_back(-1);
			result.push_back(temp);
		}
	}
	return result;
}

void Perceptron::addBias(std::vector<std::vector<double> > &X) {
	for (int i = 0; i < X.size(); i++) {
		X[i].push_back(1);
	}
}

double Perceptron::score(std::vector<std::vector<double> > X, std::vector<std::vector<double> > y) {
	return MatrixSum(MatrixEqual(X, y)) / y.size();
}

int main() {
//	std::vector<std::vector<double> > A = {{1,2,3,4}};
//	std::vector<std::vector<double> > B = {{5,3,4},{1,1,3},{3,1,3},{6,2,1}};
//
//	std::vector<std::vector<double> > temp;
//	temp = MatrixTranspose(A);


	std::vector<std::vector<double> > data;
	std::vector<std::string> labels;

//	Loading data
	load_data("../data/iris.txt", data, labels, "Iris-setosa");
//	Converting std::string labels into -1 and 1
	std::vector<std::vector<double> > convertedLabels = convertLabels(labels);

	std::vector<std::vector<double> > X_train;
	std::vector<std::vector<double> > X_test;
	std::vector<std::vector<double> > y_train;
	std::vector<std::vector<double> > y_test;

//	Split data into training and testing dataset
	train_test_split(data, convertedLabels, X_train, X_test, y_train, y_test, 0.2);

//	Train perceptron
	Perceptron perceptron(0.0001, 100);
	perceptron.fit(X_train, y_train);

	std::vector<std::vector<double> > predictions;
	predictions = perceptron.predict(X_test);

	for (int i = 0; i < predictions.size(); i++) {
		for (int k = 0; k < predictions[0].size(); k++) {
			std::cout << predictions[i][k] << " ";
		}
		std::cout << y_test[i][0] << std::endl;
	}

	for (double x : perceptron.errors){
		std::cout << x << "   ";
	}
	std::cout << std::endl;
	std::cout << "Dokładność perceptronu to: " << perceptron.score(predictions, y_test) * 100 << " %"<< std::endl;



	std::vector<std::vector<double> > results;
	std::vector<double> epochs;
	for (int i = 1; i <= 100; i++){
		epochs.push_back(i);
	}

	results.push_back(epochs);
	results.push_back(perceptron.errors);


	Gnuplot gp("errors");
	gp.set_style("lines");
	gp.plot_xy(epochs, perceptron.errors, "Wykres");
	wait_for_key();
	return 0;
}