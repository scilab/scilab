/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2016 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.palette;

import java.awt.Point;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.InvalidDnDOperationException;
import java.util.logging.Logger;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.handler.mxGraphTransferHandler;
import com.mxgraph.swing.util.mxGraphTransferable;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;

/**
 * A palette block is the representation of the block in the palette. All the
 * operations there are used to render, load and put (on a diagram) a block.
 */
public final class PaletteBlockCtrl {
    /**
     * Internal graph used to render each block.
     */
    public static final XcosDiagram INTERNAL_GRAPH;
    static {
        JavaController controller = new JavaController();
        INTERNAL_GRAPH = new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, "");
        INTERNAL_GRAPH.installListeners();
    }

    private static final double BLOCK_DEFAULT_POSITION = 10.;
    private static final Logger LOG = Logger.getLogger(PaletteBlockCtrl.class.getName());

    private static final String UNABLE_TO_LOAD_BLOCK = Messages.gettext("Unable to load block from %s .");
    private static final String LOADING_THE_BLOCK = Messages.gettext("Loading the block") + XcosMessages.DOTS;

    private static PaletteBlockCtrl previouslySelected;

    private final PaletteBlock model;
    private final PaletteBlockView view;

    /**
     * Default constructor
     *
     * @param model
     *            the block data
     */
    public PaletteBlockCtrl(PaletteBlock model) {
        this.model = model;
        this.view = new PaletteBlockView(this);
        installListeners(this.view);
    }

    /**
     * @param view
     *            The view to setup
     */
    private void installListeners(PaletteBlockView view) {
        view.addMouseListener(new PaletteBlockMouseListener());

        DragSource dragSource = DragSource.getDefaultDragSource();
        dragSource.createDefaultDragGestureRecognizer(this.getView(), DnDConstants.ACTION_MOVE, new PaletteDragGestureListener(getModel(), this));
    }

    /**
     * @return the view
     */
    public PaletteBlockView getView() {
        return view;
    }

    /**
     * @return the model
     */
    public PaletteBlock getModel() {
        return model;
    }

    /**
     * This function is the only access to get the block.
     *
     * @return the transferable object
     */
    public Transferable getTransferable() {
        Transferable transfer;

        BasicBlock block;
        try {
            block = XcosCellFactory.createBlock(model.getName());
        } catch (ScilabInterpreterManagement.InterpreterException ex) {
            LOG.finest(String.format(UNABLE_TO_LOAD_BLOCK, model.getName()));
            getView().setEnabled(false);
            throw new InvalidDnDOperationException();
        }
        getView().setEnabled(true);

        /* Render it and export it */
        block.getGeometry().setX(BLOCK_DEFAULT_POSITION);
        block.getGeometry().setY(BLOCK_DEFAULT_POSITION);

        INTERNAL_GRAPH.addCell(block);
        INTERNAL_GRAPH.selectAll();

        BlockPositioning.updateBlockView(INTERNAL_GRAPH, block);

        mxGraphTransferHandler handler = ((mxGraphTransferHandler) INTERNAL_GRAPH.getAsComponent().getTransferHandler());
        Object[] cells = new Object[] {block};
        transfer = new mxGraphTransferable(cells, INTERNAL_GRAPH.getPaintBounds(cells), handler.createTransferableImage(INTERNAL_GRAPH.getAsComponent(), cells));

        INTERNAL_GRAPH.removeCells();
        return transfer;
    }

    /**
     * This function load the block and render it on the hidden diagram.
     *
     * @return a rendered block
     */
    public BasicBlock getBlock() {
        return (BasicBlock) ((mxGraphTransferable) getTransferable()).getCells()[0];
    }

    /**
     * @return true if it is selected, false otherwise
     */
    public boolean isSelected() {
        return this == previouslySelected;
    }

    /**
     * @param selected
     *            the selected state to set
     */
    public void setSelected(boolean selected) {
        if (selected) {
            if (previouslySelected != null) {
                previouslySelected.setSelected(false);
            }
            previouslySelected = this;
        }
        getView().setSelectedUI(selected);
    }

    /**
     * Drag 'n drop implementation, allocate a block on demand.
     */
    private static final class PaletteDragGestureListener implements DragGestureListener {

        private final PaletteBlock block;
        private final PaletteBlockCtrl controller;

        public PaletteDragGestureListener(final PaletteBlock block, final PaletteBlockCtrl controller) {
            this.block = block;
            this.controller = controller;
        }

        @Override
        public void dragGestureRecognized(DragGestureEvent event) {
            if (PaletteManagerView.get() == null) {
                PaletteManagerView.restore(null);
            }
            final PaletteManagerView winView = PaletteManagerView.get();

            winView.setInfo(LOADING_THE_BLOCK);
            try {
                Transferable transfer = controller.getTransferable();
                if (transfer != null) {
                    event.startDrag(null, null, new Point(), transfer, null);
                } else {
                    throw new InvalidDnDOperationException();
                }
            } catch (InvalidDnDOperationException exception) {
                final String msg = String.format(UNABLE_TO_LOAD_BLOCK, block.getName());
                ScilabModalDialog.show(winView, msg, XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
            } finally {
                winView.setInfo(XcosMessages.EMPTY_INFO);
            }
        }

    }
}
