/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Color;
import java.awt.Font;
import java.util.Enumeration;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * Wrapper for a JTree node
 */
public class UINode extends UIComponent {

    private DefaultMutableTreeNode node;
    private String text;
    private ImageIcon leafIcon;
    private ImageIcon openIcon;
    private ImageIcon closedIcon;
    private Color fg;
    private Font font;
    private boolean editable = true;
    private boolean checkbox;
    private boolean checked;
    private JComponent jparent;
    private int position;

    /**
     * {@inheritDoc}
     */
    public UINode(UIComponent parent) throws UIWidgetException {
        super(parent);
        if (parent instanceof UITree) {
            jparent = parent.getJComponent();
        } else if (parent instanceof UINode) {
            jparent = ((UINode) parent).jparent;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        node = new DefaultMutableTreeNode(this);

        return node;
    }

    @UIComponentAnnotation(attributes = {"text", "leaf-icon", "open-icon", "closed-icon", "foreground", "position"})
    public Object newInstance(String text, ImageIcon leafIcon, ImageIcon openIcon, ImageIcon closedIcon, Color fg, int position) {
        this.text = text;
        this.leafIcon = leafIcon;
        this.openIcon = openIcon;
        this.closedIcon = closedIcon;
        this.fg = fg;
        justSetPosition(position);

        node = new DefaultMutableTreeNode(this);

        return node;
    }

    /**
     * {@inheritDoc}
     */
    public Object getModifiableComponent() {
        return this;
    }

    /**
     * {@inheritDoc}
     */
    public void add(UIComponent comp) throws UIWidgetException {
        if (comp instanceof UINode) {
            UINode ui = (UINode) comp;
            int pos = addNode(node, ui);
            if (jparent instanceof JTree && jparent.isVisible()) {
                ((DefaultTreeModel) ((JTree) jparent).getModel()).nodesWereInserted(node, new int[] { pos });
            }
        } else if (comp instanceof UINodeList) {
            addNodeList(node, (UINodeList) comp);
            if (jparent instanceof JTree && jparent.isVisible()) {
                ((DefaultTreeModel) ((JTree) jparent).getModel()).nodeStructureChanged(node);
            }
        } else {
            super.add(comp);
        }
    }

    /**
     * Add a node to a parent TreeNode
     * @param parent the parent node
     * @param ui the child
     * @return the position where the node was inserted
     */
    private final int addNode(final DefaultMutableTreeNode parent, final UINode ui) {
        int pos = ui.position;
        if (ui.position == Integer.MAX_VALUE) {
            parent.add((DefaultMutableTreeNode) ui.getComponent());
            pos = parent.getChildCount() - 1;
        } else {
            parent.insert((DefaultMutableTreeNode) ui.getComponent(), ui.position);
        }

        ui.setParent(jparent);

        return pos;
    }

    /**
     * Add a nodes list to a parent TreeNode
     * @param parent the parent node
     * @param ui the children
     */
    private void addNodeList(final DefaultMutableTreeNode parent, UINodeList ui) throws UIWidgetException {
        for (UINode n : ui.list) {
            addNode(parent, n);
            n.setParent(this);
        }
        ui.remove();
    }

    /**
     * Set the JComponent (a JTree) which contains the node
     */
    public void setParent(JComponent c) {
        if (c != this.jparent) {
            this.jparent = c;
            Enumeration e = node.children();
            while (e.hasMoreElements()) {
                Object o = ((DefaultMutableTreeNode) e.nextElement()).getUserObject();
                if (o instanceof UINode) {
                    ((UINode) o).setParent(c);
                }
            }
        }
    }

    /**
     * Set the font
     * @param f the font
     */
    public void setFont(Font f) {
        this.font = f;
        update();
    }

    /**
     * Get the font
     * @return the font
     */
    public Font getFont() {
        if (font != null) {
            return font;
        }
        if (jparent != null) {
            return jparent.getFont();
        }

        return null;
    }

    private void justSetPosition(int pos) {
        if (pos <= 0) {
            this.position = Integer.MAX_VALUE;
        } else {
            this.position = pos == Integer.MAX_VALUE ? Integer.MAX_VALUE : pos - 1;
        }
    }

    public void setPos(int pos) {
        if (pos != position) {
            justSetPosition(pos);

            if (parent instanceof UINode) {
                UINode nparent = (UINode) parent;
                if (nparent.node != null && node != null) {
                    nparent.node.remove(node);
                    addNode(nparent.node, this);
                }
            }
        }
    }

    public int getPos() {
        return position;
    }

    public void setEditable(boolean b) {
        this.editable = b;
    }

    public boolean getEditable() {
        return editable;
    }

    public void setCheckbox(boolean b) {
        this.checkbox = b;
        update();
    }

    public boolean getCheckbox() {
        return checkbox;
    }

    public void setChecked(boolean b) {
        this.checked = b;
        repaint();
    }

    void setCheckedNoUpdate(boolean b) {
        this.checked = b;
    }

    public boolean getChecked() {
        return checked;
    }

    public void setText(String s) {
        this.text = s;
        update();
    }

    void setTextNoUpdate(String s) {
        this.text = s;
    }

    public String getText() {
        return text;
    }

    public void setLeafIcon(ImageIcon i) {
        ImageIcon ii = this.leafIcon;
        this.leafIcon = i;
        if (ii == null) {
            update();
        } else {
            repaint();
        }
        update();
    }

    public ImageIcon getLeafIcon() {
        return leafIcon;
    }

    public void setOpenIcon(ImageIcon i) {
        ImageIcon ii = this.openIcon;
        this.openIcon = i;
        if (ii == null) {
            update();
        } else {
            repaint();
        }
    }

    public ImageIcon getOpenIcon() {
        return openIcon;
    }

    public void setClosedIcon(ImageIcon i) {
        ImageIcon ii = this.closedIcon;
        this.closedIcon = i;
        if (ii == null) {
            update();
        } else {
            repaint();
        }
    }

    public ImageIcon getClosedIcon() {
        return closedIcon;
    }

    public void setForeground(Color c) {
        this.fg = c;
        repaint();
    }

    public Color getForeground() {
        return fg;
    }

    public String toString() {
        return text;
    }

    private void update() {
        if (jparent != null) {
            jparent.updateUI();
        }
    }

    private void repaint() {
        if (jparent != null) {
            jparent.repaint();
        }
    }
}
