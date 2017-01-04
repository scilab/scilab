/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2017 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * Copyright (C) 2012 - 2017 - Scilab Enterprises
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

import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.event.KeyListener;
import java.awt.event.MouseListener;

import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.palette.PaletteCtrl;
import org.scilab.modules.xcos.palette.listener.PaletteBlockKeyListener;
import org.scilab.modules.xcos.palette.listener.PaletteBlockMouseListener;
import org.scilab.modules.xcos.palette.listener.PaletteDragGestureListener;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteBlockView.StatusUI;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.util.mxGraphTransferable;

/**
 * A palette block is the representation of the block in the palette.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 * @author Clement DAVID
 */
public final class PaletteBlockCtrl {

    private static final DragGestureListener DRAG_LISTENER = new PaletteDragGestureListener();
    private static final KeyListener KEY_LISTENER = new PaletteBlockKeyListener();
    private static final MouseListener MOUSE_LISTENER = new PaletteBlockMouseListener();

    private static final String UNABLE_TO_LOAD_BLOCK = Messages.gettext("Unable to load block from %s .");
    private static final String LOADING_THE_BLOCK = Messages.gettext("Loading the block") + XcosMessages.DOTS;

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
        dragSource.createDefaultDragGestureRecognizer(this.getView(), DnDConstants.ACTION_COPY, DRAG_LISTENER);
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
            basicBlock = (BasicBlock) ((mxGraphTransferable) paletteCtrl.getTransferable()).getCells()[0];
        }
        return basicBlock;
    }
}
