/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uibar.progressionbar;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PROGRESSIONBAR__;

import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.uibar.Uibar;

/**
 * @author Vincent COUVERT
 */
public class Progressionbar extends Uibar {

    /**
     * Constructor
     */
    public Progressionbar() {
        super();
    }

    /**
     * Get this object type
     * @return waitbar
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public Integer getType() {
        return __GO_PROGRESSIONBAR__;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {
    }
}
