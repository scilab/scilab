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

import java.util.ArrayList;

/**
 * Highlight and discolor the selected datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipHighlight {

    private static boolean isSelected = false;
    public static String datatipid;
    public static String markerid;
    public static Integer firstColorDatatip;
    public static Integer firstColorMarker;

    /**
    * Highlight a datatip and its marker when selected
    *
    * @param datatipIndex Integer referring to the selected datatip.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    */
    public static void highlightSelected (Integer datatipIndex, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {
        if (datatipIndex != null) {
            if (!isSelected) {
                datatipid = datatipsUid.get (datatipIndex / 2);
                markerid = markersUid.get (datatipIndex / 2);
                firstColorDatatip = (Integer) GraphicController.getController().getProperty(datatipid, GraphicObjectProperties.__GO_LINE_COLOR__);
                firstColorMarker = (Integer) GraphicController.getController().getProperty(markerid, GraphicObjectProperties.__GO_LINE_COLOR__);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_LINE_COLOR__, 12);
                GraphicController.getController().setProperty(markerid, GraphicObjectProperties.__GO_LINE_COLOR__, 12);
            } else {
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_LINE_COLOR__, firstColorDatatip);
                GraphicController.getController().setProperty(markerid, GraphicObjectProperties.__GO_LINE_COLOR__, firstColorMarker);
                datatipid = datatipsUid.get (datatipIndex / 2);
                markerid = markersUid.get (datatipIndex / 2);
                firstColorDatatip = (Integer) GraphicController.getController().getProperty(datatipid, GraphicObjectProperties.__GO_LINE_COLOR__);
                firstColorMarker = (Integer) GraphicController.getController().getProperty(markerid, GraphicObjectProperties.__GO_LINE_COLOR__);
                GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_LINE_COLOR__, 12);
                GraphicController.getController().setProperty(markerid, GraphicObjectProperties.__GO_LINE_COLOR__, 12);
            }
        isSelected = true;
        } else {
            GraphicController.getController().setProperty(datatipid, GraphicObjectProperties.__GO_LINE_COLOR__, firstColorDatatip);
            GraphicController.getController().setProperty(markerid, GraphicObjectProperties.__GO_LINE_COLOR__, firstColorMarker);
            isSelected = false;
        }
    }
}
