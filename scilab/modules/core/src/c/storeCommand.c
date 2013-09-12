/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2008 - ENPC  - Jean-Philippe Chancelier <jpc@cermics.enpc.fr>
 * Copyright (C) 2004-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
 * Copyright (C) 2008-2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "Thread_Wrapper.h"
#include "stack-c.h"

#include "MALLOC.h"
#include "storeCommand.h"
#include "sciprint.h"
#include "sciprint_full.h"
#include "localization.h"


#ifdef _MSC_VER
#include "mmapWindows.h"
#else
#include <sys/mman.h>
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif
#endif
/*--------------------------------------------------------------------------*/
/*
 *  Command queue functions
 *  This function is used to store Scilab command in a queue
 *
 *  PUBLIC : int StoreCommand( char *command)
 *           int C2F(ismenu)()
 *           int C2F(getmen)(char * btn_cmd,int * lb, int * entry)
*/
/*--------------------------------------------------------------------------*/
#define IMPORT_SIGNAL extern
/*--------------------------------------------------------------------------*/
typedef struct commandRec
{
    char              *command;		/* command info one string two integers */
    int               flag; /* 1 if the command execution cannot be interrupted */
    struct commandRec *next;
} CommandRec;
/*--------------------------------------------------------------------------*/
/* Extern Signal to say we git a StoreCommand. */
IMPORT_SIGNAL __threadSignal LaunchScilab;
/*--------------------------------------------------------------------------*/
static CommandRec *commandQueue = NULL;

static void release(void);

static __threadLock* getCommandQueueSingleAccess()
{
    static __threadLock* ptr = NULL;
    if (!ptr)
    {
        ptr = mmap(0, sizeof(__threadLock), PROT_READ | PROT_WRITE, MAP_SHARED |  MAP_ANONYMOUS, -1, 0);
#ifdef _MSC_VER
        *ptr =  __StaticInitLock;
#else
        __InitSignalLock(ptr);
#endif
        atexit(release);
    }
    return ptr;
}

static void release(void)
{
    if (getCommandQueueSingleAccess())
    {
        __UnLock(getCommandQueueSingleAccess());
    }
}
/*--------------------------------------------------------------------------*/
int StoreCommand (char *command)
{
    return (StoreCommandWithFlag (command, 0));
}
/*--------------------------------------------------------------------------*/
/*
* try to execute a command or add it to the end of command queue
* flag = 0 : the command is not shown in scilab window
* flag = 1 : the command is shown in scilab window (if at prompt) and executed sequentially
*/
int StoreCommandWithFlag (char *command, int flag)
{
    CommandRec *q = NULL, *r = NULL;

    CommandRec *p = (CommandRec *) MALLOC (sizeof (CommandRec));
    if (p == (CommandRec *) 0)
    {
        sciprint(_("%s: No more memory.\n"), "send_command");
        return (1);
    }
    p->flag = flag;
    p->command = (char *) MALLOC ((strlen (command) + 1) * sizeof (char));
    if (p->command == (char *) 0)
    {
        FREE(p);
        sciprint(_("%s: No more memory.\n"), "send_command");
        return (1);
    }
    strcpy (p->command, command);
    p->next = NULL;
    __Lock(getCommandQueueSingleAccess());
    if (commandQueue == NULL)
    {
        commandQueue = p;
    }
    else
    {
        q = commandQueue;
        while ((r = q->next) != NULL)
        {
            q = r;
        }
        q->next = p;
    }
    __UnLock(getCommandQueueSingleAccess());
    //**
    //** We have something to do, awake Scilab !!!!!!
    //**
    __Signal(&LaunchScilab);
    return (0);
}
/*--------------------------------------------------------------------------*/
/*
* try to execute a command or add it to the _BEGINNING_ of command queue
* flag = 0 : the command is not shown in scilab window
* flag = 1 : the command is shown in scilab window (if at prompt) and executed sequentially
*/
int StorePrioritaryCommandWithFlag (char *command, int flag)
{
    CommandRec *p = (CommandRec *) MALLOC (sizeof (CommandRec));
    if (p == (CommandRec *) 0)
    {
        sciprint(_("%s: No more memory.\n"), "send_command");
        return (1);
    }
    p->flag = flag;
    p->command = (char *) MALLOC ((strlen (command) + 1) * sizeof (char));
    if (p->command == (char *) 0)
    {
        FREE(p);
        sciprint(_("%s: No more memory.\n"), "send_command");
        return (1);
    }
    strcpy (p->command, command);
    p->next = NULL;
    __Lock(getCommandQueueSingleAccess());
    if (commandQueue == NULL)
    {
        commandQueue = p;
    }
    else
    {
        p->next = commandQueue;
        commandQueue = p;
    }
    __UnLock(getCommandQueueSingleAccess());
    //**
    //** We have something to do, awake Scilab !!!!!!
    //**
    __Signal(&LaunchScilab);
    return (0);
}
/*--------------------------------------------------------------------------*/
int isEmptyCommandQueue(void)
{
    int isEmpty = 0;
    __Lock(getCommandQueueSingleAccess());
    isEmpty = (commandQueue == NULL);
    __UnLock(getCommandQueueSingleAccess());
    return isEmpty;
}
/*--------------------------------------------------------------------------*/
/*
* Gets info on the first queue element
* and remove it from the queue
*/
int GetCommand ( char *str)
{
    int flag = 0;
    __Lock(getCommandQueueSingleAccess());
    if (commandQueue != NULL)
    {
        CommandRec *p = commandQueue;

        strcpy (str, p->command);
        flag = p->flag;

        commandQueue = p->next;
        FREE (p->command);
        FREE (p);
        if (C2F(iop).ddt == -1)
        {
            if (flag == 0)
            {
                sciprint_full(_("Unqueuing %s - No option.\n"), str);
            }
            else
            {
                sciprint_full(_("Unqueuing %s - seq.\n"), str);
            }
        }
    }
    __UnLock(getCommandQueueSingleAccess());

    return flag;
}
/*--------------------------------------------------------------------------*/
int ismenu(void)
{
    /* Do not manage commands while compiling scilab function */
    return (commandQueue == NULL || (C2F(com).comp[0] != 0)) ? 0 : 1;
}
/*--------------------------------------------------------------------------*/
/* menu/button info for Scilab */
int C2F(getmen)(char * btn_cmd, int * lb, int * entry)
{
    int flag = 0;
    if (ismenu() == 1)
    {
        flag = GetCommand(btn_cmd);
        *lb = (int) strlen(btn_cmd);
        *entry = 0;  /* This parameter entry seems to be unused. Probably a very old thing... */
    }
    else
    {
        flag = 0;
        *lb = 0;
        *entry = 0;  /* This parameter entry seems to be unused. Probably a very old thing... */
    }
    return flag;
}
/*--------------------------------------------------------------------------*/
