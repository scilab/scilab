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

package org.scilab.modules.uiwidget.components;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;
import java.util.EventObject;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JTree;
import javax.swing.event.CellEditorListener;
import javax.swing.event.ChangeEvent;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellEditor;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;
import org.scilab.modules.uiwidget.callback.UICallback;

/**
 * JTree wrapper
 */
public class UITree extends UIComponent {

    // TODO: ajouter un onclick sur les noeuds

    protected JTree tree;
    protected DefaultTreeModel model;
    protected DefaultMutableTreeNode root;
    protected boolean oneditEnable = true;
    protected UICallback oneditAction;
    protected CellEditorListener listener;
    protected DefaultTreeCellRenderer renderer;
    protected JCheckBox cbrenderer;
    protected JCheckBox cbeditor;
    protected Icon leafIcon;
    protected Icon openIcon;
    protected Icon closedIcon;

    /**
     * {@inheritDoc}
     */
    public UITree(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        tree = new JTree();
        root = new DefaultMutableTreeNode();
        model = new DefaultTreeModel(root);
        tree.setModel(model);

        return tree;
    }

    @UIComponentAnnotation(attributes = {"root-visible", "root-text", "leaf-icon", "open-icon", "closed-icon", "editable"})
    public Object newInstance(boolean rootvisible, String roottext, ImageIcon leaficon, ImageIcon openicon, ImageIcon closedicon, boolean editable) {
        this.leafIcon = leaficon;
        this.openIcon = openicon;
        this.closedIcon = closedicon;
        tree = new JTree() {
            public boolean isPathEditable(TreePath path) {
                if (isEditable()) {
                    Object o = path.getLastPathComponent();
                    if (o instanceof DefaultMutableTreeNode && ((DefaultMutableTreeNode) o).getUserObject() instanceof UINode) {
                        return ((UINode) ((DefaultMutableTreeNode) o).getUserObject()).getEditable();
                    }
                }
                return false;
            }
        };
        root = roottext == null ? new DefaultMutableTreeNode("") : new DefaultMutableTreeNode(roottext);
        model = new DefaultTreeModel(root);
        tree.setModel(model);
        tree.setRootVisible(rootvisible);
        tree.setScrollsOnExpand(true);
        tree.setShowsRootHandles(true);
        tree.setInvokesStopCellEditing(true);
        tree.setEditable(editable);

        renderer = new DefaultTreeCellRenderer() {

            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean sel, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                if (value instanceof DefaultMutableTreeNode && ((DefaultMutableTreeNode) value).getUserObject() instanceof UINode) {
                    UINode node = (UINode) ((DefaultMutableTreeNode) value).getUserObject();
                    if (node.getCheckbox()) {
                        JLabel label = (JLabel) super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);
                        if (cbrenderer == null) {
                            cbrenderer = new JCheckBox();
                        }
                        String stringValue = tree.convertValueToText(value, sel, expanded, leaf, row, false);
                        cbrenderer.setText(stringValue);
                        cbrenderer.setSelected(node.getChecked());
                        cbrenderer.setEnabled(tree.isEnabled());
                        Font f = node.getFont();
                        if (f != null) {
                            cbrenderer.setFont(f);
                        } else {
                            cbrenderer.setFont(label.getFont());
                        }

                        Color c = node.getForeground();
                        if (c != null ) {
                            if (sel) {
                                cbrenderer.setForeground(label.getForeground());
                            } else {
                                cbrenderer.setForeground(c);
                            }
                        } else {
                            cbrenderer.setForeground(label.getForeground());
                        }

                        return cbrenderer;
                    } else {
                        ImageIcon i;
                        if (leaf) {
                            i = node.getLeafIcon();
                            this.setLeafIcon(i == null ? UITree.this.leafIcon : i);
                        } else {
                            if (expanded) {
                                i = node.getOpenIcon();
                                this.setOpenIcon(i == null ? UITree.this.openIcon : i);
                            } else {
                                i = node.getClosedIcon();
                                this.setClosedIcon(i == null ? UITree.this.closedIcon : i);
                            }
                        }

                        JLabel label = (JLabel) super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);
                        Font f = node.getFont();
                        if (f != null) {
                            label.setFont(f);
                        } else {
                            label.setFont(tree.getFont());
                        }

                        Color c = node.getForeground();
                        if (c != null) {
                            if (!sel) {
                                label.setForeground(c);
                            }
                        } else {
                            label.setForeground(tree.getForeground());
                        }

                        return label;
                    }
                }

