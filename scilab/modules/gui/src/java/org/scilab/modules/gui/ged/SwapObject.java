/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 2013 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.graphic_objects.arc.Arc;
import org.scilab.modules.gui.ged.graphic_objects.axes.Axes;
import org.scilab.modules.gui.ged.graphic_objects.champ.Champ;
import org.scilab.modules.gui.ged.graphic_objects.datatip.Datatip;
import org.scilab.modules.gui.ged.graphic_objects.fec.Fec;
import org.scilab.modules.gui.ged.graphic_objects.figure.Figure;
import org.scilab.modules.gui.ged.graphic_objects.imageplot.Imageplot;
import org.scilab.modules.gui.ged.graphic_objects.label.Label;
import org.scilab.modules.gui.ged.graphic_objects.legend.Legend;
import org.scilab.modules.gui.ged.graphic_objects.polyline.Polyline;
import org.scilab.modules.gui.ged.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.gui.ged.graphic_objects.segs.Segs;
import org.scilab.modules.gui.ged.graphic_objects.surface.Surface;

/**
* Manager object exchange.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class SwapObject {
    private static int lastType;

    /**
    * Manager which property window will open.
    * @param objectID Enters the identification of object.
    */
    public SwapObject(Integer objectID) {
        Integer type = (Integer) GraphicController.getController().getProperty(objectID, GraphicObjectProperties.__GO_TYPE__);
        lastType = type;
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
            case GraphicObjectProperties.__GO_LABEL__:
                Label label = new Label(objectID);
                SwingInspector.setPanel(label, MessagesGED.label);
                break;
            case GraphicObjectProperties.__GO_ARC__:
                Arc arc = new Arc(objectID);
                SwingInspector.setPanel(arc, MessagesGED.arc);
                break;
            case GraphicObjectProperties.__GO_CHAMP__:
                Champ champ = new Champ(objectID);
                SwingInspector.setPanel(champ, MessagesGED.champ);
                break;
            case GraphicObjectProperties.__GO_FEC__:
                Fec fec = new Fec(objectID);
                SwingInspector.setPanel(fec, MessagesGED.fec);
                break;
            case GraphicObjectProperties.__GO_RECTANGLE__:
                Rectangle rectangle = new Rectangle(objectID);
                SwingInspector.setPanel(rectangle, MessagesGED.rectangle);
                break;
            case GraphicObjectProperties.__GO_SEGS__:
                Segs segs = new Segs(objectID);
                SwingInspector.setPanel(segs, MessagesGED.segs);
                break;
            case GraphicObjectProperties.__GO_DATATIP__:
                Datatip datatip = new Datatip(objectID);
                SwingInspector.setPanel(datatip, MessagesGED.datatip);
                break;
            case GraphicObjectProperties.__GO_AXES__:
                Axes axes = new Axes(objectID);
                SwingInspector.setPanel(axes, MessagesGED.axes);
                break;
            default:
                try {
                    Figure figure = new Figure(objectID);
                    SwingInspector.setPanel(figure, MessagesGED.figure);
                } catch (NullPointerException e) { }
                break;
        }
    }

    /**
    * Get the last type of object.
    * @return type
    */
    public static int getLastType() {
        return lastType;
    }
}