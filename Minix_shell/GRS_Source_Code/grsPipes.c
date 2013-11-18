#include <stdio.h>
#include <string.h>

struct grs_pipecmd{
  FILE *read;
  FILE *write;
  char *cmd;
};

int grsPipeHandle(short cmdlen,char *cmds[]);

int grsPipeHandle(short cmdlen,char *cmds[]){

  // struct grs_pipecmd pipecmds[cmdlen];
  int i=0;
  char *cmd = " ";
  FILE *write;

  if(cmdlen < 2){
	  printf(" Insufficient number of arguments for |");
	  return -1;
  }

  strcat(cmd,*&cmds[0]);
   strcat(cmd," | ");

   for(i=1;i<cmdlen-1;i++){
     strcat(cmd,*&cmds[i]);
     strcat(cmd," | ");
     }

   strcat(cmd,*&cmds[cmdlen-1]);
//   printf("\ncmd %s \n",cmd);

   write = popen(cmd,"w");

   if(pclose(write)){
	   perror("pclose(3)");
	   return -1;
   }

   cmd = NULL;
   write = NULL;

 return 0;
}
#if 0
int main(int argc, char *argv[]){
 
   char* cmds[]={"ls -l"," test "};
   grsPipeHandle(cmds,1);
   return 0;
}
 #endif
