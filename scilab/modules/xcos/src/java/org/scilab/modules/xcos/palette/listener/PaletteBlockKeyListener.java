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

import java.awt.Component;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.List;

import javax.swing.JScrollPane;

import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.palette.view.PaletteSearchView;
import org.scilab.modules.xcos.palette.view.PaletteView;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

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
        /** add the current block to the most recent diagram **/
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

            PaletteManagerView.get().getPanel().addRecentltyUsedBlock(control.getModel());
            return;
        }

        /** deselects block and loses focus **/
        if (e.getKeyCode() == KeyEvent.VK_ESCAPE || e.getKeyCode() == KeyEvent.VK_TAB) {
            PaletteBlockView currentBlockView = ((PaletteBlockView) e.getSource());
            currentBlockView.getController().setSelected(false);
            currentBlockView.transferFocusUpCycle();
            return;
        }

        /** move selection (arrow keys) **/
        int x = 0;
        int y = 0;
        if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            x = 0;
            y = 1;
        } else if (e.getKeyCode() == KeyEvent.VK_UP) {
            x = 0;
            y = -1;
        } else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
            x = 1;
            y = 0;
        } else if (e.getKeyCode() == KeyEvent.VK_LEFT) {
            x = -1;
            y = 0;
        } else {
            // nothing to do!
            return;
        }

        // get position of the next block
        PaletteBlockView currentBlockView = ((PaletteBlockView) e.getSource());
        PaletteBlockSize blockSize = currentBlockView.getPaletteBlockSize();

        x *= blockSize.getBlockDimension().width + XcosConstants.PALETTE_HMARGIN;
        x += currentBlockView.getLocation().x;

        y *= blockSize.getBlockDimension().height + XcosConstants.PALETTE_VMARGIN;
        y += currentBlockView.getLocation().y;

        if (x < 0 || y < 0) {
            return;
        }

        // select the block
        try {
            JScrollPane jsp = (JScrollPane) PaletteManagerView.get().getPanel().getRightComponent();
            Component c = jsp.getViewport().getComponent(0);
            String cName = c.getName();
            PaletteView pview;
            if (cName.equals("PaletteView")) {
                pview = (PaletteView) c;
            } else if (cName.equals("PaletteSearchView")) {
                PaletteSearchView sview = (PaletteSearchView) c;
                pview = (PaletteView) sview.getComponent(1);
            } else {
                return;
            }
            PaletteBlockView bview = (PaletteBlockView) pview.getComponentAt(x, y);
            bview.getController().setSelected(true);
        } catch (ClassCastException err) {
        } catch (NullPointerException err) {
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
