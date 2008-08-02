/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SETGETLANGUAGE_H__
#define __SETGETLANGUAGE_H__

#include "machine.h"

/** 
 * change language
 * @param lang
 */
BOOL setlanguage(char *lang);


/** 
 * returns current language
 * @return a string
 */
char *getlanguage(void);

/**
 * returns language if it was a alias
 * for compatibility with scilab 4 (only en,fr)
 * example en --> en_US
 * @return a string
 */

char *getLanguageFromAlias(char *alias);

#endif /* __SETGETLANGUAGE_H__ */
/*--------------------------------------------------------------------------*/ 
