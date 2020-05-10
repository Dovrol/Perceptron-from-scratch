//
// Created by Wiktor Kubis on 04/05/2020.
//

#include <vector>

using namespace std;

vector<vector<double> > MatrixMul(vector<vector<double> > A, double B) {
	for (int i = 0; i < A.size(); i++) {
		for (int k = 0; k < A[0].size(); k++) {
			A[i][k] *= B;
		}
	}
	return A;
}

vector<vector<double> > MatrixMul(vector<vector<double> > A, vector<vector<double> > B) {
	vector<vector<double> > result;
	if (A[0].size() != B.size()) {
		throw 1;
	}
	for (int i = 0; i < A.size(); i++) {
		vector<double> temp;
		for (int k = 0; k < B[0].size(); k++) {
			double x = 0;
			for (int j = 0; j < A[0].size(); j++) {
				x += A[i][j] * B[j][k];
			}
			temp.push_back(x);
		}
		result.push_back(temp);
	}
	return result;
}
vector<vector<double> > MatrixSub(vector<vector<double> > A, vector<vector<double> > B) {
	vector<vector<double> > result;
	// Checking if size is correct
	if (A.size() != B.size() || A[0].size() != B[0].size()) {
		throw 1;
	}

	for (int i = 0; i < A.size(); i++) {
		for (int k = 0; k < A[0].size(); k++) {
			vector<double> temp;
			temp.push_back(A[i][k] - B[i][k]);
			result.push_back(temp);
		}
	}
	return result;
}


vector<vector<double> > MatrixAdd(vector<vector<double> > A, vector<vector<double> > B) {
	vector<vector<double> > result;
	// Checking if size is correct
	if (A.size() != B.size() || A[0].size() != B[0].size()) {
		throw 1;
	}

	for (int i = 0; i < A.size(); i++) {
		for (int k = 0; k < A[0].size(); k++) {
			vector<double> temp;
			temp.push_back(A[i][k] + B[i][k]);
			result.push_back(temp);
		}
	}
	return result;
}

vector<vector<double> > MatrixTranspose(vector<vector<double> > A) {
	vector<vector<double> > result;
	double size1 = A[0].size();
	double size2 = A.size();

	for (int i = 0; i < size1; i++) {
		vector<double> temp;
		for (int k = 0; k < size2; k++) {
			temp.push_back(A[k][i]);
		}
		result.push_back(temp);
	}
	return result;
}

vector<double> MatrixEqual(vector<vector<double> > A, vector<vector<double> > B) {
	vector<double> result;

	// Checking if size is correct
	if (A.size() != B.size() || A[0].size() != B[0].size()) {
		throw 1;
	}

	for (int i = 0; i < A.size(); i++) {
		for (int k = 0; k < A[0].size(); k++) {
			if (A[i][k] == B[i][k]){
				result.push_back(1);
			} else {
				result.push_back(0);
			}
		}
	}
	return result;
}

vector<double> MatrixNotEqual(vector<vector<double> > A, vector<vector<double> > B) {
	vector<double> result;

	// Checking if size is correct
	if (A.size() != B.size() || A[0].size() != B[0].size()) {
		throw 1;
	}

	for (int i = 0; i < A.size(); i++) {
		for (int k = 0; k < A[0].size(); k++) {
			if (A[i][k] != B[i][k]){
				result.push_back(1);
			} else {
				result.push_back(0);
			}
		}
	}
	return result;
}


double MatrixSum(vector<double> A) {
	double result = 0;

	for (double x : A){
		result+= x;
	}

	return result;
}