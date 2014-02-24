/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol.layer;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB_VALUE__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.utils.LayoutType;

/**
 * @author Vincent COUVERT
 */
public class Layer extends Uicontrol {
    private enum LayerProperty {
        LAYER_VALUE,
        LAYER_STRING
    };


    /**
     * Constructor
     */
    public Layer() {
        super();
        setStyle(__GO_UI_LAYER__);
        UIDefaults defaults = UIManager.getDefaults();

        //font
        setFont(defaults.getFont("Panel.font"));

        //h-alignment
        setHorizontalAlignment("left");

        //v-alignement
        setVerticalAlignment("middle");

        //layout
        setLayout(LayoutType.BORDER);
    }

    public Object  getPropertyFromName(int property) {
        if (property == __GO_UI_TAB_VALUE__) {
            return LayerProperty.LAYER_VALUE;
        } else if (property == __GO_UI_TAB_STRING__) {
            return LayerProperty.LAYER_STRING;
        } else {
            return super.getPropertyFromName(property);
        }
    }

    public UpdateStatus setProperty(Object property, Object value) {
        if (!(property instanceof LayerProperty)) {
            return super.setProperty(property, value);
        }

        LayerProperty p = (LayerProperty) property;
        switch (p) {
            case LAYER_STRING:
                return setUiTabString((String[])value);
            case LAYER_VALUE:
                return setUiTabValue((Double[])value);
            default:
                return super.setProperty(property, value);
        }
    }

    public UpdateStatus setUiTabString(String[] value) {
        setString(value);
        return UpdateStatus.NoChange;
    }

    public UpdateStatus setUiTabValue(Double[] value) {
        setUiValue(value);
        return UpdateStatus.NoChange;
    }

}
