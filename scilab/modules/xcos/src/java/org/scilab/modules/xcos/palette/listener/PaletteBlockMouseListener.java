/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.utils.XcosMessages;

/** Implement the default mouse listener for the block */
public final class PaletteBlockMouseListener implements MouseListener {
    /** Default constructor */
    public PaletteBlockMouseListener() {
    }

    /**
     * Load and perform display update on mouse click
     *
     * @param e
     *            The associated event
     */
    @Override
    public void mouseClicked(MouseEvent e) {
        if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
                || e.isPopupTrigger() || XcosMessages.isMacOsPopupTrigger(e)) {

            ContextMenu menu = ScilabContextMenu.createContextMenu();

            final List<XcosDiagram> allDiagrams = Xcos.getInstance()
                                                  .openedDiagrams();
            final PaletteBlockCtrl control = ((PaletteBlockView) e.getSource())
                                             .getController();

            // No diagram opened: should never happen as Xcos opens an empty
            // diagram when it is launched
            assert allDiagrams.size() != 0;

            if (allDiagrams.size() == 1) {
                // A single diagram opened: add to this diagram
                MenuItem addTo = ScilabMenuItem.createMenuItem();

                addTo.setText(XcosMessages.ADDTO + " "
                              + XcosTab.get(allDiagrams.get(0)).getName());
                final XcosDiagram theDiagram = allDiagrams.get(0);
                addTo.setCallback(new CommonCallBack(e.toString()) {
                    private static final long serialVersionUID = 0L;

                    @Override
                    public void callBack() {
                        BasicBlock current = control.getBlock();
                        theDiagram.addCell(current);
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
                    diagram.setCallback(new CommonCallBack(e.toString()) {
                        private static final long serialVersionUID = 0L;

                        @Override
                        public void callBack() {
                            BasicBlock current = control.getBlock();
                            theDiagram.addCell(current);
                        }
                    });
                    addTo.add(diagram);
                }

                menu.add(addTo);
            }

            menu.getAsSimpleContextMenu().addSeparator();

            MenuItem help = ScilabMenuItem.createMenuItem();
            help.setText("Block help");
            help.setCallback(new CommonCallBack(e.toString()) {
                private static final long serialVersionUID = 0L;

                @Override
                public void callBack() {
                    try {
                        ScilabInterpreterManagement.asynchronousScilabExec(
                            null, "help", control.getModel().getName());
                    } catch (InterpreterException e) {
                        e.printStackTrace();
                    }
                }
            });
            menu.add(help);

            menu.setVisible(true);

            ((SwingScilabContextMenu) menu.getAsSimpleContextMenu())
            .setLocation(MouseInfo.getPointerInfo().getLocation().x,
                         MouseInfo.getPointerInfo().getLocation().y);
        }
    }

    /**
     * Not used
     *
     * @param e
     *            Not used
     */
    @Override
    public void mouseEntered(MouseEvent e) {
    }

    /**
     * Not used
     *
     * @param e
     *            Not used
     */
    @Override
    public void mouseExited(MouseEvent e) {
    }

    /**
     * Select on mouse press
     *
     * @param e
     *            The associated event
     */
    @Override
    public void mousePressed(MouseEvent e) {
        PaletteBlockView view = (PaletteBlockView) e.getSource();
        view.getController().setSelected(true);
    }

    /**
     * Not used
     *
     * @param e
     *            Not used
     */
    @Override
    public void mouseReleased(MouseEvent e) {
    }
}
