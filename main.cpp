/*
	1. I found that the implementation of the ChainingHash was easier compared to
		ProbingHash because all I had to do for the collisons was push an element
		to the back where as for probing I had to manually search for an open slot.
		The time results made sense since my hash tabel was perfectly made for the
		hash tables meaning that insertation and search would favor probing since
		it's simplier to check an element in array compared to checking a linked 
		list for an element, but failed search and deletion would favor chaining
		you would have to check the whole table for probing where as for chaining
		you would only have to check one short linked list.
	2. I found that little needed to be changed between the parallel and serial
		implementation of ProbingHash. It didn't give me the speedup I was
		expecting and I am guessing this is because the rehash function and my use 
		of critical. I think whats happening is that every time a thread enters the 
		rehash function or goes into a critical section, all other threads are put 
		on pause for a duration to avoid memory conflicts, but the duration of this 
		pause is much longer than needed, leading to a general slowdown compared to 
		no parallel processing.
	3. From what I read online, it's best to utilize atomic rather than critcal since
		atomic allows for more control around overhead and thus could lead to less or
		or shorter pauses. I also think that I could restructure the code to be more
		parallel friendly by reducing the amount of memory accesses in hopes to reduce
		the amount of critcal areas which should help speed up the parallel processing
*/

#include "ChainingHash.hpp"
#include "ProbingHash.hpp"
#include "ParallelProbingHash.hpp"

#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#define NUM_THREADS 12  // update this value with the number of cores in your system. 

