#include <ctime>


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

	return MatrixSum(MatrixEqual(predict(X), y)) / y.size();
}
