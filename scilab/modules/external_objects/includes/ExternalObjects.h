/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __EXTERNALOBJECTS_H__
#define __EXTERNALOBJECTS_H__

#define __USE_DEPRECATED_STACK_FUNCTIONS__

//#define EODEBUG

#define FIELDS_LENGTH 3

#define VOID_OBJECT -1

#define EXTERNAL_INVALID -1
#define EXTERNAL_OBJECT 0
#define EXTERNAL_CLASS 1
#define EXTERNAL_VOID 2

#define EXTERNAL_ENV_ID_POSITION 2
#define EXTERNAL_OBJ_ID_POSITION 3

#define SAE_NOTHING 0
#define SAE_NULL 1
#define SAE_SINGLE_DOUBLE 2
#define SAE_ROW_DOUBLE 3
#define SAE_MAT_DOUBLE 4
#define SAE_SINGLE_STRING 5
#define SAE_ROW_STRING 6
#define SAE_MAT_STRING 7
#define SAE_SINGLE_BOOLEAN 8
#define SAE_ROW_BOOLEAN 9
#define SAE_MAT_BOOLEAN 10
#define SAE_SINGLE_CHAR 11
#define SAE_ROW_CHAR 12
#define SAE_MAT_CHAR 13
#define SAE_SINGLE_UCHAR 14
#define SAE_ROW_UCHAR 15
#define SAE_MAT_UCHAR 16
#define SAE_SINGLE_SHORT 17
#define SAE_ROW_SHORT 18
#define SAE_MAT_SHORT 19
#define SAE_SINGLE_USHORT 20
#define SAE_ROW_USHORT 21
#define SAE_MAT_USHORT 22
#define SAE_SINGLE_INT 23
#define SAE_ROW_INT 24
#define SAE_MAT_INT 25
#define SAE_SINGLE_UINT 26
#define SAE_ROW_UINT 27
#define SAE_MAT_UINT 28
#define SAE_SINGLE_LONG 29
#define SAE_ROW_LONG 30
#define SAE_MAT_LONG 31
#define SAE_SINGLE_ULONG 32
#define SAE_ROW_ULONG 33
#define SAE_MAT_ULONG 34
#define SAE_SINGLE_FLOAT 35
#define SAE_ROW_FLOAT 36
#define SAE_MAT_FLOAT 37
#define SAE_SINGLE_COMPLEX 38
#define SAE_ROW_COMPLEX 39
#define SAE_MAT_COMPLEX 40

typedef enum { Nothing = SAE_NOTHING, Null = SAE_NULL,
               SingleDouble = SAE_SINGLE_DOUBLE, RowDouble = SAE_ROW_DOUBLE, MatDouble = SAE_MAT_DOUBLE,
               SingleString = SAE_SINGLE_STRING, RowString = SAE_ROW_STRING, MatString = SAE_MAT_STRING,
               SingleBoolean = SAE_SINGLE_BOOLEAN, RowBoolean = SAE_ROW_BOOLEAN, MatBoolean = SAE_MAT_BOOLEAN,
               SingleChar = SAE_SINGLE_CHAR, RowChar = SAE_ROW_CHAR, MatChar = SAE_MAT_CHAR,
               SingleUChar = SAE_SINGLE_UCHAR, RowUChar = SAE_ROW_UCHAR, MatUChar = SAE_MAT_UCHAR,
               SingleShort = SAE_SINGLE_SHORT, RowShort = SAE_ROW_SHORT, MatShort = SAE_MAT_SHORT,
               SingleUShort = SAE_SINGLE_USHORT, RowUShort = SAE_ROW_USHORT, MatUShort = SAE_MAT_USHORT,
               SingleInt = SAE_SINGLE_INT, RowInt = SAE_ROW_INT, MatInt = SAE_MAT_INT,
               SingleUInt = SAE_SINGLE_UINT, RowUInt = SAE_ROW_UINT, MatUInt = SAE_MAT_UINT,
               SingleLong = SAE_SINGLE_LONG, RowLong = SAE_ROW_LONG, MatLong = SAE_MAT_LONG,
               SingleULong = SAE_SINGLE_ULONG, RowULong = SAE_ROW_ULONG, MatULong = SAE_MAT_ULONG,
               SingleFloat = SAE_SINGLE_FLOAT, RowFloat = SAE_ROW_FLOAT, MatFloat = SAE_MAT_FLOAT,
               SingleComplex = SAE_SINGLE_COMPLEX, RowComplex = SAE_ROW_COMPLEX, MatComplex = SAE_MAT_COMPLEX
             } VariableType;

#define SAE_TRANSP 0
#define SAE_ADD 1
#define SAE_SUB 2
#define SAE_MUL 3
#define SAE_DIV 4
#define SAE_BACKSLASH 5
#define SAE_POW 6
#define SAE_DOT_MUL 7
#define SAE_DOT_DIV 8
#define SAE_DOT_BACKSLASH 9
#define SAE_DOT_MUL_DOT 10
#define SAE_DOT_DIV_DOT 11
#define SAE_DOT_BACKSLASH_DOT 12
#define SAE_EQ 13
#define SAE_NEQ 14
#define SAE_OR 15
#define SAE_AND 16
#define SAE_DOT_POW 17
#define SAE_NOT 18
#define SAE_DOT_TRANSP 19
#define SAE_LT 20
#define SAE_GT 21
#define SAE_LEQ 22
#define SAE_GEQ 23

typedef enum { Transp = SAE_TRANSP, Add = SAE_ADD, Sub = SAE_SUB, Mul = SAE_MUL, Div = SAE_DIV, Backslash = SAE_BACKSLASH,
               Pow = SAE_POW, DotMul = SAE_DOT_MUL, DotDiv = SAE_DOT_DIV, DotBackslash = SAE_DOT_BACKSLASH,
               DotMulDot = SAE_DOT_MUL_DOT, DotDivDot = SAE_DOT_DIV_DOT, DotBackslashDot = SAE_DOT_BACKSLASH_DOT, Eq = SAE_EQ,
               Neq = SAE_NEQ, Or = SAE_OR, And = SAE_AND, DotPow = SAE_DOT_POW, Not = SAE_NOT, DotTransp = SAE_DOT_TRANSP,
               Lt = SAE_LT, Gt = SAE_GT, Leq = SAE_LEQ, Geq = SAE_GEQ
             } OperatorsType;

#endif //__EXTERNALOBJECTS_H__
