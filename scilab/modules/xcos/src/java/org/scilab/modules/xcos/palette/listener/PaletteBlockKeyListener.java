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
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.PaletteCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteBlockView.StatusUI;
import org.scilab.modules.xcos.palette.view.PaletteManagerPanel;
import org.scilab.modules.xcos.palette.view.PaletteView;
import org.scilab.modules.xcos.utils.BlockPositioning;
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
        /** add the selected blocks into the most recent diagram **/
        if (e.getKeyCode() == KeyEvent.VK_ENTER) {
            final List<XcosDiagram> allDiagrams = Xcos.getInstance().openedDiagrams();
            final PaletteBlockCtrl control = ((PaletteBlockView) e.getSource()).getController();

            // No diagram opened: should never happen as Xcos opens an empty
            // diagram when it is launched
            int size = allDiagrams.size();
            assert size != 0;

            final XcosDiagram theDiagram = allDiagrams.get(size - 1);
            Object[] cells = control.getPaletteCtrl().getSelectedBlocks();
            if (cells != null) {
                BlockPositioning.updatePortsPosition(theDiagram, cells);
                theDiagram.addCells(cells);
            }

        /** deselects blocks and loses focus **/
        } else if (e.getKeyCode() == KeyEvent.VK_ESCAPE || e.getKeyCode() == KeyEvent.VK_TAB) {
            PaletteCtrl.clearSelections();
            PaletteBlockView currentBlockView = ((PaletteBlockView) e.getSource());
            currentBlockView.transferFocusUpCycle();

        /** toggle selection **/
        } else if (e.getKeyCode() == KeyEvent.VK_SPACE) {
            PaletteBlockView currentBlockView = ((PaletteBlockView) e.getSource());
            boolean isSelected = currentBlockView.getController().isSelected();
            if ((e.getModifiers() & KeyEvent.CTRL_MASK) == 0) {
                PaletteCtrl.clearSelections();
            }
            currentBlockView.getController().setSelected(!isSelected); // toggle
            currentBlockView.requestFocus();

        /** move selection (arrow keys) **/
        } else if (e.getKeyCode() == KeyEvent.VK_DOWN) {
            getNextBlock(e, 0, 1);
        } else if (e.getKeyCode() == KeyEvent.VK_UP) {
            getNextBlock(e, 0, -1);
        } else if (e.getKeyCode() == KeyEvent.VK_RIGHT) {
            getNextBlock(e, 1, 0);
        } else if (e.getKeyCode() == KeyEvent.VK_LEFT) {
            getNextBlock(e, -1, 0);
        }
    }

    /**
     * Get the next block position and handle the selection status
     * @param e KeyEvent
     * @param x Initialize x
     * @param y Initialize y
     */
    // CSOFF: ParameterAssignment
    private void getNextBlock(KeyEvent e, int x, int y) {
        PaletteBlockView currentBlockView = ((PaletteBlockView) e.getSource());
        PaletteBlockSize blockSize = PaletteManagerPanel.getCurrentSize();

        x *= blockSize.getBlockDimension().width + XcosConstants.PALETTE_HMARGIN;
        x += currentBlockView.getLocation().x;

        y *= blockSize.getBlockDimension().height + XcosConstants.PALETTE_VMARGIN;
        y += currentBlockView.getLocation().y;

        if (x < 0 || y < 0) {
            return;
        }

        try {
            // gets the current palette view
            PaletteView pview = currentBlockView.getController().getPaletteCtrl().getView();

            // gets the next PaletteBlockView
            PaletteBlockView nextBlockView = (PaletteBlockView) pview.getComponentAt(x, y);

            // handle keys to decide the selection type
            boolean ctrlIsDown = (e.getModifiers() & KeyEvent.CTRL_MASK) != 0;
            boolean isSelected = nextBlockView.getController().isSelected();
            if (!ctrlIsDown) {
                PaletteCtrl.clearSelections();
                nextBlockView.getController().setSelected(!isSelected); // toggle
            } else if (e.getKeyCode() == KeyEvent.VK_SPACE) {
                nextBlockView.getController().setSelected(!isSelected); // toggle
            } else if (!isSelected) {
                nextBlockView.setStatusUI(StatusUI.HOVER);
            }

            pview.scrollRectToVisible(nextBlockView.getBounds());
            nextBlockView.requestFocus();
            if (currentBlockView.getStatusUI().equals(StatusUI.HOVER)) {
                currentBlockView.setStatusUI(StatusUI.NON_SELECTED);
            }
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
