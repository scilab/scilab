/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.synchronousScilabExec;

import java.awt.Point;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.InvalidDnDOperationException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.lang.ref.WeakReference;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.handler.mxGraphTransferHandler;
import com.mxgraph.swing.util.mxGraphTransferable;

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
        INTERNAL_GRAPH = new XcosDiagram();
        INTERNAL_GRAPH.installListeners();
    }

    private static final double BLOCK_DEFAULT_POSITION = 10.0;
    private static final MouseListener MOUSE_LISTENER = new PaletteBlockMouseListener();
    private static final Logger LOG = Logger.getLogger(PaletteBlockCtrl.class.getName());

    private static final String UNABLE_TO_LOAD_BLOCK = Messages.gettext("Unable to load block from %s .");
    private static final String LOADING_THE_BLOCK = Messages.gettext("Loading the block") + XcosMessages.DOTS;

    private static PaletteBlockCtrl previouslySelected;

    private final PaletteBlock model;
    private final PaletteBlockView view;

    private transient WeakReference<Transferable> transferable = new WeakReference<Transferable>(null);

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
        view.addMouseListener(MOUSE_LISTENER);
        installDnd();
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
     * @throws ScicosFormatException
     *             on decoding error
     */
    public synchronized Transferable getTransferable() throws ScicosFormatException {
        Transferable transfer = transferable.get();
        if (transfer == null) {
            /* Load the block from the H5 file */
            BasicBlock block;
            try {
                block = loadBlock();
            } catch (ScicosFormatException ex) {
                getView().setEnabled(false);
                throw ex;
            }
            if (block == null) {
                if (LOG.isLoggable(Level.FINEST)) {
                    LOG.finest(String.format(UNABLE_TO_LOAD_BLOCK, getModel().getData().getEvaluatedPath()));
                }
                getView().setEnabled(false);
                throw new InvalidDnDOperationException();
            }
            getView().setEnabled(true);

            /* Render it and export it */
            block.getGeometry().setX(BLOCK_DEFAULT_POSITION);
            block.getGeometry().setY(BLOCK_DEFAULT_POSITION);

            INTERNAL_GRAPH.addCell(block);
            INTERNAL_GRAPH.selectAll();

            BlockPositioning.updateBlockView(block);

            mxGraphTransferHandler handler = ((mxGraphTransferHandler) INTERNAL_GRAPH.getAsComponent().getTransferHandler());
            transfer = handler.createTransferable(INTERNAL_GRAPH.getAsComponent());
            transferable = new WeakReference<Transferable>(transfer);

            INTERNAL_GRAPH.removeCells();
        }
        return transfer;
    }

    /**
     * @return the loaded block.
     * @throws ScicosFormatException
     *             on error
     */
    private BasicBlock loadBlock() throws ScicosFormatException {
        BasicBlock block;
        if (model.getName().compareTo("TEXT_f") != 0) {

            // Load the block with a reference instance
            final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
            if (handler == null) {
                return null;
            }

            try {
                synchronousScilabExec(ScilabDirectHandler.BLK + " = " + buildCall(model.getName(), "define"));
                block = handler.readBlock();
            } catch (InterpreterException e1) {
                LOG.severe(e1.toString());
                block = null;
            } finally {
                handler.release();
            }

            // invalid block case
            if (block == null) {
                return null;
            }

            if (block.getStyle().compareTo("") == 0) {
                block.setStyle(block.getInterfaceFunctionName());
            }
        } else {
            block = BlockFactory.createBlock(BlockInterFunction.TEXT_f);
        }
        return block;
    }

    /**
     * @param callback
     *            called after the block loading
     */
    protected void loadBlock(final ActionListener callback) {
        if (model.getName().compareTo("TEXT_f") != 0) {

            // Load the block with a reference instance
            final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
            if (handler == null) {
                return;
            }

            final ActionListener internalCallback = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    try {
                        final BasicBlock block = handler.readBlock();

                        // invalid block case
                        if (block == null) {
                            return;
                        }

                        // update style
                        if (block.getStyle().compareTo("") == 0) {
                            block.setStyle(block.getInterfaceFunctionName());
                        }

                        callback.actionPerformed(new ActionEvent(block, 0, "loaded"));
                    } catch (ScicosFormatException e1) {
                        e1.printStackTrace();
                    } finally {
                        handler.release();
                    }
                }
            };

            try {
                asynchronousScilabExec(internalCallback, ScilabDirectHandler.BLK + " = " + buildCall(model.getName(), "define"));
            } catch (InterpreterException e1) {
                LOG.severe(e1.toString());
            } finally {
                handler.release();
            }
        } else {
            final BasicBlock block = BlockFactory.createBlock(BlockInterFunction.TEXT_f);
            callback.actionPerformed(new ActionEvent(block, 0, "loaded"));
        }
    }

    /**
     * This function load the block and render it on the hidden diagram. This
     * can be time-consuming and each block should be cached on the caller when
     * possible.
     *
     * @return a rendered block
     */
    public BasicBlock getBlock() {
        try {
            return (BasicBlock) ((mxGraphTransferable) getTransferable()).getCells()[0];
        } catch (ScicosFormatException e) {
            LOG.severe(e.toString());
            return null;
        }
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
     * Install the Drag'n'Drop on this instance.
     */
    public void installDnd() {
        // Install the handler for dragging nodes into a graph
        DragGestureListener dragGestureListener = new DragGestureListener() {
            @Override
            public void dragGestureRecognized(DragGestureEvent e) {
                if (PaletteManagerView.get() == null) {
                    PaletteManagerView.restore(null);
                }
                final PaletteManagerView winView = PaletteManagerView.get();
                final DragGestureEvent event = e;
                final String msg = String.format(UNABLE_TO_LOAD_BLOCK, getModel().getName());

                winView.setInfo(LOADING_THE_BLOCK);
                try {
                    Transferable transfer = getTransferable();

                    if (transfer != null) {
                        event.startDrag(null, null, new Point(), transfer, null);
                    } else {
                        throw new InvalidDnDOperationException();
                    }
                } catch (InvalidDnDOperationException exception) {
                    ScilabModalDialog.show(winView, msg, XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
                } catch (ScicosFormatException ex) {
                    ScilabModalDialog.show(winView, ex.getMessage(), XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
                } finally {
                    winView.setInfo(XcosMessages.EMPTY_INFO);
                }
            }

        };

        DragSource dragSource = DragSource.getDefaultDragSource();
        dragSource.createDefaultDragGestureRecognizer(this.getView(), DnDConstants.ACTION_COPY, dragGestureListener);
    }
}