                return super.getTreeCellRendererComponent(tree, value, sel, expanded, leaf, row, hasFocus);
            }
        };
        renderer.setLeafIcon(leaficon);
        renderer.setClosedIcon(closedicon);
        renderer.setOpenIcon(openicon);

        tree.setCellRenderer(renderer);

        DefaultTreeCellEditor editor = new DefaultTreeCellEditor(tree, renderer) {

            UINode editingNode;

            public boolean isCellEditable(EventObject e) {
                boolean ret = false;
                if (e instanceof MouseEvent) {
                    MouseEvent mouseEvent = (MouseEvent) e;
                    TreePath path = tree.getPathForLocation(mouseEvent.getX(), mouseEvent.getY());
                    if (path != null) {
                        Object node = path.getLastPathComponent();
                        if (node != null && node instanceof DefaultMutableTreeNode) {
                            DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) node;
                            if (treeNode.getUserObject() instanceof UINode) {
                                UINode uinode = (UINode) treeNode.getUserObject();
                                if (uinode.getCheckbox()) {
                                    return true;
                                }
                            }
                        }
                    }
                }

                return super.isCellEditable(e);
            }

            public Object getCellEditorValue() {
                if (editingNode != null) {
                    if (editingNode.getCheckbox()) {
                        editingNode.setCheckedNoUpdate(cbeditor.isSelected());
                    } else {
                        editingNode.setTextNoUpdate((String) super.getCellEditorValue());
                    }

                    return editingNode;
                }

                return super.getCellEditorValue();
            }

            public Component getTreeCellEditorComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row) {
                if (value instanceof DefaultMutableTreeNode && ((DefaultMutableTreeNode) value).getUserObject() instanceof UINode) {
                    UINode node = (UINode) ((DefaultMutableTreeNode) value).getUserObject();
                    editingNode = node;
                    if (node.getCheckbox()) {
                        if (cbeditor == null) {
                            cbeditor = new JCheckBox();
                            final JTree jt = tree;
                            cbeditor.addItemListener(new ItemListener() {
                                public void itemStateChanged(ItemEvent itemEvent) {
                                    if (stopCellEditing()) {
                                        jt.stopEditing();
                                    }
                                }
                            });
                        }
                        JCheckBox cb = (JCheckBox) renderer.getTreeCellRendererComponent(tree, value, true, expanded, leaf, row, true);
                        initCheckbox(cbeditor, cb);
                        return cbeditor;
                    } else {
                        renderer.getTreeCellRendererComponent(tree, value, true, expanded, leaf, row, true);
                    }
                }

                return super.getTreeCellEditorComponent(tree, value, selected, expanded, leaf, row);
            }
        };

        tree.setCellEditor(editor);

        return tree;
    }

    /**
     * {@inheritDoc}
     */
    public void finish() {
        model.nodeStructureChanged(root);
    }

    /**
     * {@inheritDoc}
     */
    public void add(UIComponent comp) throws UIWidgetException {
        if (comp instanceof UINode) {
            UINode ui = (UINode) comp;
            if (ui.getPos() == Integer.MAX_VALUE) {
                root.add((DefaultMutableTreeNode) comp.getComponent());
            } else {
                root.insert((DefaultMutableTreeNode) comp.getComponent(), ui.getPos());
            }
            ((UINode) comp).setParent(tree);
        } else {
            super.add(comp);
        }
    }

    /**
     * Set the leaf icon
     * @param i the icon to set
     */
    public void setLeafIcon(ImageIcon i) {
        this.leafIcon = i;
        renderer.setLeafIcon(i);
    }

    /**
     * Get the leaf icon
     * @return the leaf icon
     */
    public Icon getLeafIcon() {
        return renderer.getLeafIcon();
    }

    /**
     * Set the open icon
     * @param i the icon to set
     */
    public void setOpenIcon(ImageIcon i) {
        this.openIcon = i;
        renderer.setOpenIcon(i);
    }

    /**
     * Get the open icon
     * @return the open icon
     */
    public Icon getOpenIcon() {
        return renderer.getOpenIcon();
    }

    /**
     * Set the closed icon
     * @param i the icon to set
     */
    public void setClosedIcon(ImageIcon i) {
        this.closedIcon = i;
        renderer.setClosedIcon(i);
    }

    /**
     * Get the closed icon
     * @return the closed icon
     */
    public Icon getClosedIcon() {
        return renderer.getClosedIcon();
    }

    /**
     * Remove a listener
     */
    protected void removeListener() {
        if (listener != null) {
            tree.getCellEditor().removeCellEditorListener(listener);
            listener = null;
        }
    }

    /**
     * Get the onedit action
     * @return the action
     */
    public UICallback getOnedit() {
        return oneditAction;
    }

    /**
     * Set the onedit action
     * @param the action
     */
    public void setOnedit(final String action) {
        if (this.oneditAction == null && tree.isEditable()) {
            removeListener();
            listener = new CellEditorListener() {
                public void editingStopped(ChangeEvent e) {
                    if (oneditEnable) {
                        Object value = tree.getCellEditor().getCellEditorValue();
                        if (value instanceof UINode) {
                            UINode node = (UINode) value;
                            if (node.getCheckbox()) {
                                UIWidgetTools.execAction(node, UITree.this.oneditAction, node.getChecked() ?  "%t" : "%f");
                            } else {
                                UIWidgetTools.execAction(node, UITree.this.oneditAction, "\"" + node.getText() + "\"");
                            }
                        }
                    }
                }

                public void editingCanceled(ChangeEvent e) { }
            };
            tree.getCellEditor().addCellEditorListener(listener);
        }
        this.oneditAction = UICallback.newInstance(UINode.class, action);
    }

    /**
     * Check if the onedit is enabled
     * @return true if enabled
     */
    public boolean getOneditEnable() {
        return oneditEnable;
    }

    /**
     * Set if the onedit is enabled
     * @param b true if enabled
     */
    public void setOneditEnable(boolean b) {
        oneditEnable = b;
    }

    /**
     * Init a checkbox with the same parameters as an other one
     * @param cb1 the first checkbox
     * @param cb2 the second checkbox
     */
    private static final void initCheckbox(JCheckBox cb1, JCheckBox cb2) {
        cb1.setFont(cb2.getFont());
        cb1.setText(cb2.getText());
        cb1.setSelected(cb2.isSelected());
        cb1.setEnabled(cb2.isEnabled());
        cb1.setForeground(cb2.getForeground());
    }
}
