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
    * Delete a datatip of a specific index
    *
    * @param index Position to delete de datatip.
    * @param datatipsUid Arraylist containing all created datatip's unique identifier.
    * @param markersUid Arraylist containing all created merker's unique identifier.
    */
    public static void deleteDatatip (Integer index, ArrayList<String> datatipsUid, ArrayList<String> markersUid) {

        String datatipUidToDelete = datatipsUid.get(index / 2);
        GraphicController.getController().removeRelationShipAndDelete(datatipUidToDelete);
        String markerUidToDelete = markersUid.get(index / 2);
        GraphicController.getController().removeRelationShipAndDelete(markerUidToDelete);
    }

    /**
    * Remove the coordinates of deleted datatip
    *
    * @param index Position to remove the coordinates from ArrayList
    * @param datatipsCoord ArrayList containing all created datatips coordinates
    * @return ArrayList containing all created datatips coordinates updated
    */
    public static ArrayList<Double> romoveDatatipCoords (Integer index, ArrayList<Double> datatipsCoord) {

        datatipsCoord.remove(index + 1);
        datatipsCoord.remove(index + 1 - 1);
        return datatipsCoord;
    }

    /**
    * Remove the unique identifier of deleted datatip
    *
    * @param index Position to remove
    * @param datatipsUid ArrayList containing all created datatips unique identifiers
    * @return ArrayList containing all created datatips unique identifiers updated
    */
    public static ArrayList<String> romoveDatatipUid (Integer index, ArrayList<String> datatipsUid) {

        datatipsUid.remove(index / 2);
        return datatipsUid;
    }

    /**
    * Remove the unique identifier of deleted datatip's marker
    *
    * @param index Position to remove
    * @param markersUid ArrayList containing all created merkers unique identifiers
    * @return ArrayList containing all created merkers unique identifiers updated
    */
    public static ArrayList<String> romoveMarkerUid (Integer index, ArrayList<String> markersUid) {

        markersUid.remove(index / 2);
        return markersUid;
    }

    /**
    * Remove the unique identifier of polyline on which datatip were created
    *
    * @param index Position to remove
    * @param polylinesUid ArrayList containing all polylines unique identifiers
    * @return ArrayList containing all polylines unique identifiers updated
    */
    public static ArrayList<String> romovePolylineUid (Integer index, ArrayList<String> polylinesUid) {

        polylinesUid.remove(index / 2);
        return polylinesUid;
    }
}
