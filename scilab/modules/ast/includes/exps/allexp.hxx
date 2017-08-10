/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 ** \file allexp.hxx
 ** Include all exported "expression related" headers.
 */

#ifndef AST_ALLEXP_HXX
#define AST_ALLEXP_HXX

#include "exp.hxx"

/*
** Constant Expressions
*/
#include "constexp.hxx"
#include "boolexp.hxx"
#include "doubleexp.hxx"
#include "stringexp.hxx"
#include "commentexp.hxx"
#include "nilexp.hxx"

/*
** Control Expressions
*/
#include "controlexp.hxx"
#include "ifexp.hxx"
#include "whileexp.hxx"
#include "forexp.hxx"
#include "trycatchexp.hxx"
#include "breakexp.hxx"
#include "continueexp.hxx"
#include "returnexp.hxx"
#include "selectexp.hxx"
#include "caseexp.hxx"

/*
** Mathematical Expressions
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
#include "optimizedexp.hxx"
#include "daxpyexp.hxx"
#include "fastselectexp.hxx"
#include "intselectexp.hxx"
#include "mapintselectexp.hxx"
#include "smallintselectexp.hxx"
#include "stringselectexp.hxx"
#include "tableintselectexp.hxx"
#include "memfillexp.hxx"

#endif /* !AST_ALLEXP_HXX */
