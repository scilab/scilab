/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.graphic_objects.uicontrol.pushbutton;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Bruno JOFRET
 */
public class PushButton extends Uicontrol {

    private static final double BUTTON_RED_BACKGROUND = 0.6;
    private static final double BUTTON_GREEN_BACKGROUND = 0.6;
    private static final double BUTTON_BLUE_BACKGROUND = 0.6;

    /**
     * Constructor
     */
    public PushButton() {
        super();
        setStyle(__GO_UI_PUSHBUTTON__);


        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(RELIEF_RAISED);
            setBackgroundColor(new Double[] {
                                   BUTTON_RED_BACKGROUND,
                                   BUTTON_GREEN_BACKGROUND,
                                   BUTTON_BLUE_BACKGROUND
                               });
        } else {
            UIDefaults defaults = UIManager.getDefaults();

            //font
            setFont(defaults.getFont("Button.font"));

            //h-alignment
            setHorizontalAlignment("center");

            //v-alignement
            setVerticalAlignment("middle");
        }
    }

}
