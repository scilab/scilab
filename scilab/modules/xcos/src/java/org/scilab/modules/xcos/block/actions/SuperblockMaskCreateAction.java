/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;
import java.util.Arrays;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Create a mask for the {@link SuperBlock}
 */
@SuppressWarnings(value = { "serial" })
public final class SuperblockMaskCreateAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.CREATE;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    /**
     * @param scilabGraph
     *            graph
     */
    public SuperblockMaskCreateAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, SuperblockMaskCreateAction.class);
    }

    /**
     * Callback to be done
     *
     * @param e
     *            parameters
     * @see org.scilab.modules.gui.events.callback.CallBack#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        SuperBlock block = (SuperBlock) graph.getSelectionCell();

        block.mask();

        /*
         * Create a valid DSUPER exprs field if not already present.
         */
        if (!(block.getExprs() instanceof ScilabList)) {

            /* Set default values */
            ScilabList exprs = new ScilabList(
                Arrays.asList(
                    new ScilabDouble(),
                    new ScilabList(
                        Arrays.asList(
                            new ScilabDouble(),
                            new ScilabString(
                                XcosMessages.MASK_DEFAULTWINDOWNAME),
                            new ScilabList(Arrays
                                           .asList(new ScilabDouble()))))));

            block.setExprs(exprs);

            /*
             * Open the customization UI on a new mask creation
             */
            GraphActionManager.getInstance(graph,
                                           SuperblockMaskCustomizeAction.class).actionPerformed(e);
        }
    }
}
