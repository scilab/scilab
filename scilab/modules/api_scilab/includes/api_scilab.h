/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __VARIABLE_API__
#define __VARIABLE_API__

/*
* 1 : First version
* 2 : Shortcut functions added
*/

#define API_SCILAB_VERSION	2

#include "api_common.h"
#include "api_double.h"
#include "api_string.h"
#include "api_int.h"
#include "api_poly.h"
#include "api_sparse.h"
#include "api_boolean.h"
#include "api_boolean_sparse.h"
#include "api_pointer.h"
#include "api_list.h"

/*Constants*/
#define ROW_LETTER		'r'
#define COL_LETTER		'c'
#define STAR_LETTER		'*'
#define MTLB_LETTER		'm'

#define BY_ROWS			1
#define BY_COLS			2
#define BY_ALL			0
#define	BY_MTLB			-1

#endif /* __VARIABLE_API__ */
