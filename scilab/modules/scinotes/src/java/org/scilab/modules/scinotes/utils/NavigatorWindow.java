/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Component;
import java.awt.Container;
import java.awt.FocusTraversalPolicy;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.KeyAdapter;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;

import javax.swing.DefaultComboBoxModel;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JTree;
import javax.swing.KeyStroke;
import javax.swing.tree.TreePath;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.text.Element;
import javax.swing.event.DocumentListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.TreeExpansionListener;
import javax.swing.event.TreeExpansionEvent;

import org.flexdock.docking.event.DockingEvent;
import org.flexdock.docking.defaults.DockingSplitPane;

import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.UIElementMapper;

import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.SciNotesGUI;
import org.scilab.modules.scinotes.SciNotes;

/**
 *
 * @author Calixte DENIZET
 */
public final class NavigatorWindow extends SwingScilabTab implements Tab, DocumentListener,
                                                               TreeExpansionListener {

    private static final String EMPTY = "";

    private static JTree functionNavigator;
    private static DefaultTreeModel model;
    private static ScilabEditorPane pane;
    private static ScilabDocument doc;
    private static NavigatorWindow navigator;
    private static boolean alphaOrder;
    private static Map<ScilabEditorPane, DefaultMutableTreeNode> mapNode = new HashMap();
    private static Map<ScilabEditorPane, TreePath> mapFunPath = new HashMap();
    private static Map<ScilabEditorPane, TreePath> mapAnchorPath = new HashMap();

    private Window parentWindow;
    private boolean isAbsolute = true;
    private boolean lineNumberActive = true;
    private boolean locked;

    private JSeparator jSeparator1;
    private JLabel labelGotoLine;
    private JLabel labelNumerotation;
    private JTextField lineNumber;
    private JComboBox numType;
    private JScrollPane scrollPane;

    /**
     * Creates new Navigator Window
     */
    private NavigatorWindow() {
        super(SciNotesMessages.CODE_NAVIGATOR);
        initComponents();
        functionNavigator.addTreeExpansionListener(this);
    }

    /**
     * @param doc the doc to update
     */
    public static void updateNavigator(ScilabDocument doc) {
        if (navigator != null) {
            if (NavigatorWindow.doc != null) {
                NavigatorWindow.doc.removeDocumentListener(navigator);
            }

            NavigatorWindow.doc = doc;
            NavigatorWindow.pane = doc.getEditorPane();

            doc.addDocumentListener(navigator);

            if (!mapNode.containsKey(pane)) {
                initTree();
            } else {
                updateTree();
            }
        }
    }

    /**
     * Update the navigator
     */
    public static void updateNavigator() {
        if (navigator != null) {
            updateTree();
        }
    }

    /**
     * @param doc to use
     */
    public static void activateNavigator(ScilabDocument doc) {
        initTree();
        if (navigator == null) {
            navigator = new NavigatorWindow();
        }

        updateNavigator(doc);
    }

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    public Window getParentWindow() {
        return this.parentWindow;
    }

    /**
     * {@inheritDoc}
     */
    public SimpleTab getAsSimpleTab() {
        return this;
    }

    /**
     * {@inheritDoc}
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        setInfoBar(infoBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        setMenuBar(menuBarToAdd);
    }

    /**
     * {@inheritDoc}
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        setToolBar(toolBarToAdd);
    }

    public static void closeCurrent() {
        if (navigator != null) {
            navigator.closeNavigator();
        }
    }

    /**
     * Close this Navigator.
     */
    public void closeNavigator() {
        ScilabWindow win = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
        win.removeTab(this);
        setVisible(false);
        close();
        mapNode.clear();
        mapFunPath.clear();
        mapAnchorPath.clear();
        navigator = null;
        functionNavigator = null;
        model = null;
        pane = null;
        doc = null;
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void changedUpdate(DocumentEvent e) { }

    /**
     * Called when an insertion is made in the doc
     * @param e the event
     */
    public void insertUpdate(DocumentEvent e) {
        handleEvent(e.getOffset(), e.getLength());
    }

    /**
     * Called when a remove is made in the doc
     * @param e the event
     */
    public void removeUpdate(DocumentEvent e) {
        handleEvent(e.getOffset(), e.getLength());
    }

    /**
     * {@inheritDoc}
     */
    public void dockingComplete(DockingEvent evt) {
        super.dockingComplete(evt);
        /* Trick to always have the editor's toolbar when the navigator
           is docked in the editor */
        if (evt.getNewDockingPort().getDockedComponent() instanceof DockingSplitPane) {
            DockingSplitPane dsp = (DockingSplitPane) evt.getNewDockingPort().getDockedComponent();
            if (dsp.getElderComponent() instanceof SciNotes) {
                addToolBar(pane.getEditor().getToolBar());
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public void undockingComplete(DockingEvent evt) {
        super.undockingComplete(evt);
        addToolBar(null);
    }

    /**
     * {@inheritDoc}
     */
    public void treeCollapsed(TreeExpansionEvent evt) { }

    /**
     * {@inheritDoc}
     */
    public void treeExpanded(TreeExpansionEvent evt) {
        if (!locked) {
            TreePath path = evt.getPath();
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
            ScilabEditorPane savePane = pane;
            ScilabDocument saveDoc = doc;
            boolean update = false;
            if (node.getUserObject() instanceof String && SciNotesMessages.FUNCTIONS.equals(node.getUserObject())) {
                pane = (ScilabEditorPane) ((DefaultMutableTreeNode) node.getParent()).getUserObject();
                doc = (ScilabDocument) pane.getDocument();
                locked = true;
                updateFunctions();
                functionNavigator.expandPath(mapFunPath.get(pane));
                update = true;
            } else if (node.getUserObject() instanceof String && SciNotesMessages.ANCHORS.equals(node.getUserObject())) {
                pane = (ScilabEditorPane) ((DefaultMutableTreeNode) node.getParent()).getUserObject();
                doc = (ScilabDocument) pane.getDocument();
                locked = true;
                updateAnchors();
                functionNavigator.expandPath(mapAnchorPath.get(pane));
                update = true;
            } else if (node.getUserObject() instanceof ScilabEditorPane) {
                pane = (ScilabEditorPane) node.getUserObject();
                doc = (ScilabDocument) pane.getDocument();
                boolean expanded = functionNavigator.isExpanded(mapFunPath.get(pane));
                updateFunctions();
                if (expanded) {
                    locked = true;
                    functionNavigator.expandPath(mapFunPath.get(pane));
                }
                update = true;
            }
            if (update) {
                pane = savePane;
                doc = saveDoc;
            }
        } else {
            locked = false;
        }
    }

    /**
     * @param pane the pane which has been splitted
     * @param split the new pane
     */
    public static void changePaneOnSplit(ScilabEditorPane pane, ScilabEditorPane split) {
        if (mapNode.containsKey(pane)) {
            DefaultMutableTreeNode node = mapNode.get(pane);
            node.setUserObject(split);
            mapNode.put(split, mapNode.get(pane));
            mapNode.remove(pane);
            mapFunPath.put(split, mapFunPath.get(pane));
            mapFunPath.remove(pane);
            mapAnchorPath.put(split, mapAnchorPath.get(pane));
            mapAnchorPath.remove(pane);
        }
    }

    /**
     * @param pane to remove
     */
    public static void removePane(ScilabEditorPane pane) {
        if (mapNode.containsKey(pane)) {
            DefaultMutableTreeNode node = mapNode.get(pane);
            ((DefaultMutableTreeNode) model.getRoot()).remove(node);
            model.reload();
            mapNode.remove(pane);
            mapFunPath.remove(pane);
            mapAnchorPath.remove(pane);
        }
    }

    /**
     * This method is called from within the constructor to
     * initialize the form.
     */
    private void initComponents() {
        parentWindow = ScilabWindow.createWindow();
        parentWindow.addTab(this);
        labelNumerotation = new JLabel();
        lineNumber = new JTextField();
        numType = new JComboBox();
        jSeparator1 = new JSeparator();
        labelGotoLine = new JLabel();
        scrollPane = new JScrollPane();

        parentWindow.setTitle(SciNotesMessages.CODE_NAVIGATOR);
        updateUI();

        labelNumerotation.setText(SciNotesMessages.NUMEROTATION);
        labelNumerotation.setFocusable(false);

        lineNumber.addKeyListener(new KeyListener() {
                public void keyTyped(KeyEvent ke) { }
                public void keyPressed(KeyEvent ke) { }

                public void keyReleased(KeyEvent ke) {
                    updateCaretPosition();
                }
            });

        lineNumber.addFocusListener(new FocusListener() {
                public void focusGained(FocusEvent e) {
                    updateCaretPosition();
                }

                public void focusLost(FocusEvent e) {
                }
            });

        numType.setModel(new DefaultComboBoxModel(new String[] {SciNotesMessages.ABSOLUTE, SciNotesMessages.RELATIVE}));
        numType.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent evt) {
                    int i = numType.getSelectedIndex();
                    if (i == 0 && !isAbsolute) {
                        lineNumber.setText(EMPTY);
                        isAbsolute = true;
                    } else if (i == 1 && isAbsolute) {
                        lineNumber.setText(EMPTY);
                        isAbsolute = false;
                    }
                }
            });

        labelGotoLine.setText(SciNotesMessages.GO_TO_LINE);
        labelGotoLine.setFocusable(false);

        functionNavigator.addMouseListener(new MouseAdapter() {
                public void mousePressed(MouseEvent e) {
                    int row = functionNavigator.getRowForLocation(e.getX(), e.getY());
                    if (e.getClickCount() == 2) {
                        handleSelectedItem(row);
                    }
                }
            });

        functionNavigator.getInputMap().put(KeyStroke.getKeyStroke("ENTER"), "toggle");
        functionNavigator.addKeyListener(new KeyAdapter() {
                public void keyTyped(KeyEvent e) {
                    if (e.getKeyChar() == '\n') {
                        int row = functionNavigator.getMinSelectionRow();
                        handleSelectedItem(row);
                    }
                }
            });
        createPopupMenuOnJTree();

        scrollPane.setViewportView(functionNavigator);

        /* Begin NetBeans */
        GroupLayout layout = new GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(layout.createParallelGroup(Alignment.LEADING)
                                  .addGroup(layout.createSequentialGroup()
                                            .addContainerGap()
                                            .addGroup(layout.createParallelGroup(Alignment.LEADING)
                                                      .addComponent(scrollPane, Alignment.TRAILING, GroupLayout.DEFAULT_SIZE, 200, Short.MAX_VALUE)
                                                      .addComponent(jSeparator1, GroupLayout.DEFAULT_SIZE, 200, Short.MAX_VALUE)
                                                      .addGroup(layout.createSequentialGroup()
                                                                .addGroup(layout.createParallelGroup(Alignment.LEADING)
                                                                          .addComponent(labelGotoLine)
                                                                          .addComponent(labelNumerotation))
                                                                .addGap(18, 18, 18)
                                                                .addGroup(layout.createParallelGroup(Alignment.LEADING)
                                                                          .addComponent(lineNumber, GroupLayout.PREFERRED_SIZE, 58, GroupLayout.PREFERRED_SIZE)
                                                                          .addComponent(numType, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))))
                                            .addContainerGap())
                                  );
        layout.setVerticalGroup(layout.createParallelGroup(Alignment.LEADING)
                                .addGroup(Alignment.TRAILING, layout.createSequentialGroup()
                                          .addContainerGap()
                                          .addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 439, Short.MAX_VALUE)
                                          .addPreferredGap(ComponentPlacement.RELATED)
                                          .addComponent(jSeparator1, GroupLayout.PREFERRED_SIZE, 10, GroupLayout.PREFERRED_SIZE)
                                          .addPreferredGap(ComponentPlacement.RELATED)
                                          .addGroup(layout.createParallelGroup(Alignment.BASELINE)
                                                    .addComponent(labelGotoLine)
                                                    .addComponent(lineNumber, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                                          .addGap(4, 4, 4)
                                          .addGroup(layout.createParallelGroup(Alignment.BASELINE)
                                                    .addComponent(labelNumerotation)
                                                    .addComponent(numType, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                                          .addContainerGap())
                                );
        /* End NetBeans */

        List<Component> components = new ArrayList(3);
        components.add(functionNavigator);
        components.add(lineNumber);
        components.add(numType);
        setFocusable(true);
        setFocusCycleRoot(true);
        setFocusTraversalPolicy(new NavigatorFocusTraversalPolicy(components));

        setMinimumSize(new Dimension(224, 0));
        setPreferredSize(new Dimension(224, 543));
        ((SwingScilabWindow) parentWindow.getAsSimpleWindow()).pack();
        ((SwingScilabWindow) parentWindow.getAsSimpleWindow()).setVisible(true);

        CallBack callback = new CallBack(null) {
                public void callBack() {
                    NavigatorWindow.this.closeNavigator();
                }

                public void actionPerformed(ActionEvent e) {
                    callBack();
                }
            };
        setCallback(callback);
        MenuBar menubar = ScilabMenuBar.createMenuBar();
        Menu fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(SciNotesMessages.FILE);
        fileMenu.setMnemonic('F');
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(callback);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(SciNotesGUI.getActionKeyMap().get("ExitAction"));
        menu.setText(SciNotesMessages.EXIT);
        fileMenu.add(menu);
        menubar.add(fileMenu);

        Menu orderMenu = ScilabMenu.createMenu();
        orderMenu.setText(SciNotesMessages.ORDER);
        orderMenu.setMnemonic('O');
        menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new CallBack(null) {
                public void callBack() {
                    doc.setAlphaOrderInTree(true);
                    updateTree();
                }

                public void actionPerformed(ActionEvent e) {
                    callBack();
                }
            });
        menu.setText(SciNotesMessages.ALPHABETIC_ORDER);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(KeyStroke.getKeyStroke("alt A"));
        orderMenu.add(menu);

        menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new CallBack(null) {
                public void callBack() {
                    doc.setAlphaOrderInTree(false);
                    updateTree();
                }

                public void actionPerformed(ActionEvent e) {
                    callBack();
                }
            });
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(KeyStroke.getKeyStroke("alt N"));
        menu.setText(SciNotesMessages.NATURAL_ORDER);
        orderMenu.add(menu);

        menubar.add(orderMenu);
        addMenuBar(menubar);

        TextBox infobar = ScilabTextBox.createTextBox();
        addInfoBar(infobar);
    }

    /**
     * Init the tree
     */
    private static void initTree() {
        mapNode.clear();
        mapFunPath.clear();
        List<SciNotes> eds = SciNotes.getSciNotesList();
        if (functionNavigator == null) {
            functionNavigator = new JTree();
        }
        DefaultMutableTreeNode root = new DefaultMutableTreeNode("root");
        model = new DefaultTreeModel(root);

        for (SciNotes ed : eds) {
            int count = ed.getTabPane().getTabCount();
            for (int i = 0; i < count; i++) {
                ScilabEditorPane sep = (ScilabEditorPane) ed.getTextPane(i);
                DefaultMutableTreeNode base = new DefaultMutableTreeNode(sep);
                mapNode.put(sep, base);
                DefaultMutableTreeNode funs = new DefaultMutableTreeNode(SciNotesMessages.FUNCTIONS);
                ScilabDocument scidoc = (ScilabDocument) sep.getDocument();
                scidoc.fillTreeFuns(funs);
                DefaultMutableTreeNode anchors = new DefaultMutableTreeNode(SciNotesMessages.ANCHORS);
                scidoc.fillTreeAnchors(anchors);
                base.add(funs);
                base.add(anchors);
                root.add(base);
                mapFunPath.put(sep, new TreePath(model.getPathToRoot(funs)));
                mapAnchorPath.put(sep, new TreePath(model.getPathToRoot(anchors)));
            }
        }

        functionNavigator.setModel(model);
        functionNavigator.setRootVisible(false);
    }

    /**
     * Update the tree
     */
    private static void updateTree() {
        DefaultMutableTreeNode current = mapNode.get(pane);
        TreePath funPath = mapFunPath.get(pane);
        TreePath anchorPath = mapAnchorPath.get(pane);
        DefaultMutableTreeNode funs = (DefaultMutableTreeNode) current.getFirstChild();
        DefaultMutableTreeNode anchors = (DefaultMutableTreeNode) current.getChildAt(1);
        boolean expFuns = false;
        boolean expAnchors = false;

        if (functionNavigator.isExpanded(funPath) || funs.isLeaf()) {
            current.removeAllChildren();
            funs.removeAllChildren();
            doc.fillTreeFuns(funs);
            current.add(funs);
            current.add(anchors);
            model.reload(current);
            expFuns = true;
        }

        if (functionNavigator.isExpanded(anchorPath) || anchors.isLeaf()) {
            current.removeAllChildren();
            anchors.removeAllChildren();
            doc.fillTreeAnchors(anchors);
            current.add(funs);
            current.add(anchors);
            model.reload(current);
            expAnchors = true;
        }

        if (expFuns) {
            functionNavigator.expandPath(funPath);
        }
        if (expAnchors) {
            functionNavigator.expandPath(anchorPath);
        }
    }

    /**
     * Update a branch Functions in the tree
     */
    private static void updateFunctions() {
        DefaultMutableTreeNode current = mapNode.get(pane);
        DefaultMutableTreeNode funs = (DefaultMutableTreeNode) current.getFirstChild();
        DefaultMutableTreeNode anchors = (DefaultMutableTreeNode) current.getChildAt(1);
        current.removeAllChildren();
        funs.removeAllChildren();
        doc.fillTreeFuns(funs);
        current.add(funs);
        current.add(anchors);
        model.reload(current);
    }

    /**
     * Update a branch Anchors in the tree
     */
    private static void updateAnchors() {
        DefaultMutableTreeNode current = mapNode.get(pane);
        DefaultMutableTreeNode funs = (DefaultMutableTreeNode) current.getFirstChild();
        DefaultMutableTreeNode anchors = (DefaultMutableTreeNode) current.getChildAt(1);
        current.removeAllChildren();
        anchors.removeAllChildren();
        doc.fillTreeAnchors(anchors);
        current.add(funs);
        current.add(anchors);
        model.reload(current);
    }

    /**
     * Creates a popup menu on right click
     */
    private void createPopupMenuOnJTree() {
        final JPopupMenu popup = new JPopupMenu();

        final JMenuItem alpha = new JMenuItem(SciNotesMessages.ALPHABETIC_ORDER);
        final JMenuItem natural = new JMenuItem(SciNotesMessages.NATURAL_ORDER);

        alpha.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    handleOrder(true);
                }
            });
        popup.add(alpha);

        natural.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    handleOrder(false);
                }
            });
        popup.add(natural);
        functionNavigator.setComponentPopupMenu(popup);
    }

    /**
     * @param alpha true for alphabetic order
     */
    private static void handleOrder(boolean alpha) {
        int row = functionNavigator.getMinSelectionRow();
        if (row == -1) {
            functionNavigator.setSelectionRow(0);
            row = 0;
        }

        TreePath path = functionNavigator.getPathForRow(row);
        ScilabDocument saveDoc = doc;
        ScilabEditorPane savePane = pane;
        doc = getDocumentInNode((DefaultMutableTreeNode) path.getLastPathComponent());
        pane = getPaneInNode((DefaultMutableTreeNode) path.getLastPathComponent());
        doc.setAlphaOrderInTree(alpha);
        updateTree();
        doc = saveDoc;
        pane = savePane;
    }

    /**
     * Update the line numbering on a change in the document
     * @param offset offset where the event occured
     * @param length length of inserted or removed text
     */
    private void handleEvent(int offset, int length) {
        Element root = doc.getDefaultRootElement();
        Element line = root.getElement(root.getElementIndex(offset));
        if (line instanceof ScilabDocument.ScilabLeafElement) {
            ((ScilabDocument.ScilabLeafElement) line).resetType();
            updateTree();
        }
    }

    /**
     * Handle a selected item in the JTree
     * @param row to handle
     */
    private void handleSelectedItem(int row) {
        TreePath path = functionNavigator.getPathForRow(row);
        if (row != -1) {
            DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
            if (node.isLeaf() && node.getUserObject() instanceof ScilabDocument.ScilabLeafElement) {
                ScilabEditorPane sep = (ScilabEditorPane) ((DefaultMutableTreeNode) node.getParent().getParent()).getUserObject();
                int pos = ((ScilabDocument.ScilabLeafElement) node.getUserObject()).getStart();
                if (pos != -1) {
                    sep.getEditor().getTabPane().setSelectedComponent(sep.getEditorComponent());
                    sep.scrollTextToPos(pos);
                    pos = sep.getDocument().getDefaultRootElement().getElementIndex(pos) + 1;
                    numType.setSelectedItem(SciNotesMessages.ABSOLUTE);
                    lineNumber.setText(Integer.toString(pos));
                }
            }
        }
    }

    /**
     * Update the caret position when a number is entered in the field
     */
    private void updateCaretPosition() {
        int line = 0;
        boolean correct = false;
        TreePath path = functionNavigator.getSelectionPath();
        DefaultMutableTreeNode node = null;
        if (path != null) {
            node = (DefaultMutableTreeNode) path.getLastPathComponent();
        }

        try {
            line = Integer.decode(lineNumber.getText()).intValue();
            if (isAbsolute) {
                correct = true;
                line = correctLineNumber(line);
                updatePaneDoc(node);
            } else if (functionNavigator.getRowCount() >= 2) {
                if (functionNavigator.isSelectionEmpty()) {
                    functionNavigator.setSelectionRow(1);
                }

                if (node.isLeaf() && node.getUserObject() instanceof ScilabDocument.ScilabLeafElement) {
                    int pos = ((ScilabDocument.ScilabLeafElement) node.getUserObject()).getStart();
                    if (pos != -1) {
                        updatePaneDoc(node);
                        line += doc.getDefaultRootElement().getElementIndex(pos);
                        correct = true;
                        line = correctLineNumber(line);
                    }
                }
            }
        } catch (NumberFormatException e) {
            correct = false;
        }

        setLineNumberColor(correct);
        if (correct) {
            pane.scrollTextToLineNumber(line, false);
        }
    }

    /**
     * @param node corresponding to the pane to select
     */
    private static void updatePaneDoc(DefaultMutableTreeNode node) {
        if (node != null) {
            ScilabEditorPane sep = getPaneInNode(node);
            if (sep != pane) {
                pane = sep;
                doc = (ScilabDocument) pane.getDocument();
                pane.getEditor().getTabPane().setSelectedComponent(pane.getEditorComponent());
            }
        }
    }

    /**
     * @param node corresponding to the document to get
     * @return the doc
     */
    private static ScilabDocument getDocumentInNode(DefaultMutableTreeNode node) {
        return (ScilabDocument) getPaneInNode(node).getDocument();
    }

    /**
     * @param node corresponding to the pane to get
     * @return the pane
     */
    private static ScilabEditorPane getPaneInNode(DefaultMutableTreeNode node) {
        if (node.isLeaf()) {
            DefaultMutableTreeNode node1 = (DefaultMutableTreeNode) node.getParent();
            if (node1 != null) {
                DefaultMutableTreeNode node2 = (DefaultMutableTreeNode) node1.getParent();
                if (node2 != null && node2.getUserObject() instanceof ScilabEditorPane) {
                    return (ScilabEditorPane) node2.getUserObject();
                }
            }
        } else if (node.getUserObject() instanceof String && SciNotesMessages.FUNCTIONS.equals(node.getUserObject())) {
            return (ScilabEditorPane) ((DefaultMutableTreeNode) node.getParent()).getUserObject();
        } else if (node.getUserObject() instanceof ScilabEditorPane) {
            return (ScilabEditorPane) node.getUserObject();
        }

        return pane;
    }

    /**
     * Checks if a line number is correct or not
     * @param line the line number
     * @return a correct line number
     */
    private int correctLineNumber(int line) {
        if (line < 0) {
            return 0;
        }

        int maxLine = doc.getDefaultRootElement().getElementCount();
        if (line >= maxLine) {
            return maxLine - 1;
        }

        return line;
    }

    /**
     * @param correct true if the lineNumber contains a number
     */
    private void setLineNumberColor(boolean correct) {
        if (correct && !lineNumberActive) {
            lineNumber.setForeground(Color.BLACK);
            lineNumberActive = true;
        } else if (!correct && lineNumberActive) {
            lineNumber.setForeground(Color.RED);
            lineNumberActive = false;
        }
    }

    /**
     * {@inheritDoc}
     */
    private static class NavigatorFocusTraversalPolicy extends FocusTraversalPolicy {

        private List<Component> cycle;

        /**
         * Default constructor
         * @param cycle the componenents where the focus move
         */
        public NavigatorFocusTraversalPolicy(List<Component> cycle) {
            this.cycle = cycle;
        }

        /**
         * {@inheritDoc}
         */
        public Component getComponentAfter(Container focusCycleRoot, Component aComponent) {
            int n = (cycle.indexOf(aComponent) + 1) % cycle.size();
            return cycle.get(n);
        }

        /**
         * {@inheritDoc}
         */
        public Component getComponentBefore(Container focusCycleRoot, Component aComponent) {
            int n = (cycle.size() + cycle.indexOf(aComponent) - 1) % cycle.size();
            return cycle.get(n);
        }

        /**
         * {@inheritDoc}
         */
        public Component getDefaultComponent(Container focusCycleRoot) {
            return cycle.get(0);
        }

        /**
         * {@inheritDoc}
         */
        public Component getLastComponent(Container focusCycleRoot) {
            return cycle.get(cycle.size() - 1);
        }

        /**
         * {@inheritDoc}
         */
        public Component getFirstComponent(Container focusCycleRoot) {
            return cycle.get(0);
        }
    }
}
