#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int grs_multitasking();

#if 1
int main(int argc, char **argv){

	grs_multitasking();
	return 0;
}
#endif
/*
 * API to demonstrate multitasking
 */
int grs_multitasking(){
	int pid, wpid;
	int status;

	pid = fork();

	if(pid == -1){
		perror("fork(2)");
		return 0;
	}else{
		printf("\n Child process .. ");
		printf("\n  Parent process id : %ld",(long)getppid());
		printf("\n  Child process id : %ld \n",(long)getpid());

		return 0;
	}

	
	/*printf("\n Parent process .. ");
	printf("\n Parent: Parent process id : %ld",(long)getpid());
	printf("\n Parent: Child process id : %d\n",pid);*/
        

	wpid = wait(&status);
	if(wpid == -1){
		perror("wait(2)");
	}

	return 0;
}
