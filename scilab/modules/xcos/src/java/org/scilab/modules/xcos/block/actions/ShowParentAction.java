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
            long[] parent = new long[1];
            Kind kind = Kind.BLOCK;
            controller.getObjectProperty(graph.getUID(), kind, ObjectProperties.PARENT_BLOCK, parent);
            if (parent[0] == 0) {
                kind = Kind.DIAGRAM;
                controller.getObjectProperty(graph.getUID(), kind, ObjectProperties.PARENT_DIAGRAM, parent);
            }

            XcosView view = (XcosView) JavaController.lookup_view(Xcos.class.getName());

            XcosDiagram diagram = (XcosDiagram) view.getVisibleObjects().get(parent[0]);
            if (diagram == null) {
                String[] strUID = new String[] { "" };
                controller.getObjectProperty(graph.getUID(), kind, ObjectProperties.UID, strUID);

                diagram = new XcosDiagram(controller, parent[0], kind, strUID[0]);
                view.getVisibleObjects().put(parent[0], diagram);
            }

            final XcosTab tab = XcosTab.get(diagram);
            if (tab == null) {
                XcosTab.restore(diagram);
            } else {
                tab.setCurrent();
            }
        }
    }
}
