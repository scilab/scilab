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

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.List;

import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;

/**
 * Implement the default key listener for the block
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public final class PaletteBlockKeyListener implements KeyListener {
    /** Default constructor */
    public PaletteBlockKeyListener() {
    }

    /**
     * @param e The associated event
     */
    @Override
    public void keyPressed(KeyEvent e) {
        // add the current block to the most recent diagram
        if (e.getKeyCode() == KeyEvent.VK_ENTER) {
            final List<XcosDiagram> allDiagrams = Xcos.getInstance().openedDiagrams();
            final PaletteBlockCtrl control = ((PaletteBlockView) e.getSource()).getController();

            // No diagram opened: should never happen as Xcos opens an empty
            // diagram when it is launched
            int size = allDiagrams.size();
            assert size != 0;

            final XcosDiagram theDiagram = allDiagrams.get(size - 1);
            BasicBlock current = control.getBlock();
            theDiagram.addCell(current);
        }
    }

    /**
     * Not used
     * @param e Not used
     */
    @Override
    public void keyReleased(KeyEvent e) {
    }

    /**
     * Not used
     * @param e Not used
     */
    @Override
    public void keyTyped(KeyEvent e) {
    }
}
