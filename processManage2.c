#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
int main( ){
	pid_t pid;
	int count = 1;
	int i;
	pid = fork(); // fork another process 
	if (pid< 0 ) { // error occurred
		fprintf(stderr,"Fork Failed");
		exit(-1); 
	} else if (pid == 0) {
		//execlp("/bin/ls","ls",NULL);
		count --; //process b count = 1
	}
	
	if (count == 1 ){
		//process c count = 0
		pid = fork();	
	}

	if (pid < 0) { // error occurred
		fprintf(stderr,"Fork Failed");
		exit(-1); 
	} else if (pid > 0) {
	 // parent process
			count --;
			for (i=0; i<100; i++)
				//printf("parent");
				printf("Hello world, I am parent %d, my id is %4d and my parent's id %d . \n",count,getppid(),pid);




	} else{// child process 
			for (i=0; i<100; i++)
				//printf("child%d",count);
				printf("Hello world, I am child %d, my id is %4d and my parent's id is%d \n",count,getppid(),pid);


		exit(0); 
	}
}