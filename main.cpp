#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include "params.h"
#include "sequence.h"
#include "heur/localSearch.h"
#include "heur/tabuSearch.h"
#include "greedy/nearestNeighbour.h"
#include "greedy/nfInsertion.h"
#include "util/trimatrix.h"

using std::cout;
using std::cerr;
using std::endl;
using std::chrono::microseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

void fail(const char * const myname, char errCode) {
	cerr << "Usage: " << myname << " filename ord/nn/ni/fi ls" << endl;
	cerr << "Usage: " << myname << " filename ord/nn/ni/fi ts mi tl mwm dm mp" << endl;
	exit(errCode);
}


int main(int argc, char const *argv[]) {
	if (argc <= 3 || (strcmp(argv[3], "ts") == 0 && argc <= 8)) fail(argv[0], 11);

	const string filename = argv[1];
	const Params params("test-case/" + filename + ".distances.txt");

	void (*initFunction)(Sequence&, const Params&) = nullptr;
	if (strcmp(argv[2], "ord") == 0) {
		initFunction = orderInit;
	} else if (strcmp(argv[2], "nn") == 0) {
		initFunction = nearestNeighbourInit;
	} else if (strcmp(argv[2], "ni") == 0) {
		initFunction = nfInsertionInit<true>;
	} else if (strcmp(argv[2], "fi") == 0) {
		initFunction = nfInsertionInit<false>;
	} else fail(argv[0], 12);

	auto start = high_resolution_clock::now();
	Sequence sequence(initFunction, params);
	sequence.print();

	if (strcmp(argv[3], "ls") == 0) {
		localSearch(sequence, params);

	} else if (strcmp(argv[3], "ts") == 0) {
		TabuParams tabuParams(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]));

		tabuSearch(sequence, params, tabuParams);

	} else fail(argv[0], 15);
	auto end = high_resolution_clock::now();
	double t = duration_cast<microseconds>(end-start).count() * 1E-6;

	printf("Time: %9.6f\n", t);

	std::ofstream sf(("test-results/" + filename + "-" + argv[3] + ".txt").c_str());
	sf << "Obj: " << std::fixed << std::setprecision(5) << sequence.cost * Params::costUnit << std::setprecision(0) << endl;

	sf << sequence[0];
	for (int i = 1; i <= params.N; ++i) {
		sf << " -> " << sequence[i];
	}
	sf << endl;

	sf << "Time: " << std::setprecision(6) << t << endl;

	if (strcmp(argv[3], "ts") == 0) {
		sf << "TS params: " << argv[4] << " " << argv[5] << " " << argv[6] << " " << argv[7] << " " << argv[8] << endl;
	}

	sf.close();

	return 0;
}