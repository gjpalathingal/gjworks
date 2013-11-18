#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "grs.h"

int grsRedirectonHandle(char* grsCmdArgs[], char grsRedirectionChar);
//int grs_redirect(char const *cmd[],char filename[],char redirect);
int grs_output_redirect(char  *outcmd[],char filename[]);
int grs_input_redirect(char  *incmd[], char filename[]);

 int grsRedirectonHandle(char  *cmd[], char redirect){

  if(redirect == '>'){
  // printf("output redirect");
   grs_output_redirect(cmd, *&cmd[1]);
   return 0;
  }

  if(redirect == '<'){
  // printf("intput redirect");
   grs_input_redirect(cmd, *&cmd[1]);
   return 0;
  }
  return -1; 
 }

  /* API to implement the output redirect system call*/
 int grs_output_redirect(char  *outcmd[], char filename[]){

	 int file, status;
	 int dupStdout = dup(fileno(stdout));
	 file = open(filename,O_WRONLY | O_TRUNC | O_CREAT , S_IRUSR | S_IRGRP |S_IWGRP | S_IWUSR | S_IRWXU);
	 if(file <0){
		 printf("\n Output file open error \n");
		 return -1;
	 }
	 dup2(file, fileno(stdout));
	 close(file);
	 status = grsSpawnProcess(outcmd[0], false);

	 dup2(dupStdout, fileno(stdout));

	 close(dupStdout);

	 if(status < 0){
	 		  printf("\n Error executing command %s \n",outcmd[0]);
	 		  return -1;
	}
	 return 0;
 }

  /* API to implement the input redirect system call*/
  int grs_input_redirect(char *incmd[], char filename[]){

	  int status;
	  FILE *input;
	  int dupStdin = dup(fileno(stdin));
	  input = fopen(filename,"r");
	  if(input == NULL){
		  printf("\n Error opening input file \n");
		  return -1;
	  }
	  dup2(fileno(input), fileno(stdin));
	  close(fileno(input));
	  status = grsSpawnProcess(incmd[0], false);
	  dup2(dupStdin, fileno(stdin));
	  close(dupStdin);

	  if(status < 0){
		  printf("\n Error executing command %s \n",incmd[0]);
		  return -1;
	  }
	  return 0;
  }

  #if 0
   int main(int argc, char **argv){
     //char filename[20] = "test.txt";
     char  *outcmd[]={"/bin/ls -l", "test2.txt"};
     char  *incmd[] = {"wc","test2.txt"};
   //  printf("redirect command");

     grsRedirectonHandle(outcmd,'>');
     grsRedirectonHandle(incmd,'<');
     return 0;
   
   }
#endif
