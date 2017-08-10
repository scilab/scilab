/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos Cardinot
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
package org.scilab.modules.gui.ged.graphic_objects.rectangle;

import org.scilab.modules.gui.ged.graphic_objects.SimpleObject;

/**
* Properties of the rectangle.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Rectangle extends SimpleObject {
    private DataProperties data;
    private Mark mark;
    private Style style;

    /**
    * Initializes all sections (JPanel's) and Add in Main JPanel of Object.
    * @param objectID Enters the identification of object.
    */
    public Rectangle(Integer objectID) {
        super(objectID);
        data = new DataProperties(objectID);
        mark = new Mark(objectID);
        style = new Style(objectID);
        addSectionPanel(this, data, getPosition(data.getName()));
        addSectionPanel(this, mark, getPosition(mark.getName()));
        addSectionPanel(this, style, getPosition(style.getName()));
        this.fillerV(getSectionsName());
    }

    /**
    * Get the name of all sections.
    * @return sections name
    */
    @Override
    public final String[] getSectionsName() {
        return new String[] {data.getName(), mark.getName(), style.getName()};
    }
}