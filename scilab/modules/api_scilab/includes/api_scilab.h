/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */


#ifndef __VARIABLE_API__
#define __VARIABLE_API__


/*
* 1 : First version
* 2 : Shortcut functions added
* 3 : Add some functions/macros in replacement of stack functions/macros :
        - Rhs -> nbInputArgument
        - Lhs -> nbOutputArgument
        - LhsVar -> AssignOutputVariable
        - PutLhsVar -> ReturnArguments
        - CheckRhs -> CheckInputArgument
        - CheckLhs -> CheckOutputArgument
*/

#define API_SCILAB_VERSION	4

#define __INTERNAL_API_SCILAB__

#include "api_stack_common.h"
#include "api_stack_double.h"
#include "api_stack_string.h"
#include "api_stack_int.h"
#include "api_stack_poly.h"
#include "api_stack_sparse.h"
#include "api_stack_boolean.h"
#include "api_stack_boolean_sparse.h"
#include "api_stack_pointer.h"
#include "api_stack_list.h"
#include "api_stack_error.h"
#include "api_stack_handle.h"
#include "api_stack_hypermat.h"
#include "api_stack_optional.h"

/*API Scilab 6*/

#ifdef __API_SCILAB_UNSAFE__
#define API_PROTO(x) scilab_internal_##x##_unsafe
#define API_EXPORT_PROTO(x) scilab_##x
#else
#define API_PROTO(x) scilab_internal_##x##_safe
#define API_EXPORT_PROTO(x) scilab_##x
#endif

typedef void* scilabEnv;
typedef int* scilabVar;
typedef int* scilabOpt;


#include "api_error.h"
#include "api_boolean.h"
#include "api_common.h"
#include "api_double.h"
#include "api_int.h"
#include "api_cell.h"
#include "api_struct.h"
#include "api_list.h"
#include "api_poly.h"
#include "api_string.h"
#include "api_handle.h"
#include "api_optional.h"
#include "api_pointer.h"

#undef __INTERNAL_API_SCILAB__

#include "core_math.h"
#include "sci_types.h"
#include "BOOL.h"
#include "sci_malloc.h"

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
