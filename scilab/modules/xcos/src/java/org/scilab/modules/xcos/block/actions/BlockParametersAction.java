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

package org.scilab.modules.xcos.block.actions;

import java.awt.Cursor;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.asynchronousScilabExec;

/**
 * Open dialog to set block parameters
 */
@SuppressWarnings(value = { "serial" })
public class BlockParametersAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.BLOCK_PARAMETERS;
    /** Icon name of the action */
    public static final String SMALL_ICON = "applications-system";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_B;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit()
            .getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public BlockParametersAction(ScilabGraph scilabGraph) {
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
        return createMenu(scilabGraph, BlockParametersAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        actionPerformed();
    }

    public void actionPerformed() {
        XcosDiagram graph = (XcosDiagram) getGraph(null);
        Object selectedCell = graph.getSelectionCell();
        if (selectedCell != null && selectedCell instanceof XcosCell) {
            XcosCell cell = (XcosCell) selectedCell;

            if (cell.getKind() != Kind.BLOCK) {
                return;
            }

            final JavaController controller = new JavaController();

            String[] interfaceFunction = new String[1];
            controller.getObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

            BlockInterFunction func = XcosCellFactory.lookForInterfunction(interfaceFunction[0]);
            if (func.equals(BlockInterFunction.SUPER_f)) {
                // this is a super-block, open it
                XcosDiagram sub = new XcosDiagram(controller, cell.getUID(), cell.getKind(), cell.getId());
                XcosCellFactory.insertChildren(controller, sub);

                Xcos.getInstance().addDiagram(sub);
                XcosTab.restore(sub, true);
            } else {
                BasicBlock block = (BasicBlock) cell;
                // prevent to open twice
                if (block.isLocked()) {
                    return;
                }

                graph.setCellsLocked(true);
                graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

                try {

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
                                block.setLocked(false);

                                graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                                graph.setCellsLocked(false);
                            }
                        }
                    };

                    ScilabDirectHandler handler = ScilabDirectHandler.acquire();
                    try {
                        handler.writeContext(graph.getContext());
                    } finally {
                        handler.release();
                    }

                    block.setLocked(true);
                    String blk = buildCall("scicos_new", Long.toHexString(cell.getUID()));
                    String xcosBlockInterface = buildCall("xcosBlockInterface", interfaceFunction[0].toCharArray(), "set", blk.toCharArray(), ScilabDirectHandler.CONTEXT.toCharArray());

                    asynchronousScilabExec(action, "xcosCellCreated", xcosBlockInterface.toCharArray());
                } catch (InterpreterException ex) {
                    block.setLocked(false);

                    graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                    graph.setCellsLocked(false);
                }
            }
        }
    }
}
