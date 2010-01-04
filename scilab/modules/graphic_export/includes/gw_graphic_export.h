/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#ifndef __GW_GRAPHIC_EXPORT__
#define __GW_GRAPHIC_EXPORT__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_graphic_export.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
GRAPHIC_EXPORT_IMPEXP int gw_graphic_export(void);
/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
/*--------------------------------------------------------------------------*/ 
GRAPHIC_EXPORT_IMPEXP int sci_xs2bmp( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2eps( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2gif( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2jpg( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2pdf( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2png( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2ppm( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2svg( char * fname, unsigned long fname_len ) ;
GRAPHIC_EXPORT_IMPEXP int sci_xs2ps( char * fname, unsigned long fname_len ) ;
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_GRAPHIC_EXPORT__ */
/*--------------------------------------------------------------------------*/ 
