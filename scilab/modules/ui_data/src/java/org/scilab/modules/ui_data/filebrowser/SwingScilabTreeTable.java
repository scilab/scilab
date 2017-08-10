/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.lang.reflect.Method;
import java.text.DateFormat;
import java.util.Date;
import java.util.regex.Pattern;

import javax.swing.ActionMap;
import javax.swing.ImageIcon;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.border.AbstractBorder;
import javax.swing.border.Border;
import javax.swing.plaf.basic.BasicTreeUI;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.SwingWorker;
import javax.swing.tree.TreePath;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.filebrowser.actions.ChangeCWDAction;
import org.scilab.modules.ui_data.filebrowser.actions.EditFileWithDefaultAppAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteFileInConsoleAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteFileInXcosAction;
import org.scilab.modules.ui_data.filebrowser.actions.ExecuteMatFileAction;
import org.scilab.modules.ui_data.filebrowser.actions.LoadFileAsGraphAction;
import org.scilab.modules.ui_data.filebrowser.actions.LoadFileInScilabAction;
import org.scilab.modules.ui_data.filebrowser.actions.OpenFileInSciNotesAction;
import org.scilab.modules.ui_data.filebrowser.actions.OpenFileWithDefaultAppAction;
import org.scilab.modules.ui_data.filebrowser.actions.ValidateAction;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * The tree table model abstract implementation
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class SwingScilabTreeTable extends JTable {

    private static final Insets INSETS = new Insets(0, 2, 0, 0);
    private static final DateFormat DATEFORMAT = DateFormat.getDateTimeInstance(DateFormat.SHORT, DateFormat.MEDIUM);

    private static final Border BORDER = new AbstractBorder() {
        public void paintBorder(Component c, Graphics g, int x, int y, int width, int height) {
            g.setColor(Color.LIGHT_GRAY);
            g.drawLine(x, y, x, y + height);
        }

        public Insets getBorderInsets(Component c) {
            return INSETS;
        }

        public Insets getBorderInsets(Component c, Insets insets) {
            return INSETS;
        }
    };

    private SwingWorker dirRefresher;
    private ScilabFileBrowserModel model;

    private Method isLocationInExpandControl;

    protected ScilabTreeTableCellRenderer tree;
    protected ScilabFileSelectorComboBox combobox;
    protected ScilabFileBrowserHistory history;

    /**
     * Default Constructor
     * @param treeTableModel the tree table model
     * @param combobox the combox used to set the path
     */
    public SwingScilabTreeTable(ScilabTreeTableModel treeTableModel, ScilabFileSelectorComboBox combobox) {
        super();
        this.combobox = combobox;
        combobox.setTreeTable(this);
        history = new ScilabFileBrowserHistory(this);
        tree = new ScilabTreeTableCellRenderer(this, treeTableModel);
        super.setModel(new ScilabTreeTableModelAdapter(treeTableModel, tree));

        // Install the tree editor renderer and editor.
        setDefaultRenderer(ScilabTreeTableModel.class, tree);
        setDefaultRenderer(Date.class, new DefaultTableCellRenderer() {
            {
                setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
            }

            public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
                JLabel label = (JLabel) super.getTableCellRendererComponent(table, value, selected, focus, row, col);
                label.setText(DATEFORMAT.format((Date) value));
                if (col == 1) {
                    label.setBorder(BORDER);
                }
                return label;
            }
        });
        setDefaultRenderer(ScilabFileBrowserModel.FileSize.class, new DefaultTableCellRenderer() {
            {
                setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
            }

            public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
                Component c = super.getTableCellRendererComponent(table, value, selected, focus, row, col);
                if (col == 1) {
                    JLabel jl = (JLabel) c;
                    jl.setBorder(BORDER);
                }
                return c;
            }
        });
        setDefaultRenderer(String.class, new DefaultTableCellRenderer() {
            {
                setHorizontalTextPosition(DefaultTableCellRenderer.LEFT);
            }

            public Component getTableCellRendererComponent(JTable table, Object value, boolean selected, boolean focus, int row, int col) {
                Component c = super.getTableCellRendererComponent(table, value, selected, focus, row, col);
                if (col == 1) {
                    JLabel jl = (JLabel) c;
                    jl.setBorder(BORDER);
                }
                return c;
            }
        });

        setShowGrid(false);
        setFillsViewportHeight(true);
        setIntercellSpacing(new Dimension(0, 0));
        setRowSorter(new FileBrowserRowSorter(tree, this));
        setAutoResizeMode(AUTO_RESIZE_NEXT_COLUMN);

        try {
            isLocationInExpandControl = BasicTreeUI.class.getDeclaredMethod("isLocationInExpandControl", new Class[] {TreePath.class, int.class, int.class});
            isLocationInExpandControl.setAccessible(true);
        } catch (NoSuchMethodException e) { }

        addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                Point p = e.getPoint();
                int col = columnAtPoint(p);
                if (getColumnClass(col) == ScilabTreeTableModel.class && SwingUtilities.isLeftMouseButton(e)) {
                    MouseEvent me = e;
                    if (isLocationInExpandControl != null) {
                        try {
                            int row = rowAtPoint(p);
                            TreePath path = tree.getPathForRow(row);
                            boolean isOnExpander = ((Boolean) isLocationInExpandControl.invoke(tree.getUI(), path, e.getX(), e.getY())).booleanValue();
                            Rectangle r = tree.getRowBounds(row);
                            if (!isOnExpander && !r.contains(p)) {
                                me = new MouseEvent((Component) e.getSource(), e.getID(), e.getWhen(), e.getModifiers(), r.x, r.y, e.getClickCount(), e.isPopupTrigger());
                            }
                        } catch (Exception ex) { }
                    }
                    tree.dispatchEvent(me);
                }
            }
        });

        addKeyListener(new KeyAdapter() {
            public void keyTyped(KeyEvent e) {
                char c = e.getKeyChar();
                if (Character.isLetter(c)) {
                    int step = 1;
                    if (Character.isUpperCase(c)) {
                        step = -1;
                    }
                    c = Character.toLowerCase(c);
                    int[] rows = getSelectedRows();
                    int count = getRowCount();
                    int start = 0;
                    if (rows != null && rows.length != 0) {
                        start = modulo(rows[0] + step, count);
                    }
                    for (int i = start; i != start - step; i = modulo(i + step, count)) {
                        char first = ((FileNode) tree.getPathForRow(i).getLastPathComponent()).toString().charAt(0);
                        first = Character.toLowerCase(first);
                        if (first == c) {
                            scrollRectToVisible(tree.getRowBounds(i));
                            setRowSelectionInterval(i, i);
                            break;
                        }
                    }
                }
            }
        });


        initActions();
        setComponentPopupMenu(createPopup());
    }

    /**
     * @return the Next button used in history
     */
    public JButton getNextButton() {
        return history.getNextButton();
    }

    /**
     * @return the Previous button used in history
     */
    public JButton getPreviousButton() {
        return history.getPreviousButton();
    }

    /**
     * @return the combobox used to set the path
     */
    public ScilabFileSelectorComboBox getComboBox() {
        return combobox;
    }

    /**
     * Get the selected rows as file path
     * @return the paths
     */
    public String[] getSelectedPaths() {
        int[] rows = getSelectedRows();
        String[] paths = new String[rows.length];
        for (int i = 0; i < rows.length; i++) {
            TreePath path = tree.getPathForRow(rows[i]);
            FileNode fn = (FileNode) path.getLastPathComponent();
            paths[i] = fn.getFile().getAbsolutePath();
        }

        return paths;
    }

    /**
     * Get the selected rows as file
     * @return the paths
     */
    public File[] getSelectedFiles() {
        int[] rows = getSelectedRows();
        File[] files = new File[rows.length];
        for (int i = 0; i < rows.length; i++) {
            TreePath path = tree.getPathForRow(rows[i]);
            FileNode fn = (FileNode) path.getLastPathComponent();
            files[i] = fn.getFile();
        }

        return files;
    }

    /**
     * {@inheritDoc}
     */
    public int getRowHeight(int row) {
        return getRowHeight();
    }

    /**
     * {@inheritDoc}
     */
    public boolean isOpaque() {
        return false;
    }

    /**
     * Set the base directory
     * @param baseDir the base directory
     */
    public void setBaseDir(String baseDir) {
        setBaseDir(baseDir, true);
    }

    /**
     * Set the base directory
     * @param baseDir the base directory
     * @param addInHistory if true the dir is add in the history
     */
    public synchronized void setBaseDir(String baseDir, boolean addInHistory) {
        boolean cancelled = false;
        ScilabFileBrowserModel model;
        if (dirRefresher != null) {
            dirRefresher.cancel(true);
            dirRefresher = null;
            model = this.model;
            this.model = null;
            cancelled = true;
        } else {
            model = (ScilabFileBrowserModel) tree.getModel();
        }
        combobox.setBaseDir(baseDir);
        if (model != null) {
            File f = new File(baseDir);
            if (cancelled || (!baseDir.equals(model.getBaseDir()) && f.exists() && f.isDirectory() && f.canRead())) {
                tree.setModel(null);
                if (addInHistory) {
                    history.addPathInHistory(baseDir);
                }
                model.setBaseDir(baseDir, this);
            }
        }
    }

    /**
     * Set the file filter to use in table
     * @param pat the pattern to use
     */
    public void setFilter(Pattern pat) {
        ScilabFileBrowserModel model = (ScilabFileBrowserModel) tree.getModel();
        TreePath rootPath = new TreePath(model.getRoot());
        tree.setModel(null);
        model.setFilter(pat);
        reload(model);
    }

    /**
     * Reload the table
     */
    public void reload(ScilabFileBrowserModel model) {
        tree.setModel(model);
        tree.setRowHeight(getRowHeight());
        tree.setLargeModel(true);
        TreePath path = new TreePath(model.getRoot());
        tree.collapsePath(path);
        ((JScrollPane) SwingUtilities.getAncestorOfClass(JScrollPane.class, this)).getVerticalScrollBar().setValue(0);
        tree.expandPath(path);
        if (getRowCount() >= 1) {
            repaint(tree.getRowBounds(0));
        }
        editingRow = 0;
    }

    /* Workaround for BasicTableUI anomaly. Make sure the UI never tries to
     * paint the editor. The UI currently uses different techniques to
     * paint the renderers and editors and overriding setBounds() below
     * is not the right thing to do for an editor. Returning -1 for the
     * editing row in this case, ensures the editor is never painted.
     */
    public int getEditingRow() {
        if (getColumnClass(editingColumn) == ScilabTreeTableModel.class) {
            return -1;
        } else {
            return editingRow;
        }
    }

    /**
     * Init the actions
     */
    private void initActions() {
        final ActionMap actions = getActionMap();
        actions.put("scinotes", new OpenFileInSciNotesAction(this));
        actions.put("xcos", new ExecuteFileInXcosAction(this));
        actions.put("mat", new ExecuteMatFileAction(this));
        actions.put("console", new ExecuteFileInConsoleAction(this));
        actions.put("load", new LoadFileInScilabAction(this));
        actions.put("graph", new LoadFileAsGraphAction(this));
        actions.put("cwd", new ChangeCWDAction(this));
        if (EditFileWithDefaultAppAction.isSupported()) {
            actions.put("edit", new EditFileWithDefaultAppAction(this));
        }
        if (OpenFileWithDefaultAppAction.isSupported()) {
            actions.put("open", new OpenFileWithDefaultAppAction(this));
        }
        actions.put("validate", new ValidateAction(this));
        actions.put("validateorexpand", new CommonCallBack(null) {
            public void callBack() {
                int[] rows = getSelectedRows();
                if (rows != null && rows.length != 0) {
                    TreePath path = tree.getPathForRow(rows[0]);
                    FileNode fn = (FileNode) path.getLastPathComponent();
                    if (fn.isLeaf()) {
                        ((CommonCallBack) actions.get("validate")).callBack();
                    } else {
                        if (tree.isExpanded(path)) {
                            tree.collapsePath(path);
                        } else {
                            tree.expandPath(path);
                        }
                        setRowSelectionInterval(rows[0], rows[0]);
                    }
                }
            }
        });

        combobox.setAction((CommonCallBack) actions.get("cwd"));
        InputMap map = getInputMap();
        map.put(KeyStroke.getKeyStroke("ENTER"), "validateorexpand");
    }

    /**
     * Create the popup menu
     */
    private JPopupMenu createPopup() {
        ActionMap actions = getActionMap();
        JPopupMenu popup = new JPopupMenu();
        JMenuItem item = new JMenuItem(UiDataMessages.OPENINSCINOTES);
        item.addActionListener(actions.get("scinotes"));
        item.setIcon(new ImageIcon(FindIconHelper.findIcon("accessories-text-editor")));
        popup.add(item);

        item = new JMenuItem(UiDataMessages.EXECINCONSOLE);
        item.addActionListener(actions.get("console"));
        item.setIcon(new ImageIcon(FindIconHelper.findIcon("media-playback-start")));
        popup.add(item);

        item = new JMenuItem(UiDataMessages.OPENINXCOS);
        item.addActionListener(actions.get("xcos"));
        item.setIcon(new ImageIcon(FindIconHelper.findIcon("utilities-system-monitor")));
        popup.add(item);

        item = new JMenuItem(UiDataMessages.LOADINSCILAB);
        item.addActionListener(actions.get("load"));
        item.setIcon(new ImageIcon(FindIconHelper.findIcon("scilab")));
        popup.add(item);

        if (actions.get("edit") != null || actions.get("open") != null) {
            popup.addSeparator();
        }

        if (actions.get("edit") != null) {
            item = new JMenuItem(UiDataMessages.EDITWITHDEFAULT);
            item.addActionListener(actions.get("edit"));
            popup.add(item);
        }

        if (actions.get("open") != null) {
            item = new JMenuItem(UiDataMessages.OPENWITHDEFAULT);
            item.addActionListener(actions.get("open"));
            popup.add(item);
        }

        popup.pack();

        return popup;
    }


    public synchronized void setDirRefresher(SwingWorker refresher, ScilabFileBrowserModel model) {
        dirRefresher = refresher;
        this.model = model;
    }

    /**
     * A modulo for negative numbers
     * @param n an int
     * @param p an other int
     * @return n modulo p
     */
    private static final int modulo(int n, int p) {
        if (n >= 0) {
            return n % p;
        }
        return p - (-n % p);
    }

}
