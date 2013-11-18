/**********************************************************
* File Name: main.c
* Purpose: Contains the "main" function 
* Author: Rekha Balasubramanian (A20274931)
*         George J Palathingal (A20294134)
***********************************************************/

#include "grs.h"

/* Main file where the shell begins */

typedef void (*sighandler_t)(int);

/* Handler for INTERRUPT signal */
void handle_signal(int signo)
{
    char c;
    printf("\nAre you sure? [y/n] ");
    scanf("%c", &c);
    switch(c)
    {
        case 'y':
            exit(0);
            break;
        default:
            grsCmdLoop();
            break;
    }
    // fflush(stdout);
}

/* Function to load profile */
int loadProfile()
{
    FILE *file;
    char filepath[1024] = {0};
    char *filename = GRS_PROFILE_FILENAME;
    struct stat file_stat;

    /* open the profile file in read-only mode.
       See if grs.profile exists in current home
       directory. If not look in current directory */
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/");
    strcat(filepath, filename);

    if (stat(filepath, &file_stat) == 0)
        file = fopen(filepath, "r");
    else if (stat(filename, &file_stat) == 0)
        file = fopen(filename, "r");
    else
        printf("Profile file not found.\n");

    if (file != NULL)
    {
        char line[1024] = {0};

        /* read line by line from the file */
        while (fgets(line, sizeof(line), file) != NULL)
        {
            if (DEBUG) printf("Original line %s\n", line);

            char line_dup[1024] = {0};
            trimWhitespace(line_dup, strlen(line), line);

            if (DEBUG) printf("Trimmed line %s\n", line_dup);

            /* split into name/value pair */
            char name[strlen(line_dup)], value[strlen(line_dup)];
            if (sscanf(line_dup, "%[^=]=%[^\r]", name, value) == 2)
            {
                char name_dup[strlen(name)], value_dup[strlen(value)];

                /* trim spaces in name/value */
                trimWhitespace(name_dup, strlen(name), name);
                trimWhitespace(value_dup, strlen(value), value);

                /* set the environment variable */
                setenv(name_dup, value_dup, true);

                if (DEBUG) printf("Name: %s, Value: %s\n", name_dup, getenv(name_dup));
            }
        }
        
        fclose(file);
    }

    return SUCCESS;
}

/* Main function of GRS Shell */
int main(int argc, char **argv)
{
    signal(SIGINT, handle_signal);
    loadProfile(); // load profile
    setCwd(); // set current home directory
    grsCmdLoop();
    return SUCCESS;
}

/********************** END OF FILE **********************/
