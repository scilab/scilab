/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;


import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.gui.editor.ObjectSearcher;
import org.scilab.modules.graphic_objects.PolylineData;


/**
* Implements all polyline manipulation functions for the editor.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class PolylineHandler {

    static PolylineHandler instance = null;

    public PolylineHandler() {
    }

    public static PolylineHandler getInstance() {
        if (instance == null) {
            instance = new PolylineHandler();
        }
        return instance;
    }

    /**
    * Check if the polyline uses mark.
    *
    * @param uid Polyline unique identifier.
    * @return True if mark is enabled, false otherwise.
    */
    public Boolean isMarkEnabled(String uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_MODE__);
    }

    /**
    * Change polyline line/mark color.
    *
    * @param uid Polyline unique identifier.
    * @param newColor Color to be used.
    * @return Returns the old color of the polyline.
    */
    public Integer setColor(String uid, Integer newColor) {

        if (uid == null) {
            return 0;
        }
        Integer oldColor;
        Boolean markon = isMarkEnabled(uid);
        if (markon == true) {
            oldColor = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
            GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_MARK_FOREGROUND__, newColor );
        } else {
            oldColor = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_LINE_COLOR__);
            GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_LINE_COLOR__, newColor );
        }

        return oldColor;

    }

    /**
    * Duplicate the polyline and its data.
    *
    * @param uid Polyline unique identifier.
    * @return New duplicated polyline uid or null if it fails.
    */
    public String duplicate(String uid) {
        String dup = null;
        try {
            dup = GraphicController.getController().cloneObject(uid);
            GraphicController.getController().setGraphicObjectRelationship("", dup);
            dup = PolylineData.createPolylineData(uid, dup);
            if (dup == null) {
                delete(dup);
            }
        } catch (Exception e) {
            System.out.println("Exception at Duplicate!" + e);
        }
        return dup;

    }

    /**
    * Inserts the given polyline in the given axes.
    *
    * @param axes Axes unique identifier.
    * @param polyline Polyline unique identifier.
    */
    public void insert(String axes, String polyline) {
        GraphicController.getController().setGraphicObjectRelationship(axes, polyline);
        String newCompound = (String)GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_COMPOUND__));
        GraphicController.getController().setGraphicObjectRelationship(axes, newCompound);
        GraphicController.getController().setGraphicObjectRelationship(newCompound, polyline);
    }

    /**
    * Remove the relationship from the given polyline
    * from its parent.
    *
    * @param uid Polyline unique identifier.
    */
    public void cut(String uid) {
        GraphicController.getController().setGraphicObjectRelationship("", uid);
    }

    /**
    * Remove the relationship from the given polyline
    * from its parent and deletes it.
    *
    * @param uid Polyline unique identifier.
    */
    public void delete(String uid) {
        GraphicController.getController().removeRelationShipAndDelete(uid);
    }


    /**
    * Deletes all child entities from the given object.
    *
    * @param uid Object unique identifier.
    */
    public void deleteAll(String uid) {

        String type = (String)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        Integer childCount = (Integer)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN_COUNT__);
        String[] child = (String[])GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_CHILDREN__);
        if (type == GraphicObjectProperties.__GO_AXES__) {
            for (Integer i = 0; i < childCount; ++i) {
                String cType = (String)GraphicController.getController().getProperty(child[i], GraphicObjectProperties.__GO_TYPE__);
                if (cType != GraphicObjectProperties.__GO_LABEL__) {
                    delete(child[i]);
                } else {
                    String[] text = { "" };
                    GraphicController.getController().setProperty(child[i], GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
                }
            }
        } else {

            for (Integer i = 0; i < childCount; ++i) {
                deleteAll(child[i]);
            }
        }

    }

    /**
    * If the given object is a polyline change its state.
    * Else change the state of all child polylines from the given object.
    *
    * @param uid Polyline/Object unique identifier.
    * @param state True for visible, false for hidden.
    */
    public void visible(String uid, Boolean state) {

        String objType = (String)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_TYPE__);
        if (objType == GraphicObjectProperties.__GO_POLYLINE__) {
            GraphicController.getController().setProperty(uid, GraphicObjectProperties.__GO_VISIBLE__, state);
        } else {
            String[] polylines = (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_POLYLINE__);

            for (Integer i = 0; i < polylines.length; ++i) {
                GraphicController.getController().setProperty(polylines[i], GraphicObjectProperties.__GO_VISIBLE__, state);
            }
        }
    }

    /**
    * Retrieve if the polyline is visible.
    *
    * @param uid Polyline unique identifier.
    * @return True if is visible, false otherwise.
    */
    public Boolean isVisible(String uid) {
        return (Boolean)GraphicController.getController().getProperty(uid, GraphicObjectProperties.__GO_VISIBLE__);
    }

    private String[] searchCompound(String uid) {
        return (new ObjectSearcher()).search(uid, GraphicObjectProperties.__GO_COMPOUND__);
    }
}

