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

import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.MutableTreeNode;

import org.scilab.modules.xcos.palette.Palette;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Implement the default model the PaletteManager
 */
public final class PaletteManagerModel {
	private DefaultTreeModel treeModel;
	private MutableTreeNode mainRoot;
	private MutableTreeNode userDefinedRoot;
	
	/**
	 * Default constructor
	 */
	public PaletteManagerModel() {
		mainRoot = new DefaultMutableTreeNode(XcosMessages.PALETTES);
		treeModel = new DefaultTreeModel(mainRoot);
		userDefinedRoot = new DefaultMutableTreeNode(XcosMessages.USER_DEFINED);
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

	/** Load the tree descriptor on the model*/
	public void loadTree() {
		/* Add the categories */
		final int length = Palette.getDatas().length;
		for (int i = 0; i < length; i++) {
			mainRoot.insert(new DefaultMutableTreeNode(Palette.getDatas()[i]), i);
		}
	}
}
