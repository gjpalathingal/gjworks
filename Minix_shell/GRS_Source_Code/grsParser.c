/*
File name : grsParser.c
Author    : George J Palathingal (A20294134)
Date      : 15/09/2012
Last modified : 16/09/2012
Version   : 1.1


Assumptions
- The module assumes that redirection, background, pipe functions are exclusive.
- The string entry length in the shell will not exceed 100 characters.

Algorithm
- grsCmdLoop() keeps on adding the characters to grsTmpStr buffer until enter key is pressed.
- grsCmdLoop() then calls grsFillCmd() which splits the commands and sets any special funcion flags if necessary.
- The control returns to grsCmdLoop() which calls the corresponding module depending on the global flags.
*/


#include "grs.h"

/* The global flags to set special functions - Redirections, background, ifthenelse, pipes */

struct grsFlags{
unsigned int grsPipeFlag		:1;
unsigned int grsRedirOutFlag	:1;
unsigned int grsRedirInFlag     :1;
unsigned int grsBackgroundFlag	:1;
unsigned int grsIfThenElseFlag	:1;
}grsGlobalFlags;

/* This function splits the commands according to |,<,>,;,& and sets the corresponding flags */

unsigned int grsFillCmd(char *grsTmpBuf_argv, char *grsLocalBuf[])
{
    char *grsTmpChar = grsTmpBuf_argv;
    short index = 0,b = 0;
    char grsTmpStr[100];
    bzero(grsTmpStr, 100);
//    printf("The entered string is : %s\n",grsTmpBuf_argv);    //Debug print
    while(*grsTmpChar != '\0') {

        if((*grsTmpChar == '&') | (*grsTmpChar == '|') | (*grsTmpChar == '>') | (*grsTmpChar == '<') | (*grsTmpChar == ';')) {
            if(grsLocalBuf[index] == NULL)
            {
                grsLocalBuf[index] = (char *)malloc(sizeof(char) * strlen(grsTmpStr) + 1);
            }
            else {
                bzero(grsLocalBuf[index], strlen(grsLocalBuf[index]));
            }
            b = strlen(grsTmpStr);
            if(grsTmpStr[b-1]==' ')
            grsTmpStr[b-1] = '\0';

            strcpy(grsLocalBuf[index], grsTmpStr);
            strncat(grsLocalBuf[index], "\0", 1);
#ifdef DEBUG
//            printf("%s\n",grsLocalBuf[index]);
#endif
            if(*grsTmpChar == '&')
                grsGlobalFlags.grsBackgroundFlag = 1;
            if(*grsTmpChar == '|')
                grsGlobalFlags.grsPipeFlag = 1;
            if(*grsTmpChar == '>')
                grsGlobalFlags.grsRedirOutFlag = 1;
            if(*grsTmpChar == '<')
                grsGlobalFlags.grsRedirInFlag = 1;
            if(*grsTmpChar == ';')
                grsGlobalFlags.grsIfThenElseFlag = 1;                
            bzero(grsTmpStr, 100);
            index++;
        } else {
                if((grsTmpStr[0]==0) & (*grsTmpChar==' '))
                {
                 grsTmpChar++;
                 continue;
                }
            strncat(grsTmpStr, grsTmpChar, 1);
        }
        grsTmpChar++;
        /*printf("grsTmpChar is %c\n", *grsTmpChar);*/
    }
    if(*grsTmpStr!=0)
    {
    grsLocalBuf[index] = (char *)malloc(sizeof(char) * strlen(grsTmpStr) + 1);
    strcpy(grsLocalBuf[index], grsTmpStr);
#ifdef DEBUG
//    printf("%s\n",grsLocalBuf[index]);
#endif
    index++;
    }
    return index;
}

/* This function resets all the global flags */
void grsClearFlags()
{
grsGlobalFlags.grsPipeFlag = 0;
grsGlobalFlags.grsRedirOutFlag = 0;
grsGlobalFlags.grsRedirInFlag = 0;
grsGlobalFlags.grsBackgroundFlag = 0;
grsGlobalFlags.grsIfThenElseFlag = 0;    
}

void grsFreeArgv(char* grsTmpBuf[])
{
    short index = 0;
    for(index=0;grsTmpBuf[index]!=NULL;index++) {
    grsTmpBuf[index] = NULL;
    free(grsTmpBuf[index]);
    }
}

/* This function tracks the character entries in the shell prompt and parses the string entered once enter key is pressed */
void grsCmdLoop()
{
    char grsCharInput;
    char *grsCmdArgs[100]={NULL};
    short grsNoOfCmds =0;
    char *grsTmpBuf = (char *)malloc(sizeof(char) * 100);
    bzero(grsTmpBuf, 100);
    grsClearFlags();
    printf("%s ",getenv("PROMPT"));
  while(grsCharInput != EOF) {
        grsCharInput = getchar();
        switch(grsCharInput) {
            case '\n': if(grsTmpBuf[0] == '\0') {
                       printf("%s ", getenv("PROMPT"));
                   } else {
                       strncat(grsTmpBuf, "\0", 1);
//                       printf("Debug 1\n");
                       grsNoOfCmds = grsFillCmd(grsTmpBuf,grsCmdArgs);
                        if(grsGlobalFlags.grsBackgroundFlag == 1)
                            grsBackgroundProcess(grsCmdArgs[0]); // No of commnads will always be 1
                        else if(grsGlobalFlags.grsPipeFlag == 1)
                            grsPipeHandle(grsNoOfCmds,grsCmdArgs); //No of commands is passed to the Fn
                        else if(grsGlobalFlags.grsRedirOutFlag == 1)
                            grsRedirectonHandle(grsCmdArgs,'>'); // The no of commands will always be 1 before < or > 
                        else if(grsGlobalFlags.grsRedirInFlag == 1)
                            grsRedirectonHandle(grsCmdArgs,'<'); // The no of commands will always be 1 before < or > 
                        else if(grsGlobalFlags.grsIfThenElseFlag == 1)
                            grsIfThenElse(grsNoOfCmds,grsCmdArgs); // The no of commands should be 3
                        else
                            {
                            if(strcmp(grsCmdArgs[0],"exit")==0)
                            {
                                    grsFreeArgv(grsCmdArgs);
                                    free(grsTmpBuf);
                                    exit(0);
                            }
                                else
                                grsCmdExec(grsCmdArgs[0]); // No of commands should be 1.
                            }
                        grsFreeArgv(grsCmdArgs);
                        grsClearFlags();
                        printf("%s ", getenv("PROMPT"));
                   }
                   bzero(grsTmpBuf, 100);
                   break;
            default: strncat(grsTmpBuf, &grsCharInput, 1);
                 break;
        }
    }
    free(grsTmpBuf);
    return;
}
