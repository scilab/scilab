/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.axes;

import org.scilab.modules.gui.ged.actions.ShowHide;

/**
 * Manages the operation of the button show/hide for the current axis.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class HideAxes {
    public HideAxes(boolean hide) {
        try {
            BaseProperties.setVisibility(!hide);
            Box.setVisibility(!hide);
            Camera.setVisibility(!hide);
            Label.setVisibility(!hide);
            Position.setVisibility(!hide);
        } catch (NullPointerException nexcA) { }
    }

    /**
     * Check the status of the buttons of all sections.
     * Updates the button's icon in the toolbar
     */
    public static void checkAllButtons() {
        boolean BP = BaseProperties.getStatus();
        boolean BOX = Box.getStatus();
        boolean CAM = Camera.getStatus();
        boolean LAB = Label.getStatus();
        boolean POS = Position.getStatus();

        if (BP && BOX && CAM && LAB && POS)
            ShowHide.setStatus(false);
        else if (!BP && !BOX && !CAM && !LAB && !POS)
            ShowHide.setStatus(true);
    }
}
