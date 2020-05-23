//
// Created by Wiktor Kubis on 10/05/2020.
//
#include <map>
#include <random>
#include <cmath>
#include <string>
#include <algorithm>
#include <chrono>

void shuffle_data(vector<vector<double> > &data, vector<vector<double> > &labels) {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	shuffle(begin(data), end(data), default_random_engine(seed));
	shuffle(begin(labels), end(labels), default_random_engine(seed));
}

void train_test_split(vector<vector<double> > &data, vector<vector<double> > &labels, vector<vector<double> > &X_train,
					  vector<vector<double> > &X_test, vector<vector<double> > &y_train,
					  vector<vector<double> > &y_test, double test_size) {
	shuffle_data(data, labels); // Shuffle data before spliting
	int testSamples = data.size() * test_size;

	for (int i = 0; i < data.size(); i++) {
		if (i < testSamples) {
			X_test.push_back(data[i]);
			y_test.push_back(labels[i]);
		} else {
			X_train.push_back(data[i]);
			y_train.push_back(labels[i]);
		}
	}
}

void load_data(string path, vector<vector<double> > &data, vector<string> &labels, const string &outOfData) {
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
			labels.push_back(line);             // parameter outOfData is used for that.
			data.push_back(temp);
		}
		myfile.close();
	} else cout << "Unable to open file";
}


vector<vector<double> > convertLabels(vector<string> labels) {
	int iter_ = -1;
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