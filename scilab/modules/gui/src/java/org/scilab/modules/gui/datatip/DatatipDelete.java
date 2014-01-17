/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

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
        //convert [] in list
        List<Integer> l = new LinkedList<Integer>(Arrays.asList(tips));
        //remove me
        l.remove((Object) datatipUid);
        //convert list in []
        Integer[] var = new Integer[l.size()];
        l.toArray(var);
        //update parent datatips without me
        controller.setProperty(parent, GraphicObjectProperties.__GO_DATATIPS__, var);
        //self destruction !
        controller.deleteObject(datatipUid);
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
