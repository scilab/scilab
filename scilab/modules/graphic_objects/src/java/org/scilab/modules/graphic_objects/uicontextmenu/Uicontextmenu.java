/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.graphic_objects.uicontextmenu;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTEXTMENU__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * @author Vincent COUVERT
 */
public class Uicontextmenu extends GraphicObject {

    /**
     * Constructor
     */
    public Uicontextmenu() {
        super();
    }

    /**
     * Get this object type
     * @return uicontextmenu
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public Integer getType() {
        return __GO_UICONTEXTMENU__;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {
    }

}
