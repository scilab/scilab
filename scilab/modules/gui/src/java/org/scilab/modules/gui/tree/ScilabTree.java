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

import java.awt.Image;
import java.awt.Toolkit;
import java.util.Vector;

import javax.swing.Icon;
import javax.swing.ImageIcon;

import org.scilab.modules.gui.bridge.ScilabBridge;
import org.scilab.modules.gui.bridge.tree.ScilabTreeCellRenderer;
import org.scilab.modules.gui.events.callback.CallBack;

/**
 * Swing implementation for Scilab Tree in GUIs
 * @author Sylvestre KOUMAR
 */
public class ScilabTree implements Tree {
	
	private static final long serialVersionUID = 1L;
	
	private String label;
	private Icon icon;
	private CallBack callback;
	private Vector<Tree> children;
	
	public ScilabTree(String label, String iconPath, String callbackString) {
		this.label = label;
		
		// Creating an Iconimage with the image's path 
		Toolkit toolkit = Toolkit.getDefaultToolkit();
		if (iconPath.equals("default")) {
			this.icon = ScilabTreeCellRenderer.iconeDefaut;
		} else if (iconPath.equals("")) {
			this.icon = null;
		} else {
			Image image = (toolkit.getImage(iconPath)).getScaledInstance(25, 25, Image.SCALE_DEFAULT);
			this.icon = new ImageIcon(image);
		}
		
		// Creating a callback with the callback's string
		this.callback = CallBack.createCallback(callbackString, CallBack.SCILAB_FUNCTION);
		
		this.children = new Vector<Tree>();
	}
	
	/**
	 * GUI display of this tree 
	 */
	public void showTree() {
		ScilabBridge.showTree(this);		
	}

	/**
	 * Get label of a node
	 * @return node's label
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * Set label for a node
	 * @param label of the node
	 */
	public void setLabel(String label) {
		this.label = label;
	}

	/**
	 * Get icon of a node
	 * @return node's icon
	 */
	public Icon getIcon() {
		return icon;
	}

	/**
	 * Set icon for a node
	 * @param icon of the node
	 */
	public void setIcon(Icon icon) {
		this.icon = icon;
	}

	/**
	 * Get callback of a node
	 * @return node's callback
	 */
	public CallBack getCallback() {
		return callback;
	}

	/**
	 * Set callback for a node
	 * @param callback of the node
	 */
	public void setCallback(CallBack callback) {
		this.callback = callback;
	}	

	/**
	 * Get children of a tree
	 * @return children
	 */	
	public Vector<Tree> getChildren() {
		return children;
	}
	
	/**
	 * Add children for a tree
	 * @param firstChild of the tree
	 */
	public void addChild(Tree firstChild) {
		children.add(firstChild);
	}

	/**
	 * Gets this Bridge component object
	 * @return this Bridge component object
	 */
	public SimpleTree getAsSimpleTree() {
		//return component;
		return (SimpleTree) null;
	}

}
