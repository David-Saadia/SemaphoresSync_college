#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#define NUMOFSEM 5


union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };

typedef union semun Semun;

void semWaitSignal(int,int);
void processCode(int);
void initSema();
void delocateSema();

int semid;
int i, testFor;

struct sembuf sops[NUMOFSEM];

Semun semarg; //Union of set of instructions on how to access the semaphore array. And store data in buffer.



void main(){
	
    /*Initialize semaphores*/
    initSema(); 

    /*Start creating processes*/
    for (i = 1; i <= NUMOFSEM; i++) {
        if (fork() == 0) {
            processCode(i);
            exit(0);
        }
    }

    /*Wait for all child processes to complete*/
    for (i = 0; i < NUMOFSEM; i++)
    	wait(NULL);

    printf("All processes finished\n");
    /*Clean up the semaphores*/
    delocateSema(); 

    return;
	
}

void semWaitSignal(int procNumb, int val){	
	
	/*Introduce critical section*/
	if (val==-4){
		sops[procNumb-1].sem_op=val;
		if(testFor=semop(semid, (sops+procNumb-1), 1)==-1)
			perror("SEMOP Wait ERROR ");
	}
	/*Release critical section*/
	/*Incremenet all semaphores besides itself.*/
	else{
		for( i = 0; i < NUMOFSEM; i++){
			if(i==procNumb-1){
				sops[procNumb-1].sem_op=0;
				continue;
			}
			sops[i].sem_op=val;
		}
		if(testFor=semop(semid,sops,NUMOFSEM)==-1)
			perror("SEMOP Signal ERROR ");
	}
		
}

void initSema(){
	semid = semget (IPC_PRIVATE, NUMOFSEM, 0666); //Get the array of semaphores at size NUMOFSEM =5 .
	semarg.array = (short*)malloc(NUMOFSEM*sizeof(short)); //Intialize the value array in the Union.
	
	for (i = 0; i < NUMOFSEM; i++){
		semarg.array[i]= 4-i; //Intialize the array with the values: 4,3,2,1,0
		sops[i].sem_num = i;
		sops[i].sem_flg = 0;
	}
		
	if(testFor=semctl(semid, 0, SETALL, semarg)!=0)
		perror("Error on SET ");
		
}

void processCode(int procNum){
	int j=procNum;
    while (j < 101) {
        semWaitSignal(procNum,-4); // Wait()
        printf("Process %d is in critical section, number: %d\n", procNum, j);
        j+=5;
        semWaitSignal(procNum,1); // Signal()
    }	
}

void delocateSema(){
	if(testFor=semctl(semid, 0, IPC_RMID,semarg)!=0)
		perror("Removal ERROR ");
}





