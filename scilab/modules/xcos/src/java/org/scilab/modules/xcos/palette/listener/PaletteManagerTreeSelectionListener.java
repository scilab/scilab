/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

package org.scilab.modules.xcos.palette.listener;

import java.awt.Dimension;
import java.util.logging.Logger;

import javax.swing.JComponent;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Custom;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PaletteNode;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.palette.view.PaletteConfiguratorListView;
import org.scilab.modules.xcos.palette.view.PaletteConfiguratorListView.PaletteListModel;
import org.scilab.modules.xcos.palette.view.PaletteView;

/**
 * Implement the tree selection listener
 */
public class PaletteManagerTreeSelectionListener implements TreeSelectionListener {

    private final JScrollPane panel;

    /**
     * Default constructor
     *
     * @param panel
     *            The default scrollpane to modify
     */
    public PaletteManagerTreeSelectionListener(JScrollPane panel) {
        this.panel = panel;
    }

    /**
     * Selection handler
     *
     * @param tree
     *            The source event
     * @see javax.swing.event.TreeSelectionListener#valueChanged(javax.swing.event.TreeSelectionEvent)
     */
    @Override
    public void valueChanged(TreeSelectionEvent tree) {
        JTree component = (JTree) tree.getSource();
        JSplitPane splitPanel = (JSplitPane) component.getParent().getParent().getParent();

        PaletteNode node = (PaletteNode) component.getLastSelectedPathComponent();

        if (node == null) {
            // Nothing is selected.
            return;
        }

        JScrollPane nodeView = null;
        final Dimension dimension = splitPanel.getRightComponent().getPreferredSize();

        if (node instanceof Category) {
            final PaletteListModel model = new PaletteListModel((Category) node);
            final JComponent list = new PaletteConfiguratorListView(model);
            nodeView = new JScrollPane(list);
        } else if (node instanceof PreLoaded) {
            final PreLoaded palette = (PreLoaded) node;

            final PaletteView view = new PaletteView();
            for (PaletteBlock b : palette.getBlock()) {
                view.add(new PaletteBlockCtrl(b).getView());
            }

            panel.setViewportView(view);
            nodeView = panel;
        } else if (node instanceof Custom) {
            final Custom desc = (Custom) node;
            JavaController controller = new JavaController();
            PaletteDiagram diagram = new PaletteDiagram(controller.createObject(Kind.DIAGRAM));
            diagram.openDiagramAsPal(desc.getPath().getEvaluatedPath());
            nodeView = diagram.getAsComponent();
        } else {
            Logger.getLogger(PaletteManagerTreeSelectionListener.class.getName()).severe("tree selection is not handled");
            return;
        }

        // update
        nodeView.setPreferredSize(dimension);
        splitPanel.setRightComponent(nodeView);
        nodeView.validate();
    }

}
