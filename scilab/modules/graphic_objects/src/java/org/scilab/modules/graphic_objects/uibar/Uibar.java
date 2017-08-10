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

package org.scilab.modules.graphic_objects.uibar;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MESSAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MESSAGE_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * @author Vincent COUVERT
 */
public abstract class Uibar extends GraphicObject {

    private String[] message = {""};
    private int value;

    /**
     * All uibar properties
     */
    private enum UibarProperty {
        MESSAGE,
        MESSAGE_SIZE,
        VALUE
    };

    /**
     * Constructor
     */
    public Uibar() {
        super();
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_UI_MESSAGE__ :
                return UibarProperty.MESSAGE;
            case __GO_UI_MESSAGE_SIZE__ :
                return UibarProperty.MESSAGE_SIZE;
            case __GO_UI_VALUE__ :
                return UibarProperty.VALUE;
            default :
                return  super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        Object propertyValue = null;
        if (property == UibarProperty.MESSAGE) {
            propertyValue = getMessage();
        } else if (property == UibarProperty.MESSAGE_SIZE) {
            propertyValue = getMessage().length;
        } else if (property == UibarProperty.VALUE) {
            propertyValue = getValue();
        } else {
            propertyValue = super.getProperty(property);
        }
        return propertyValue;
    }

    /**
     * Fast property set method
     * @param property the property to set
     * @param value the property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Object property, Object value) {
        if (property == UibarProperty.MESSAGE) {
            setMessage((String[]) value);
        } else if (property == UibarProperty.VALUE) {
            setValue((Integer) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
    }

    /**
     * Get the uibar message
     * @return the message
     */
    public String[] getMessage() {
        return this.message;
    }

    /**
     * Set the uibar message
     * @param message the message
     */
    public UpdateStatus setMessage(String[] message) {
        this.message = message;
        return UpdateStatus.Success;
    }

    /**
     * Get the uibar value
     * @return the value
     */
    public Integer getValue() {
        return this.value;
    }

    /**
     * Set the uibar value
     * @param value the value
     */
    public UpdateStatus setValue(Integer value) {
        this.value = value;
        return UpdateStatus.Success;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {
    }
}
