#include <iostream>
#include <cmath>
#include <mpi.h>
#include <chrono>

/**
	@author Semih Ataman
	@since 2023-May-30
*/
double computePartialSum(int rank, int numberOfComputeNodes, long long operationStartIndex, long long operationEndIndex) {
	// printf("Rank is %d out of %d threads\n", rank, numberOfComputeNodes);
	// printf("Start Index %lld -> End Index %lld \n", operationStartIndex, operationEndIndex);
	double partialSum = 0.0;
	long long i;

	for (i = operationStartIndex; i <= operationEndIndex; i++) {
		if (i % 2 == 0) {
			partialSum += 1.0 / (2 * i + 1);
		}
		else {
			partialSum -= 1.0 / (2 * i + 1);
		}
	}

	return partialSum;
}

int main(int argc, char* argv[]) {
	int rank, numberOfComputeNodes;
	long long totalNumberOfOperations;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numberOfComputeNodes);

	if (numberOfComputeNodes < 1 || numberOfComputeNodes > 64) {
		if (rank == 0) {
			std::cerr << "Error: Number of threads should be in the range from 1 to 64." << std::endl;
		}

		MPI_Finalize();

		return 1;
	}

	// Start the timer
	double startTime = MPI_Wtime(); 

	if (rank == 0) {
		std::cout << "Enter the total number of operations: ";
		std::cin >> totalNumberOfOperations;
	}

	MPI_Bcast(&totalNumberOfOperations, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

	long long numberOfOperationsPerComputeNodes = totalNumberOfOperations / numberOfComputeNodes;
	long long operationStartIndex = rank * numberOfOperationsPerComputeNodes;
	long long operationEndIndex = (rank == numberOfComputeNodes - 1) ? (totalNumberOfOperations - 1) : (operationStartIndex + numberOfOperationsPerComputeNodes - 1);

	double partialSum = computePartialSum(rank, numberOfComputeNodes, operationStartIndex, operationEndIndex);
	double globalSum;
	MPI_Reduce(&partialSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		const double PI = 3.1415926535897932384626433832795028841971693993751058209;

		double approximatedPi = 4.0 * globalSum;
		std::cout << "Approximate value of pi: " << approximatedPi << std::endl;

		// Calculate the absolute difference
		double absolute_difference = std::abs(PI - approximatedPi);

		// Output the approximation error
		std::cout << "Approximation Error: " << absolute_difference << std::endl;
	}

	// Stop the timer
	double endTime = MPI_Wtime(); 
	// Calculate the duration in milliseconds
	double duration = (endTime - startTime) * 1000.0; 

	if (rank == 0) {
		std::cout << "Total runtime: " << duration << " ms" << std::endl;
	}

	MPI_Finalize();

	return 0;
}
