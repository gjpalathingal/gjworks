/**************************************************
* File Name: grsShellUtils.c
* Purpose: Implementing utilities for the GRS Shell
* Author: Rekha Balasubramanian
* CWID: A20274931
***************************************************/

#include "grs.h"

/* Function to trim whitespace */
size_t trimWhitespace(char *out, size_t in_len, const char *in)
{
    if (in_len == 0)
        return in_len;

    const char *end;
    size_t out_len;

    /* trim leading space */
    while (isspace(*in)) in++;

    /* all are spaces */
    if (*in == 0)
    {
        *out = 0;
        return 1;
    }

    /* trim trailing space */
    end = in + strlen(in) - 1;
    while ((end > in) && isspace(*end)) end--;
    end++;

    out_len = end - in;

    /* copy trimmed string to out buffer */
    memcpy(out, in, out_len);
    out[out_len] = 0;

    return out_len;
}

/* Function to set current working directory */
void setCwd()
{
    char *home_dir = getenv("HOME");
    printf("Home Directory: %s\n", home_dir);
    chdir(home_dir);
}

