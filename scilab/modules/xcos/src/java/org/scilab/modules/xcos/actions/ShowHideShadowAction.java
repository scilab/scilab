/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.xcos.actions;

import java.awt.event.ActionEvent;
import java.util.Map;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

/**
 * Block shadow handling
 */
@SuppressWarnings(value = { "serial" })
public final class ShowHideShadowAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.SHOWHIDE_SHADOW;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public ShowHideShadowAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu added to the menubar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, ShowHideShadowAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        if (graph.getSelectionCells().length != 0) {

            Object[] allCells = graph.getSelectionCells();

            for (int i = 0; i < allCells.length; ++i) {
                if (allCells[i] instanceof BasicBlock) {
                    // ((BasicBlock) allCells[i])
                    mxCellState state = graph.getView().getState(
                                            allCells[i]);
                    Map<String, Object> style;
                    if (state != null) {
                        style = state.getStyle();
                    } else {
                        style = graph.getCellStyle(allCells[i]);
                    }

                    if (style != null) {
                        String value = Boolean.toString(mxUtils.isTrue(style,
                                                        mxConstants.STYLE_SHADOW, false));
                        graph.setCellStyles(mxConstants.STYLE_SHADOW,
                                            value, new Object[] { allCells[i] });
                    }
                }
            }
        }
    }

}
