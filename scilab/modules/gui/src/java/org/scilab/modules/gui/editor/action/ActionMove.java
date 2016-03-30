/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
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
package org.scilab.modules.gui.editor.action;

import org.scilab.modules.gui.editor.action.BaseAction;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.PolylineHandler;
import org.scilab.modules.gui.editor.LegendHandler;

/**
* Implements the object to store Move related actions
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/
public class ActionMove extends BaseAction {

    /* Store the position of the mouse when MouseDown and when MouseRelease */
    Integer[] myClick, myNewClick;

    /* Legend or Polyline */
    boolean isLegend;

    /**
    * Constructor
    *
    * @param objectUID The moved object
    * @param click The position of mousedown action
    * @param newClick The position of mouseup action
    * @param isLegend True if is legend, false if is polyline
    */
    public ActionMove(Integer objectUID, Integer[] click, Integer[] newClick, boolean isLegend) {

        super(objectUID, (Integer)GraphicController.getController().getProperty(objectUID, GraphicObjectProperties.__GO_PARENT__));
        myClick = new Integer[2];
        myNewClick = new Integer[2];
        myClick[0] = click[0];
        myClick[1] = click[1];
        myNewClick[0] = newClick[0];
        myNewClick[1] = newClick[1];
        this.isLegend = isLegend;
    }

    /**
    * Undo the  move, putting the object back to the original position
    */
    public void undo() {

        if (exists()) {
            if (!isLegend) {
                PolylineHandler.getInstance().dragPolyline(objectUID, myNewClick, myClick);
            } else {
                LegendHandler.dragLegend(objectUID, myNewClick, myClick);
            }
        } else {
            objectUID = null;
        }
    }

    /**
    * Redo the  move, putting the object back to the moved position
    */
    public void redo() {

        if (exists()) {
            if (!isLegend) {
                PolylineHandler.getInstance().dragPolyline(objectUID, myClick, myNewClick);
            } else {
                LegendHandler.dragLegend(objectUID, myClick, myNewClick);
            }
        } else {
            objectUID = null;
        }
    }

    public void dispose() {

        objectUID = null;
    }
}