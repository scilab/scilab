/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * This file is distributed under the same license as the Scilab package.
 */

/* Wrappers to call read_xml_initial_states / write_xml_states through call(). */

#include <stdlib.h>
#include <string.h>

int read_xml_initial_states(int nvar, const char *xmlfile, char **ids, double *svars);
int write_xml_states(int nvar, const char *xmlfile, char **ids, double *x);

/* call() passes a single string, so ids come ';'-separated; empty ids are kept. */
static char **split_ids(const char *list, int n, char **buffer)
{
    char **ids = (char **)malloc((n > 0 ? n : 1) * sizeof(char *));
    char *p = NULL;
    int i = 0;

    *buffer = (char *)malloc(strlen(list) + 1);
    strcpy(*buffer, list);
    p = *buffer;
    for (i = 0; i < n; i++)
    {
        char *sep = strchr(p, ';');
        ids[i] = p;
        if (sep)
        {
            *sep = '\0';
            p = sep + 1;
        }
        else
        {
            p += strlen(p);
        }
    }
    return ids;
}

void xml_states_read(char *file, char *list, int *n, double *values, int *ret)
{
    char *buffer = NULL;
    char **ids = split_ids(list, *n, &buffer);
    *ret = read_xml_initial_states(*n, file, ids, values);
    free(ids);
    free(buffer);
}

void xml_states_write(char *file, char *list, int *n, double *values, int *ret)
{
    char *buffer = NULL;
    char **ids = split_ids(list, *n, &buffer);
    *ret = write_xml_states(*n, file, ids, values);
    free(ids);
    free(buffer);
}
