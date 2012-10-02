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
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SHOWHIDDENHANDLES__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * @author Vincent COUVERT
 */
public final class Console extends GraphicObject {

    /**
     * Scilab mode enum
     * Java Console created only in STD mode
     */
    public enum ScilabMode { STD, NW };

    private static Console me;

    private boolean showHiddenHandles;

    private ScilabMode scilabMode;

    /** Console properties names */
    private enum ConsoleProperty {
        SHOWHIDDENHANDLES
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
     * Set the scilabMode property
     * @param scilabMode the new value to set
     */
    public void setScilabMode(ScilabMode scilabMode) {
        this.scilabMode = scilabMode;
    }

    /**
     * Get the javaEnabled proeprty
     * @return javaEnabled
     */
    public ScilabMode getScilabMode() {
        return this.scilabMode;
    }

    /**
     * Set the showHiddenHandles proeprty
     * @param showHiddenHandles the new value to set
     */
    public void setShowHiddenHandles(boolean showHiddenHandles) {
        this.showHiddenHandles = showHiddenHandles;
    }

    /**
     * Get the showHiddenHandles proeprty
     * @return showHiddenHandles
     */
    public boolean getShowHiddenHandles() {
        return this.showHiddenHandles;
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
        if (propertyName == __GO_SHOWHIDDENHANDLES__) {
            return ConsoleProperty.SHOWHIDDENHANDLES;
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
        if (property == ConsoleProperty.SHOWHIDDENHANDLES) {
            return getShowHiddenHandles();
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
        if (property == ConsoleProperty.SHOWHIDDENHANDLES) {
            setShowHiddenHandles((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }
        return UpdateStatus.Success;
    }

}
