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

package org.scilab.modules.graphic_objects.uimenu;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ACCELERATOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LABEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MNEMONIC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SEPARATOR__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.Visitor;

/**
 * @author Vincent COUVERT
 */
public class Uimenu extends GraphicObject {

    private boolean checked;
    private boolean enable = true;
    private Double[] foregroundColor = {0.0, 0.0, 0.0};
    private String icon = "";
    private String label = "";
    private String mnemonic = "";
    private String accelerator = "";
    private boolean separator;

    /**
     * All uimenu properties
     */
    private enum UimenuProperty {
        ACCELERATOR,
        CHECKED,
        ENABLE,
        FOREGROUNDCOLOR,
        ICON,
        LABEL,
        MNEMONIC,
        SEPARATOR
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
    public Integer getType() {
        return __GO_UIMENU__;
    }

    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(int propertyName) {
        switch (propertyName) {
            case __GO_UI_ACCELERATOR__ :
                return UimenuProperty.ACCELERATOR;
            case __GO_UI_CHECKED__ :
                return UimenuProperty.CHECKED;
            case __GO_UI_ENABLE__ :
                return UimenuProperty.ENABLE;
            case __GO_UI_FOREGROUNDCOLOR__ :
                return UimenuProperty.FOREGROUNDCOLOR;
            case __GO_UI_ICON__ :
                return UimenuProperty.ICON;
            case __GO_UI_LABEL__ :
                return UimenuProperty.LABEL;
            case __GO_UI_MNEMONIC__ :
                return UimenuProperty.MNEMONIC;
            case __GO_UI_SEPARATOR__ :
                return UimenuProperty.SEPARATOR;
            default :
                return super.getPropertyFromName(propertyName);
        }
    }

    /**
     * Fast property get method
     * @param property the property to get
     * @return the property value
     */
    public Object getProperty(Object property) {
        Object propertyValue = null;
        if (property == UimenuProperty.ACCELERATOR) {
            propertyValue = getAccelerator();
        } else if (property == UimenuProperty.CHECKED) {
            propertyValue = getChecked();
        } else if (property == UimenuProperty.ENABLE) {
            propertyValue = getEnable();
        } else if (property == UimenuProperty.FOREGROUNDCOLOR) {
            propertyValue = getForegroundColor();
        } else if (property == UimenuProperty.ICON) {
            propertyValue = getIcon();
        } else if (property == UimenuProperty.LABEL) {
            propertyValue = getLabel();
        } else if (property == UimenuProperty.MNEMONIC) {
            propertyValue = getMnemonic();
        } else if (property == UimenuProperty.SEPARATOR) {
            propertyValue = getSeparator();
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
        if (property == UimenuProperty.ACCELERATOR) {
            setAccelerator((String) value);
        } else if (property == UimenuProperty.CHECKED) {
            setChecked((Boolean) value);
        } else if (property == UimenuProperty.ENABLE) {
            setEnable((Boolean) value);
        } else if (property == UimenuProperty.FOREGROUNDCOLOR) {
            setForegroundColor((Double[]) value);
        } else if (property == UimenuProperty.ICON) {
            setIcon((String) value);
        } else if (property == UimenuProperty.LABEL) {
            setLabel((String) value);
        } else if (property == UimenuProperty.MNEMONIC) {
            setMnemonic((String) value);
        } else if (property == UimenuProperty.SEPARATOR) {
            setSeparator((Boolean) value);
        } else {
            return super.setProperty(property, value);
        }

        return UpdateStatus.Success;
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
    public UpdateStatus setChecked(Boolean checked) {
        this.checked = checked;
        return UpdateStatus.Success;
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
    public UpdateStatus setEnable(Boolean enable) {
        this.enable = enable;
        return UpdateStatus.Success;
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
    public UpdateStatus setForegroundColor(Double[] colors) {
        this.foregroundColor = colors;
        return UpdateStatus.Success;
    }

    /**
     * Get the menu icon
     * @return the icon
     */
    public String getIcon() {
        return this.icon;
    }

    /**
     * Set the menu icon
     * @param icon the icon
     */
    public UpdateStatus setIcon(String icon) {
        this.icon = icon;
        return UpdateStatus.Success;
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
    public UpdateStatus setLabel(String label) {
        this.label = label;
        return UpdateStatus.Success;
    }

    /**
     * Get the menu mnemonic
     * @return the mnemonic
     */
    public String getMnemonic() {
        return this.mnemonic;
    }

    /**
     * Set the menu mnemonic
     * @param mnemonic the mnemonic
     */
    public UpdateStatus setMnemonic(String mnemonic) {
        this.mnemonic = mnemonic;
        return UpdateStatus.Success;
    }

    /**
     * Get the menu accelerator
     * @return the accelerator
     */
    public String getAccelerator() {
        return this.accelerator;
    }

    /**
     * Set the menu accelerator
     * @param accelerator the accelerator
     */
    public UpdateStatus setAccelerator(String accelerator) {
        this.accelerator = accelerator;
        return UpdateStatus.Success;
    }

    /**
     * Get the menu separator mode
     * @return the separator mode
     */
    public Boolean getSeparator() {
        return this.separator;
    }

    /**
     * Set the menu separator mode
     * @param separator the separator mode
     */
    public UpdateStatus setSeparator(Boolean separator) {
        this.separator = separator;
        return UpdateStatus.Success;
    }

    @Override
    public Uimenu clone() {
        Uimenu copy = (Uimenu) super.clone();
        copy.setValid(true);
        return copy;
    }

    /**
     * Accept method
     * @param visitor the visitor
     * @see org.scilab.modules.graphic_objects.graphicObject.GraphicObject#accept(org.scilab.modules.graphic_objects.graphicObject.Visitor)
     */
    public void accept(Visitor visitor) {

    }

}
