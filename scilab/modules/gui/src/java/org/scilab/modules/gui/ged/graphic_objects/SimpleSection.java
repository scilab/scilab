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
* Interface for sections of property list (GED).
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public interface SimpleSection {

    /**
    * Insert show/hide button, title and main JPanel of group.
    */
    void insertBase();

    /**
    * It has all the components of the section Base Properties.
    */
    void components();

    /**
    * Loads the current properties of section.
    * @param objectID Enters the identification of polyline.
    */
    void values(String objectID);
}
