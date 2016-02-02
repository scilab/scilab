/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.modules.renderer.JoGLView.editor;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import javax.swing.*;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import java.awt.*;

/**
 * @author Pierre Lando
 */
@SuppressWarnings(value = { "serial" })
public class FigureFrame extends JFrame {
    private final TreeModel treeModel;
    public FigureFrame(Integer id) {
        treeModel = new SciTreeModel(id);
        initialize();
    }

    private void initialize() {
        JTree tree = new JTree(treeModel);

        tree.setCellRenderer(new TreeCellRenderer() {
            private final TreeCellRenderer renderer = new DefaultTreeCellRenderer();
            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                Object type = GraphicController.getController().getProperty((Integer)value, GraphicObjectProperties.__GO_TYPE__);
                return renderer.getTreeCellRendererComponent(tree, type, selected, expanded, leaf, row, hasFocus);
            }
        });

        add(tree);
        setSize(400, 500);
        setVisible(true);
    }
}
