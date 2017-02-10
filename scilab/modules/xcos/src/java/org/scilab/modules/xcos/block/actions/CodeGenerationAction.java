/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan SIMON
 * Copyright (C) 2010-2011 - DIGITEO - Clement DAVID
 * Copyright (C) 2012-2016 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.actions;

import java.awt.Cursor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import org.scilab.modules.graph.ScilabComponent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Generate code for the current graph.
 */
@SuppressWarnings(value = { "serial" })
public class CodeGenerationAction extends SuperBlockSelectedAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.CODE_GENERATION;
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
     *            associated Scilab graph
     */
    public CodeGenerationAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * Menu for diagram menubar
     *
     * @param scilabGraph
     *            associated diagram
     * @return the menu
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, CodeGenerationAction.class);
    }

    /**
     * Action !!!
     *
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

        Object selectedObj = graph.getSelectionCell();
        if (!(selectedObj instanceof SuperBlock)) {
            graph.error(XcosMessages.ERROR_GENERATING_C_CODE);
            return;
        }

        graph.info(XcosMessages.GENERATING_C_CODE);
        graph.setCellsLocked(true);
        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

        final JavaController controller = new JavaController();
        final SuperBlock block = (SuperBlock) selectedObj;

        try {
            /*
             * Export data
             */
            String blk = buildCall("scicos_new", Long.toHexString(block.getUID()));

            /*
             * Prepare command and callback
             */
            String cmd = buildCall("blk = xcosCodeGeneration", blk.toCharArray());

            final ActionListener action = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {

                    graph.getView().clear(this, true, true);

                    // Now read new Block
                    graph.getModel().beginUpdate();
                    try {
                        ScicosObjectOwner last = XcosCellFactory.getLastCreated();
                        if (last != null && last.getUID() != 0l) {
                            BasicBlock modified = XcosCellFactory.createBlock(controller, last);
                            if (modified != null) {
                                block.updateBlockSettings(controller, graph, modified);
                            }
                        }
                        BlockPositioning.updateBlockView(graph, block);
                    } finally {
                        graph.getModel().endUpdate();

                        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                        graph.setCellsLocked(false);
                        graph.info(XcosMessages.EMPTY_INFO);
                    }
                }
            };

            /*
             * Execute
             */
            asynchronousScilabExec(action, cmd);

        } catch (InterpreterException ex) {
            graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
            graph.setCellsLocked(false);
            graph.info(XcosMessages.EMPTY_INFO);
        }
    }
}
