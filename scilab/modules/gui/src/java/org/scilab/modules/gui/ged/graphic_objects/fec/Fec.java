/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.fec;

import org.scilab.modules.gui.ged.graphic_objects.SimpleObject;

/**
* Properties of the fec.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Fec extends SimpleObject {
    private DataProperties data;
    private Style style;

    /**
    * Initializes all sections (JPanel's) and Add in Main JPanel of Object.
    * @param objectID Enters the identification of object.
    */
    public Fec(Integer objectID) {
        super(objectID);
        data = new DataProperties(objectID);
        style = new Style(objectID);
        addSectionPanel(this, data, getPosition(data.getName()));
        addSectionPanel(this, style, getPosition(style.getName()));
        this.fillerV(getSectionsName());
    }

    /**
    * Get the name of all sections.
    * @return sections name
    */
    @Override
    public final String[] getSectionsName() {
        return new String[] {data.getName(), style.getName()};
    }
}