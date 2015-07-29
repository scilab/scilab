/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.listener;

import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeWillExpandListener;
import javax.swing.tree.ExpandVetoException;

/**
 * Implement the tree expand listener
 */
public class PaletteManagerTreeWillExpandListener implements TreeWillExpandListener {

    /**
     * Default constructor
     */
    public PaletteManagerTreeWillExpandListener() {
    }

    @Override
    public void treeWillCollapse(TreeExpansionEvent e)
            throws ExpandVetoException {
        JTree tree = (JTree) e.getSource();
        JSplitPane splitPanel = (JSplitPane) tree.getParent().getParent().getParent();
        splitPanel.resetToPreferredSizes();
    }

    @Override
    public void treeWillExpand(TreeExpansionEvent e)
            throws ExpandVetoException {
        JTree tree = (JTree) e.getSource();
        JSplitPane splitPanel = (JSplitPane) tree.getParent().getParent().getParent();
        splitPanel.resetToPreferredSizes();
    }

}
