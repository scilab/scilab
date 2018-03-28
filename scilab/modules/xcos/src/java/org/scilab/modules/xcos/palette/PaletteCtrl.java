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
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.palette.listener.PaletteMouseListener;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.palette.view.PaletteView;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.handler.mxGraphTransferHandler;
import com.mxgraph.swing.util.mxGraphTransferable;

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
        JavaController controller = new JavaController();
        internalGraph = new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, "");
        internalGraph.installListeners();
    }

    private static final PaletteMouseListener MOUSE_LISTENER = new PaletteMouseListener();
    private static final Logger LOG = Logger.getLogger(PaletteBlockCtrl.class.getName());

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
        this.view.addMouseMotionListener(MOUSE_LISTENER);
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
     * Get the selected palette blocks
     * @return blocks that have been added to the XcosDiagram
     */
    public Object[] getSelectedBlocks() {
        int row = 0;
        int column = 0;
        List<BasicBlock> basicBlocks = new ArrayList<BasicBlock>();
        for (PaletteBlockCtrl blockCtrl : BLOCKS) {
            if (!blockCtrl.isSelected()) {
                continue;
            }

            BasicBlock basicBlock = null;
            try {
                basicBlock = XcosCellFactory.createBlock(blockCtrl.getModel().getName());
            } catch (InterpreterException e) {
            }

            if (basicBlock == null) {
                if (LOG.isLoggable(Level.FINEST)) {
                    LOG.finest(String.format(XcosMessages.UNABLE_TO_LOAD_BLOCK,
                                             blockCtrl.getModel().getData().getEvaluatedPath()));
                }
                getView().setEnabled(false);
                continue;
            }

            // add to the 'recently used blocks' panel
            PaletteManagerView pmv = PaletteManagerView.get();
            if (pmv != null) {
                pmv.getPanel().addRecentltyUsedBlock(blockCtrl.getModel());
            }

            // render and export it
            final double margin = internalGraph.getGridSize();
            basicBlock.getGeometry().setX(margin + BLOCK_POSITION.width * column);
            basicBlock.getGeometry().setY(margin + BLOCK_POSITION.height * row);
            ++column;

            if (column >= BLOCKS_BY_ROW) {
                column = 0;
                ++row;
            }

            basicBlocks.add(basicBlock);
        }

        int size = basicBlocks.size();
        if (size == 0) {
            return null;
        }

        // convert to Object[]
        Object[] ret = new Object[size];
        for (int i = 0; i < size; i++) {
            ret[i] = basicBlocks.get(i);
        }
        return ret;
    }

    /**
     * This function is used to get all selected blocks.
     * @return the transferable object
     */
    public Transferable getTransferable() {
        Transferable transfer = null;
        Object[] cells = getSelectedBlocks();
        if (cells == null) {
            return null;
        }

        BlockPositioning.updatePortsPosition(internalGraph, cells);
        internalGraph.addCells(cells);

        mxGraphTransferHandler handler = (mxGraphTransferHandler) internalGraph.getAsComponent().getTransferHandler();
        transfer = new mxGraphTransferable(cells, internalGraph.getPaintBounds(cells),
                                           handler.createTransferableImage(internalGraph.getAsComponent(), cells));

        return transfer;
    }
}
