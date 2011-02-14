/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.io.File;
import java.util.ArrayList;
import java.util.EventObject;
import java.util.List;
import java.util.UUID;
import java.util.Vector;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dialog.ModalityType;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseEvent;

import javax.swing.AbstractAction;
import javax.swing.AbstractCellEditor;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JCheckBox;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.KeyStroke;
import javax.swing.UIManager;
import javax.swing.event.ChangeEvent;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeCellEditor;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreePath;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeNode;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * RestoreOpenedFilesAction Class
 * @author Paul Griffiths
 * @author Calixte DENIZET
 */
public class RestoreOpenedFilesAction extends DefaultCheckAction {

    private static final int GAP = 10;
    private static final String ESCAPE = "ESCAPE";
    private static final Icon SCILAB_ICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png");
    private static JDialog dialog;
    private static JTree tree;
    private static List<List<File>> selectedFiles;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated SciNotes instance
     */
    public RestoreOpenedFilesAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ConfigSciNotesManager.saveRestoreOpenedFiles(this.getState());
    }

    /**
     * Create a menu to add to SciNotes menu bar
     * @param label label of the menu
     * @param editor associated SciNotes instance
     * @param key KeyStroke
     * @return the menu
     */
    public static CheckBoxMenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        CheckBoxMenuItem cb = createCheckBoxMenu(label, null, new RestoreOpenedFilesAction(label, editor), key);
        boolean b = ConfigSciNotesManager.getRestoreOpenedFiles();
        cb.setChecked(b);
        return cb;
    }

    /**
     * @return the selected files opened in the last session
     */
    public static List<List<File>> getSelectedFiles() {
        return selectedFiles;
    }

    /**
     * Display the JDialog
     * @param owner the owner
     */
    public static void displayDialog(JFrame owner) {
        selectedFiles = null;
        int dimX = 450;
        int dimY = 300;

        dialog = new JDialog(owner);
        dialog.getRootPane().getInputMap(JComponent.WHEN_FOCUSED).put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE , 0), ESCAPE);
        dialog.getRootPane().getActionMap().put(ESCAPE, new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    dialog.dispose();
                }
            });

        dialog.setPreferredSize(new Dimension(dimX, dimY));

        JButton ok = new JButton(SciNotesMessages.OK);
        JButton cancel = new JButton(SciNotesMessages.CANCEL);
        ok.setPreferredSize(cancel.getPreferredSize());
        Object[] buttons = new Object[2];
        ok.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    selectedFiles = getOpenedFiles();
                    dialog.dispose();
                }
            });

        cancel.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    List<UUID> editorID = ConfigSciNotesManager.getOpenFilesEditorList();
                    for (int i = 0; i < editorID.size(); i++) {
                        // Remove these files from the list of open files
                        ConfigSciNotesManager.removeFromOpenFiles(editorID.get(i));
                    }
                    dialog.dispose();
                }
            });

        if (isWindows()) {
            buttons[0] = ok;
            buttons[1] = cancel;

        } else {
            buttons[0] = cancel;
            buttons[1] = ok;
        }

        JOptionPane jop = new JOptionPane(SciNotesMessages.RESTORE_FILES_QUERY, JOptionPane.QUESTION_MESSAGE, JOptionPane.CANCEL_OPTION, null, buttons);
        JPanel panel = new JPanel();
        panel.setPreferredSize(new Dimension(dimX, dimY));
        panel.setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        c.gridheight = 1;
        c.gridwidth = 1;
        c.gridx = 0;
        c.gridy = 0;
        c.weighty = 0.5;
        c.anchor = GridBagConstraints.PAGE_START;
        c.fill = GridBagConstraints.HORIZONTAL;
        panel.add(jop, c);
        c.gridx = 0;
        c.gridy = 1;
        c.gridheight = GridBagConstraints.REMAINDER;
        c.fill = GridBagConstraints.HORIZONTAL;
        fillTree();
        JScrollPane scroll = new JScrollPane(tree);
        scroll.setMinimumSize(new Dimension(dimX - 2 * GAP, dimY / 2));
        panel.add(scroll, c);

        dialog.setContentPane(panel);

        dialog.pack();
        dialog.setIconImage(((ImageIcon) SCILAB_ICON).getImage());
        dialog.setTitle(SciNotesMessages.RESTORE_FILES_TITLE);
        dialog.setModalityType(ModalityType.APPLICATION_MODAL);
        dialog.setResizable(false);
        dialog.setLocationRelativeTo(owner);

        dialog.setVisible(true);
    }

    /**
     * @return true if the OS is Windows
     */
    private static boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("windows");
    }

    /**
     * @return the files to open in the different editors
     */
    private static List<List<File>> getOpenedFiles() {
        List<List<File>> list = new ArrayList();
        List<UUID> editorID = ConfigSciNotesManager.getOpenFilesEditorList();
        Object[] eds = new Object[editorID.size()];
        TreeModel model = tree.getModel();

        for (int i = 0; i < eds.length; i++) {
            List<File> files = ConfigSciNotesManager.getOpenFilesByEditor(editorID.get(i));
            if (files.size() > 0) {
                List<File> filesToOpen = new ArrayList();
                TreeNode node = (TreeNode) model.getChild(model.getRoot(), i);
                for (int j = 0; j < files.size(); j++) {
                    DefaultMutableTreeNode mutNode = (DefaultMutableTreeNode) node.getChildAt(j);
                    CheckBoxNode cb = (CheckBoxNode) mutNode.getUserObject();
                    if (cb.isSelected()) {
                        filesToOpen.add(files.get(j));
                    }
                }
                if (filesToOpen.size() > 0) {
                    list.add(filesToOpen);
                }
            }
            // Remove these files from the list of open files
            ConfigSciNotesManager.removeFromOpenFiles(editorID.get(i));
        }

        return list;
    }

    /**
     * Fill the tree with the opened files
     */
    private static void fillTree() {
        List<UUID> editorID = ConfigSciNotesManager.getOpenFilesEditorList();
        Vector eds = new Vector(editorID.size());

        for (int i = 0; i < editorID.size(); i++) {
            List<File> filesToOpen = ConfigSciNotesManager.getOpenFilesByEditor(editorID.get(i));
            if (filesToOpen.size() > 0) {
                eds.add(new FilesVector("Editor " + (i + 1), filesToOpen));
            }
        }

        tree = new JTree(eds);

        CheckBoxNodeRenderer renderer = new CheckBoxNodeRenderer();
        tree.setCellRenderer(renderer);

        tree.setCellEditor(new CheckBoxNodeEditor(tree));
        tree.setEditable(true);

        for (int i = 0; i < tree.getRowCount(); i++) {
            tree.expandRow(i);
        }
    }

    /**
     * Inner class to handle the renderer of a checkbox in a JTree
     */
    static class CheckBoxNodeRenderer implements TreeCellRenderer {

        private JCheckBox leafRenderer = new JCheckBox();
        private DefaultTreeCellRenderer nonLeafRenderer = new DefaultTreeCellRenderer();

        private Color selectionBorderColor;
        private Color selectionForeground;
        private Color selectionBackground;
        private Color textForeground;
        private Color textBackground;

        /**
         * Constructor
         */
        public CheckBoxNodeRenderer() {
            Font fontValue = UIManager.getFont("Tree.font");
            if (fontValue != null) {
                leafRenderer.setFont(fontValue);
            }
            Boolean booleanValue = (Boolean) UIManager.get("Tree.drawsFocusBorderAroundIcon");
            leafRenderer.setFocusPainted(booleanValue != null && booleanValue.booleanValue());

            selectionBorderColor = UIManager.getColor("Tree.selectionBorderColor");
            selectionForeground = UIManager.getColor("Tree.selectionForeground");
            selectionBackground = UIManager.getColor("Tree.selectionBackground");
            textForeground = UIManager.getColor("Tree.textForeground");
            textBackground = UIManager.getColor("Tree.textBackground");
        }

        /**
         * {@inheritDoc}
         */
        public Component getTreeCellRendererComponent(JTree tree, Object value,
                                                      boolean selected, boolean expanded, boolean leaf, int row,
                                                      boolean hasFocus) {
            if (leaf) {
                String stringValue = tree.convertValueToText(value, selected, expanded, leaf, row, false);
                leafRenderer.setText(stringValue);
                leafRenderer.setSelected(false);

                leafRenderer.setEnabled(tree.isEnabled());

                if (selected) {
                    leafRenderer.setForeground(selectionForeground);
                    leafRenderer.setBackground(selectionBackground);
                } else {
                    leafRenderer.setForeground(textForeground);
                    leafRenderer.setBackground(textBackground);
                }

                if (value != null && value instanceof DefaultMutableTreeNode) {
                    Object obj = ((DefaultMutableTreeNode) value).getUserObject();
                    if (obj instanceof CheckBoxNode) {
                        CheckBoxNode node = (CheckBoxNode) obj;
                        leafRenderer.setText(node.getText());
                        leafRenderer.setSelected(node.isSelected());
                    }
                }
                return leafRenderer;
            } else {
                return nonLeafRenderer.getTreeCellRendererComponent(tree, value, selected, expanded, leaf, row, hasFocus);
            }
        }

        /**
         * @return the JCheckBox used to render a checkBox
         */
        protected JCheckBox getLeafRenderer() {
            return leafRenderer;
        }
    }

    /**
     * Inner class to handle the checkbox in a JTree
     */
    static class CheckBoxNodeEditor extends AbstractCellEditor implements TreeCellEditor {

        private CheckBoxNodeRenderer renderer = new CheckBoxNodeRenderer();
        private ChangeEvent changeEvent;
        private JTree tree;

        /**
         * Default constructor
         * @param tree the tree
         */
        public CheckBoxNodeEditor(JTree tree) {
            this.tree = tree;
        }

        /**
         * {@inheritDoc}
         */
        public Object getCellEditorValue() {
            JCheckBox checkbox = renderer.getLeafRenderer();
            CheckBoxNode checkBoxNode = new CheckBoxNode(checkbox.getText(), checkbox.isSelected());
            return checkBoxNode;
        }

        /**
         * {@inheritDoc}
         */
        public boolean isCellEditable(EventObject e) {
            boolean ret = false;
            if (e instanceof MouseEvent) {
                MouseEvent mouseEvent = (MouseEvent) e;
                TreePath path = tree.getPathForLocation(mouseEvent.getX(), mouseEvent.getY());
                if (path != null) {
                    Object node = path.getLastPathComponent();
                    if (node != null && node instanceof DefaultMutableTreeNode) {
                        DefaultMutableTreeNode treeNode = (DefaultMutableTreeNode) node;
                        ret = treeNode.isLeaf() && treeNode.getUserObject() instanceof CheckBoxNode;
                    }
                }
            }

            return ret;
        }

        /**
         * {@inheritDoc}
         */
        public Component getTreeCellEditorComponent(JTree tree, Object value,
                                                    boolean selected, boolean expanded, boolean leaf, int row) {
            Component editor = renderer.getTreeCellRendererComponent(tree, value, true, expanded, leaf, row, true);
            ItemListener itemListener = new ItemListener() {
                    public void itemStateChanged(ItemEvent itemEvent) {
                        if (stopCellEditing()) {
                            fireEditingStopped();
                        }
                    }
                };
            if (editor instanceof JCheckBox) {
                ((JCheckBox) editor).addItemListener(itemListener);
            }

            return editor;
        }
    }

    /**
     * Inner class to handle checkboxes in a tree
     */
    static class CheckBoxNode {

        private String text;
        private boolean selected;

        /**
         * Constructor
         * @param text which will be displayed
         * @param selected true if the checkbox is selected
         */
        public CheckBoxNode(String text, boolean selected) {
            this.text = text;
            this.selected = selected;
        }

        /**
         * @return true if the checkbox is selected
         */
        public boolean isSelected() {
            return selected;
        }

        /**
         * @param newValue true if the checkbox must be selected
         */
        public void setSelected(boolean newValue) {
            selected = newValue;
        }

        /**
         * @return the String which is displayed
         */
        public String getText() {
            return text;
        }

        /**
         * @param newValue the text to set
         */
        public void setText(String newValue) {
            text = newValue;
        }
    }

    /**
     * Inner class to have a named Vector to add to the tree
     */
    static class FilesVector extends Vector {

        private String name;

        /**
         * Constructor
         * @param name the name of the List which will be display in the tree
         * @param elems the elements
         */
        public FilesVector(String name, List<File> elems) {
            super();
            this.name = name;
            for (int i = 0; i < elems.size(); i++) {
                add(new CheckBoxNode(elems.get(i).getName() + " (in " + elems.get(i).getParent() + ")", true));
            }
        }

        /**
         * {@inheritDoc}
         */
        public String toString() {
            return name;
        }
    }
}
