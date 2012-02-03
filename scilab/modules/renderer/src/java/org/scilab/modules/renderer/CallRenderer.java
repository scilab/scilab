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

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.textObject.Text;

import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.text.TextManager;

/**
 * This is a static class to access the renderer module directly
 * from C/C++ code through JNI.
 * See SCI/modules/renderer/src/jni/renderer.giws.xml for other details.
 **/
public final class CallRenderer {

    /**
     * Updates the coordinate transformation of the Axes object given by the identifier.
     * @param id the Axes' identifier.
     */
    public static void updateSubwinScale(String id) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Axes) {
            AxesDrawer.updateAxesTransformation((Axes) object);
        }
    }

    /**
     * Updates the corners of the text object corresponding to the identifier.
     * @param id the text object's identifier.
     */
    public static void updateTextBounds(String id) {
        GraphicObject object = GraphicController.getController().getObjectFromId(id);

        if (object != null && object instanceof Text) {
            TextManager.updateTextCorners((Text) object);
        }
    }
}
