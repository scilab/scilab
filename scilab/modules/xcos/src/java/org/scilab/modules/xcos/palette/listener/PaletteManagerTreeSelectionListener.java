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

import javax.swing.JComponent;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTree;
import javax.swing.event.TreeSelectionEvent;
import javax.swing.event.TreeSelectionListener;
import javax.swing.tree.DefaultMutableTreeNode;

import org.scilab.modules.xcos.palette.Palette;
import org.scilab.modules.xcos.palette.PaletteConfigurator;
import org.scilab.modules.xcos.palette.view.PaletteConfiguratorListView;
import org.scilab.modules.xcos.palette.view.PaletteView;
import org.scilab.modules.xcos.utils.XcosComponent;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the tree selection listener 
 */
public class PaletteManagerTreeSelectionListener implements
		TreeSelectionListener {

	private JScrollPane panel;
	
	/**
	 * Default constructor
	 * @param panel The default scrollpane to modify
	 */
	public PaletteManagerTreeSelectionListener(JScrollPane panel) {
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

		if (node == null) {
			// Nothing is selected.
			return;
		}

		JScrollPane nodeView;
		
		if (!node.getAllowsChildren()) {
		if (node.getUserObject() instanceof Palette) {
			// This is a statically configured Palette case
			PaletteView view = ((Palette) node.getUserObject()).loadView();
			view.setVisible(true);
			panel.setViewportView(view);
			
			nodeView = panel;
		} else {
			assert node.getUserObject() instanceof XcosComponent;
			// This is a loaded diagram as Palette
			nodeView = (XcosComponent) node.getUserObject();
		}
		} else {
			assert !node.isLeaf();
			assert node.getUserObject() instanceof String;
			String userObj = (String) node.getUserObject();
			
			JComponent list = new PaletteConfiguratorListView();
			
			if (userObj.compareTo(XcosMessages.PALETTES) == 0) {
				// Always instantiate all the default palettes
				for (Palette pal : Palette.getDatas()) {
					PaletteConfigurator p = new PaletteConfigurator(pal);
					list.add(p.getView());
				}
			} else {
				// when not selecting the default palettes node adding all the
				// sub nodes
				final int childrenCount = node.getChildCount();
				for (int i = 0; i < childrenCount; i++) {
					DefaultMutableTreeNode n = (DefaultMutableTreeNode) node
							.getChildAt(i);
					PaletteConfigurator p = new PaletteConfigurator(n
							.getUserObject());
					list.add(p.getView());
				}
			}
			
			nodeView = new JScrollPane(list);
		}
		
		// update
		splitPanel.setRightComponent(nodeView);
		nodeView.validate();
	}

}
