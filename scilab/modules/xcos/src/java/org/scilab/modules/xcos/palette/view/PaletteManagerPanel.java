/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;
import java.awt.Component;
import java.awt.Toolkit;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import javax.swing.DropMode;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;

import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.listener.PaletteManagerMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeSelectionListener;
import org.scilab.modules.xcos.palette.listener.PaletteTreeTransferHandler;
import org.scilab.modules.xcos.palette.model.Custom;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosConstants.PaletteBlockSize;

/**
 * The content pane for the block view
 */
@SuppressWarnings(value = { "serial" })
public class PaletteManagerPanel extends JSplitPane {

    private static XcosConstants.PaletteBlockSize currentSize;
    private PaletteManager controller;
    private JTree tree;

    /**
     * Default constructor
     *
     * @param controller
     *            the {@link PaletteManager} instance
     */
    public PaletteManagerPanel(PaletteManager controller) {
        super(JSplitPane.HORIZONTAL_SPLIT);
        this.controller = controller;
        fillUpContentPane();
    }

    /**
     * Fill up the content pane
     */
    private void fillUpContentPane() {
        /** Default instances */
        JScrollPane panel = new JScrollPane();
        initJScrollPane(panel);

        currentSize = XcosConstants.PaletteBlockSize.NORMAL;

        // Set default left component
        JPanel rootPalette = new JPanel();

        TreeNode root = controller.getRoot();
        tree = new JTree(new PaletteTreeModel(root));
        JScrollPane treeScrollPane = new JScrollPane(tree);

        /** Setup tree */
        tree.getSelectionModel().setSelectionMode(
            TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.addMouseListener(new PaletteManagerMouseListener());
        tree.addTreeSelectionListener(new PaletteManagerTreeSelectionListener(
                                          this, panel));

        tree.setEditable(false);
        tree.setDragEnabled(true);
        tree.setExpandsSelectedPaths(true);
        tree.setDropMode(DropMode.INSERT);
        tree.setTransferHandler(new PaletteTreeTransferHandler());

        setLeftComponent(treeScrollPane);
        panel.setViewportView(rootPalette);
        setRightComponent(panel);
    }

    /**
     * Get the current PaletteBlockSize
     * @return current PaletteBlockSize
     */
    public PaletteBlockSize getCurrentSize() {
        return currentSize;
    }

    /**
     * Zoom
     * @param newSize new paletteblocksize enum
     */
    private void zoom(PaletteBlockSize newSize) {
        if (newSize == null) {
            return;
        }

        try {
            // check what's being displayed on the right panel
            JScrollPane jspR = (JScrollPane) this.getRightComponent();
            PaletteNode node = (PaletteNode) tree.getLastSelectedPathComponent();

            if (node instanceof PreLoaded) {
                JPanel panel = (JPanel) jspR.getViewport().getComponent(0);
                for (Component component : panel.getComponents()) {
                    PaletteBlockView view = (PaletteBlockView) component; 
                    view.setIconSize(newSize.getIconScale());
                    view.setFontSize(newSize.getFontSize());
                    view.setPreferredSize(newSize.getBlockDimension());
                }
            } else if (node instanceof Custom) {
                // TODO
            } else {
                return;
            }
            currentSize = newSize;
        } catch (NullPointerException e) {
        }
    }

    /** zoom in **/
    public void zoomIn() {
        zoom(currentSize.next());
    }

    /** zoom out **/
    public void zoomOut() {
        zoom(currentSize.previous());
    }

    /**
     * Init the ScrollPane component
     *
     * @param panel
     *            the component
     */
    private void initJScrollPane(JScrollPane panel) {
        panel.setBackground(Color.WHITE);

        panel.getVerticalScrollBar().setBlockIncrement(
            XcosConstants.PALETTE_BLOCK_HEIGHT
            + XcosConstants.PALETTE_VMARGIN);
        panel.getVerticalScrollBar().setUnitIncrement(
            XcosConstants.PALETTE_BLOCK_HEIGHT
            + XcosConstants.PALETTE_VMARGIN);

        panel.getHorizontalScrollBar().setBlockIncrement(
            XcosConstants.PALETTE_BLOCK_WIDTH
            + XcosConstants.PALETTE_HMARGIN);
        panel.getHorizontalScrollBar().setUnitIncrement(
            XcosConstants.PALETTE_BLOCK_WIDTH
            + XcosConstants.PALETTE_HMARGIN);

        MouseWheelListener mouseListener = new CustomMouseWheelListener(
                this, panel.getVerticalScrollBar());
        panel.addMouseWheelListener(mouseListener);
    }

    /**
     * Setup the default layout
     */
    public void performStartUpLayout() {
        /* Tree layout */
        final Object root = tree.getModel().getRoot();
        final Object firstChild = tree.getModel().getChild(root, 0);
        final Object secondChild = tree.getModel().getChild(firstChild, 0);
        tree.setSelectionPath(new TreePath(new Object[] {root, firstChild, secondChild}));

        tree.setRootVisible(false);
        tree.setScrollsOnExpand(true);

        /* Global layout */
        setContinuousLayout(true);
    }

    /**
     * Implement custom mouse handling for the zoom
     */
    private static final class CustomMouseWheelListener implements MouseWheelListener {
        private static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
        private PaletteManagerPanel panel;
        private JScrollBar verticalScrollBar;
        private int unitIncrement;

        /**
         * Default constructor
         * @param pmp PaletteManagerPanel instance
         * @param verticalScrollBar VerticalScrollBar
         */
        public CustomMouseWheelListener(PaletteManagerPanel pmp,
                                        JScrollBar verticalScrollBar) {
            this.panel = pmp;
            this.verticalScrollBar = verticalScrollBar;
            this.unitIncrement = verticalScrollBar.getUnitIncrement();
        }

        /**
         * When the wheel is used
         * @param e The parameters
         * @see java.awt.event.MouseWheelListener#mouseWheelMoved(java.awt.event.MouseWheelEvent)
         */
        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
            if ((e.getModifiers() & ACCELERATOR_KEY) != 0) {
                verticalScrollBar.setUnitIncrement(0);
                if (e.getWheelRotation() < 0) {
                    panel.zoomIn();
                } else if (e.getWheelRotation() > 0) {
                    panel.zoomOut();
                }
            } else {
                verticalScrollBar.setUnitIncrement(unitIncrement);
            }
        }
    }
}
