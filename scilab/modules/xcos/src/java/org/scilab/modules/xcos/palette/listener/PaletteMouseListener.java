/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.listener;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import org.scilab.modules.xcos.palette.PaletteCtrl;

/**
 * Implement the default mouse listener for the palette view
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public final class PaletteMouseListener implements MouseListener {

    /** Default constructor */
    public PaletteMouseListener() {
    }

    /**
     * Clear selection when mouse is pressed
     * @param e MouseEvent
     */
    @Override
    public void mousePressed(MouseEvent e) {
        PaletteCtrl.clearSelections();
    }

    /**
     * Invoked when the mouse enters a palette view.
     * @param e MouseEvent
     */
    @Override
    public void mouseEntered(MouseEvent e) {
    }

    /**
     * Invoked when the mouse exits a palette view.
     * @param e MouseEvent
     */
    @Override
    public void mouseExited(MouseEvent e) {
    }

    /**
     * Not used
     * @param e Not used
     */
    @Override
    public void mouseReleased(MouseEvent e) {
    }

    /**
     * Not used
     * @param e Not used
     */
    @Override
    public void mouseClicked(MouseEvent e) {
    }
}
