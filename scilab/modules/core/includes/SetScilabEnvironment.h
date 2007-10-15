/**
 * @author Allan CORNET INRIA 2005 
 * @author Sylvestre LEDRU INRIA 2007 - Update 
 * Set the set Scilab mandatory variable like SCI or SCIHOME
 */

#ifndef __SCIENV_H__
#define __SCIENV_H__
#include "version.h"
#ifdef _MSC_VER
#include <windows.h>
#else
#include <stdio.h>
#endif

/**
 * Set the set Scilab mandatory variable like SCI or SCIHOME
 *
 */
void SetScilabEnvironment(void);

#endif /* __SCIENV_H__ */
