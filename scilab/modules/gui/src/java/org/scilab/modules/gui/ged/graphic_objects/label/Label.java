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