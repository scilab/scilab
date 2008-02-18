/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __SETLOOKANDFEEL_H__
#define __SETLOOKANDFEEL_H__

#include "machine.h" /* BOOL */

/**
* set current look and feel
* "javax.swing.plaf.metal.MetalLookAndFeel"
* "com.sun.java.swing.plaf.motif.MotifLookAndFeel"
* "com.sun.java.swing.plaf.windows.WindowsLookAndFeel"
* "com.sun.java.swing.plaf.windows.WindowsClassicLookAndFeel"
* @param a string
* @return BOOL
*/
BOOL setlookandfeel(char *lookandfeekstr);

/*
* set system default look and feel
* @return BOOL
*/
BOOL setsystemlookandfeel(void);

#endif /* __SETLOOKANDFEEL_H__ */
/*--------------------------------------------------------------------------*/
