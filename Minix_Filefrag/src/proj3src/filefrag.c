#include <stdio.h>
#include <stdlib.h>
#include <ext_frag.h>

void main(int argc, char *argv[])
{
	if(argc < 2)
	{
	 do_grsExtFrag();  // Print External  fragmentation report for the whole FS
	 do_grsIntFrag();  // Print Internal  fragmentation report for all the files in the FS
	}
	else
	{
	   // Internal fragmentation sys calls for individual files and directories.
	  // check if dir
	   DIR *dir = opendir(argv[1]);
           if(dir!=NULL)
   	   {
              do_grsFragIntDir(argv[1]);
           }else{
               FILE *file = fopen( argv[1], "r" );

               if(file != NULL){
                 do_grsFragIntFile(argv[1]);    
               }else{

                  printf("invalid file/directory");
               }
           }
	}
	return;
}
