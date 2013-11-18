/*************************************************
* File Name: grs.h
* Purpose: Header file for MACROS and function 
*          prototypes
* Author: Rekha Balasubramanian
* CWID: A20274931
**************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdbool.h>

#define _POSIX_SOURCE
#include <unistd.h>

#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

#define SUCCESS 0
#define FAILURE 1

#define DEBUG 0

#define GRS_PROFILE_FILENAME "grs.profile"
#define GRS_ALIAS_FILENAME "grs.aliases"
#define EXIT_CMD "exit"
#define NOTHING ""

#define MAX_ARGS 32

/* Function to trim whitespace */
size_t trimWhitespace(char *out, size_t in_len, const char *in);

/* Function to load profile */
int loadProfile();

/* Function to set current working directory */
void setCwd();

/* Function to load the prompt */
void loadPrompt();

int grsBackgroundProcess(char *cmd);

int grsCmdExec(char *cmd);

int grsSpawnProcess(char *cmd, bool background);

void grsCheckBgProcs();

void parseCmd(char *cmd /* IN */, char **argv /* OUT */);

void grsSetAlias(char *cmd);

void grsGetCmdAlias(char **argv /* IN */ /* OUT */);

void grsCmdLoop();

void grsIfThenElse(short noargs,char *cmd[]);

int grsPipeHandle(short cmdlen,char *cmds[]);

int grsRedirectonHandle(char* grsCmdArgs[], char grsRedirectionChar);


/********************** END OF FILE **********************/