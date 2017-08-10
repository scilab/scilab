/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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

#ifndef __TERMCOMMAND_H__
#define __TERMCOMMAND_H__

/**
* Do action Control C in scilab
*/
void ControlC_Command(void);

/**
* Do action Control X in scilab
*/
void ControlX_Command(void);

/**
* Do action F1 in scilab
* launch help
*/
void F1_Command(void);

/**
* Do action F2 in scilab
* launch help
*/
void F2_Command(void);

/**
* Do action ALT-F4 in scilab
* Close Scilab (standard Windows)
*/
void ALTF4_Command(void);

#endif /* __TERMCOMMAND_H__ */