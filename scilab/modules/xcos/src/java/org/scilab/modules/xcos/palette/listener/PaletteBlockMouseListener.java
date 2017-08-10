/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.xcos.palette.listener;

import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.List;

import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.PaletteCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteBlockView.StatusUI;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default mouse listener for the block
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 * @author Clement DAVID
 */
public final class PaletteBlockMouseListener implements MouseListener {

    /** Default constructor */
    public PaletteBlockMouseListener() {
    }

    /**
     * @return opened diagrams
     */
    private List<XcosDiagram> getOpenedDiagrams() {
        final List<XcosDiagram> allDiagrams = Xcos.getInstance().openedDiagrams();
        // No diagram opened: should never happen as Xcos opens an empty
        // diagram when it is launched
        assert allDiagrams.size() != 0;
        return allDiagrams;
    }

    /**
     * Create the context menu.
     * @param ctrl PaletteBlockCtrl
     * @param cmd event
     * @return ContextMenu
     */
    private ContextMenu createContextMenu(final PaletteBlockCtrl ctrl, final String cmd) {
        final List<XcosDiagram> allDiagrams = getOpenedDiagrams();

        ContextMenu menu = ScilabContextMenu.createContextMenu();

        if (allDiagrams.size() == 1) {
            // A single diagram opened: add to this diagram
            MenuItem addTo = ScilabMenuItem.createMenuItem();
            addTo.setText(XcosMessages.ADDTO + " " + XcosTab.get(allDiagrams.get(0)).getName());

            final XcosDiagram theDiagram = allDiagrams.get(0);
            addTo.setCallback(new CommonCallBack(cmd) {
                private static final long serialVersionUID = 0L;

                @Override
                public void callBack() {
                    Object[] cells = ctrl.getPaletteCtrl().getSelectedBlocks();
                    if (cells != null) {
                        BlockPositioning.updatePortsPosition(theDiagram, cells);
                        theDiagram.addCells(cells);
                    }
                }
            });
            menu.add(addTo);
        } else {
            // The user has to choose
            Menu addTo = ScilabMenu.createMenu();
            addTo.setText(XcosMessages.ADDTO);

            for (int i = 0; i < allDiagrams.size(); i++) {
                MenuItem diagram = ScilabMenuItem.createMenuItem();
                final XcosDiagram theDiagram = allDiagrams.get(i);
                diagram.setText(XcosTab.get(allDiagrams.get(i)).getName());
                diagram.setCallback(new CommonCallBack(cmd) {
                    private static final long serialVersionUID = 0L;

                    @Override
                    public void callBack() {
                        Object[] cells = ctrl.getPaletteCtrl().getSelectedBlocks();
                        if (cells != null) {
                            BlockPositioning.updatePortsPosition(theDiagram, cells);
                            theDiagram.addCells(cells);
                        }
                    }
                });
                addTo.add(diagram);
            }
            menu.add(addTo);
        }

        menu.getAsSimpleContextMenu().addSeparator();

        MenuItem help = ScilabMenuItem.createMenuItem();
        help.setText("Block help");
        help.setCallback(new CommonCallBack(cmd) {
            private static final long serialVersionUID = 0L;

            @Override
            public void callBack() {
                try {
                    ScilabInterpreterManagement.asynchronousScilabExec(
                        null, "help", ctrl.getModel().getName());
                } catch (InterpreterException e) {
                    e.printStackTrace();
                }
            }
        });
        menu.add(help);

        return menu;
    }

    /**
     * Invoked when the mouse is clicked.
     * @param e MouseEvent
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        final PaletteBlockView blockView = (PaletteBlockView) e.getSource();
        final PaletteBlockCtrl blockCtrl = blockView.getController();

        if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
                || e.isPopupTrigger() || XcosMessages.isMacOsPopupTrigger(e)) {
            // open a context menu
            ContextMenu menu = createContextMenu(blockCtrl, e.toString());
            menu.setVisible(true);
            ((SwingScilabContextMenu) menu.getAsSimpleContextMenu())
            .setLocation(MouseInfo.getPointerInfo().getLocation().x,
                         MouseInfo.getPointerInfo().getLocation().y);

            // ctrl is not down ? clear selections
            if ((e.getModifiers() & MouseEvent.CTRL_MASK) == 0) {
                PaletteCtrl.clearSelections();
            }
            blockCtrl.setSelected(true);
        } else if (SwingUtilities.isLeftMouseButton(e)) {
            if (e.getClickCount() == 1) {
                boolean isSelected = blockCtrl.isSelected();
                // ctrl is not down ? clear selections
                if ((e.getModifiers() & MouseEvent.CTRL_MASK) == 0) {
                    PaletteCtrl.clearSelections();
                }
                blockCtrl.setSelected(!isSelected); // toggle
            } else if (e.getClickCount() == 2) {
                blockCtrl.setSelected(true);
                // add the selected blocks into the most recent diagram
                final List<XcosDiagram> allDiagrams = getOpenedDiagrams();
                XcosDiagram theDiagram = allDiagrams.get(allDiagrams.size() - 1);
                Object[] cells = blockCtrl.getPaletteCtrl().getSelectedBlocks();
                if (cells != null) {
                    BlockPositioning.updatePortsPosition(theDiagram, cells);
                    theDiagram.addCells(cells);
                }
            }
        } else {
            return;
        }

        // make sure it is visible
        blockCtrl.getPaletteCtrl().getView().scrollRectToVisible(blockView.getBounds());
    }

    /**
     * Invoked when the mouse enters a palette block.
     * @param e MouseEvent
     */
    @Override
    public void mouseEntered(MouseEvent e) {
        PaletteBlockView view = (PaletteBlockView) e.getSource();
        if (!view.getController().isSelected()) {
            view.setStatusUI(StatusUI.HOVER);
        }
    }

    /**
     * Invoked when the mouse exits a palette block.
     * @param e MouseEvent
     */
    @Override
    public void mouseExited(MouseEvent e) {
        PaletteBlockView view = (PaletteBlockView) e.getSource();
        if (view.getController().isSelected()) {
            view.setStatusUI(StatusUI.SELECTED);
        } else {
            view.setStatusUI(StatusUI.NON_SELECTED);
        }
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
    public void mousePressed(MouseEvent e) {
    }
}
