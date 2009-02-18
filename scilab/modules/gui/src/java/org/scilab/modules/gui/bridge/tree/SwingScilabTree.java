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

package org.scilab.modules.gui.bridge.tree;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.util.Vector;

import javax.swing.Icon;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.tree.SimpleTree;
import org.scilab.modules.gui.tree.Tree;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;

public class SwingScilabTree extends DefaultMutableTreeNode implements SimpleTree {

	private static final long serialVersionUID = 1L;
	
	private Icon icon;
	private CallBack callback;
	
	
	/**
	 * Default constructor
	 * @param tree structure to display
	 */
	public SwingScilabTree(Tree tree) {
		super(tree.getLabel());
		this.setIcon(tree.getIcon());
		this.setCallback(tree.getCallback());
		
		Vector<Tree> children = tree.getChildren();
		
		for (int i = 0 ; i < children.size() ; ++i)	{
			SwingScilabTree swingScilabTree = new SwingScilabTree(children.get(i));
			this.add(swingScilabTree);
		}
	}
	
	public Icon getIcon() {
		return icon;
	}

	public void setIcon(Icon icon) {
		this.icon = icon;
	}
	
	public CallBack getCallback() {
		return callback;
	}

	public void setCallback(CallBack callback) {
		this.callback = callback;
	}
	
	public Component getAsComponent() {
		// Tree Model
		DefaultTreeModel model = new DefaultTreeModel(this);		
		// Renderer
		ScilabTreeCellRenderer renderer = new ScilabTreeCellRenderer();
        // Swing tree component
		JTree jtree = new JTree();	
		// Show Root Handles
		jtree.setShowsRootHandles(true);
		// Set model to the JTree
		jtree.setModel(model);		
		// Set renderer
		jtree.setCellRenderer(renderer);
		
		jtree.setVisible(true);
		return jtree;
	}
	
	public static void  showTree(Tree tree) {
		
		// Scilab tree
		SwingScilabTree swingScilabTree = new SwingScilabTree(tree);
		
		SwingScilabWindow window = new SwingScilabWindow();
		SwingScilabTab tab = new SwingScilabTab("Tree example", 666);
		tab.addTree(swingScilabTree);
		window.add(tab);
		tab.setVisible(true);
		window.setVisible(true);
	}

	public void destroy() {
		// TODO Auto-generated method stub
		
	}

	public Color getBackground() {
		// TODO Auto-generated method stub
		return null;
	}

	public Font getFont() {
		// TODO Auto-generated method stub
		return null;
	}

	public Color getForeground() {
		// TODO Auto-generated method stub
		return null;
	}

	public String getText() {
		// TODO Auto-generated method stub
		return null;
	}

	public boolean isEnabled() {
		// TODO Auto-generated method stub
		return false;
	}

	public void requestFocus() {
		// TODO Auto-generated method stub
		
	}

	public void setBackground(Color color) {
		// TODO Auto-generated method stub
		
	}

	public void setEnabled(boolean status) {
		// TODO Auto-generated method stub
		
	}

	public void setFont(Font font) {
		// TODO Auto-generated method stub
		
	}

	public void setForeground(Color color) {
		// TODO Auto-generated method stub
		
	}

	public void setHorizontalAlignment(String alignment) {
		// TODO Auto-generated method stub
		
	}

	public void setRelief(String reliefType) {
		// TODO Auto-generated method stub
		
	}

	public void setText(String text) {
		// TODO Auto-generated method stub
		
	}

	public void setVerticalAlignment(String alignment) {
		// TODO Auto-generated method stub
		
	}

	public void addInfoBar(TextBox infoBarToAdd) {
		// TODO Auto-generated method stub
		
	}

	public void addMenuBar(MenuBar menuBarToAdd) {
		// TODO Auto-generated method stub
		
	}

	public void addToolBar(ToolBar toolBarToAdd) {
		// TODO Auto-generated method stub
		
	}

	public void draw() {
		// TODO Auto-generated method stub
		
	}

	public Size getDims() {
		// TODO Auto-generated method stub
		return null;
	}

	public TextBox getInfoBar() {
		// TODO Auto-generated method stub
		return null;
	}

	public MenuBar getMenuBar() {
		// TODO Auto-generated method stub
		return null;
	}

	public Position getPosition() {
		// TODO Auto-generated method stub
		return null;
	}

	public ToolBar getToolBar() {
		// TODO Auto-generated method stub
		return null;
	}

	public boolean isVisible() {
		// TODO Auto-generated method stub
		return false;
	}

	public void setDims(Size newSize) {
		// TODO Auto-generated method stub
		
	}

	public void setPosition(Position newPosition) {
		// TODO Auto-generated method stub
		
	}

	public void setVisible(boolean newVisibleState) {
		// TODO Auto-generated method stub
		
	}
}
