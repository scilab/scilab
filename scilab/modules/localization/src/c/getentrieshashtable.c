/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "getentrieshashtable.h"
#include "hashtable_private.h"
#include "MALLOC.h"
#include "hashtable_localization.h"
/*--------------------------------------------------------------------------*/
char **getTAGSinhashtable(struct hashtable *tableIN, int *numbersEntries)
{
    char **return_entries = NULL;
    unsigned int i = 0;
    unsigned int j = 0;
    struct entry *e = NULL;
    struct entry **table = tableIN->table;

    *numbersEntries = tableIN->entrycount;
    if (*numbersEntries > 0)
    {
        return_entries = (char **)MALLOC(sizeof(char *) * tableIN->entrycount);
        if (return_entries)
        {
            j = 0;
            for (i = 0; i < tableIN->tablelength; i++)
            {
                e = table[i];
                while (NULL != e)
                {
                    struct key_string *Key = NULL;
                    Key = e->k;
                    return_entries[j] = (char *)MALLOC(sizeof(char) * (strlen(Key->Key_String) + 1));
                    if (return_entries[j])
                    {
                        strcpy(return_entries[j], Key->Key_String);
                    }
                    e = e->next;
                    j++;
                }
            }
        }
    }
    return return_entries;
}
/*--------------------------------------------------------------------------*/
char **getSTRINGSinhashtable(struct hashtable *tableIN, int *numbersEntries)
{
    char **return_entries = NULL;
    unsigned int i = 0;
    unsigned int j = 0;
    struct entry *e = NULL;
    struct entry **table = tableIN->table;

    *numbersEntries = tableIN->entrycount;
    if (*numbersEntries > 0)
    {
        return_entries = (char **)MALLOC(sizeof(char *) * tableIN->entrycount);
        if (return_entries)
        {
            j = 0;
            for (i = 0; i < tableIN->tablelength; i++)
            {
                e = table[i];
                while (NULL != e)
                {
                    struct value_string *Key = NULL;
                    Key = e->v;
                    return_entries[j] = (char *)MALLOC(sizeof(char) * (strlen(Key->Value_String) + 1));
                    if (return_entries[j])
                    {
                        strcpy(return_entries[j], Key->Value_String);
                    }
                    e = e->next;
                    j++;
                }
            }
        }
    }
    return return_entries;
}
/*--------------------------------------------------------------------------*/
char **getPATHSinhashtable(struct hashtable *tableIN, int *numbersEntries)
{
    char **return_entries = NULL;
    unsigned int i = 0;
    unsigned int j = 0;
    struct entry *e = NULL;
    struct entry **table = tableIN->table;

    *numbersEntries = tableIN->entrycount;
    if (*numbersEntries > 0)
    {
        return_entries = (char **)MALLOC(sizeof(char *) * tableIN->entrycount);
        if (return_entries)
        {
            j = 0;
            for (i = 0; i < tableIN->tablelength; i++)
            {
                e = table[i];
                while (NULL != e)
                {
                    struct value_string *Key = NULL;
                    Key = e->v;
                    return_entries[j] = (char *)MALLOC(sizeof(char) * (strlen(Key->Path_String) + 1));
                    if (return_entries[j])
                    {
                        strcpy(return_entries[j], Key->Path_String);
                    }
                    e = e->next;
                    j++;
                }
            }
        }
    }
    return return_entries;
}
/*--------------------------------------------------------------------------*/
