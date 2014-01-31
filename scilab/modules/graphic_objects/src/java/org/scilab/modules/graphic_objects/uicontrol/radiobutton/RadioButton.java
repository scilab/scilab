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

package org.scilab.modules.graphic_objects.uicontrol.radiobutton;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Bruno JOFRET
 */
public class RadioButton extends Uicontrol {

    /**
     * Constructor
     */
    public RadioButton() {
        super();
        setStyle(__GO_UI_RADIOBUTTON__);

        // Default value is the min value (not checked)
        Double[] value = new Double[1];
        value[0] = getMin();
        setUiValue(value);

        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(FLAT_RELIEF);
            setBackgroundColor(new Double[] {
                                   DEFAULT_RED_BACKGROUND,
                                   DEFAULT_GREEN_BACKGROUND,
                                   DEFAULT_BLUE_BACKGROUND
                               });

            setHorizontalAlignment("center");
            setVerticalAlignment("middle");

            setFontName(DEFAULTFONTNAME);
            setFontSize(DEFAULTFONTSIZE);
            setFontAngle(DEFAULTFONTANGLE);
            setFontWeight(DEFAULTFONTWEIGHT);
        }
    }
}
