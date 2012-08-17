/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;


import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public enum SelectionEnum {
    POLYLINE,
    LEGEND,
    SURFACE,
    FIGURE,
    DATATIP,
    PLOT3D,
    GRAYPLOT,
    FAC3D,
    AXES_OR_FIGURE;

    public static SelectionEnum objectToEnum(String uid) {
        String type = (String) GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        if (type == GraphicObjectProperties.__GO_POLYLINE__) {
            return POLYLINE;
        } else if (type == GraphicObjectProperties.__GO_LEGEND__) {
            return LEGEND;
        } else if (type == GraphicObjectProperties.__GO_PLOT3D__) {
            return PLOT3D;
        } else if (type == GraphicObjectProperties.__GO_FAC3D__) {
            return FAC3D;
        } else if (type == GraphicObjectProperties.__GO_GRAYPLOT__) {
            return GRAYPLOT;
        } else {
            return null;
        }
    }
}
