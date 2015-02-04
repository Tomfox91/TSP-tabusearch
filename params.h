#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

typedef long Cost;

class Params {
public:
	int N;
	vector<vector<Cost> > C;
	Params(const string& filename);

	static const double costUnit;
	static const Cost costMax;
};
