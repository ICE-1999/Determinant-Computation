# Determinant-Computation
Compilation and Run:
	gcc main.c -o main
	./main
	OR
	make
	./main

Testing:
	Test 1: Mandatory Test Data
		Input:
			M = 20 20 50
			    10 6  70
			    40 3  2
	
		Output:
			Determinant of matrix: 41140
			Largest number in matrix: 70
			
	Test 2:
		Input:
			M = 2 23 10
			    7 16 20
			    4 32 21
			    
		Output:
			Determinant of matrix: -549
			Largest number in matrix: 32
			
	Test 3:
		Input:
			M = 8 27 40
			    3 34 30
			    6 18 29
			    
		Output:
			Determinant of matrix: 79
			Largest number in matrix: 40
			

Design:
	First of all two arrays D and L of size 3 was created using shared memory functions. It is the memory that will be shared by the parent and childs to read and write data into.In our case, childs will write data into the shared memory and parent will read from the shared memory once childs finish writing. Three child processes are created each of them will calculate determinant of its part and store in the corresponding index of array D which is the shared memory. Also each process checks for the largest integer in its corresponding row and store it in the corresponding index of array L which is also the shared memory. In the parent process we wait for all the three childs to finish their task. Once they finish parent will calculate the result R by reading and adding elements of array D and print R on the screen. Then parent will compare all the values in the array L and print the largest on the screen.In the end detached the shared memory arrays and print the total time taken to do the processes.
