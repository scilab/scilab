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

package org.scilab.modules.xcos.palette;

import java.awt.Dimension;
import java.awt.datatransfer.Transferable;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.listener.PaletteMouseListener;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.palette.view.PaletteView;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;

import com.mxgraph.swing.handler.mxGraphTransferHandler;

/**
 * All the operations which are used to render, load and put
 * the selected blocks on a diagram.
 * @author Marcos Cardinot <mcardinot@gmail.com>
 */
public final class PaletteCtrl {
    /**
     * Internal graph used to render the selected blocks.
     */
    private static XcosDiagram internalGraph;
    static {
        internalGraph = new XcosDiagram();
        internalGraph.installListeners();
    }

    private static final MouseListener MOUSE_LISTENER = new PaletteMouseListener();

    private static final int BLOCKS_BY_ROW = 5;
    private static final Dimension BLOCK_POSITION = XcosConstants.PaletteBlockSize.NORMAL.getBlockDimension();

    private static final List<PaletteBlockCtrl> BLOCKS = new ArrayList<PaletteBlockCtrl>();
    private final PaletteView view;

    /**
     * Default constructor
     */
    public PaletteCtrl() {
        this.view = new PaletteView();

        // install listeners
        this.view.addMouseListener(MOUSE_LISTENER);
    }

    /**
     * @return the view
     */
    public PaletteView getView() {
        return view;
    }

    /**
     * Clear the view
     */
    public void clear() {
        BLOCKS.clear();
        view.removeAll();
    }

    /**
     * Deselects all blocks
     */
    public static void clearSelections() {
        for (PaletteBlockCtrl block : BLOCKS) {
            block.setSelected(false);
        }
    }

    /**
     * Adds a block to the view.
     * @param ctrl PaletteBlockCtrl
     */
    public void addBlock(PaletteBlockCtrl ctrl) {
        BLOCKS.add(ctrl);
        view.add(ctrl.getView());
    }

    /**
     * Add all the selected blocks to a XcosDiagram.
     * @param diagram XcosDiagram
     * @return true if some block was added
     */
    public boolean addSelectedBlocks(final XcosDiagram diagram) {
        int row = 0;
        int column = 0;
        boolean hasNewBlocks = false;
        for (PaletteBlockCtrl blockCtrl : BLOCKS) {
            if (!blockCtrl.isSelected()) {
                continue;
            }

            BasicBlock basicBlock = (BasicBlock) BlockFactory.createClone(blockCtrl.getBlock());
            if (basicBlock == null) {
                continue;
            }

            // add to the 'recently used blocks' panel
            PaletteManagerView.get().getPanel().addRecentltyUsedBlock(blockCtrl.getModel());

            // Render it and export it
            final double margin = 15.0;
            basicBlock.getGeometry().setX(margin + BLOCK_POSITION.width * column);
            basicBlock.getGeometry().setY(margin + BLOCK_POSITION.height * row);
            ++column;

            if (column >= BLOCKS_BY_ROW) {
                column = 0;
                ++row;
            }

            diagram.addCell(basicBlock);
            diagram.addSelectionCell(basicBlock);
            BlockPositioning.updateBlockView(basicBlock);
            hasNewBlocks = true;
        }
        return hasNewBlocks;
    }

    /**
     * This function is used to get all selected blocks.
     * @return the transferable object
     */
    public synchronized Transferable getTransferable() {
        Transferable transfer = null;
        internalGraph.removeCells();
        if (addSelectedBlocks(internalGraph)) {
            mxGraphTransferHandler handler = (mxGraphTransferHandler) internalGraph.getAsComponent().getTransferHandler();
            transfer = handler.createTransferable(internalGraph.getAsComponent());
            internalGraph.removeCells();
        }
        return transfer;
    }
}
