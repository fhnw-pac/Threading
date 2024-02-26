#include <iostream>
#include <random>
#include <chrono>
#include <thread>

constexpr int SEARCHNUMBER = 42;
constexpr int NO_OF_CORES = 8;

int seq(int* data, int size) {
	for (int i = 0; i < size; ++i) {
		if (data[i] == SEARCHNUMBER) {
			return i;
		}
	}
	return -1;
}

void searchPart(int* data, int from, int to, int* res) {
	for (int i = from; i < to; ++i) {
		if (data[i] == SEARCHNUMBER) {
			*res = i;
			return;
		}
	}
}

int par(int* data, int size) {

	std::thread threads[NO_OF_CORES];
	int results[NO_OF_CORES];
	memset(results, 0, NO_OF_CORES * sizeof(int));

	// TODO create and start the threads here
	// ...


	// TODO join the threads after the work is done
	// ...

	for (int i = 0; i < NO_OF_CORES; ++i) {
		if (results[i] != 0)
			return results[i];
	}

	throw "NOT_FOUND";
}

int main() {

	// create data
	const unsigned size = ((16 * 1024 * 1024) / NO_OF_CORES) * NO_OF_CORES;
	int* data = new int[size]; // 16M * 4B = 64MB

	// random gen
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, size - 1);
	generator.seed((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

	// fill data array with noise
	for (size_t i = 0; i < size; ++i) {
		int num = distribution(generator);
		data[i] = num == SEARCHNUMBER ? 0 : num;
	}

	double seqTime = 0;
	int n = 15;

	for (int i = 0; i < n; ++i) {

		data[distribution(generator)] = SEARCHNUMBER;

		auto start = std::chrono::high_resolution_clock::now();

		int res = seq(data, size);

		auto end = std::chrono::high_resolution_clock::now();

		seqTime += (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count();
		data[res] = 0;
	}
	seqTime /= n;

	// parallel
	double parTime = 0;
	for (int i = 0; i < n; ++i) {

		data[distribution(generator)] = SEARCHNUMBER;

		auto start = std::chrono::high_resolution_clock::now();

		int res = par(data, size);

		auto end = std::chrono::high_resolution_clock::now();

		parTime += (std::chrono::duration_cast<std::chrono::milliseconds>(end - start)).count();
		data[res] = 0;
	}
	parTime /= n;

	// cleanup
	delete[] data;

	std::cout << "seq time = " << seqTime << "ms" << std::endl;
	std::cout << "par time = " << parTime << "ms" << std::endl;
	std::cout << "speedup = " << seqTime / parTime << std::endl;
}
