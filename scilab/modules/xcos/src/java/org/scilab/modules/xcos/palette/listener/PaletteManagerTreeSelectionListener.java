/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.listener;

import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;

import org.scilab.modules.xcos.palette.Palette;
import org.scilab.modules.xcos.palette.view.PaletteManagerScrollPane;
import org.scilab.modules.xcos.palette.view.PaletteView;
import org.scilab.modules.xcos.utils.XcosComponent;

/**
 * Implement the tree selection listener 
 */
public class PaletteManagerTreeSelectionListener implements
		TreeSelectionListener {

	private PaletteManagerScrollPane panel;
	
	/**
	 * Default constructor
	 * @param panel The default scrollpane to modify
	 */
	public PaletteManagerTreeSelectionListener(PaletteManagerScrollPane panel) {
		this.panel = panel;
	}
	
	/**
	 * Selection handler
	 * @param tree The source event
	 * @see javax.swing.event.TreeSelectionListener#valueChanged(javax.swing.event.TreeSelectionEvent)
	 */
	public void valueChanged(TreeSelectionEvent tree) {
		JTree component = (JTree) tree.getSource();
		JSplitPane splitPanel = (JSplitPane) component.getParent();

		DefaultMutableTreeNode node = (DefaultMutableTreeNode) component
				.getLastSelectedPathComponent();

		if (node == null || node.getUserObject() instanceof String) {
			// Nothing is selected.
			return;
		}

		JScrollPane nodeView;
		if (node.getUserObject() instanceof Palette) {
			// This is a statically configured Palette case
			PaletteView view = ((Palette) node.getUserObject()).loadView();
			view.setVisible(true);
			panel.setContent(view);
			
			nodeView = panel;
		} else {
			// node.getUserObject() instanceof XcosComponent
			// This is a loaded diagram as Palette
			nodeView = (XcosComponent) node.getUserObject();
		}
		
		// update
		splitPanel.setRightComponent(nodeView);
		nodeView.validate();
	}

}
