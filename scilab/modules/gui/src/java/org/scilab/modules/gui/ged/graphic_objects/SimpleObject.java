/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects;

/**
* Interface for objects of property list (GED).
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public interface SimpleObject {

    /**
    * Initializes all sections (JPanel's).
    * @param objectID Enters the identification of object.
    */
    void initSections(String objectID);

    /**
    * Get the position of section for current language (alphabetic order).
    * @param section Name of section.
    * @return position
    */
    int getPosition(String section);

    /**
    * Set the Object ID.
    * @param objectID Enters the identification of object.
    */
    void setObjectID(String objectID);

    /**
    * Get the Object ID.
    * @return Object ID
    */
    String getObjectID();
}