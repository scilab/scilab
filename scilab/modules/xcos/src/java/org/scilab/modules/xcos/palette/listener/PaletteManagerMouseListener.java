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
import java.util.logging.Logger;

import javax.swing.JOptionPane;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.tree.TreePath;

import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Palette;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Manage the click on the category tree
 */
public class PaletteManagerMouseListener implements MouseListener {

    /** Default constructor */
    public PaletteManagerMouseListener() {
    }

    /**
     * Manage category selection
     *
     * @param e
     *            Not used
     * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
     */
    @Override
    public void mouseClicked(final MouseEvent e) {
        // Right click
        if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e)) || e.isPopupTrigger() || XcosMessages.isMacOsPopupTrigger(e)) {

            if (PaletteManagerView.get() == null) {
                PaletteManagerView.restore(null);
            }
            final JTree paletteTree = PaletteManagerView.get().getTree();
            final TreePath path = paletteTree.getPathForLocation(e.getX(), e.getY());
            paletteTree.setSelectionPath(path);

            final ContextMenu menu = ScilabContextMenu.createContextMenu();

            final MenuItem create = ScilabMenuItem.createMenuItem();
            setupCreateOrAdd(paletteTree, path, create);
            menu.add(create);

            final MenuItem rename = ScilabMenuItem.createMenuItem();
            setupRename(paletteTree, path, rename);
            menu.add(rename);

            final MenuItem remove = ScilabMenuItem.createMenuItem();
            setupRemove(paletteTree, path, remove);
            menu.add(remove);

            menu.setVisible(true);
            ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                    .getLocation().y);

        }
    }

    /**
     * Setup the create item
     *
     * @param paletteTree
     *            the current tree
     * @param path
     *            the current path
     * @param create
     *            the menu item
     */
    private void setupCreateOrAdd(final JTree paletteTree, final TreePath path, final MenuItem create) {
        PaletteNode node;
        if (path != null) {
            node = (PaletteNode) (path.getLastPathComponent());
        } else {
            node = (PaletteNode) paletteTree.getModel().getRoot();
        }
        final PaletteNode currentNode = node;

        if (currentNode instanceof Category) {
            create.setText(XcosMessages.CREATE);
        } else if (currentNode instanceof Palette) {
            create.setText(XcosMessages.ADDTO_CATEGORY);
        } else {
            throw new IllegalArgumentException("Invalid node selected");
        }

        create.setCallback(new CommonCallBack(XcosMessages.CREATE) {
            private static final long serialVersionUID = 0L;

            @Override
            public void callBack() {
                String name = JOptionPane.showInputDialog(XcosMessages.ASK_FOR_A_NAME, XcosMessages.DEFAULT_CATEGORY_NAME);
                if (name == null || name.isEmpty()) {
                    return;
                }

                Category nonModifiedRoot = currentNode.getParent();
                final Category c = new Category();
                c.setEnable(true);
                c.setName(name);

                if (currentNode instanceof Category) {
                    ((Category) currentNode).getNode().add(c);
                    c.setParent((Category) currentNode);

                    PaletteNode.refreshView(currentNode, c);
                } else if (currentNode instanceof Palette) {
                    final int index = nonModifiedRoot.getIndex(currentNode);

                    nonModifiedRoot.getNode().set(index, c);
                    c.getNode().add(currentNode);
                    currentNode.setParent(c);
                    c.setParent(nonModifiedRoot);

                    PaletteNode.refreshView(nonModifiedRoot, currentNode);
                }
            }
        });

        create.setEnabled(true);
    }

    /**
     * Setup the rename item
     *
     * @param paletteTree
     *            the current tree
     * @param path
     *            the current path
     * @param rename
     *            the menu item
     */
    // CSOFF: IllegalCatch
    private void setupRename(final JTree paletteTree, final TreePath path, final MenuItem rename) {
        rename.setText(XcosMessages.RENAME);
        rename.setCallback(new CommonCallBack(XcosMessages.RENAME) {
            private static final long serialVersionUID = 0L;

            @Override
            public void callBack() {
                if (path == null) {
                    return;
                }

                try {
                    final PaletteNode currentNode = (PaletteNode) path.getLastPathComponent();

                    final String s = JOptionPane.showInputDialog(XcosMessages.ASK_FOR_A_NAME, currentNode.getName());
                    if (s == null || s.isEmpty()) {
                        return;
                    }

                    currentNode.setName(s);
                } catch (final Exception e) {
                    Logger.getLogger(PaletteManagerMouseListener.class.getName()).severe(e.toString());
                }
            }
        });
    }

    /**
     * Setup the remove item
     *
     * @param paletteTree
     *            the current tree
     * @param path
     *            the current path
     * @param remove
     *            the menu item
     */
    // CSOFF: IllegalCatch
    private void setupRemove(final JTree paletteTree, final TreePath path, final MenuItem remove) {
        remove.setText(XcosMessages.REMOVE);
        remove.setCallback(new CommonCallBack(XcosMessages.REMOVE) {
            private static final long serialVersionUID = 0L;

            @Override
            public void callBack() {
                if (path == null) {
                    return;
                }

                try {
                    final PaletteNode currentNode = (PaletteNode) path.getLastPathComponent();
                    PaletteNode.remove(currentNode);
                } catch (final Exception e) {
                    Logger.getLogger(PaletteManagerMouseListener.class.getName()).severe(e.toString());
                }
            }
        });

        boolean canBeRemoved = true;
        try {
            org.scilab.modules.xcos.palette.model.PaletteNode.checkRemoving((PaletteNode) path.getLastPathComponent());
        } catch (final Exception exception) {
            canBeRemoved = false;
        }

        remove.setEnabled(canBeRemoved);
    }

    // CSON: IllegalCatch

    /**
     * Not used
     *
     * @param e
     *            Not used
     * @see java.awt.event.MouseListener#mouseEntered(java.awt.event.MouseEvent)
     */
    @Override
    public void mouseEntered(final MouseEvent e) {
    }

    /**
     * Not used
     *
     * @param e
     *            Not used
     * @see java.awt.event.MouseListener#mouseExited(java.awt.event.MouseEvent)
     */
    @Override
    public void mouseExited(final MouseEvent e) {
    }

    /**
     * Not used
     *
     * @param e
     *            Not used
     * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
     */
    @Override
    public void mousePressed(final MouseEvent e) {
    }

    /**
     * Not used
     *
     * @param e
     *            Not used
     * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
     */
    @Override
    public void mouseReleased(final MouseEvent e) {
    }

}
