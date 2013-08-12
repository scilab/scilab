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
package org.scilab.modules.gui.ged.graphic_objects.imageplot;

import org.scilab.modules.gui.ged.actions.ShowHide;

/**
 * Manages the operation of the button show/hide for the current imageplot.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class HideImageplot {
    public HideImageplot(boolean hide) {
        try {
            BaseProperties.setVisibility(!hide);
            DataProperties.setVisibility(!hide);
        } catch (NullPointerException nexcC) { }
    }

    /**
     * Check the status of the buttons of all sections.
     * Updates the button's icon in the toolbar
     */
    public static void checkAllButtons() {
        boolean BP = BaseProperties.getStatus();
        boolean DP = DataProperties.getStatus();

        if (BP && DP)
            ShowHide.setStatus(false);
        else if (!BP && !DP)
            ShowHide.setStatus(true);
    }
}