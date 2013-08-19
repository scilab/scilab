/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
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
import org.scilab.modules.gui.editor.AxesHandler;
import org.scilab.modules.gui.ged.graphic_objects.axes.Axes;
import org.scilab.modules.gui.ged.graphic_objects.datatip.Datatip;
import org.scilab.modules.gui.ged.graphic_objects.figure.Figure;
import org.scilab.modules.gui.ged.graphic_objects.imageplot.Imageplot;
import org.scilab.modules.gui.ged.graphic_objects.legend.Legend;
import org.scilab.modules.gui.ged.graphic_objects.polyline.Polyline;
import org.scilab.modules.gui.ged.graphic_objects.surface.Surface;

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
                Datatip datatip = new Datatip();
                datatip.initSections(objectID);
                SwingInspector.setPanel(datatip, MessagesGED.datatip);
                break;
            case FAC3D:
                Surface fac3d = new Surface();
                fac3d.initSections(objectID);
                SwingInspector.setPanel(fac3d, MessagesGED.fac3d);
                break;
            case GRAYPLOT:
                Imageplot grayplot = new Imageplot();
                grayplot.initSections(objectID);
                SwingInspector.setPanel(grayplot, MessagesGED.grayplot);
                break;
            case LEGEND:
                Legend legend = new Legend();
                legend.initSections(objectID);
                SwingInspector.setPanel(legend, MessagesGED.legend);
                break;
            case MATPLOT:
                Imageplot matplot = new Imageplot();
                matplot.initSections(objectID);
                SwingInspector.setPanel(matplot, MessagesGED.matplot);
                break;
            case PLOT3D:
                Surface plot3d = new Surface();
                plot3d.initSections(objectID);
                SwingInspector.setPanel(plot3d, MessagesGED.plot3d);
                break;
            case POLYLINE:
                Polyline polyline = new Polyline();
                polyline.initSections(objectID);
                SwingInspector.setPanel(polyline, MessagesGED.polyline);
                break;
            default:
                //If the user clicks on an axis, return axesID
                String axesID = getAxesClicked(objectID, clickX, clickY);
                if (axesID != null) {
                    Axes axes = new Axes();
                    axes.initSections(axesID);
                    SwingInspector.setPanel(axes, MessagesGED.axes);
                } else {
                    Figure figure = new Figure();
                    figure.initSections(objectID);
                    SwingInspector.setPanel(figure, MessagesGED.figure);
                }
                break;
        }
    }

    /**
    * Checks whether the click corresponds to the axis or the figure.
    *
    * @param objectID Enters the identification of object.
    * @param clickX x coordinate of mouse.
    * @param clickY y coordinate of mouse.
    * @return if axes, return true
    */
    private String getAxesClicked(String objectID, Integer clickX, Integer clickY) {
        Integer[] position = new Integer[] {clickX, clickY};
        String axesID = AxesHandler.clickedAxes(objectID, position);
        Double[] axesMargins = (Double[]) GraphicController.getController()
                               .getProperty(axesID, GraphicObjectProperties.__GO_MARGINS__);

        Double left = axesMargins[0];
        Double right = axesMargins[1];
        Double top = axesMargins[2];
        Double bottom = axesMargins[3];

        Integer[] axesSize = (Integer[]) GraphicController.getController()
                             .getProperty(objectID, GraphicObjectProperties.__GO_AXES_SIZE__);

        Integer x = axesSize[0];
        Integer y = axesSize[1];

        Double leftBorder = left * x;
        Double rightBorder = x - right * x;
        Double bottomBorder = bottom * y;
        Double topBorder = y - top * y;

        if (position[0] > leftBorder && position[0] < rightBorder && position[1] > bottomBorder && position[1] < topBorder) {
            return axesID;
        } else {
            return null;
        }
    }
}