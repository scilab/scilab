/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#ifndef	READER_H_
#define	READER_H_

#include	<wchar.h>

typedef struct s_list_cmd t_list_cmd;

#define SCI_PRINT_WSTRING "%ls"
#define SCI_PRINT_WCHAR "%lc"

/* TODO: remove and use ./libs/doublylinkedlist/includes/DoublyLinkedList.h instead */
struct s_list_cmd
{
    int index;
    int bin;
    wchar_t *cmd;
    struct s_list_cmd *next;
    struct s_list_cmd *previous;
};

/**
 * Get the command line.
 * @param pointer of adress of the structure.
 * mean the value of the adress may change.
 * @return command line put by user.
 */
char *getCmdLine(t_list_cmd **);

/* TODO: document */
t_list_cmd *initUserInput(t_list_cmd * listCmd);

#endif /* !READER_H_ */
