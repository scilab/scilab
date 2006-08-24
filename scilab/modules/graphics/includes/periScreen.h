/*------------------------------------------------------------------------*/
/* file: periScreen.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : include file for periX11 or periWin files                       */
/*------------------------------------------------------------------------*/

#ifndef _PERI_SCREEN_H_
#define _PERI_SCREEN_H_

#include "machine.h"

#ifdef _MSC_VER
#include "periWin.h"
#else
#include "periX11.h"
#endif

#endif /* _PERI_SCREEN_H_ */
