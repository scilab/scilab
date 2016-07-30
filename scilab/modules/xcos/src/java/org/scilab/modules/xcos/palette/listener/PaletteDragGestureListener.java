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

package org.scilab.modules.xcos.palette.listener;

import java.awt.Point;
import java.awt.datatransfer.Transferable;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.InvalidDnDOperationException;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.PaletteCtrl;
import org.scilab.modules.xcos.palette.view.PaletteBlockView;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Install the handler for dragging nodes into a graph.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public final class PaletteDragGestureListener implements DragGestureListener {

    /**
     * Default constructor
     */
    public PaletteDragGestureListener() {
    }

    /**
     * @param e The associated event
     */
    @Override
    public void dragGestureRecognized(DragGestureEvent e) {
        PaletteBlockCtrl blockCtrl = ((PaletteBlockView) e.getComponent()).getController();
        PaletteCtrl paletteCtrl = blockCtrl.getPaletteCtrl();

        if (paletteCtrl == null) {
            return;
        }

        if (!blockCtrl.isSelected()) {
            PaletteCtrl.clearSelections();
        }
        blockCtrl.setSelected(true);

        if (PaletteManagerView.get() == null) {
            PaletteManagerView.restore(null);
        }
        final PaletteManagerView winView = PaletteManagerView.get();

        winView.setInfo(XcosMessages.LOADING_BLOCKS);
        try {
            Transferable transfer = paletteCtrl.getTransferable();
            if (transfer != null) {
                e.startDrag(null, null, new Point(), transfer, null);
            }
        } catch (InvalidDnDOperationException exception) {
            ScilabModalDialog.show(winView, XcosMessages.UNABLE_TO_LOAD_SELECTED_BLOCKS,
                                   XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
        } finally {
            winView.setInfo(XcosMessages.EMPTY_INFO);
        }
    }
}
