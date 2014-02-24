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

package org.scilab.modules.graphic_objects.uicontrol.tab;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import org.scilab.modules.graphic_objects.utils.LayoutType;

/**
 * @author Vincent COUVERT
 */
public class Tab extends Uicontrol {
    private enum TabProperty {
        TAB_VALUE,
        TAB_STRING
    };

    /**
     * Constructor
     */
    public Tab() {
        super();
        setStyle(__GO_UI_TAB__);
        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(RELIEF_FLAT);
        } else {
            UIDefaults defaults = UIManager.getDefaults();

            //font
            setFont(defaults.getFont("TabbedPane.font"));

            //h-alignment
            setHorizontalAlignment("left");

            //v-alignement
            setVerticalAlignment("middle");

            //layout
            setLayout(LayoutType.BORDER);
        }
    }

    public Object  getPropertyFromName(int property) {
        if (property == __GO_UI_TAB_VALUE__) {
            return TabProperty.TAB_VALUE;
        } else if (property == __GO_UI_TAB_STRING__) {
            return TabProperty.TAB_STRING;
        } else {
            return super.getPropertyFromName(property);
        }
    }

    public UpdateStatus setProperty(Object property, Object value) {
        if (!(property instanceof TabProperty)) {
            return super.setProperty(property, value);
        }

        TabProperty p = (TabProperty) property;
        switch (p) {
            case TAB_STRING:
                return setUiTabString((String[])value);
            case TAB_VALUE:
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
