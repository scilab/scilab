/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __PREPARSE_COMPLETION_NW_H__
#define __PREPARSE_COMPLETION_NW_H__

/**
 * pre parse current line to search with completion (bug 3059)
 * @param[in] current line
 * @return word to find with completion
 */
const char *preparse_line_for_completion_nw(const char *current_line);

#endif /* __PREPARSE_COMPLETION_NW_H__ */

/*--------------------------------------------------------------------------*/
