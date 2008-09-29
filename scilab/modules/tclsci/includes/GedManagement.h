/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------------*/
/* Desc   : C functions to manage ged (only destroy for now)                               */
/*--------------------------------------------------------------------------------*/

#ifndef __GEDMANAGEMENT_H_
#define __GEDMANAGEMENT_H_
/*--------------------------------------------------------------------------------*/
/* close the graphic editor linked with the window figureId */
int sciDestroyGed( int figureId ) ;
/*--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*/
/* return true if ged is opened on the window figureId */
int isGedOpenedOn( int figureId ) ;
/*--------------------------------------------------------------------------------*/
#endif /* __GEDMANAGEMENT_H_ */
