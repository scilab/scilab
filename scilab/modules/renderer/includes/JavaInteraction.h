/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
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
 * @param initialRect if not null, specify the initial rectangle to use
 * @return [usedButton x y z l L h]
 */
RENDERER_IMPEXP double *javaClickRubberBox(int iObjUID, double *initialRect, int iRectSize);
RENDERER_IMPEXP double *javaDragRubberBox(int iObjUID);

#ifdef __cplusplus
}
#endif

#endif /* _JAVA_INTERACTION_H_ */
