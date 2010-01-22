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
	
	private Map<PaletteDiagram, DefaultMutableTreeNode> userDiagrams; 
	
	/**
	 * Default constructor
	 * @param controller the associated controller
	 */
	public PaletteManagerModel(PaletteManager controller) {
		this.controller = controller;
		mainRoot = new DefaultMutableTreeNode(XcosMessages.PALETTES);
		treeModel = new DefaultTreeModel(mainRoot);
		userDiagrams = new HashMap<PaletteDiagram, DefaultMutableTreeNode>();
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
			int nextRootIndex = mainRoot.getChildCount();
			userDefinedRoot = new DefaultMutableTreeNode(XcosMessages.USER_DEFINED);
			treeModel.insertNodeInto(userDefinedRoot, mainRoot, nextRootIndex);
		}
		int nextUserIndex = userDefinedRoot.getChildCount();
		DefaultMutableTreeNode node = new DefaultMutableTreeNode(diagram.getAsComponent());
		userDiagrams.put(diagram, node);
		treeModel.insertNodeInto(node, userDefinedRoot, nextUserIndex);
	}

	/**
	 * @param diagram
	 *            A user diagram to remove from the tree. if null, all the user
	 *            diagrams will be removed
	 */
	public void removeUserDefinedNode(PaletteDiagram diagram) {
		assert userDefinedRoot != null;
		
		if (diagram == null) {
			// Clean up
			userDefinedRoot.removeFromParent();
			userDefinedRoot = null;
		} else {
			MutableTreeNode node = userDiagrams.get(diagram);
			userDefinedRoot.remove(node);
			userDiagrams.remove(diagram);
			
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
		return userDiagrams.get(diagram);
	}
	
	/** Load the tree descriptor on the model*/
	public void loadTree() {
		/* Add the categories */
		final int length = Palette.getDatas().length;
		int disableCounter = 0;
		for (int i = 0; i < length; i++) {
			Palette p = Palette.getDatas()[i];
			if (p.getModel().isEnable()) {
				mainRoot.insert(new DefaultMutableTreeNode(p), i - disableCounter);
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
}
