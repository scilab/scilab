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

import java.awt.Dimension;
import javax.swing.JPanel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.ged.actions.ShowHide;
import org.scilab.modules.gui.ged.axes.Axes;
import org.scilab.modules.gui.ged.figure.Figure;
import org.scilab.modules.gui.ged.legend.Legend;
import org.scilab.modules.gui.ged.polyline.Polyline;
import org.scilab.modules.gui.ged.surface.grayplot.Grayplot;
import org.scilab.modules.gui.ged.surface.Surface;

/**
* Manager object exchange.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class SwapObject {
    /**
    * Manager which property window will open.
    *
    * @param selected Name of the object chosen.
    * @param objectID Enters the identification of object.
    * @param clickX x coordinate of mouse.
    * @param clickY y coordinate of mouse.
    */
    public SwapObject(SelectionEnum selected, String objectID, Integer clickX, Integer clickY) {
        switch (selected) {
            case DATATIP:
                /*not implemented yet */
                break;
            case FAC3D:
                fac3d(objectID);
                break;
            case GRAYPLOT:
                grayplot(objectID);
                break;
            case LEGEND:
                legend(objectID);
                break;
            case PLOT3D:
                plot3d(objectID);
                break;
            case POLYLINE:
                polyline(objectID);
                break;
            default:
                axesORfigure(objectID, clickX, clickY);
                break;
        }
    }

    /**
    * Prepare the panel to receive a new JPanel.
    */
    private void adjust() {
        //Resets the panel.
        SwingInspector.pReceive.removeAll();
        SwingInspector.pReceive.repaint();
        //Resets the Show/Hide button.
        ShowHide.click = true;
        ShowHide.setIcon(1);
    }

    /**
    * Checks whether the click corresponds to the axis or the figure.
    *
    * @param objectID Enters the identification of object.
    * @param clickX x coordinate of mouse.
    * @param clickY y coordinate of mouse.
    */
    private void axesORfigure(String objectID, Integer clickX, Integer clickY) {
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

    /**
    * Loads the properties of the axis.
    *
    * @param axesID Enters the identification of axis.
    */
    private void axes(String axesID) {
        adjust();
        //Load the Axes panel.
        SwingInspector.pReceive.add(new Axes(axesID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.axes);
        } catch (NullPointerException npe) { }
    }

    /**
    * Loads the properties of the figure.
    *
    * @param objectID Enters the identification of figure.
    */
    private void figure(String objectID) {
        adjust();
        //Load the figure panel.
        SwingInspector.pReceive.add(new Figure(objectID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.figure);
        } catch (NullPointerException npe) { }
    }

    /**
    * Loads the properties of the legend.
    *
    * @param objectID Enters the identification of legend.
    */
    private void legend(String objectID) {
        adjust();
        //Load the legend panel.
        SwingInspector.pReceive.add(new Legend(objectID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.legend);
        } catch (NullPointerException npe) { }
    }

    /**
    * Loads the properties of the polyline.
    *
    * @param objectID Enters the identification of polyline.
    */
    private void polyline(String objectID) {
        adjust();
        //Load the polyline panel.
        SwingInspector.pReceive.add(new Polyline(objectID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.polyline);
        } catch (NullPointerException npe) { }
    }

    /**
    * Loads the properties of the plot3d.
    *
    * @param objectID Enters the identification of surface.
    */
    private void plot3d(String objectID) {
        adjust();
        //Load the polyline panel.
        SwingInspector.pReceive.add(new Surface(objectID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.plot3d);
        } catch (NullPointerException npe) { }
    }

    /**
    * Loads the properties of the fac3d.
    *
    * @param objectID Enters the identification of surface.
    */
    private void fac3d(String objectID) {
        adjust();
        //Load the polyline panel.
        SwingInspector.pReceive.add(new Surface(objectID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.fac3d);
        } catch (NullPointerException npe) { }
    }

    /**
    * Loads the properties of the grayplot.
    *
    * @param objectID Enters the identification of surface.
    */
    private void grayplot(String objectID) {
        adjust();
        //Load the polyline panel.
        SwingInspector.pReceive.add(new Grayplot(objectID), "");
        try {
            Inspector.inspectorTab.setTitle(MessagesGED.quick_ged + ": " + MessagesGED.grayplot);
        } catch (NullPointerException npe) { }
    }
}