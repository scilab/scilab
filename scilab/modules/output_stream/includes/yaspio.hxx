/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __YASPIO_HXX__
#define __YASPIO_HXX__

typedef void (*YASP_OUTPUT)(char *text);
typedef char *(*YASP_INPUT)(void);

void setYaspOutputMethod(YASP_OUTPUT writer);

void setYaspInputMethod(YASP_INPUT reader);

char *YaspRead();

void YaspWrite(char* text);

#endif /* __YASPIO_HXX__ */
