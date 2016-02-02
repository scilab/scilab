/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
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
