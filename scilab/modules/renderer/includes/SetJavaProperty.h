/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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
RENDERER_IMPEXP void sciJavaUpdateSubwinScale(int iubwinUID);

/**
 * Update bounding box of a text object if needed
 */
RENDERER_IMPEXP void sciJavaUpdateTextBoundingBox(int iTextUID);

#ifdef __cplusplus
}
#endif

#endif /* _SET_JAVA_PROPERTY_H_ */
