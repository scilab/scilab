/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graphic_objects.uicontrol.browser;

import org.scilab.modules.graphic_objects.uicontrol.Uicontrol;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BROWSER__;

public final class Browser extends Uicontrol {
    public Browser() {
        super();
        setStyle(__GO_UI_BROWSER__);
    }
}
