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

package org.scilab.modules.graphic_objects.uicontrol.edit;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;

import javax.swing.UIDefaults;
import javax.swing.UIManager;

import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;

/**
 * @author Vincent COUVERT
 */
public class Edit extends Uicontrol {

    /**
     * Constructor
     */
    public Edit() {
        super();
        setStyle(__GO_UI_EDIT__);

        if (Console.getConsole().getUseDeprecatedLF()) {
            setRelief(RELIEF_SUNKEN);
        } else {
            UIDefaults defaults = UIManager.getDefaults();

            //font
            setFont(defaults.getFont("TextField.font"));

            //h-alignment
            setHorizontalAlignment("left");

            //v-alignement
            setVerticalAlignment("middle");
        }
    }
}
