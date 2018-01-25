#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
int main( ){
	pid_t pid;
	int count = 1;
	pid = fork(); // fork another process 
	if (pid < 0 ) { // error occurred
		fprintf(stderr,"Fork Failed");
		exit(-1);
	} else if (pid == 0) {
		count --; //process b count = 0
	}
	
	if (count == 1 ){
		//process c count = 1
		pid = fork();
	}

	if (pid < 0) { // error occurred
		fprintf(stderr,"Fork Failed");
		exit(-1);
	} else if (pid == 0) {// child process
		if (count == 0)
			printf("b\n");
		else if (count == 1)
			printf("c\n");

	} else{
	 // parent process
		printf("a\n");
		exit(0); 
	}
}