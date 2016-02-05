/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

import org.scilab.modules.commons.gui.FindIconHelper;

/**
 * Class define the Scilab tree renderer
 * @author Sylvestre Koumar
 *
 */
public class ScilabTreeCellRenderer extends DefaultTreeCellRenderer {

    private static final int IMAGE_SIZE = 25;

    /**
     * Images
     */
    public static Toolkit toolkit = Toolkit.getDefaultToolkit();
    public static Image plus = toolkit.getImage(FindIconHelper.findIcon("list-add"));
    public static Image minus = toolkit.getImage(FindIconHelper.findIcon("list-remove"));
    public static Image scilab = toolkit.getImage(FindIconHelper.findIcon("scilab"));

    public static Image myNewPlus = plus.getScaledInstance(IMAGE_SIZE, IMAGE_SIZE, Image.SCALE_DEFAULT);
    public static Image myNewMinus = minus.getScaledInstance(IMAGE_SIZE, IMAGE_SIZE, Image.SCALE_DEFAULT);
    public static Image myNewScilab = scilab.getScaledInstance(IMAGE_SIZE, IMAGE_SIZE, Image.SCALE_DEFAULT);

    public static Icon iconPlus = new ImageIcon(myNewPlus);
    public static Icon iconMinus = new ImageIcon(myNewMinus);
    public static Icon iconScilab = new ImageIcon(myNewScilab);

    public static Image defaut = toolkit.getImage("");
    public static Image myNewDefaut = defaut.getScaledInstance(IMAGE_SIZE, IMAGE_SIZE, Image.SCALE_DEFAULT);
    public static Icon iconDefaut = new ImageIcon(myNewDefaut);


    private static final long serialVersionUID = 1L;

    /**
     * Default Constructor
     */
    public ScilabTreeCellRenderer() {
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
     * @param hasFocus boolean
     * @return component renderer
     */
    @Override
    public Component getTreeCellRendererComponent(JTree tree, Object value,
            boolean sel, boolean expanded, boolean leaf,
            int row, boolean hasFocus) {

        SwingScilabTree node = (SwingScilabTree) value;

        // Case of leaf
        if (leaf) {
            // Test the kind of selected component in the tree
            if (node instanceof SwingScilabTree) {
                // Set icon for the type of leaf
                if (node.getIcon() != null) {
                    if (node.getIcon() == iconDefaut) {
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
        if (expanded) {
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
        } else {
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
        super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);
        return this;
    }

}
