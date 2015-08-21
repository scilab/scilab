/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
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

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.synchronousScilabExec;

import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.InvalidDnDOperationException;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyListener;
import java.awt.event.MouseListener;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.palette.listener.PaletteBlockKeyListener;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteDragGestureListener;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteBlockView.StatusUI;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * A palette block is the representation of the block in the palette.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 * @author Clement DAVID
 */
public final class PaletteBlockCtrl {

    private static final DragGestureListener DRAG_LISTENER = new PaletteDragGestureListener();
    private static final KeyListener KEY_LISTENER = new PaletteBlockKeyListener();
    private static final MouseListener MOUSE_LISTENER = new PaletteBlockMouseListener();
    private static final Logger LOG = Logger.getLogger(PaletteBlockCtrl.class.getName());

    private final PaletteCtrl paletteCtrl;
    private final PaletteBlock model;
    private final PaletteBlockView view;
    private BasicBlock basicBlock;
    private boolean isSelected;

    /**
     * Default constructor
     * @param ctrl The palette in which it belongs
     * @param model the block data
     */
    public PaletteBlockCtrl(PaletteCtrl ctrl, PaletteBlock model) {
        this.paletteCtrl = ctrl;
        this.model = model;
        this.view = new PaletteBlockView(this);
        this.isSelected = false;
        this.basicBlock = null;

        // add block to its parent palette
        this.paletteCtrl.addBlock(this);

        // install listeners
        this.view.setFocusTraversalKeysEnabled(false);
        this.view.addKeyListener(KEY_LISTENER);
        this.view.addMouseListener(MOUSE_LISTENER);

        DragSource dragSource = DragSource.getDefaultDragSource();
        dragSource.createDefaultDragGestureRecognizer(this.getView(),
                DnDConstants.ACTION_COPY, DRAG_LISTENER);
    }

    /**
     * @return true if it is selected, false otherwise
     */
    public boolean isSelected() {
        return isSelected;
    }

    /**
     * @param selected The selected state to set
     */
    public void setSelected(boolean selected) {
        if (selected) {
            getView().requestFocus();
            getView().setStatusUI(StatusUI.SELECTED);
        } else {
            PaletteManagerView.get().getPanel().requestFocus();
            getView().setStatusUI(StatusUI.NON_SELECTED);
        }
        isSelected = selected;
    }

    /**
     * @return the parent palette (PaletteCtrl)
     */
    public PaletteCtrl getPaletteCtrl() {
        return paletteCtrl;
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
     * Get the loaded block.
     * @return basicblock
     */
    public BasicBlock getBlock() {
        if (basicBlock == null) {
            try {
                basicBlock = loadBlock();
            } catch (ScicosFormatException ex) {
                ScilabModalDialog.show(PaletteManagerView.get(), ex.getMessage(),
                        XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
            }
        }
        return basicBlock;
    }

    /**
     * @return the loaded block.
     * @throws ScicosFormatException error
     */
    // CSOFF: MultipleStringLiterals
    public BasicBlock loadBlock() throws ScicosFormatException {
        BasicBlock block = null;
        if (model.getName().compareTo("TEXT_f") == 0) {
            block = BlockFactory.createBlock(BlockInterFunction.TEXT_f);
        } else {
            // Load the block with a reference instance
            final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
            if (handler != null) {
                try {
                    synchronousScilabExec(ScilabDirectHandler.BLK + " = " + buildCall(model.getName(), "define"));
                    block = handler.readBlock();
                } catch (InterpreterException e1) {
                    LOG.severe(e1.toString());
                    block = null;
                    getView().setEnabled(false);
                } finally {
                    handler.release();
                }

                if (block != null && block.getStyle().compareTo("") == 0) {
                    block.setStyle(block.getInterfaceFunctionName());
                }
            }
        }

        if (block == null) {
            if (LOG.isLoggable(Level.FINEST)) {
                LOG.finest(String.format(XcosMessages.UNABLE_TO_LOAD_BLOCK,
                        getModel().getData().getEvaluatedPath()));
            }
            getView().setEnabled(false);
            throw new InvalidDnDOperationException();
        } else {
            getView().setEnabled(true);
        }

        return block;
    }

    /**
     * @param callback Called after the block loading
     */
    protected void loadBlock(final ActionListener callback) {
        if (model.getName().compareTo("TEXT_f") == 0) {
            final BasicBlock block = BlockFactory.createBlock(BlockInterFunction.TEXT_f);
            callback.actionPerformed(new ActionEvent(block, 0, "loaded"));
            return;
        }

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
            asynchronousScilabExec(internalCallback,
                    ScilabDirectHandler.BLK + " = " + buildCall(model.getName(), "define"));
        } catch (InterpreterException e1) {
            LOG.severe(e1.toString());
        } finally {
            handler.release();
        }
    }
}
