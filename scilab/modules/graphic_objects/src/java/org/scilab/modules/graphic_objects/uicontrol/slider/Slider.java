/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.uicontrol.slider;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Vincent COUVERT
 */
public class Slider extends Uicontrol {

    /**
     * Constructor
     */
    public Slider() {
        super();
        setStyle(__GO_UI_SLIDER__);

        // Default value is the min value
        Double[] value = new Double[1];
        value[0] = getMin();
        setUiValue(value);

        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(RELIEF_FLAT);
        } else {
            UIDefaults defaults = UIManager.getDefaults();

            //font
            setFont(defaults.getFont("Slider.font"));

            //h-alignment
            setHorizontalAlignment("left");

            //v-alignement
            setVerticalAlignment("middle");
        }
    }
}
