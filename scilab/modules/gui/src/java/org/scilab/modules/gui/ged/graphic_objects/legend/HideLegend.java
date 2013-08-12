/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.legend;

import org.scilab.modules.gui.ged.actions.ShowHide;

/**
 * Manages the operation of the button show/hide for the current legend.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class HideLegend {
    public HideLegend(boolean hide) {
        try {
            BaseProperties.setVisibility(!hide);
            DataProperties.setVisibility(!hide);
            Font.setVisibility(!hide);
            Position.setVisibility(!hide);
            Style.setVisibility(!hide);
        } catch (NullPointerException nexcC) { }
    }

    /**
     * Check the status of the buttons of all sections.
     * Updates the button's icon in the toolbar
     */
    public static void checkAllButtons() {
        boolean BP = BaseProperties.getStatus();
        boolean DP = DataProperties.getStatus();
        boolean FT = Font.getStatus();
        boolean PO = Position.getStatus();
        boolean SA = Style.getStatus();

        if (BP && DP && FT && PO && SA)
            ShowHide.setStatus(false);
        else if (!BP && !DP && !FT && !PO && !SA)
            ShowHide.setStatus(true);
    }
}