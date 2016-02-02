/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#ifndef __REALMAIN_H__
#define __REALMAIN_H__
/*--------------------------------------------------------------------------*/
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "version.h"

/**
 * When Scilab is started with arguments ... defines if it is a scilab
 * script or a scilab code
 */
typedef enum
{
    SCILAB_SCRIPT = 0, /* SCILAB_SCRIPT means filename as argument */
    SCILAB_CODE = 1 /*  SCILAB_CODE means a scilab code */
} InitScriptType;


/**
* The real main of Scilab
*
* @param no_startup_flag_l
* @param initial_script
* @param initial_script_type
* @param memory
* @return 0 is everything went fine. not 0 is everything went wrong. (@TODO: should be changed... returning only 0 here)
*/
int realmain(int no_startup_flag_l, char *initial_script, InitScriptType initial_script_type, int memory);

/**
 * @TODO add comment
 *
 * @param void
 * @return <ReturnValue>
 */
int Get_no_startup_flag(void);

/**
 * @TODO add comment
 *
 * @param start
 */
void Set_no_startup_flag(int start);

#ifndef _MSC_VER
char ** create_argv(int *argc);
#endif

#endif /* __REALMAIN_H__ */
/*--------------------------------------------------------------------------*/

