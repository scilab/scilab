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

package org.scilab.modules.xcos.palette.model;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.MutableTreeNode;

import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.palette.Palette;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default model the PaletteManager
 */
public final class PaletteManagerModel {
	private PaletteManager controller;
	
	private DefaultTreeModel treeModel;
	private MutableTreeNode mainRoot;
	private MutableTreeNode userDefinedRoot;
	
	private Map<Object, DefaultMutableTreeNode> palettes; 
	
	/**
	 * Default constructor
	 * @param controller the associated controller
	 */
	public PaletteManagerModel(PaletteManager controller) {
		this.controller = controller;
		mainRoot = new DefaultMutableTreeNode(XcosMessages.PALETTES);
		((DefaultMutableTreeNode) mainRoot).setAllowsChildren(true);
		treeModel = new DefaultTreeModel(mainRoot);
		palettes = new HashMap<Object, DefaultMutableTreeNode>();
	}
	
	/**
	 * @return The tree model
	 */
	public DefaultTreeModel getTreeModel() {
		return treeModel;
	}

	/**
	 * @return The root of the user defined nodes
	 */
	public MutableTreeNode getUserDefinedRoot() {
		return userDefinedRoot;
	}

	/**
	 * @param diagram a user diagram to append on the tree 
	 */
	public void addUserDefinedNode(PaletteDiagram diagram) {
		if (userDefinedRoot == null) {
			userDefinedRoot = new DefaultMutableTreeNode(
					XcosMessages.USER_DEFINED);
			((DefaultMutableTreeNode) userDefinedRoot).setAllowsChildren(true);
		}
		
		if (userDefinedRoot.getParent() == null) {
			int nextRootIndex = mainRoot.getChildCount();
			treeModel.insertNodeInto(userDefinedRoot, mainRoot, nextRootIndex);
		}
		
		int nextUserIndex = userDefinedRoot.getChildCount();
		
		DefaultMutableTreeNode node;
		if (palettes.containsKey(diagram)) {
			node = palettes.get(diagram);
		} else {
			node = new DefaultMutableTreeNode(diagram.getAsComponent());
			node.setAllowsChildren(false);
			palettes.put(diagram, node);
		}
		treeModel.insertNodeInto(node, userDefinedRoot, nextUserIndex);
	}

	/**
	 * @param diagram
	 *            A user diagram to remove from the tree. if null, all the user
	 *            palettes will be removed
	 */
	public void removeUserDefinedNode(PaletteDiagram diagram) {
		assert userDefinedRoot != null;
		
		if (diagram == null) {
			// Clean up
			userDefinedRoot.removeFromParent();
			userDefinedRoot = null;
		} else {
			MutableTreeNode node = palettes.get(diagram);
			userDefinedRoot.remove(node);
			palettes.remove(diagram);
			
			// Clean up if needed
			if (userDefinedRoot.getChildCount() == 0) {
				userDefinedRoot.removeFromParent();
				userDefinedRoot = null;
			}
		}
	}
	
	/**
	 * @param diagram The key value
	 * @return The associated TreeNode
	 */
	public DefaultMutableTreeNode getUserNode(PaletteDiagram diagram) {
		return palettes.get(diagram);
	}
	
	/** Load the tree descriptor on the model*/
	public void loadTree() {
		/* Add the categories */
		final int length = Palette.getDatas().length;
		int disableCounter = 0;
		for (int i = 0; i < length; i++) {
			Palette p = Palette.getDatas()[i];
			
			DefaultMutableTreeNode node;
			if (!palettes.containsKey(p)) {
				node = new DefaultMutableTreeNode(p);
				node.setAllowsChildren(false);
				palettes.put(p, node);
			} else {
				node = palettes.get(p);
			}
			
			if (p.getModel().isEnable()) {
				mainRoot.insert(node, i - disableCounter);
			} else {
				disableCounter++;
			}
		}
		
		/* Add the user defined palette */
		List<String> files = ConfigXcosManager.getUserDefinedPalettes();
		for (String file : files) {
		    controller.loadUserPalette(file);
		}
	}
	
	/** Reload the three descriptor */
	public void reloadTree() {
		// Clean up the three and avoid child reordering.
		final int childCount = mainRoot.getChildCount(); 
		for (int i = childCount - 1; i >= 0; i--) {
			mainRoot.remove(i);
		}
		
		if (userDefinedRoot != null) {
			final int userCount = userDefinedRoot.getChildCount();
			for (int i = userCount - 1; i >= 0; i--) {
				userDefinedRoot.remove(i);
			}
		}
		
		// Load the entire tree
		loadTree();
		
		treeModel.reload();
	}
}
