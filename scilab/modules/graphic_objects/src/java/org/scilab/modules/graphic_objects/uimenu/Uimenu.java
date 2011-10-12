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

package org.scilab.modules.graphic_objects.uimenu;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LABEL__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

/**
 * @author Vincent COUVERT
 */
public class Uimenu extends GraphicObject {

    private boolean checked;
    private boolean enable = true;
    private Double[] foregroundColor = {0.0, 0.0, 0.0};
    private String label = "";

    /**
     * All uimenu properties
     */
    private enum UimenuProperty {
        CHECKED,
        ENABLE,
        FOREGROUNDCOLOR,
        LABEL
    };

    /**
     * Constructor
     */
    public Uimenu() {
        super();
        setCallbackString("");
        setCallbackType(0);
    }

    /**
     * Get this object type
     * @return uicontrol
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#getType()
     */
    public String getType() {
        return __GO_UIMENU__;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        Object property = null;
        if (propertyName.equals(__GO_UI_CHECKED__)) {
            property = UimenuProperty.CHECKED;
        } else if (propertyName.equals(__GO_UI_ENABLE__)) {
            property = UimenuProperty.ENABLE;
       } else if (propertyName.equals(__GO_UI_FOREGROUNDCOLOR__)) {
           property = UimenuProperty.FOREGROUNDCOLOR;
       } else if (propertyName.equals(__GO_UI_LABEL__)) {
           property = UimenuProperty.LABEL;
        } else {
            property = super.getPropertyFromName(propertyName);
        }
        return property;
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        Object propertyValue = null;
        if (property == UimenuProperty.CHECKED) {
            propertyValue = getChecked();
        } else if (property == UimenuProperty.ENABLE) {
            propertyValue = getEnable();
        } else if (property == UimenuProperty.FOREGROUNDCOLOR) {
            propertyValue = getForegroundColor();
        } else if (property == UimenuProperty.LABEL) {
            propertyValue = getLabel();
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
    public boolean setProperty(Object property, Object value) {
        if (property == UimenuProperty.CHECKED) {
            setChecked((Boolean) value);
        } else if (property == UimenuProperty.ENABLE) {
            setEnable((Boolean) value);
        } else if (property == UimenuProperty.FOREGROUNDCOLOR) {
            setForegroundColor((Double[]) value);
        } else if (property == UimenuProperty.LABEL) {
            setLabel((String) value);
        } else {
            return super.setProperty(property, value);
        }

        return true;
    }

    /**
     * Get the menu checked status
     * @return the status 
     */
    public Boolean getChecked() {
        return this.checked;
    }

    /**
     * Set the menu checked status
     * @param checked the status
     */
    public void setChecked(Boolean checked) {
        this.checked = checked;
    }

    /**
     * Get the menu enable status
     * @return the status 
     */
    public Boolean getEnable() {
        return this.enable;
    }

    /**
     * Set the menu enable status
     * @param enable the status
     */
    public void setEnable(Boolean enable) {
        this.enable = enable;
    }

    /**
     * Get the menu Foreground Color
     * @return the RGB color 
     */
    public Double[] getForegroundColor() {
        return this.foregroundColor;
    }

    /**
     * Set the menu Foreground Color
     * @param colors the RGB color 
     */
    public void setForegroundColor(Double[] colors) {
        this.foregroundColor = colors;
    }

    /**
     * Get the menu label
     * @return the label 
     */
    public String getLabel() {
        return this.label;
    }

    /**
     * Set the menu label
     * @param label the label
     */
    public void setLabel(String label) {
        this.label = label;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.IVisitor)
     */
    public void accept(IVisitor visitor) {

    }

}
