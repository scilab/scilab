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
package org.scilab.modules.gui.ged.graphic_objects.label;

import org.scilab.modules.gui.ged.graphic_objects.SimpleObject;

/**
* Properties of the label.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Label extends SimpleObject {
    private BaseProperties base;
    private Font font;
    private Position position;
    private Style style;

    /**
    * Initializes all sections (JPanel's) and Add in Main JPanel of Object.
    * @param objectID Enters the identification of object.
    */
    public Label(Integer objectID) {
        super(objectID);
        base = new BaseProperties(objectID);
        font = new Font(objectID);
        position = new Position(objectID);
        style = new Style(objectID);
        addSectionPanel(this, base, getPosition(base.getName()));
        addSectionPanel(this, font, getPosition(font.getName()));
        addSectionPanel(this, position, getPosition(position.getName()));
        addSectionPanel(this, style, getPosition(style.getName()));
        this.fillerV(getSectionsName());
    }

    /**
    * Get the name of all sections.
    * @return sections name
    */
    @Override
    public final String[] getSectionsName() {
        return new String[] {
                   base.getName(),
                   font.getName(),
                   position.getName(),
                   style.getName()
               };
    }
}