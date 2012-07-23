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
package org.scilab.modules.gui.ged.figure;

/**
 * Manages the operation of the button show/hide for the current Figure.
 *
 * @author cardinot
 */
public class HideFigure {
    public HideFigure(boolean hide) {
        try {
            BaseProperties.pBaseProperties.setVisible(!hide);
            BaseProperties.bBaseProperties.setSelected(hide);

            Control.pControl.setVisible(!hide);
            Control.bControl.setSelected(hide);

            DataProperties.pData.setVisible(!hide);
            DataProperties.bData.setSelected(hide);

            Style.pStyle.setVisible(!hide);
            Style.bStyle.setSelected(hide);
        } catch (NullPointerException nexcF) { }
    }
}