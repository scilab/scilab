/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.AxesHandler;

import org.scilab.modules.renderer.CallRenderer;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import java.lang.Math;

/**
* Implements all Label manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class LabelHandler {


    /**
     * Set the text of label x, y or z.
     *
     * @param axes The axes that will recieve the label.
     * @param text An array of text to set.
     * @param axis The axis to set x, y or z.
     */

    public static void setLabel(String axes, String[] text, AxesHandler.axisTo axis) {

        String label = null;

        switch(axis) {
            case __X__:
                label = (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                break;
            case __Y__:
                label = (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                break;
            case __Z__:
                label = (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                break;
            default:
                return;
        }
        if (label != null) {
            GraphicController.getController().setProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
        }
    }

    /**
     * Get the text of the label x, y or z.
     *
     * @param axes The axes where is the label.
     * @param axis The axis of the label (x, y or z).
     * @return The text of the label.
     */

    public static String getLabelText(String axes, AxesHandler.axisTo axis) {

        String label = null;
        switch(axis) {
            case __X__:
                label = (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_X_AXIS_LABEL__);
                break;
            case __Y__:
                label = (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Y_AXIS_LABEL__);
                break;
            case __Z__:
                label = (String)GraphicController.getController().getProperty(axes, GraphicObjectProperties.__GO_Z_AXIS_LABEL__);
                break;
            default:
                return null;
        }
        String[] text = (String[])GraphicController.getController().getProperty(label, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        if (text != null) {
            return text[0];
        }
        return null;
    }
}
