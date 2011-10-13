/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.console;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CONSOLE__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

/**
 * @author Vincent COUVERT
 */
public final class Console extends GraphicObject {

    private static Console me;

    /**
     * Constructor
     */
    private Console() {
        super();
    }

    /**
     * Returns the console object
     * @return the console object
     */
    public static Console getConsole() {
        if (me == null) {
            me = new Console();
        }

        return me;
        
    }

    /**
     * Get this object type
     * @return "Console"
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public String getType() {
        return __GO_CONSOLE__;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.IVisitor)
     */
    public void accept(IVisitor visitor) {

    }

}
