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

import java.awt.Component;
import java.awt.Image;
import java.awt.Toolkit;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JTree;
import javax.swing.tree.DefaultTreeCellRenderer;

public class ScilabTreeCellRenderer extends DefaultTreeCellRenderer {

    // Images
    public static Toolkit toolkit = Toolkit.getDefaultToolkit();
    public static Image plus = toolkit.getImage(System.getenv("SCI")+"/modules/gui/images/icons/list-add.png");
    public static Image minus = toolkit.getImage(System.getenv("SCI")+"/modules/gui/images/icons/list-remove.png");
    public static Image scilab = toolkit.getImage(System.getenv("SCI")+"/modules/gui/images/icons/scilab.png");
    public static Image puff = toolkit.getImage(System.getenv("SCI")+"/modules/gui/images/icons/puff.png");

    public static Image myNewPlus = plus.getScaledInstance(25, 25, Image.SCALE_DEFAULT);
    public static Image myNewMinus = minus.getScaledInstance(25, 25, Image.SCALE_DEFAULT);
    public static Image myNewScilab = scilab.getScaledInstance(25, 25, Image.SCALE_DEFAULT);
    public static Image myNewPuff = puff.getScaledInstance(25, 25, Image.SCALE_DEFAULT);

    public static Icon iconPlus = new ImageIcon(myNewPlus);
    public static Icon iconMinus = new ImageIcon(myNewMinus);        
    public static Icon iconScilab = new ImageIcon(myNewScilab);
    public static Icon iconPuff = new ImageIcon(myNewPuff);

    public static Image defaut = toolkit.getImage("");
    public static Image myNewDefaut = defaut.getScaledInstance(25, 25, Image.SCALE_DEFAULT);
    public static Icon iconDefaut = new ImageIcon(myNewDefaut);


    private static final long serialVersionUID = 1L;

    /**
     * Default Constructor
     */
    public ScilabTreeCellRenderer () {
	super();
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

	SwingScilabTree node = (SwingScilabTree) value;	

	// Case of leaf
	if(leaf){
	    // Test the kind of selected component in the tree 
	    if (node instanceof SwingScilabTree) {
		// Set icon for the type of leaf
		if (node.getIcon() != null) {
		    if (node.getIcon() == iconDefaut){				
			this.setLeafIcon(iconScilab);
		    } else {	
			this.setLeafIcon(node.getIcon());
		    }
		} else {				
		    this.setLeafIcon(null);
		}
	    }
	}

	// Case of node
	if(expanded){
	    // Node is opened
	    if (node.getIcon() != null) {
		if (node.getIcon() == iconDefaut) {
		    this.setOpenIcon(iconMinus);
		} else {
		    this.setOpenIcon(node.getIcon());
		}
	    } else {
		this.setOpenIcon(null);
	    }				
	}
	else{
	    // Node is closed
	    if (node.getIcon() != null) {
		if (node.getIcon() == iconDefaut) {
		    this.setClosedIcon(iconPlus);
		} else {
		    this.setClosedIcon(node.getIcon());
		}
	    } else {
		this.setClosedIcon(null);
	    }
	}

	// Calling super class
	super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf,row, hasFocus);
	return this;
    }

}
