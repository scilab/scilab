/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 ** \file allexp.hxx
 ** Include all exported "expression related" headers.
 */

#ifndef AST_ALLEXP_HXX
# define AST_ALLEXP_HXX

# include "exp.hxx"

/*
** Constant Expressions
** \{
*/
#include "constexp.hxx"
#include "boolexp.hxx"
#include "intexp.hxx"
#include "floatexp.hxx"
#include "doubleexp.hxx"
#include "stringexp.hxx"
#include "commentexp.hxx"
#include "nilexp.hxx"
/*
** \}
*/

/*
** Control Expressions
** \{
*/
#include "controlexp.hxx"
#include "ifexp.hxx"
#include "whileexp.hxx"
#include "forexp.hxx"
#include "trycatchexp.hxx"
#include "breakexp.hxx"
#include "returnexp.hxx"
#include "selectexp.hxx"
#include "caseexp.hxx"
/*
** \}
*/

/*
** Mathematical Expressions
** \{
*/
#include "mathexp.hxx"
#include "notexp.hxx"
#include "transposeexp.hxx"
#include "opexp.hxx"
#include "logicalopexp.hxx"
// Matrix Expression
#include "matrixexp.hxx"
#include "matrixlineexp.hxx"
// Cell Expressions
#include "cellexp.hxx"
/*
** \}
*/


#include "callexp.hxx"
#include "cellcallexp.hxx"
#include "assignexp.hxx"

// List of expressions
#include "seqexp.hxx"
#include "arraylistexp.hxx"
#include "assignlistexp.hxx"

// FieldExp
#include "fieldexp.hxx"

// Type Dedicated Expressions
#include "listexp.hxx"

#endif /* !AST_ALLEXP_HXX */
