/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_NB_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUND_COLOR__;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;

public class Uicontrol extends GraphicObject {

    
    private enum UicontrolProperty {
        STYLE,
        BACKGROUNDCOLOR,
        POSITION,
        STRING,
        NBSTRING
    };

    private enum UicontrolStyle {
        PUSHBUTTON
    };
    
    private String styleEnumToString(UicontrolStyle style) {
        if(style == UicontrolStyle.PUSHBUTTON) {
            return __GO_UI_PUSHBUTTON__;
        }
        return null;
    }
    
    private UicontrolStyle stringToStyleEnum(String style) {
        if (style.equals(__GO_UI_PUSHBUTTON__)) {
            return UicontrolStyle.PUSHBUTTON;
        }
        return null;
    }
    
    private UicontrolStyle style;
    private Double[] backgroundColor = {0.0, 0.0, 0.0};
    private Double[] position = {0.0, 254.0, 0.0, 0.0};
    private String[] string = {""};
    
    public Uicontrol() {
        super();
    }
    
    public String getType() {
        return __GO_UICONTROL__;
    }
    
    /**
     * Returns the enum associated to a property name
     * @param propertyName the property name
     * @return the property enum
     */
    public Object getPropertyFromName(String propertyName) {
        if (propertyName.equals(__GO_STYLE__)) {
            return UicontrolProperty.STYLE;
        } else if (propertyName.equals(__GO_UI_BACKGROUND_COLOR__)) {
            return UicontrolProperty.BACKGROUNDCOLOR;
        } else if (propertyName.equals(__GO_POSITION__)) {
            return UicontrolProperty.POSITION;
        } else if (propertyName.equals(__GO_UI_STRING__)) {
            return UicontrolProperty.STRING;
        } else if (propertyName.equals(__GO_UI_NB_STRING__)) {
            return UicontrolProperty.NBSTRING;
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
        if (property == UicontrolProperty.STYLE) {
            return getStyle();
        } else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
            return getBackgroundColor();
        } else if (property == UicontrolProperty.POSITION) {
            return getPosition2();
        } else if (property == UicontrolProperty.STRING) {
            return getString();
        } else if (property == UicontrolProperty.NBSTRING) {
            return getString().length;
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
    public boolean setProperty(Object property, Object value) {
        if (property == UicontrolProperty.STYLE) {
            setStyle((String) value);
        } else if (property == UicontrolProperty.BACKGROUNDCOLOR) {
            setBackgroundColor((Double[]) value);
        } else if (property == UicontrolProperty.POSITION) {
            setPosition2((Double[]) value);
        } else if (property == UicontrolProperty.STRING) {
            setString((String[]) value);
        } else {
            return super.setProperty(property, value);
        }

        return true;
    }
    
    /* Style */
    public String getStyle() {
        return styleEnumToString(this.style);
    }

    public void setStyle(String style) {
        this.style = stringToStyleEnum(style);
    }
    
    /* Background Color */
    public Double[] getBackgroundColor() {
        return this.backgroundColor;
    }
    
    public void setBackgroundColor(Double[] colors) {
        this.backgroundColor = colors;
    }
    
    /* Position */
    public Double[] getPosition2() {
        for (int i = 0 ; i < 4 ; ++i) {
            System.err.println("[DEBUG] getPosition2 " + position[i]);
        }
        return this.position;
    }
    
    public void setPosition2(Double[] position) {
        this.position = position;
    }
    
    /* String */
    public String[] getString() {
        return this.string;
    }
    
    public void setString(String[] string) {
        this.string = string;
    }
    
    public void accept(IVisitor visitor) {
      
    }

}
