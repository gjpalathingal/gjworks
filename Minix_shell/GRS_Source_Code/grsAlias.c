/*****************************************************
* File Name: grsAlias.c
* Purpose: Implementation of Alias feature in GRS Shell
* Author: Rekha Balasubramanian
* CWID: A20274931
*****************************************************/

#include "grs.h"

/* Function to get alias (name=value) */
void grsGetCmdAlias(char **argv /* IN */ /* OUT */)
{
    char *cmd = strdup(argv[0]);
    FILE *file;
    char filepath[1024] = {0};
    char *filename = GRS_ALIAS_FILENAME;
    struct stat file_stat;

    /* open the profile file in read-only mode
       see if grs.aliases exists in current home directory
       if not, look in current directory */
    strcpy(filepath, getenv("HOME"));
    strcat(filepath, "/");
    strcat(filepath, filename);

    if (stat(filepath, &file_stat) == 0)
        file = fopen(filepath, "r");
    else if (stat(filename, &file_stat) == 0)
        file = fopen(filename, "r");
    else
    {
        if (DEBUG) printf("Alias file not found.\n");
        return;
    }

    if (file != NULL)
    {
        char line[1024] = {0};
        while (fgets(line, sizeof(line), file) != NULL)
        {
            char line_dup[1024] = {0};
            trimWhitespace(line_dup, strlen(line), line);

            /* split into name/value pair */
            char name[1024] = {0}, value[1024] = {0};
            if (sscanf(line_dup, "%[^=]=%[^\r]", name, value) == 2)
            {
                char name_dup[1024] = {0}, value_dup[1024] = {0};

                /* trim spaces in name/value */
                trimWhitespace(name_dup, strlen(name), name);
                trimWhitespace(value_dup, strlen(value), value);

                if (strcmp(cmd, name_dup) == 0)
                {
                    argv[0] = strdup(value_dup);
                    break;
                }
            }
        }

        fclose(file);
    }
}

/* Function to set the alias (name=value) */
void grsSetAlias(char *cmd)
{
    if (DEBUG) printf("grsSetAlias - incoming command %s\n", cmd);
    char *token;
    char *name, *value;

    char command[1024] = {0};
    strcpy(command, cmd);
    if (DEBUG) printf("grsSetAlias - command %s\n", command);

    token = strtok(command, "=");
    if (DEBUG) printf("grsSetAlias - 1st token %s\n", token);
    if (token != NULL)
    {
        name = strdup(token);
        token = strtok(NULL, "=");
        if (DEBUG) printf("grsSetAlias - 2nd token %s\n", token);
        value = strdup(token);
    }

    if (DEBUG) printf("grsSetAlias - name %s value %s\n", name, value);

    if ((strcmp(name, NOTHING) != 0) && (strcmp(value, NOTHING) != 0))
    {
        FILE *file;
        char filepath[1024] = {0};
        char *filename = GRS_ALIAS_FILENAME;

        /* open the alias file in append mode
           see if grs.aliases exists in current home directory */
        strcpy(filepath, getenv("HOME"));
        strcat(filepath, "/");
        strcat(filepath, filename);
        if (DEBUG) printf("grsSetAlias - filepath %s\n", filepath);

        file = fopen(filepath, "a+");

        if (file != NULL)
        {
            fprintf(file, "%s=%s\n", name, value);
            fclose(file);
        }
        else
        {
            printf("Could not open grs.aliases for writing\n");
        }
    }
}

/****************** END OF FILE *******************/
