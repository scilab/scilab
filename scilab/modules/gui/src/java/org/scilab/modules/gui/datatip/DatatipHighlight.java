/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * Highlight and discolor the selected datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipHighlight {

    private static boolean isSelected = false;
    public static Integer firstColorMarker;
    public static String oldDatatip = null;

    /**
    * Highlight a datatip and its mark when selected
    *
    * @param datatipUid Datatip unique identifier.
    */
    public static void highlightSelected (String datatipUid) {
        if (datatipUid != null) {
            if (!isSelected) {
                firstColorMarker = (Integer) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
                GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_MARK_BACKGROUND__, -3);
                oldDatatip = datatipUid;
                isSelected = true;
            } else {
                GraphicController.getController().setProperty(oldDatatip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, firstColorMarker);
                firstColorMarker = (Integer) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
                GraphicController.getController().setProperty(datatipUid, GraphicObjectProperties.__GO_MARK_BACKGROUND__, -3);
                oldDatatip = datatipUid;
                isSelected = false;
            }
        } else {
            if (oldDatatip != null) {
                GraphicController.getController().setProperty(oldDatatip, GraphicObjectProperties.__GO_MARK_BACKGROUND__, firstColorMarker);
                isSelected = false;
            }
        }
    }
}
