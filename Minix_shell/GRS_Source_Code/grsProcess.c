/****************************************************
* File Name: grsProcess.c
* Purpose: Implementation of process logic - spawning
*          new process, background processing
* Author: Rekha Balasubramanian
* CWID: A20274931
*****************************************************/

#include "grs.h"

typedef struct bgprocs bgprocs;
struct bgprocs
{
    int index;
    pid_t pid;
    char cmd[1024];
    int status;
    bgprocs *next;
};

static bgprocs *root = NULL;
static bgprocs *tail = NULL;

sigset_t maskList;

/* Child Signal Handler for child processes */
void childSignalHandler(int signum)
{
    pid_t pid;
    int status;

    /* Wait for child processes to complete */
    pid = waitpid(-1, &status, WNOHANG);

    /* Update appropriate bgprocs structure based on pid returned */
    bgprocs *current = root;
    while (current != NULL)
    {
        if (pid == current->pid)
        {
            current->status = status;
            break;
        }

        current = current->next;
    }
}

/* Function to parse the command into arg list */
void parseCmd(char *cmd /* IN */, char **argv /* OUT */)
{
    if (strcmp(cmd, NOTHING) == 0)
        return;

    int index = -1;
    char command[1024] = {0};
    char *token;

    strcpy(command, cmd);
    token = strtok(command, " ");
    if (DEBUG) printf("parseCmd - 1st token %s\n", token);
    if (DEBUG) printf("parseCmd - 1st token length %d\n", strlen(token));

    while (token != NULL)
    {
        ++index;

        /*
        int token_length = strlen(token);

        argv[index] = malloc(token_length);
        memset(argv[index], 0, token_length);
        strcpy(argv[index], token);
        */

        argv[index] = strdup(token);

        token = strtok(NULL, " ");
    }

    if (DEBUG) printf("parseCmd - image name %s\n", argv[0]);

    if (strcmp(argv[index], "&") == 0)
        argv[index] = NULL;
}

/* Function to spawn a background process */
int grsBackgroundProcess(char *cmd)
{
    return(grsSpawnProcess(cmd, true));
}

static int bg_index = 1;
/* Function to spawn a process */
int grsSpawnProcess(char *cmd, bool background)
{
    int ret = SUCCESS;
    if (DEBUG) printf("grsSpawnProcess - incoming command %s\n", cmd);
    pid_t child_pid;

    sigaddset(&maskList, SIGTSTP);
    sigaddset(&maskList, SIGINT);
    sigprocmask(SIG_BLOCK, &maskList, NULL);

    char *argv[MAX_ARGS];
    int count;
    for (count = 0; count < MAX_ARGS; ++count)
    {
        argv[count] = NULL;
    }

    parseCmd(cmd, argv);
    grsGetCmdAlias(argv);
    if (DEBUG) printf("grsSpawnProcess - argv[0] %s\n", argv[0]);

    if (strcmp(argv[0], "cd") == 0)
    {
        chdir(argv[1]);
        return SUCCESS;
    }
    else if (strcmp(argv[0], "pwd") == 0)
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd))) printf("%s\n", cwd);
        return SUCCESS;
    }
    else if (strcmp(argv[0], "alias") == 0)
    {
        grsSetAlias(argv[1]);
        return SUCCESS;
    }

    signal(SIGCHLD, childSignalHandler);

    /* fork a child process */
    child_pid = fork();
    if (child_pid == 0)  /* child will execute */
    {
        // setpgid(0, 0);
        sigprocmask(SIG_UNBLOCK, &maskList, NULL);

        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (DEBUG) printf("grsSpawnProcess - image name %s\n\n", argv[0]);

        if (execvp(argv[0], argv) == -1)
        {
            if (DEBUG) printf("%s %s %s\n", argv[0], argv[1], argv[0]);
            if (errno == ENOENT)
                printf("%s: command not found\n", argv[0]);
            else
                printf("%s: error %d", argv[0], errno);

            return -1;
        }

        return SUCCESS;
    }

    if (background) // for background processes
    {
        /* update bgprocs structure */
        if (root == NULL)
        {
            if (DEBUG) printf("root is null");
            root = malloc(sizeof(bgprocs));
            tail = root;
        }
        else
        {
            if (DEBUG) printf("root is not null");
            tail->next = malloc(sizeof(bgprocs));
            tail = tail->next;
            tail->next = NULL;
        }

        tail->pid = child_pid;
        tail->index = bg_index;
        tail->next = NULL;
        strcpy(tail->cmd, cmd);

        printf("[%d]   %05d                    %s\n", bg_index, child_pid, tail->cmd);

        bg_index++;
    }
    else
    {
        int status = -1;
        if (DEBUG) printf("waiting for process to complete\n");
        // wait(&status); // wait for child process to complete
        waitpid(child_pid, &status, 0); // wait for child process to complete

        ret = WEXITSTATUS(status);
        if (DEBUG) printf("return value %d\n", ret);
    }

    sigprocmask(SIG_UNBLOCK, &maskList,NULL);

    return ret;
}

/* Function to check the background processes */
void grsCheckBgProcs()
{
    if (root == NULL) return;

    bgprocs *current = root;
    bgprocs *previous = root;
    while (current)
    {
        if (WIFEXITED(tail->status))
        {
            printf("[%d]   Done                     %s\n", tail->index, tail->cmd);

            if (current == root)
                root = current->next;
            else
                previous->next = current->next;

            free (current);
        }

        previous = current;
        current = current->next;
    }
    
}

