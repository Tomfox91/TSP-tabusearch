#include "params.h"
#include <fstream>
#include <iostream>
#include <climits>

using std::ifstream;
using std::cerr;
using std::endl;

const double Params::costUnit = 1E-5;
const Cost Params::costMax = INT_MAX;

Params::Params(const string& filename) {
	ifstream f(filename.c_str());
	if (!f.is_open()) {
		cerr << "Cannot open file " << filename << endl;
		exit(25);
	}

	f >> N;
	C.resize(N);
	double c;

	for (int i = 0; i < N; ++i) {
		C[i].resize(N);
		for (int j = 0; j < N; ++j) {
			f >> c;
			c /= costUnit;
			if (c >= costMax) {
				cerr << "Cost exceeds " << costMax << endl;
				exit(26);
			}
			C[i][j] = c;
		}
	}

	f.close();
}
