/*
File name : grsShellCommands.c
Author    : George J Palathingal (A20294134)
Date      : 15/09/2012
Version   : 1.0

Assumptions
- The module assumes grsCmdArgs has three entries
	grsCmdArgs[0] has "if Cmd"
	grsCmdArgs[1] has "then Cmd1"
	grsCmdArgs[2] has "else Cmd2"
- The module also assumes that grsCmdExec() returns -1 on error, anyother value otherwise.

Algorithm
- The algorithm first extracts Cmd and calls grsCmdExec to execute it
- If the success code from grsCmdExec is -1(error) it then extracts Cmd2 and executes it
- Otherwise it extracts Cmd1 and executes it. 
*/

#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "grs.h"
//#define DEBUG

void grsIfThenElse(short grsLocalNoCmds, char *grsLocalBuf[])
{
	char* grsTmpCmd=NULL,*grsTmp=NULL;
	char* grsCmd=NULL, *strtmp=NULL; 
	int grsSuccessCode = 0;
	if(grsLocalNoCmds < 3)
	{
		printf("Error : Command not complete\n");
		return;
	}	
	grsTmp=strstr(grsLocalBuf[2],"fi");
	if(grsTmp==NULL)
	{
	 printf("Syntax error\n");
	 return;
	} 
	grsCmd = (char*)malloc(sizeof(char)*10);
	bzero(grsCmd, 10);
	strtmp = (char *)malloc(sizeof(char)*100);
	bzero(strtmp, 100);
	strncpy(grsCmd,grsLocalBuf[0],2);
	if(strcmp(grsCmd,"if")!=0)
	{
		printf("Command %s is invalid\n",grsCmd);
		free(strtmp);
		free(grsCmd);
		return;
	}
	grsTmpCmd = grsLocalBuf[0]+2;
	while(*grsTmpCmd==' ')
		grsTmpCmd++;
	strcpy(strtmp,grsTmpCmd);
#ifdef DEBUG
//	printf("If command is %s\n",strtmp);
#endif
	grsSuccessCode=grsCmdExec(strtmp);
//	grsSuccessCode = -1;
	if(grsSuccessCode==-1)
	{
		strncpy(grsCmd,grsLocalBuf[2],4);
		if(strcmp(grsCmd,"else")!=0)
		{
			printf("Command %s invalid\n",grsCmd);
			free(strtmp);
			free(grsCmd);
			return;
		}
	grsTmpCmd = grsLocalBuf[2]+4;
	while(*grsTmpCmd==' ')
	 grsTmpCmd++;
	strcpy(strtmp,grsTmpCmd);
	
	grsTmp=strstr(strtmp,"fi");
//	if(grsTmp==NULL)
//	 printf("Syntax error\n");
//	 else
//	 {
	 strcpy(grsTmp,"\0");
//	 printf("fi found\n");
//	 }
#ifdef DEBUG
//	printf("Else command is %s\n",strtmp);
#endif
	grsSuccessCode=grsCmdExec(strtmp);
	if(grsSuccessCode == -1)
		printf("Failed to execute %s command\n",strtmp);
	}
	
	else
	{
		strncpy(grsCmd,grsLocalBuf[1],4);
		if(strcmp(grsCmd,"then")!=0)
		{
			printf("Command %s invalid\n",grsCmd);
			free(strtmp);
			free(grsCmd);
			return;
		}
	grsTmpCmd = grsLocalBuf[1]+4;
	while(*grsTmpCmd==' ')
	 grsTmpCmd++;
	strcpy(strtmp,grsTmpCmd);
#ifdef DEBUG
//	printf("Then command is %s\n",strtmp);
#endif
	grsSuccessCode=grsCmdExec(strtmp);
	if(grsSuccessCode == -1)
		printf("Failed to execute %s command\n",strtmp);
	}
	free(grsCmd);
	free(strtmp);
	return;
}