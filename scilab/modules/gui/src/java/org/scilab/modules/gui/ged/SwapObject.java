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
    * @param objectID Enters the identification of object.
    */
    public SwapObject(String objectID) {
        Integer type = (Integer) GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_TYPE__);
        switch (type) {
            case GraphicObjectProperties.__GO_POLYLINE__:
                Polyline polyline = new Polyline(objectID);
                SwingInspector.setPanel(polyline, MessagesGED.polyline);
                break;
            case GraphicObjectProperties.__GO_PLOT3D__:
                Surface plot3d = new Surface(objectID);
                SwingInspector.setPanel(plot3d, MessagesGED.plot3d);
                break;
            case GraphicObjectProperties.__GO_GRAYPLOT__:
                Imageplot grayplot = new Imageplot(objectID);
                SwingInspector.setPanel(grayplot, MessagesGED.grayplot);
                break;
            case GraphicObjectProperties.__GO_FAC3D__:
                Surface fac3d = new Surface(objectID);
                SwingInspector.setPanel(fac3d, MessagesGED.fac3d);
                break;
            case GraphicObjectProperties.__GO_MATPLOT__:
                Imageplot matplot = new Imageplot(objectID);
                SwingInspector.setPanel(matplot, MessagesGED.matplot);
                break;
            case GraphicObjectProperties.__GO_LEGEND__:
                Legend legend = new Legend(objectID);
                SwingInspector.setPanel(legend, MessagesGED.legend);
                break;
            case GraphicObjectProperties.__GO_ARC__:
            case GraphicObjectProperties.__GO_CHAMP__:
            case GraphicObjectProperties.__GO_FEC__:
            case GraphicObjectProperties.__GO_RECTANGLE__:
            case GraphicObjectProperties.__GO_SEGS__:
            case GraphicObjectProperties.__GO_DATATIP__:
                Datatip datatip = new Datatip(objectID);
                SwingInspector.setPanel(datatip, MessagesGED.datatip);
                break;
            case GraphicObjectProperties.__GO_AXES__:
                Axes axes = new Axes(objectID);
                SwingInspector.setPanel(axes, MessagesGED.axes);
                break;
            default:
                Figure figure = new Figure(objectID);
                SwingInspector.setPanel(figure, MessagesGED.figure);
                break;
        }
    }
}