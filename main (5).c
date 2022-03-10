#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <limits.h>


int main(int argc, char** argv){

	int i,j;
	int shmid,shmid2;

	int M[3][3] = {{20,20,50},
		       {10,6,70},
		       {40,3,2}};
	
	//Creating shared memory for D which will store determinants
	int *D;
	shmid = shmget(IPC_PRIVATE,sizeof(int)*3,IPC_CREAT| SHM_R| SHM_W);
	D = (int*)shmat(shmid,0,0);
	for (i=0;i<3;i++)D[i] = 0;
	
	
	//Creating shared memory for L array which store largest integer in a row
	int *L;
	shmid2 = shmget(IPC_PRIVATE,sizeof(int)*3,IPC_CREAT| SHM_R| SHM_W);
	L = (int*)shmat(shmid2,0,0);
	for (i=0;i<3;i++)L[i] = INT_MIN;
	
	
	clock_t begin = clock();
	
	pid_t pid = fork();
	
	if (pid == 0){      //Child process 1
	
		printf("Child process: Working with element 1 of D\n");
	
		D[0] = M[0][0]*((M[1][1]*M[2][2])-(M[1][2]*M[2][1]));
		
		for (i=0;i<3;i++){
			if (M[0][i]>L[0])
				L[0] = M[0][i];
		}
		
		exit(0);
	
	
	}
	else{
		pid_t pid2 = fork();
	
		if (pid2 == 0){       //Child process 2
		
			printf("Child process: Working with element 2 of D\n");
			
			D[1] = M[0][1]*((M[1][2]*M[2][0])-(M[1][0]*M[2][2]));
		
			for (i=0;i<3;i++){
				if (M[1][i]>L[1])
					L[1] = M[1][i];
			}
			
			exit(0);
	
		}
		else{
			
			pid_t pid3 = fork();
	
			if (pid3 == 0){     //Child process 3
			
				printf("Child process: Working with element 3 of D\n");
			
				D[2] = M[0][2]*((M[1][0]*M[2][1])-(M[2][0]*M[1][1]));
		
				for (i=0;i<3;i++){
					if (M[2][i]>L[2])
						L[2] = M[2][i];
				}
				
				exit(0);
	
	
			}
			else{
				
				//parent will wait for all 3 childs to finish
				wait(pid);
				wait(pid2);
				wait(pid3);
				
				
				
				//Calculate Determinant R and print it
				int R = D[0]+D[1]+D[2];
				printf("Determinant of matrix: %d\n",R);
				
				//Calculate Largest element and print it
				int Max=L[0];
				for (i=0;i<3;i++)
					if (L[i]>Max)
						Max = L[i];
						
				printf("Largest number in matrix: %d\n",Max);
				
				//Detaching the shared memory
				shmdt((void*)D);
				shmdt((void*)L);
				
				clock_t end = clock();
				float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
				printf("Time Taken = %f\n",time_spent);
				
				return 0;		
		
	
			}
	
		}	
	
	}



}
