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

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.XcosView;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Show the parent graph (only installed on a {@link SuperBlockDiagram})
 */
@SuppressWarnings(value = { "serial" })
public class ShowParentAction extends DefaultAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.SHOW_PARENT;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_HOME;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();

    /**
     * Constructor
     *
     * @param scilabGraph
     *            associated diagram
     */
    public ShowParentAction(ScilabGraph scilabGraph) {
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
        return createMenu(scilabGraph, ShowParentAction.class);
    }

    /**
     * @param e
     *            parameter
     * @see org.scilab.modules.graph.actions.base.DefaultAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        if (graph.getKind() == Kind.BLOCK) {
            JavaController controller = new JavaController();

            XcosCell defaultParent = (XcosCell) graph.getDefaultParent();
            XcosDiagram parentGraph = Xcos.findParent(controller, defaultParent.getUID(), defaultParent.getKind());

            // the parent graph is not visible yet, load it into the UI
            if (parentGraph == null) {
                long[] parent = {0};
                Kind parentKind = Kind.BLOCK;

                // use parent / children model property
                controller.getObjectProperty(defaultParent.getUID(), defaultParent.getKind(), ObjectProperties.PARENT_BLOCK, parent);
                if (parent[0] == 0) {
                    parentKind = Kind.DIAGRAM;
                    controller.getObjectProperty(defaultParent.getUID(), defaultParent.getKind(), ObjectProperties.PARENT_DIAGRAM, parent);
                }

                parentGraph = new XcosDiagram(controller, parent[0], parentKind, "");
                XcosCellFactory.insertChildren(controller, parentGraph);
                parentGraph.installListeners();

                Xcos.getInstance().addDiagram(Xcos.findRoot(controller, graph), parentGraph);
            }

            // restore the parent graph tab
            final XcosTab tab = XcosTab.get(parentGraph);
            if (tab == null) {
                XcosTab.restore(parentGraph);
            } else {
                tab.setCurrent();
                tab.requestFocus();
            }
        }
    }
}
