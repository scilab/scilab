/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.ObjectSearcher;

/**
 * Delete a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDelete {

    /**
    * Delete a specific datatip using the right mouse click;
    *
    * @param datatipUid Datatip unique identifier.
    */
    public static void deleteDatatip(int datatipUid) {
        GraphicController controller = GraphicController.getController();

        //get parent
        Integer parent = (Integer) controller.getProperty(datatipUid, GraphicObjectProperties.__GO_PARENT__);
        //get parent datatips
        Integer[] tips = (Integer[]) controller.getProperty(parent, GraphicObjectProperties.__GO_DATATIPS__);
        int index = -1;
        for (int i = 0; i < tips.length; i++) {
            if (tips[i] == datatipUid) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            Integer[] var = new Integer[tips.length - 1];
            for (int i = 0; i < tips.length; i++) {
                if (i < index) {
                    var[i] = tips[i];
                } else if (i > index) {
                    var[i - 1] = tips[i];
                }
            }

            controller.setProperty(parent, GraphicObjectProperties.__GO_DATATIPS__, var);
            //self destruction !
            controller.deleteObject(datatipUid);
        }
    }

    /**
    * Delete a specific datatip using the command "datatipRemove(polyline_handle,ind)";
    *
    * @param polylineUid Polyline unique identifier.
    * @param indexRemove Index of the datatip to be removed.
    */
    public static void datatipRemoveProgramIndex(int polylineUid, int indexRemove) {
        Integer[] datatips = (new ObjectSearcher()).search(polylineUid, GraphicObjectProperties.__GO_DATATIP__, true);
        deleteDatatip(datatips[datatips.length - indexRemove]);
    }

    /**
    * Delete a specific datatip using the command "datatipRemove(datatip_handle)";
    *
    * @param datatipUid Datatip unique identifier.
    * @param figureUid Figure unique identifier.
    */
    public static void datatipRemoveProgramHandler(int datatipUid, int figureUid) {
        deleteDatatip(datatipUid);
    }
}
