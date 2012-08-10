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
 * Delete a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDelete {

    /**
    * Delete a specific datatip;
    *
    * @param markerUid Datatip marker unique identifier.
    */
    public static void deleteDatatip (String markerUid) {

        String parentUid = (String) GraphicController.getController().getProperty(markerUid, GraphicObjectProperties.__GO_PARENT__);
        String[] childrenUid = (String[]) GraphicController.getController().getProperty(parentUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < childrenUid.length ; i++) {
            if (markerUid == childrenUid[i]) {
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[i]);
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[i + 1]);
            }
        }
    }

    public static void datatipRemoveProgramIndex (String polylineUid, int indexRemove) {

        int numDatatips = 0;
        String compoundUid = (String) GraphicController.getController().getProperty(polylineUid, GraphicObjectProperties.__GO_PARENT__);
        String axesUid = (String) GraphicController.getController().getProperty(compoundUid, GraphicObjectProperties.__GO_PARENT__);
        String[] childrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        for (int i = 0 ; i < childrenUid.length ; i++) {
            String objType = (String) GraphicController.getController().getProperty(childrenUid[i], GraphicObjectProperties.__GO_TYPE__);
            if (objType == "Text") {
                numDatatips++;
            }
        }
        numDatatips = (numDatatips / 2);
        if (indexRemove <= numDatatips & indexRemove > 0) {
            int indexDel = (2 * indexRemove) - 1;
            String datatipUid = childrenUid[indexDel];
            String markerUid = childrenUid[indexDel - 1];
            GraphicController.getController().removeRelationShipAndDelete(datatipUid);
            GraphicController.getController().removeRelationShipAndDelete(markerUid);
        }
    }

    public static void datatipRemoveProgramHandler (String datatipUid, String figureUid) {

        int posDelete = -1;
        String[] childrenUid = (String[]) GraphicController.getController().getProperty(figureUid, GraphicObjectProperties.__GO_CHILDREN__);
        String axesUid = childrenUid[0];
        childrenUid = (String[]) GraphicController.getController().getProperty(axesUid, GraphicObjectProperties.__GO_CHILDREN__);
        String objType = (String) GraphicController.getController().getProperty(datatipUid, GraphicObjectProperties.__GO_TYPE__);
        if (objType == "Text") {
            for (int i = 0 ; i < childrenUid.length ; i++) {
                if (datatipUid == childrenUid[i]) {
                    posDelete = i;
                }
            }
        }
        if (posDelete >= 0) {
            GraphicController.getController().removeRelationShipAndDelete(childrenUid[posDelete]);
            if (posDelete % 2 == 0) {
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[posDelete + 1]);
            } else {
                GraphicController.getController().removeRelationShipAndDelete(childrenUid[posDelete - 1]);
            }
        }
    }
}
