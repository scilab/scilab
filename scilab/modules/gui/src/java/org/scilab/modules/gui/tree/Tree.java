/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.tree;

import java.util.Vector;

import javax.swing.Icon;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.widget.Widget;


/**
 * Interface for tree used in Scilab 
 * @author Sylvestre KOUMAR
 */
public interface Tree extends Widget {	
	
	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	SimpleTree getAsSimpleTree();
	
	/**
	 * Get the label of a tree node
	 * @return label of a node
	 */
	public String getLabel();
	
	/**
	 * Get the icon image of a tree node
	 * @return icon of a node
	 */
	public Icon getIcon();
	
	/**
	 * Get the callback of a tree node
	 * @return callback of a node
	 */
	public CallBack getCallback();
	
	/**
	 * Add child to a tree
	 * @param firstChild to add
	 */
	public void addChild(Tree firstChild);
	
	/**
	 * Get children of a tree
	 * @return vector of children
	 */
	public Vector<Tree> getChildren();

	/**
	 * Display a tree
	 */
	public void showTree();
	
}
