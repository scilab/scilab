/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __STACKTYPEVARIABLE_H__
#define __STACKTYPEVARIABLE_H__

/** @def STRING_DATATYPE 
 * The string/char * datatype in the Scilab stack
 */
#define STRING_DATATYPE "c" 

/** @def MATRIX_OF_STRING_DATATYPE
 * The string/char ** datatype in the Scilab stack
 */
#define MATRIX_OF_STRING_DATATYPE "S"


/** @def MATRIX_OF_DOUBLE_DATATYPE
 * A matrix of double 
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_DOUBLE_DATATYPE "d"


/** @def MATRIX_OF_RATIONAL_DATATYPE
 * A matrix of rational
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_RATIONAL_DATATYPE "r"


/** @def MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE
 * A matrix of integer
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE "I"

/** @def MATRIX_OF_INTEGER_DATATYPE
 * A matrix of 'little' integer
 * 'little' because in reality, this int is a complex with the imaginary
 * part set to 0
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_INTEGER_DATATYPE "i"

/** @def MATRIX_OF_BOOLEAN_DATATYPE
 * A matrix of boolean
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_BOOLEAN_DATATYPE "b"

/** @def MATRIX_OF_COMPLEX_DATATYPE
 * A matrix of complex
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_COMPLEX_DATATYPE "z"


/** @def SPARSE_MATRIX_DATATYPE
 * An sparse matrix
 * if the size of the matrix is 1,1, it is a single value
 */
#define SPARSE_MATRIX_DATATYPE "s"


/** @def LIST_DATATYPE
 * A list
 */
#define LIST_DATATYPE "l"


/** @def TYPED_LIST_DATATYPE
 * A typed list
 */
#define TYPED_LIST_DATATYPE "t"

/** @def MATRIX_ORIENTED_TYPED_LIST_DATATYPE
 * A Matrix oriented typed list 
 * mlist object are very similar to tlist objects. But
 * if M is an mlist, for any index i which is not a field name, 
 * M(i) is not the i th field of the list but is interpreted as
 * the i th entry of M seen as a vector. 
 * This is the only difference between mlist and tlist .
 * @see Scilab documentation at mlist for more information
 */
#define MATRIX_ORIENTED_TYPED_LIST_DATATYPE "m"

/** @def SCILAB_POINTER_DATATYPE
 * The scilab pointer datatype in the Scilab stack
 */
#define SCILAB_POINTER_DATATYPE "p"

/** @def GRAPHICAL_HANDLE_DATATYPE
 * The scilab graphic handle datatype in the Scilab stack
 */
#define GRAPHICAL_HANDLE_DATATYPE "h"

/** @def EXTERNAL_DATATYPE
 * An "external" is a function or routine which is used as an argument
 * of some high-level primitives (such as ode, optim, schur...).
 */
#define EXTERNAL_DATATYPE "f"

/** @def MATRIX_OF_POLYNOMIAL_DATATYPE
 * A matrix of polynomial coeff
 * if the size of the matrix is 1,1, it is a single value
 */
#define MATRIX_OF_POLYNOMIAL_DATATYPE "x"

#endif /*__STACKTYPEVARIABLE_H__ */
