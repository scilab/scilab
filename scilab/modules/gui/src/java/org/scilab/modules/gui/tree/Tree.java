package org.scilab.modules.gui.tree;

import java.util.ArrayList;
import java.util.Vector;

import javax.swing.tree.DefaultMutableTreeNode;


/**
 * Interface for tree used in Scilab 
 * @author Sylvestre KOUMAR
 */
public interface Tree {
	
	
	public String getLabel();
	
	public void addChild(Tree firstChild);
	
	public Vector<Tree> getChildren();
	
	/**
	 * Create a node(subTree) 
	 * @param label text label of the node
	 * @param iconPath icon image of the node
	 * @param callback callback function of the node
	 * @return the created node
	 */
	DefaultMutableTreeNode createNode(String label, String iconPath, String callback);
	
	
	/**
	 * Create a Tree with node given in input 
	 * @param nodes will contain the tree
	 * @return the created tree
	 */
	DefaultMutableTreeNode createTree(DefaultMutableTreeNode[] nodes);
	
	
	/**
	 * Insert a node into a tree at the given position
	 * @param tree into we want to insert
	 * @param position of the insertion
	 * @param node we want to insert
	 * @return a tree with the node inserted
	 */
	DefaultMutableTreeNode insertNode(DefaultMutableTreeNode tree, String position, DefaultMutableTreeNode node);
	
	
	/**
	 * Insert a node into a tree at the given position
	 * @param tree into we want to insert
	 * @param parentNode of the node we want to insert
	 * @param node we want to insert
	 * @return a tree with the node inserted
	 */
	DefaultMutableTreeNode insertNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode parentNode, DefaultMutableTreeNode node);
	
	
	/**
	 * Delete a node at the given position from a tree 
	 * @param tree into we want to delete
	 * @param position of the deletion
	 * @return a tree without the deleted node
	 */
	DefaultMutableTreeNode deleteNode(DefaultMutableTreeNode tree, String position);
	
	
	/**
	 * Delete a node from a tree 
	 * @param tree into we want to delete
	 * @param node we want to delete
	 * @return a tree without the deleted node
	 */
	DefaultMutableTreeNode deleteNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode node);
	
	
	/**
	 * Text display of this tree 
	 * @param tree we want to display
	 * @param displayTreeDetail display or not of each node features (text display)
	 */
	void dumpTree(DefaultMutableTreeNode tree, boolean displayTreeDetail);
	
	
	/**
	 * GUI display of this tree 
	 * @param tree we want to display
	 */
	void showTree();
	
	
	/**
	 * Concatenate tree2 into tree1
	 * @param tree1 into we concatenate tree2
	 * @param tree2 concatenate into tree1
	 * @return a tree1 concatenated with tree2
	 */
	DefaultMutableTreeNode concatTree(DefaultMutableTreeNode tree1, DefaultMutableTreeNode tree2);
	
	
	/**
	 * Get the parent node of the node at position  
	 * @param tree into we want to get parent node
	 * @param position of the child node
	 * @return the parent node
	 */
	DefaultMutableTreeNode getParentNode(DefaultMutableTreeNode tree, String position);
	
	
	/**
	 * Get the parent node of a child node 
	 * @param tree into we want to get parent node
	 * @param node we want to get the parent
	 * @return the parent node
	 */
	DefaultMutableTreeNode getParentNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode node);
	
	
	/**
	 * Get children node(s) node of the node at position 
	 * @param tree into we want to get children node(s)
	 * @param position of the parent node
	 * @return children node(s)
	 */
	DefaultMutableTreeNode[] getChildrenNodes(DefaultMutableTreeNode tree, String position);
	
	
	/**
	 * Get children node(s) of a node 
	 * @param tree into we want to get children node(s)
	 * @param node we want to get children node(s)
	 * @return children node(s)
	 */
	DefaultMutableTreeNode[] getChildrenNodes(DefaultMutableTreeNode tree, DefaultMutableTreeNode node);
	
	
	/**
	 * Find the matching node 
	 * @param tree into we find
	 * @param position of finding node 
	 * @return founded node
	 */
	DefaultMutableTreeNode[] findNode(DefaultMutableTreeNode tree, String position);
	
	
	/**
	 * Find matching node(s) 
	 * @param tree into we find
	 * @param node we find 
	 * @return founded node(s)
	 */
	DefaultMutableTreeNode[] findNode(DefaultMutableTreeNode tree, DefaultMutableTreeNode node);
	
	
	/**
	 * Find matching node(s) 
	 * @param tree into we find
	 * @param property of the node
	 * @param value of the property
	 * @return founded node(s)
	 */
	DefaultMutableTreeNode[] findNode(DefaultMutableTreeNode tree, String property, String value);
	
	
	/**
	 * Get the position of a node 
	 * @param tree into we find the position
	 * @param node of the wanted position
	 * @return founded node(s)
	 */
	String[] getNodePosition(DefaultMutableTreeNode tree, DefaultMutableTreeNode node);



	
}
