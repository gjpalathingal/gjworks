/*************************************************
* File Name: grsCmdExec.c
* Purpose: Implementation of Command Utilities
* Author: Rekha Balasubramanian
* CWID: A20274931
*************************************************/

#include "grs.h"

/* Function to execute commands */
int grsCmdExec(char *cmd)
{
    return grsSpawnProcess(cmd, false);
}
