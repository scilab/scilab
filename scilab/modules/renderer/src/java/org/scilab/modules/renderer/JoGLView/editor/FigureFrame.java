/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    public FigureFrame(String id) {
        treeModel = new SciTreeModel(id);
        initialize();
    }

    private void initialize() {
        JTree tree = new JTree(treeModel);

        tree.setCellRenderer(new TreeCellRenderer() {
            private final TreeCellRenderer renderer = new DefaultTreeCellRenderer();
            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                Object type = GraphicController.getController().getProperty((String) value, GraphicObjectProperties.__GO_TYPE__);
                return renderer.getTreeCellRendererComponent(tree, type, selected, expanded, leaf, row, hasFocus);
            }
        });

        add(tree);
        setSize(400, 500);
        setVisible(true);
    }
}
