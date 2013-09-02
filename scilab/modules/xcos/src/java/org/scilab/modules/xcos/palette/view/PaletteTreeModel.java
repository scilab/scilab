/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.view;

import java.util.List;

import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;

import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.Palette;
import org.scilab.modules.xcos.palette.model.PaletteNode;

/**
 * Wrap the PaletteNode on a specific model which handle the enable flag.
 */
@SuppressWarnings(value = { "serial" })
public class PaletteTreeModel extends DefaultTreeModel {
    private static final String ARG_EXCEPTION_MESSAGE = "The argument must be a PaletteNode instance";

    /**
     * {@link DefaultTreeModel} constructor
     *
     * @param root
     *            the associated node
     */
    public PaletteTreeModel(TreeNode root) {
        super(root);
        if (!(root instanceof PaletteNode)) {
            throw new IllegalArgumentException(ARG_EXCEPTION_MESSAGE);
        }
    }

    /**
     * Return the real child count
     *
     * @param parent
     *            the parent node
     * @return the number of enabled children
     * @see javax.swing.tree.DefaultTreeModel#getChildCount(java.lang.Object)
     */
    @Override
    public int getChildCount(Object parent) {
        if (!(parent instanceof PaletteNode)) {
            throw new IllegalArgumentException(ARG_EXCEPTION_MESSAGE);
        }

        /* Any Palette has no child */
        if (parent instanceof Palette) {
            return 0;
        }

        /* Any Category may have children and disabled children */
        final List<PaletteNode> children = ((Category) parent).getNode();
        int diff = 0;
        for (int i = 0; i < children.size(); i++) {
            if (!children.get(i).isEnable()) {
                diff++;
            }
        }

        return children.size() - diff;
    }

    /**
     * Return the child at the enable dependant index
     *
     * @param parent
     *            the parent
     * @param index
     *            the enable-dependant index
     * @return the associated node
     * @see javax.swing.tree.DefaultTreeModel#getChild(java.lang.Object, int)
     */
    @Override
    public Object getChild(Object parent, int index) {
        if (!(parent instanceof PaletteNode)) {
            throw new IllegalArgumentException(ARG_EXCEPTION_MESSAGE);
        }

        /* Any Palette has no child */
        if (parent instanceof Palette) {
            return null;
        }

        /* Any Category may have children and disabled children */
        final List<PaletteNode> children = ((Category) parent).getNode();
        int diff = 0;
        for (int i = 0; i < children.size() && i <= (index + diff); i++) {
            if (!children.get(i).isEnable()) {
                diff++;
            }
        }

        return children.get(index + diff);
    }
}
