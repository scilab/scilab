/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged;

import java.awt.Dimension;
import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.ged.axes.Axes;
import org.scilab.modules.gui.ged.curve.Curve;
import org.scilab.modules.gui.ged.figure.Figure;

/**
* Manager object exchange.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class SwapObject {

    /**
    * Manager which property window will open.
    *
    * @param select Name of the object chosen.
    * @param objectID Enters the identification of object.
    * @param clickX x coordinate of mouse.
    * @param clickY y coordinate of mouse.
    */
    public SwapObject(String select, String objectID, Integer clickX, Integer clickY) {
        if (select.equals("curve")) {
            curve(objectID);
        } else {
            Integer[] position = new Integer[2];
            position[0] = clickX;
            position[1] = clickY;
            String axesID = AxesHandler.clickedAxes(objectID, position);
            Double[] axesMargins = (Double[]) GraphicController.getController()
                                   .getProperty(axesID, GraphicObjectProperties.__GO_MARGINS__);
            Double left, right, top, bottom;

            left = axesMargins[0];
            right = axesMargins[1];
            top = axesMargins[2];
            bottom = axesMargins[3];

            Integer[] axesSize = (Integer[])GraphicController.getController()
                                 .getProperty(objectID, GraphicObjectProperties.__GO_AXES_SIZE__);
            Integer x, y;

            x = axesSize[0];
            y = axesSize[1];

            Double leftBorder, rightBorder, bottomBorder, topBorder;

            leftBorder = left * x;
            rightBorder = x - right * x;
            bottomBorder = bottom * y;
            topBorder = y - top * y;

            if (position[0] > leftBorder && position[0] < rightBorder && position[1] > bottomBorder && position[1] < topBorder) {
                axes(axesID);
            } else {
                figure(objectID);
            }
        }
    }

    /**
    * Loads the properties of the axis.
    *
    * @param axesID Enters the identification of axis.
    */
    private void axes(String axesID) {
        //Resets the panel.
        SwingInspector.pReceive.removeAll();
        SwingInspector.pReceive.repaint();
        //Resets the Show/Hide button.
        ShowHide.click = true;
        ShowHide.setIcon(1);
        //Load the Axes panel.
        SwingInspector.pReceive.add(new Axes(axesID));
    }

    /**
    * Loads the properties of the curve.
    *
    * @param objectID Enters the identification of curve.
    */
    private void curve(String objectID) {
        //Resets the panel.
        SwingInspector.pReceive.removeAll();
        SwingInspector.pReceive.repaint();
        //Resets the Show/Hide button.
        ShowHide.click = true;
        ShowHide.setIcon(1);
        //Load the curve panel.
        SwingInspector.pReceive.add(new Curve(objectID));
    }

    /**
    * Loads the properties of the figure.
    *
    * @param objectID Enters the identification of figure.
    */
    private void figure(String objectID) {
        //Resets the panel.
        SwingInspector.pReceive.removeAll();
        SwingInspector.pReceive.repaint();
        //Resets the Show/Hide button.
        ShowHide.click = true;
        ShowHide.setIcon(1);
        //Load the figure panel.
        SwingInspector.pReceive.add(new Figure(objectID));
    }
}