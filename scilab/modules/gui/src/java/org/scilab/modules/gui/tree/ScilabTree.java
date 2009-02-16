package org.scilab.modules.gui.tree;

import java.awt.Component;
import java.util.ArrayList;
import java.util.Vector;

import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;

import org.scilab.modules.gui.bridge.ScilabBridge;



/**
 * Swing implementation for Scilab Tree in GUIs
 * @author Sylvestre KOUMAR
 */
public class ScilabTree implements Tree {
	
	private String label;
	private String icon;
	private String callback;
	private Vector<Tree> children;
	
	public ScilabTree(String label, String icon, String callback) {
		this.label = label;
		this.icon = icon;
		this.callback = callback;
		this.children = new Vector<Tree>();
	}

	private static final long serialVersionUID = 1L;
	

	/**
	 * Concatenate tree2 into tree1
	 * @param tree1 into we concatenate tree2
	 * @param tree2 concatenate into tree1
	 * @return a tree1 concatenated with tree2
	 */
	public DefaultMutableTreeNode concatTree(DefaultMutableTreeNode tree1, DefaultMutableTreeNode tree2) {
		return null;
	}

	/**
	 * Create a node(subTree) 
	 * @param label text label of the node
	 * @param iconPath icon image of the node
	 * @param callback callback function of the node
	 * @return the created node
	 */
	public DefaultMutableTreeNode createNode(String label, String iconPath, String callback) {
		return null;
	}

	/**
	 * Create a Tree with node given in input 
	 * @param nodes will contain the tree
	 * @return the created tree
	 */
	public DefaultMutableTreeNode createTree(DefaultMutableTreeNode[] nodes) {
		return null;
	}

	/**
	 * Delete a node at the given position from a tree 
	 * @param tree into we want to delete
	 * @param position of the deletion
	 * @return a tree without the deleted node
	 */
	public DefaultMutableTreeNode deleteNode(DefaultMutableTreeNode tree, String position) {
		return null;
	}

	/**
	 * Delete a node from a tree 
	 * @param tree into we want to delete
	 * @param node we want to delete
	 * @return a tree without the deleted node
	 */
	public DefaultMutableTreeNode deleteNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * Text display of this tree 
	 * @param tree we want to display
	 * @param displayTreeDetail display or not of each node features (text display)
	 */
	public void dumpTree(DefaultMutableTreeNode tree, boolean displayTreeDetail) {
		
	}

	/**
	 * Find the matching node 
	 * @param tree into we find
	 * @param position of finding node 
	 * @return founded node
	 */
	public DefaultMutableTreeNode[] findNode(DefaultMutableTreeNode tree, String position) {
		return null;
	}

	/**
	 * Find matching node(s) 
	 * @param tree into we find
	 * @param node we find 
	 * @return founded node(s)
	 */
	public DefaultMutableTreeNode[] findNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * Find matching node(s) 
	 * @param tree into we find
	 * @param property of the node
	 * @param value of the property
	 * @return founded node(s)
	 */
	public DefaultMutableTreeNode[] findNode(DefaultMutableTreeNode tree, String property, String value) {
		return null;
	}

	/**
	 * Get children node(s) node of the node at position 
	 * @param tree into we want to get children node(s)
	 * @param position of the parent node
	 * @return children node(s)
	 */
	public DefaultMutableTreeNode[] getChildrenNodes(DefaultMutableTreeNode tree, String position) {
		return null;
	}

	/**
	 * Get children node(s) of a node 
	 * @param tree into we want to get children node(s)
	 * @param node we want to get children node(s)
	 * @return children node(s)
	 */
	public DefaultMutableTreeNode[] getChildrenNodes(DefaultMutableTreeNode tree, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * Get the position of a node 
	 * @param tree into we find the position
	 * @param node of the wanted position
	 * @return founded node(s)
	 */
	public String[] getNodePosition(DefaultMutableTreeNode tree, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * Get the parent node of the node at position  
	 * @param tree into we want to get parent node
	 * @param position of the child node
	 * @return the parent node
	 */
	public DefaultMutableTreeNode getParentNode(DefaultMutableTreeNode tree, String position) {
		return null;
	}

	/**
	 * Get the parent node of a child node 
	 * @param tree into we want to get parent node
	 * @param node we want to get the parent
	 * @return the parent node
	 */
	public DefaultMutableTreeNode getParentNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * Insert a node into a tree at the given position
	 * @param tree into we want to insert
	 * @param position of the insertion
	 * @param node we want to insert
	 * @return a tree with the node inserted
	 */
	public DefaultMutableTreeNode insertNode(DefaultMutableTreeNode tree, String position, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * Insert a node into a tree at the given position
	 * @param tree into we want to insert
	 * @param parentNode of the node we want to insert
	 * @param node we want to insert
	 * @return a tree with the node inserted
	 */
	public DefaultMutableTreeNode insertNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode parentNode, DefaultMutableTreeNode node) {
		return null;
	}

	/**
	 * GUI display of this tree 
	 * @param tree we want to display
	 */
	public void showTree() {
		ScilabBridge.showTree(this);
		
	}

	/**
	 * Get tree renderer 
	 * @param tree we want to display
	 * @param value of the node
	 * @param sel is selected
	 * @param expanded is expanded
	 * @param leaf is a leaf
	 * @param row of the tree
	 * @param hasFocus 
	 * @return component renderer
	 */
	public Component getTreeCellRendererComponent(JTree tree, Object value, 
			  									  boolean sel, boolean expanded, boolean leaf, 
			  									  int row, boolean hasFocus)  {	
		return null;
	}

	public String getLabel() {
		return label;
	}

	public void setLabel(String label) {
		this.label = label;
	}

	public void addChild(Tree firstChild) {
		children.add(firstChild);
		
	}

	public Vector<Tree> getChildren() {
		return children;
	}

}
