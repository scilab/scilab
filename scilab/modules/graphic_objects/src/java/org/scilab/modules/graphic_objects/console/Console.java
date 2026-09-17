/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2021 - Stéphane MOTTELET
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

package org.scilab.modules.graphic_objects.console;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CONSOLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TOOLBAR_VISIBLE__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * @author Vincent COUVERT
 */
public final class Console extends GraphicObject {

    /**
     * Scilab mode enum
     * Java Console created only in STD mode, API mode checking is not needed
     */

    private static Console me;

    private boolean toolbarVisible = false;

    /** Console properties names */
    private enum ConsoleProperty {
        TOOLBARVISIBLE
    };

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
    public Integer getType() {
        return __GO_CONSOLE__;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {

    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        if (propertyName == __GO_TOOLBAR_VISIBLE__) {
            return ConsoleProperty.TOOLBARVISIBLE;
        } else {
            return super.getPropertyFromName(propertyName);
        }
    }
    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        if (property == ConsoleProperty.TOOLBARVISIBLE) {
            return getToolbarVisible();
        } else {
            return super.getProperty(property);
        }
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == ConsoleProperty.TOOLBARVISIBLE) {
            setToolbarVisible((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }
        return UpdateStatus.Success;
    }

    public Boolean getToolbarVisible() {
        return toolbarVisible;
    }

    public UpdateStatus setToolbarVisible(Boolean status) {
        if (status.equals(toolbarVisible)) {
            return UpdateStatus.NoChange;
        }

        toolbarVisible = status;
        return UpdateStatus.Success;
    }
}
