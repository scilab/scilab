/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/**
 * @file JavaInteraction.h
 *
 * File regrouping routines used to interact with Java graphic objects
 */

#ifndef _JAVA_INTERACTION_H_
#define _JAVA_INTERACTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_renderer.h"

    /**
     * Start interactive zoom on given figure
     */
    RENDERER_IMPEXP void startInteractiveZoom(int iObjUID);

    /**
     * Call rubber box on a figure
     * @param pstObjUID figure UID on which to apply the rubber box
     * @param intialRect if not null, specify the initial rectangle to use
     * @return [usedButton x y z l L h]
     */
    RENDERER_IMPEXP double *javaClickRubberBox(int iObjUID, double *initialRect, int iRectSize);
    RENDERER_IMPEXP double *javaDragRubberBox(int iObjUID);

#ifdef __cplusplus
}
#endif

#endif /* _JAVA_INTERACTION_H_ */
