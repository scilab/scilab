/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
public class HideAxis {
    public HideAxis(boolean hide) {
        try {
            AxisRulers.pAxisRulers.setVisible(!hide);
            AxisRulers.bAxisRulers.setSelected(hide);
            //AxisRulers.pLocation.setVisible(!hide);
            //AxisRulers.bLocation.setSelected(hide);
            //AxisRulers.pReverse.setVisible(!hide);
            //AxisRulers.bReverse.setSelected(hide);

            Label.pLabel.setVisible(!hide);
            Label.bLabel.setSelected(hide);
            //Label.pAxisTitle.setVisible(!hide);
            //Label.bAxisTitle.setSelected(hide);

            Position.pPosition.setVisible(!hide);
            Position.bPosition.setSelected(hide);
            //Position.pAxesBounds.setVisible(!hide);
            //Position.bAxesBounds.setSelected(hide);
            //Position.pMargins.setVisible(!hide);
            //Position.bMargins.setSelected(hide);

            Style.pStyle.setVisible(!hide);
            Style.bStyle.setSelected(hide);
        } catch (NullPointerException nexcA) { }
    }

    /**
     * Check the status of the buttons of all sections.
     * Updates the button's icon in the toolbar
     */
    public static void checkAllButtons() {
        boolean AR = AxisRulers.pAxisRulers.isVisible();
        boolean LA = Label.pLabel.isVisible();
        boolean PO = Position.pPosition.isVisible();
        boolean SA = Style.pStyle.isVisible();
        if (AR && LA && PO && SA)
            ShowHide.setStatus(false);
        else if (!AR && !LA && !PO && !SA)
            ShowHide.setStatus(true);
    }
}
