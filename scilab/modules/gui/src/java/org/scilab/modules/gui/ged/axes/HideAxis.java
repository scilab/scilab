/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.axes;

/**
 * Manages the operation of the button show/hide for the current axis.
 *
 * @author cardinot
 */
public class HideAxis {
    public HideAxis(boolean hide) {
        if (hide) {
            AxisRulers.pAxisRulers.setVisible(false);
            AxisRulers.bAxisRulers.setSelected(true);
            AxisRulers.pLocation.setVisible(false);
            AxisRulers.bLocation.setSelected(true);
            AxisRulers.pReverse.setVisible(false);
            AxisRulers.bReverse.setSelected(true);

            Label.pLabel.setVisible(false);
            Label.bLabel.setSelected(true);
            Label.pAxisTitle.setVisible(false);
            Label.bAxisTitle.setSelected(true);

            Position.pPosition.setVisible(false);
            Position.bPosition.setSelected(true);
            Position.pAxesBounds.setVisible(false);
            Position.bAxesBounds.setSelected(true);
            Position.pMargins.setVisible(false);
            Position.bMargins.setSelected(true);

            Style.pStyle.setVisible(false);
            Style.bStyle.setSelected(true);   
        } else {
            AxisRulers.pAxisRulers.setVisible(true);
            AxisRulers.bAxisRulers.setSelected(false);

            Label.pLabel.setVisible(true);
            Label.bLabel.setSelected(false);

            Position.pPosition.setVisible(true);
            Position.bPosition.setSelected(false);

            Style.pStyle.setVisible(true);
            Style.bStyle.setSelected(false);
        }
    }
}