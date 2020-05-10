#include <iostream>
#include <vector>
#include "gnuplot_i.hpp"
#include "matrixMul.hpp"
#include "perceptron.hpp"
#include "preprocessing.hpp"

using namespace std;

int EPOCHS = 100; // number of epochs
double ETA = 0.0001; // learning rate

int main() {
	vector<vector<double> > data;
	vector<string> labels;

//	Loading data, remove 1 class because perceptron is only binary classifier.
//	In this case we are removing "Iris-virginica" from dataset.
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
	Perceptron perceptron(ETA, EPOCHS);
	perceptron.fit(X_train, y_train);

//	vector<vector<double> > predictions;
//	predictions = perceptron.predict(X_test);

	cout << endl;
	cout << "Dokładność perceptronu na zbiorze testowym to: " << perceptron.score(X_test, y_test) * 100 << " %" << endl;


//	Plot of errors to epochs - GNUPLOT required (apt-get gnuplot (linux), brew install gnuplot (mac)), windows - ?)

	Gnuplot gp("lines");
	gp.set_terminal_std("pngcairo");
	gp.plot_x(perceptron.errors, "Error");
	gp.set_xrange(1, EPOCHS);
	gp.set_xlabel("Epochs");
	gp.set_ylabel("Number of errors");


	return 0;
}