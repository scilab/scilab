/*------------------------------------------------------------------------*/
/* file: DefaultCommandArg.h                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : access to default values for graphics arguments                 */
/*------------------------------------------------------------------------*/

#ifndef _DEFAULT_COMMAND_ARG_H_
#define _DEFAULT_COMMAND_ARG_H_

#include "machine.h"

/*------------------------------------------------------------------------*/
/* real default */
#define DEFSTRF "081" 
#define DEFSTRFN "099" 
#define DEFLOGFLAGS "gnn" 

/*------------------------------------------------------------------------*/
/* Accessors to to default values */
double * getDefRect(      void ) ;
char   * getDefStrf(      void ) ;
char   * getDefLegend(    void ) ;
double * getDefZminMax(   void ) ;
int    * getDefNax(       void ) ;
int    * getDefColMinMax( void ) ;
int    * getDefColOut(    void ) ;
BOOL     getDefWithMesh(  void ) ;
char   * getDefLogFlags(  void ) ;
/*------------------------------------------------------------------------*/
/* set default values by copying the newDef */
void setDefRect(      const double newDef[4] ) ;
void setDefStrf(      const char   newDef[4] ) ;
void setDefZminMax(   const double newDef[2] ) ;
void setDefNax(       const int    newDef[4] ) ;
void setDefColMinMax( const int    newDef[2] ) ;
void setDefColOut(    const int    newDef[2] ) ;
void setDefWithMesh(        BOOL   newDef    ) ;
void setDefLogFlags(  const char   newDef[4] ) ;
/*------------------------------------------------------------------------*/
/* change values */
void reinitDefStrf(     void ) ;
void reinitDefStrfN(    void ) ;
void reinitDefLogFlags( void ) ;
/*------------------------------------------------------------------------*/
/* check if we are dealing with the default value */
BOOL isDefRect(      const double * rect      ) ;
BOOL isDefStrf(      const char   * strf      ) ;
BOOL isDefLegend(    const char   * legend    ) ;
BOOL isDefZminMax(   const double * zminmax   ) ;
BOOL isDefNax(       const int    * nax       ) ;
BOOL isDefColMinMax( const int    * colminmax ) ;
BOOL isDefColOut(    const int    * colout    ) ;
BOOL isDefWithMesh(  const BOOL     withmesh  ) ;
BOOL isDefLogFlags(  const char   * logflags  ) ;
/*------------------------------------------------------------------------*/

#endif /* _DEFAULT_COMMAND_ARG_H_ */