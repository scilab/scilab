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
import org.scilab.modules.graphic_objects.graphicObject.*;

import java.util.ArrayList;

/**
 * Delete a datatip
 * @author Gustavo Barbosa Libotte
 */
public class DatatipDelete {

    /**
    * Delete a datatip of a specific index
    *
    * @param index Position to delete de datatip.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    */
    public static void deleteDatatip (Integer index, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {
    
        String datatipUidToDelete = datatipsUid.get(index/2);
        GraphicController.getController().removeRelationShipAndDelete(datatipUidToDelete);
        String markerUidToDelete = markersUid.get(index/2);
        GraphicController.getController().removeRelationShipAndDelete(markerUidToDelete);
    }

}
