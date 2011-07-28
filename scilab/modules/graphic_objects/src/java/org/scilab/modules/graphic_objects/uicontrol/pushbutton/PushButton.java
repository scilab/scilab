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

package org.scilab.modules.graphic_objects.uicontrol.pushbutton;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;

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
		setBackgroundColor(new Double[]{BUTTON_RED_BACKGROUND, BUTTON_GREEN_BACKGROUND, BUTTON_BLUE_BACKGROUND});
	}

}
