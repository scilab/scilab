/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Interface to use class GraphicSynchronizer from C code
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAPHIC_SYNCHRONIZER_INTERFACE_H_
#define _GRAPHIC_SYNCHRONIZER_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ObjectStructure.h"

/**
 * Protect the data of a figure and its children from being accessed from an other thread.
 */
void startFigureDataWriting(sciPointObj * pFigure) ;

/**
 * Should be called after a call to startFigureDataWriting to remove figure locking.
 */
void endFigureDataWriting(sciPointObj * pFigure);

/**
 * Protect the data of a figure from being writen by an other thread.
 */
void startFigureDataReading(sciPointObj * pFigure);

/**
 * Should be called after a call to startFigureDataReading to remove figure locking.
 */
void endFigureDataReading(sciPointObj * pFigure);

/**
 * Protect the data of a figure from being writen or displayed by an other thread.
 */
void startFigureDataDisplaying(sciPointObj * pFigure);

/**
 * Should be called after a call to startFigureDataDisplaying to remove figure locking.
 */
void endFigureDataDisplaying(sciPointObj * pFigure);

/**
 * Protect all graphic hierarchy from being accessed from an other thread.
 */
void startGraphicDataWriting( void );

/**
 * Should be called after a call to startGraphicDataWriting.
 */
void endGraphicDataWriting( void );

/**
 * Protect all graphic hierarchy from being written by an other thread.
 */
void startGraphicDataReading( void );

/**
 * Should be called after a call to startGraphicDataWriting.
 */
void endGraphicDataReading( void );

/**
 * Protect all graphic hierarchy from being written by an other thread.
 */
void startGraphicDataDisplaying( void ) ;

/**
 * Should be called after a call to endFigureDataDisplaying.
 */
void endGraphicDataDisplaying( void ) ;

/**
 * Disable synchronization for a figure.
 * By default synchrnonization is enalbe
 * To be used with caution.
 */
void enableFigureSynchronization(sciPointObj * pFigure);

/**
 * Enable synchronization for a figure.
 * By default synchrnonization is enalbe
 * To be used with caution.
 */
void disableFigureSynchronization(sciPointObj * pFigure);

/**
 * To be called before any call to startGraphicData***
 * Allocates data needed by these functions.
 */
void createGraphicSynchronizer( void ) ;

/**
 * Deallocates data needed by startGraphicData*** functions.
 */
void destroyGraphicSynchronizer( void ) ;

#ifdef __cplusplus
}
#endif

#endif /* _GRAPHIC_SYNCHRONIZER_INTERFACE_H_ */
