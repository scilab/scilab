/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#ifndef __TCLGLOBAL__
#define __TCLGLOBAL__

/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
  #include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

#include "tcl.h"
#include "tk.h"
#include "IsAScalar.h"
#include "MALLOC.h"

/*--------------------------------------------------------------------------*/
extern Tcl_Interp *TCLinterp;
extern Tk_Window TKmainWindow;
extern int XTKsocket;
extern int TK_Started;
/*--------------------------------------------------------------------------*/

/**
 * @TODO : add comment
 *
 * @param s 
 */
void nocase (char *s);

/**
 * @TODO : add comment
 *
 * @param RhsMatrix 
 * @return <ReturnValue>
 */
char *Matrix2String(int RhsMatrix);

/**
 * @TODO : add comment
 *
 * @param StringIn  
 * @param nbelemOut 
 * @return <ReturnValue>
 */
double *String2Matrix(char *StringIn,int *nbelemOut);

/**
 * @TODO : add comment
 *
 * @param FieldPropertie    
 * @return <ReturnValue>
 */
int MustReturnAMatrix(char *FieldPropertie);

/**
 * @TODO : add comment
 *
 * @param FieldPropertie    
 * @return <ReturnValue>
 */
int MustReturnAString(char *FieldPropertie);

/**
 * @TODO : add comment
 *
 * @param FieldPropertie    
 * @return <ReturnValue>
 */
int ValueMustBeAMatrix(char *FieldPropertie);

/**
 * @TODO : add comment
 *
 * @param FieldPropertie    
 * @return <ReturnValue>
 */
int ValueMustBeAString(char *FieldPropertie);

/**
 * @TODO : add comment
 *
 * @param FieldPropertie    
 * @return <ReturnValue>
 */
int CheckPropertyField(char *FieldPropertie);

/**
 * @TODO : add comment
 *
 * @param TCLinterp 
 * @param StringUTF8    
 * @return <ReturnValue>
 */
char *UTF8toANSI(Tcl_Interp *TCLinterp,char *StringUTF8);

/*--------------------------------------------------------------------------*/
#endif /* __TCLGLOBAL__ */
/*--------------------------------------------------------------------------*/ 
