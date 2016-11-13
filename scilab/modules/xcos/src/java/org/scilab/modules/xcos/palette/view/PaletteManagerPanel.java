/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.palette.view;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.DropMode;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeSelectionModel;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;

import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.listener.PaletteManagerMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeSelectionListener;
import org.scilab.modules.xcos.palette.listener.PaletteManagerTreeWillExpandListener;
import org.scilab.modules.xcos.palette.listener.PaletteTreeTransferHandler;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.utils.Stack;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * The content pane for the block view
 */
@SuppressWarnings(value = { "serial" })
public class PaletteManagerPanel extends JSplitPane {
    private static final int NUMBER_OF_DUMMY_BLOCKS = 10; // 10 blocks loaded,
    private static final int LOAD_DUMMY_BLOCK_DELAY = 200; // one each 200 milliseconds

    private static final class LoadBlock implements ActionListener {
        private final PaletteManager controller;
        private int loadedBlocks;

        private Stack<TreeNode> nodeStack;
        private Stack<PaletteBlock> blocksStack;

        LoadBlock(final PaletteManager controller, final int loadedBlocks) {
            this.controller = controller;
            this.loadedBlocks = loadedBlocks;

            nodeStack = new Stack<>();
            nodeStack.push(controller.getRoot());
            blocksStack = new Stack<>();
        }

        @Override
        public void actionPerformed(ActionEvent e) {

            // load only a limited set of blocks
            if (loadedBlocks <= 0) {
                Timer timer = (Timer) e.getSource();
                timer.stop();
            }

            // consume a block
            if (blocksStack.size() != 0) {
                loadTransferable(blocksStack.pop());
                return;
            }

            // no block is available, look for a Preloaded palette
            while (nodeStack.size() != 0) {
                TreeNode o = nodeStack.pop();

                if (o instanceof PreLoaded) {
                    // found it a palette, create a dummy transferable from the first block and store the others for later use.
                    blocksStack.addAll(((PreLoaded) o).getBlock());

                    if (blocksStack.size() != 0) {
                        loadTransferable(blocksStack.pop());
                        return;
                    }
                } else {
                    for (int i = 0; i < o.getChildCount(); i++) {
                        nodeStack.push(o.getChildAt(i));
                    }
                }
            }
        }

        private void loadTransferable(final PaletteBlock blk) {
            new PaletteBlockCtrl(blk).getTransferable();
            loadedBlocks -= 1;
        }
    }

    private final PaletteManager controller;

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

        // Set default left component
        JPanel rootPalette = new JPanel();

        TreeNode root = controller.getRoot();
        JTree tree = new JTree(new PaletteTreeModel(root));
        JScrollPane treeScrollPane = new JScrollPane(tree);

        /** Setup tree */
        tree.getSelectionModel().setSelectionMode(
            TreeSelectionModel.SINGLE_TREE_SELECTION);
        tree.addMouseListener(new PaletteManagerMouseListener());
        tree.addTreeWillExpandListener(new PaletteManagerTreeWillExpandListener());
        tree.addTreeSelectionListener(new PaletteManagerTreeSelectionListener(
                                          panel));

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
    }

    /**
     * Setup the default layout
     */
    public void performStartUpLayout() {
        JTree tree = (JTree) ((JScrollPane) getLeftComponent()).getViewport()
                     .getView();

        /* Tree layout */
        final Object root = tree.getModel().getRoot();
        final Object firstChild = tree.getModel().getChild(root, 0);
        final Object secondChild = tree.getModel().getChild(firstChild, 0);
        tree.setSelectionPath(new TreePath(new Object[] {root, firstChild, secondChild}));

        tree.setRootVisible(false);
        tree.setScrollsOnExpand(true);

        /* Global layout */
        setContinuousLayout(true);

        // Delay-load some blocks to pre-load jars used on the rendering.
        // A timer is used to avoid busying EDT and add a small delay between blocks
        Timer timer = new Timer(LOAD_DUMMY_BLOCK_DELAY, new LoadBlock(controller, NUMBER_OF_DUMMY_BLOCKS));
        timer.start();
    }
}
