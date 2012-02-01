/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer;

/**
 * This is a static class to access the renderer module directly
 * from C/C++ code through JNI.
 * See SCI/modules/renderer/src/jni/renderer.giws.xml for other details.
 **/
public final class CallRenderer {

    /**
     * Updates the coordinate transformations of the Axes object given by the identifier.
     * It will be implemented later.
     * @param id the Axes' identifier.
     */
    public static void updateSubwinScale(String id) {
        // The implementation will be provided later.
    }

    /**
     * Updates the corners of the text object corresponding to the identifier.
     * It will be implemented later.
     * @param id the text object's identifier.
     */
    public static void updateTextBounds(String id) {
        // The implementation will be provided later.
    }

}
