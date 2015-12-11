/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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