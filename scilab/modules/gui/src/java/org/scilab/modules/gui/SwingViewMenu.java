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

package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LABEL__;

import java.awt.Color;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.ScilabCallBack;
import org.scilab.modules.gui.widget.Widget;

/**
 * @author Vincent COUVERT
 */
public final class SwingViewMenu {

    private static final int COLORS_COEFF = 255;

    /**
     * Constructor
     */
    private SwingViewMenu() {
        throw new UnsupportedOperationException();
    }

    /**
     * Update the component in the view
     * @param uimenu the component
     * @param property the property name
     * @param value the property value
     */
    public static void update(Widget uimenu, String property, Object value) {
        String uid = ((SwingViewObject) uimenu).getId();
        if (property.equals(__GO_CALLBACK__)) {
            int cbType = (Integer) GraphicController.getController().getProperty(uid, __GO_CALLBACKTYPE__);
            uimenu.setCallback(ScilabCallBack.createCallback((String) value, cbType));
        } else if (property.equals(__GO_CALLBACKTYPE__)) {
            String cbString = (String) GraphicController.getController().getProperty(uid, __GO_CALLBACK__);
            uimenu.setCallback(ScilabCallBack.createCallback(cbString, (Integer) value));
        } else if (property.equals(__GO_UI_CHECKED__)) {
            if (uimenu instanceof SwingScilabCheckBoxMenuItem) { 
                ((SwingScilabCheckBoxMenuItem) uimenu).setChecked((Boolean) value);
            }
        } else if (property.equals(__GO_UI_ENABLE__)) {
            uimenu.setEnabled((Boolean) value);
        } else if (property.equals(__GO_UI_FOREGROUNDCOLOR__)) {
            Double[] allColors = ((Double[]) value);
            uimenu.setForeground(new Color((int) (allColors[0] * COLORS_COEFF),
                    (int) (allColors[1] * COLORS_COEFF),
                    (int) (allColors[2] * COLORS_COEFF)));
        } else if (property.equals(__GO_UI_LABEL__)) {
            uimenu.setText((String) value);
        }
    }
}

