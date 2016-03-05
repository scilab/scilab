/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged.graphic_objects;

import java.awt.Dimension;
import java.util.Arrays;
import javax.swing.Box;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

/**
* Interface for objects of property list (GED).
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public abstract class SimpleObject extends ContentLayout {
    private Integer objectID;

    /**
    * Constructor
    * @param objectID uid.
    */
    public SimpleObject(Integer objectID) {
        this.objectID = objectID;
    }

    /**
    * Get the name of all sections.
    * @return sections name
    */
    public abstract String[] getSectionsName();

    /**
    * Get the position of section for current language (alphabetic order).
    * @param section Name of section.
    * @return position
    */
    public final int getPosition(String section) {
        String [] sectionsName = getSectionsName();
        Arrays.sort(sectionsName);
        return Arrays.binarySearch(sectionsName, section);
    }

    /**
     * Add a vertical filler - makes the jpanels are always on top.
     * @param sectionsName name of sections
     */
    public void fillerV(String[] sectionsName) {
        Box.Filler filler = new Box.Filler(new Dimension(1, 1), new Dimension(1, 1), new Dimension(1, 32767));
        this.addFiller(this, filler, sectionsName.length);
    }

    /**
    * Set the Object ID.
    * @param objectID Enters the identification of object.
    */
    public final void setObjectID(Integer objectID) {
        this.objectID = objectID;
    }

    /**
    * Get the Object ID.
    * @return Object ID
    */
    public final Integer getObjectID() {
        return objectID;
    }

    /**
    * Get Parent Figure (ID).
    * @param objectID Enters the identification of object.
    * @return FigreID
    */
    public final Integer getFigueID(Integer objectID) {
        return (Integer) GraphicController.getController()
               .getProperty(objectID, GraphicObjectProperties.__GO_PARENT_FIGURE__);
    }
}