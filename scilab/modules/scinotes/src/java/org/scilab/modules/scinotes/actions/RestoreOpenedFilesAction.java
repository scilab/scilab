/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes.actions;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.EventObject;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.Vector;

import javax.swing.AbstractAction;
import javax.swing.AbstractCellEditor;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTree;
import javax.swing.KeyStroke;
import javax.swing.UIManager;
import javax.swing.event.ChangeEvent;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeCellEditor;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menubar.SwingScilabMenuBar;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * RestoreOpenedFilesAction Class
 * @author Paul Griffiths
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class RestoreOpenedFilesAction extends DefaultCheckAction {

    private static final int GAP = 10;
    private static final String ESCAPE = "ESCAPE";
    private static final Icon SCILAB_ICON = new ImageIcon(FindIconHelper.findIcon("scilab"));
    private static List<File> selectedFiles;
    private static Map<SciNotes, List<Component>> listOfComponents;

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
    @Override
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
    public static List<File> getSelectedFiles() {
        return selectedFiles;
    }

    private static void saveEnabledComponents(SciNotes ed) {
        List<Component> list = new ArrayList<Component>();
        SwingScilabMenuBar mb = (SwingScilabMenuBar) ed.getMenuBar().getAsSimpleMenuBar();
        SwingScilabToolBar tb = (SwingScilabToolBar) ed.getToolBar().getAsSimpleToolBar();
        for (int i = 0; i < mb.getMenuCount(); i++) {
            if (mb.getMenu(i).isEnabled()) {
                list.add(mb.getMenu(i));
                mb.getMenu(i).setEnabled(false);
            }
        }
        for (int i = 0; i < tb.getComponentCount(); i++) {
            if (tb.getComponent(i).isEnabled()) {
                list.add(tb.getComponent(i));
                tb.getComponent(i).setEnabled(false);
            }
        }

        if (listOfComponents == null) {
            listOfComponents = new HashMap<SciNotes, List<Component>>();
        }
        listOfComponents.put(ed, list);
    }

    public static void restoreEnabledComponents(SciNotes ed) {
        if (listOfComponents != null && listOfComponents.containsKey(ed)) {
            SwingScilabMenuBar mb = (SwingScilabMenuBar) ed.getMenuBar().getAsSimpleMenuBar();
            SwingScilabToolBar tb = (SwingScilabToolBar) ed.getToolBar().getAsSimpleToolBar();
            List<Component> l = listOfComponents.get(ed);
            for (Component c : l) {
                c.setEnabled(true);
            }

            l.clear();
            listOfComponents.remove(ed);
            if (listOfComponents.isEmpty()) {
                listOfComponents = null;
            }
        }
    }

    public static JPanel getTab(final SciNotes ed, final String uuid) {
        final JTree tree = fillTree(uuid);
        if (tree == null) {
            return null;
        }

        JButton ok = new JButton(SciNotesMessages.OK);
        JButton cancel = new JButton(SciNotesMessages.CANCEL);
        ok.setPreferredSize(cancel.getPreferredSize());
        Object[] buttons = new Object[2];
        ok.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                List<File> list = getOpenedFiles(tree, uuid);
                ed.getTabPane().remove(0);
                if (list != null && list.size() != 0) {
                    for (File f : list) {
                        ed.openFile(f.getPath(), 0, null);
                    }
                } else if (ed.getTabPane().getTabCount() == 0) {
                    ed.addEmptyTab();
                }

                if (ed.getNavigator() != null) {
                    ed.getNavigator().updateTree();
                }

                restoreEnabledComponents(ed);
            }
        });

        cancel.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                List<UUID> editorID = ConfigSciNotesManager.getOpenFilesEditorList();
                for (int i = 0; i < editorID.size(); i++) {
                    // Remove these files from the list of open files
                    if (editorID.get(i).toString().equals(uuid)) {
                        ConfigSciNotesManager.removeFromOpenFiles(editorID.get(i));
                        break;
                    }
                }
                ed.getTabPane().remove(0);
                if (ed.getTabPane().getTabCount() == 0) {
                    ed.addEmptyTab();
                }
                restoreEnabledComponents(ed);
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
        final JPanel panel = new JPanel();
        panel.setLayout(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        c.gridheight = 1;
        c.gridwidth = 1;
        c.gridx = 0;
        c.gridy = 0;
        c.weightx = 1;
        c.weighty = 0.1;
        c.anchor = GridBagConstraints.PAGE_START;
        c.fill = GridBagConstraints.HORIZONTAL;
        panel.add(jop, c);
        c.gridx = 0;
        c.gridy = 1;
        c.gridheight = GridBagConstraints.REMAINDER;
        c.weighty = 0.9;
        c.fill = GridBagConstraints.BOTH;
        JScrollPane scroll = new JScrollPane(tree);
        panel.add(scroll, c);
        ok.requestFocusInWindow();

        saveEnabledComponents(ed);

        return panel;
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
    private static List<File> getOpenedFiles(JTree tree, String uuid) {
        List<File> list = new ArrayList();
        List<String> remove = new ArrayList();
        TreeModel model = tree.getModel();
        TreeNode node = (TreeNode) model.getChild(model.getRoot(), 0);

        for (int i = 0; i < node.getChildCount(); i++) {
            DefaultMutableTreeNode mutNode = (DefaultMutableTreeNode) node.getChildAt(i);
            CheckBoxNode cb = (CheckBoxNode) mutNode.getUserObject();
            if (cb.isSelected()) {
                list.add(cb.getFile());
            } else {
                remove.add(cb.getFile().getAbsolutePath());
            }
        }
        // Remove these files from the list of open files
        ConfigSciNotesManager.removeFromOpenFiles(UUID.fromString(uuid), remove);

        return list;
    }

    /**
     * Fill the tree with the opened files
     * @param uuid the editor uuid
     */
    private static JTree fillTree(String uuid) {
        List<UUID> editorID = ConfigSciNotesManager.getOpenFilesEditorList();
        Vector eds = new Vector(1);

        List<File> filesToOpen = removeAlreadyOpenFiles(uuid);

        if (filesToOpen.size() > 0) {
            eds.add(new FilesVector("SciNotes", filesToOpen));
        } else {
            return null;
        }

        JTree tree = new JTree(eds);

        CheckBoxNodeRenderer renderer = new CheckBoxNodeRenderer();
        tree.setCellRenderer(renderer);

        tree.setCellEditor(new CheckBoxNodeEditor(tree));
        tree.setEditable(true);

        for (int i = 0; i < tree.getRowCount(); i++) {
            tree.expandRow(i);
        }

        return tree;
    }

    /**
     * @param editorUUID the editor UUID
     * @return the files opened in the previous session minus the already opened files
     */
    private static List<File> removeAlreadyOpenFiles(String editorUUID) {
        List<File> filesToOpen = ConfigSciNotesManager.getOpenFilesByEditor(UUID.fromString(editorUUID));
        SciNotes editor = SciNotes.getEditorFromUUID(editorUUID);
        List<File> filesToRemove = new ArrayList();
        if (editor != null) {
            int n = editor.getTabPane().getTabCount();
            for (int i = 0; i < n; i++) {
                ScilabEditorPane pane = editor.getTextPane(i);
                String name = pane.getName();
                if (name != null) {
                    for (File ff : filesToOpen) {
                        if (ff.equals(new File(name))) {
                            filesToRemove.add(ff);
                            break;
                        }
                    }
                }
            }
        }

        filesToOpen.removeAll(filesToRemove);

        return filesToOpen;
    }

    /**
     * Inner class to handle the renderer of a checkbox in a JTree
     */
    static class CheckBoxNodeRenderer implements TreeCellRenderer {

        private final JCheckBox leafRenderer = new JCheckBox();
        private final DefaultTreeCellRenderer nonLeafRenderer = new DefaultTreeCellRenderer();

        private final Color selectionBorderColor;
        private final Color selectionForeground;
        private final Color selectionBackground;
        private final Color textForeground;
        private final Color textBackground;

        private File file;

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
        @Override
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
                        file = node.getFile();
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

        /**
         * @return the file associated with the renderer
         */
        protected File getFile() {
            return file;
        }
    }

    /**
     * Inner class to handle the checkbox in a JTree
     */
    static class CheckBoxNodeEditor extends AbstractCellEditor implements TreeCellEditor {

        private final CheckBoxNodeRenderer renderer = new CheckBoxNodeRenderer();
        private ChangeEvent changeEvent;
        private final JTree tree;

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
        @Override
        public Object getCellEditorValue() {
            JCheckBox checkbox = renderer.getLeafRenderer();
            CheckBoxNode checkBoxNode = new CheckBoxNode(renderer.getFile(), checkbox.getText(), checkbox.isSelected());
            return checkBoxNode;
        }

        /**
         * {@inheritDoc}
         */
        @Override
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
        @Override
        public Component getTreeCellEditorComponent(JTree tree, Object value,
                boolean selected, boolean expanded, boolean leaf, int row) {
            Component editor = renderer.getTreeCellRendererComponent(tree, value, true, expanded, leaf, row, true);
            ItemListener itemListener = new ItemListener() {
                @Override
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
        private File file;

        /**
         * Constructor
         * @param text which will be displayed
         * @param selected true if the checkbox is selected
         */
        public CheckBoxNode(File f, String text, boolean selected) {
            this.file = f;
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

        public File getFile() {
            return file;
        }
    }

    /**
     * Inner class to have a named Vector to add to the tree
     */
    static class FilesVector extends Vector {

        private final String name;

        /**
         * Constructor
         * @param name the name of the List which will be display in the tree
         * @param elems the elements
         */
        public FilesVector(String name, List<File> elems) {
            super();
            this.name = name;
            for (int i = 0; i < elems.size(); i++) {
                add(new CheckBoxNode(elems.get(i), elems.get(i).getName() + " (in " + elems.get(i).getParent() + ")", true));
            }
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public String toString() {
            return name;
        }
    }
}