int main()
{
	std::ofstream outputStream;
	double startTime = 0.0;
	double endTime = 0.0;
	/*Task I (a)- ChainingHash table*/
	outputStream.open("HashAnalysis.txt");
		//  create an object of type ChainingHash 
		
		ChainingHash<int, int> CHash;
		// In order, insert values with keys 1 – 1,000,000. For simplicity, the key and value stored are the same. 
		startTime = omp_get_wtime();
		for(int I = 0; I < 1000000; ++I) {
			CHash.emplace(I, I);
		}
		endTime = omp_get_wtime();
		// Report the total amount of time, in seconds, required to insert the values to ChainingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outputStream << "Chaining Insertion Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 177 in ChainingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		startTime = omp_get_wtime();
		CHash[177];
		endTime = omp_get_wtime();
		outputStream << "Chaining Search Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 2,000,000 in ChainingHash table. Report the time required to find the value in each table by writing it to the file.  
		startTime = omp_get_wtime();
		CHash[2000000];
		endTime = omp_get_wtime();
		outputStream << "Chaining Failed Search Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Remove the value with key 177 from ChainingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		startTime = omp_get_wtime();
		CHash.erase(177);
		endTime = omp_get_wtime();
		outputStream << "Chaining Deletion Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Also, write to the file the final size, bucket count, and load factor of the hash for ChainingHash table. 
		outputStream << "Chaining Table Size: ";
		outputStream << CHash.size() << std::endl;
		outputStream << "Chaining Bucket Count: ";
		outputStream << CHash.bucket_count() << std::endl;
		outputStream << "Chaining Load Factor: ";
		outputStream << std::fixed << std::setprecision(2) << CHash.load_factor() << std::endl;

		outputStream << std::endl;
	/*Task I (b) - ProbingHash table (using Linear Probing) */
		
		//  create an object of type ProbingHash 
		ProbingHash<int, int> PHash;
		// In order, insert values with keys 1 – 1,000,000. For simplicity, the key and value stored are the same.
		startTime = omp_get_wtime();
		for(int I = 0; I < 1000000; ++I) {
			PHash.emplace(I, I);
		}
		endTime = omp_get_wtime();
		// Report the total amount of time, in seconds, required to insert the values to ProbingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outputStream << "Probing Insertion Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 177 in ProbingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		startTime = omp_get_wtime();
		PHash[177];
		endTime = omp_get_wtime();
		outputStream << "Probing Search Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 2,000,000 in ProbingHash table. Report the time required to find the value in each table by writing it to the file.  
		startTime = omp_get_wtime();
		PHash[2000000];
		endTime = omp_get_wtime();
		outputStream << "Probing Failed Search Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Remove the value with key 177 from ProbingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		startTime = omp_get_wtime();
		PHash.erase(177);
		endTime = omp_get_wtime();
		outputStream << "Probing Deletion Time: ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Also, write to the file the final size, bucket count, and load factor of the hash for ProbingHash table. 
		outputStream << "Probing Table Size: ";
		outputStream << PHash.size() << std::endl;
		outputStream << "Probing Bucket Count: ";
		outputStream << PHash.bucket_count() << std::endl;
		outputStream << "Probing Load Factor: ";
		outputStream << std::fixed << std::setprecision(2) << PHash.load_factor() << std::endl;
		
		outputStream << std::endl;
	/*Task II -  ParallelProbingHash table (using Linear Probing) */
      
	  // (a) Using a single thread:  
		//  create an object of type ParallelProbingHash 
		ParallelProbingHash<int, int> PPHash1;
		// Set the number of threads (omp_set_num_threads()) to 1 
		omp_set_num_threads(1);
		/* In an OpenMP parallel region (#pragma omp parallel), in order, insert values with keys 1 – 1,000,000. 
		Inside the parallel region make sure that the value for the iteration number of the loop is shared among all threads. 
		For simplicity, the key and value stored are the same.
        */
        startTime = omp_get_wtime();
        #pragma omp parallel for
		for(int I = 0; I < 1000000; ++I) {
			PPHash1.emplace(I, I);
		}
		endTime = omp_get_wtime();
		// Report the total amount of time, in seconds, required to insert the values to ParallelProbingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outputStream << "Parallel Probing Insertion Time(Single Thread): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 177 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		startTime = omp_get_wtime();
		PPHash1[177];
		endTime = omp_get_wtime();
		outputStream << "Parallel Probing Search Time(Single Thread): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 2,000,000 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the file.  
		startTime = omp_get_wtime();
		PPHash1[2000000];
		endTime = omp_get_wtime();
		outputStream << "Parallel Probing Failed Search Time(Single Thread): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Remove the value with key 177 from ParallelProbingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		startTime = omp_get_wtime();
		PPHash1.erase(177);
		endTime = omp_get_wtime();
		outputStream << "Parallel Probing Deletion Time(Single Thread): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Also, write to the file the final size, bucket count, and load factor of the hash for ParallelProbingHash table. 
		outputStream << "Parallel Probing Table Size(Single Thread): ";
		outputStream << PPHash1.size() << std::endl;
		outputStream << "Parallel Probing Bucket Count(Single Thread): ";
		outputStream << PPHash1.bucket_count() << std::endl;
		outputStream << "Parallel Probing Load Factor(Single Thread): ";
		outputStream << std::fixed << std::setprecision(2) << PPHash1.load_factor() << std::endl;
		
		outputStream << std::endl;

	// (b) Using multiple threads:  
		//  create an object of type ParallelProbingHash 
		ParallelProbingHash<int, int> PPHash2;
		// i.	Change the number of threads to match the number of cores on your system 
		omp_set_num_threads(NUM_THREADS);
		/* In an OpenMP parallel region (#pragma omp parallel), in order, insert values with keys 1 – 1,000,000. 
		Inside the parallel region make sure that the value for the iteration number of the loop is shared among all threads. 
		For simplicity, the key and value stored are the same.
        */
        startTime = omp_get_wtime();
        #pragma omp parallel for
		for(int I = 0; I < 1000000; ++I) {
			PPHash2.emplace(I, I);
		}
		endTime = omp_get_wtime();
		// Report the total amount of time, in seconds, required to insert the values to ParallelProbingHash table. Write the results to a file called “HashAnalysis.txt”. 
		outputStream << "Parallel Probing Insertion Time(12 Threads): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 177 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the “HashAnalysis.txt” file. 
		startTime = omp_get_wtime();
		PPHash2[177];
		endTime = omp_get_wtime();
		outputStream << "Parallel Probing Search Time(12 Threads): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Search for the value with key 2,000,000 in ParallelProbingHash table. Report the time required to find the value in each table by writing it to the file.  
		startTime = omp_get_wtime();
		PPHash2[2000000];
		endTime = omp_get_wtime();
		outputStream << "Parallel Probing Failed Search Time(12 Threads): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Remove the value with key 177 from ParallelProbingHash table. Report the time required to remove the value with in each table by writing it to the file.  
		startTime = omp_get_wtime();
		PPHash2.erase(177);
		endTime = omp_get_wtime();
		outputStream << "Parallel Probing Deletion Time(12 Threads): ";
		outputStream << (endTime - startTime) << " Seconds" << std::endl;
		// Also, write to the file the final size, bucket count, and load factor of the hash for ParallelProbingHash table. 
		outputStream << "Parallel Probing Table Size(12 Threads): ";
		outputStream << PPHash2.size() << std::endl;
		outputStream << "Parallel Probing Bucket Count(12 Threads): ";
		outputStream << PPHash2.bucket_count() << std::endl;
		outputStream << "Parallel Probing Load Factor(12 Threads): ";
		outputStream << std::fixed << std::setprecision(2) << PPHash2.load_factor() << std::endl;
		
		
	outputStream.close();
	return 0;
}