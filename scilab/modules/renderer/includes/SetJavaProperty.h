/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*
 * desc : Interface functions between between SetProperty functions and
 * the C++/Java part of module
 */

#ifndef _SET_JAVA_PROPERTY_H_
#define _SET_JAVA_PROPERTY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_renderer.h"

/**
 * Update subwin size and scale
 */
RENDERER_IMPEXP void sciJavaUpdateSubwinScale(char * pSubwinUID);

/**
 * Update bounding box of a text object if needed
 */
RENDERER_IMPEXP void sciJavaUpdateTextBoundingBox(char * pTextUID);

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */
