/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET
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

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FocusTraversalPolicy;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

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
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.TreeExpansionEvent;
import javax.swing.event.TreeExpansionListener;
import javax.swing.text.Element;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import org.flexdock.docking.event.DockingEvent;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.SciNotesGUI;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.tabfactory.CodeNavigatorTabFactory;

/**
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public final class NavigatorWindow extends SwingScilabDockablePanel implements DocumentListener,
    TreeExpansionListener {

    private static final String EMPTY = "";

    private Map<ScilabEditorPane, DefaultMutableTreeNode> mapNode = new HashMap();
    private Map<ScilabEditorPane, TreePath> mapFunPath = new HashMap();
    private Map<ScilabEditorPane, TreePath> mapAnchorPath = new HashMap();
    private List<ScilabEditorPane> panes = new ArrayList<ScilabEditorPane>();

    private SciNotes editor;

    private JTree functionNavigator;
    private DefaultTreeModel model;
    private ScilabEditorPane pane;
    private ScilabDocument doc;
    private boolean alphaOrder;

    private boolean isAbsolute = true;
    private boolean lineNumberActive = true;
    private boolean locked;
    private boolean init;

    private JSeparator jSeparator1;
    private JLabel labelGotoLine;
    private JLabel labelNumerotation;
    private JTextField lineNumber;
    private JComboBox numType;
    private JScrollPane scrollPane;

    private SwingScilabWindow parentWindow;

    static {
        ScilabTabFactory.getInstance().addTabFactory(CodeNavigatorTabFactory.getInstance());
    }

    /**
     * Creates new Navigator Window
     */
    public NavigatorWindow(SciNotes editor, String uuid) {
        super(SciNotesMessages.CODE_NAVIGATOR, uuid == null ? UUID.randomUUID().toString() : uuid);
        this.editor = editor;
        editor.addNavigator(this);
        setContentPane(new javax.swing.JPanel());
        ConfigSciNotesManager.saveCodeNavigatorState(editor.getPersistentId(), getPersistentId());
        WindowsConfigurationManager.restorationFinished(this);
    }

    /**
     * Creates new Navigator Window
     */
    public NavigatorWindow(SciNotes editor) {
        this(editor, null);
    }

    /**
     * @param doc the doc to update
     */
    public void update(ScilabDocument doc) {
        if (!init) {
            initTree();
            initComponents();
            functionNavigator.addTreeExpansionListener(this);
            init = true;
        }

        if (doc != null) {
            if (this.doc != null) {
                this.doc.removeDocumentListener(this);
            }

            this.doc = doc;
            this.pane = doc.getEditorPane();

            this.doc.addDocumentListener(this);

            if (!mapNode.containsKey(pane)) {
                initTree();
            } else {
                updateTree();
            }
        }
    }

    public void addEditorPane(ScilabEditorPane pane) {
        panes.add(pane);
        update((ScilabDocument) pane.getDocument());
    }

    /**
     * Update the navigator
     */
    public void update() {
        updateTree();
    }

    /**
     * @param doc to use
     */
    public void activateNavigator(ScilabDocument doc) {
        initTree();
        update(doc);
    }

    /**
     * Set the parent window
     */
    public void setParentWindow() {
        this.parentWindow = SwingScilabWindow.createWindow(true);
        parentWindow.addTab(this);
        parentWindow.setVisible(true);
    }

    /**
     * Get the parent window id for this tab
     * @return the id of the parent window
     */
    public SwingScilabWindow getParentWindow() {
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

    /**
     * Close this Navigator.
     */
    public void closeNavigator() {
        editor.removeNavigator();
        mapNode.clear();
        mapFunPath.clear();
        mapAnchorPath.clear();
        functionNavigator = null;
        model = null;
        pane = null;
        doc = null;
        for (ScilabEditorPane p : panes) {
            ((ScilabDocument) p.getDocument()).removeDocumentListener(this);
        }
        panes.clear();
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
        changeToolBar();
    }

    /**
     * Try to add a SciNotes toolbar
     */
    public void changeToolBar() {
        SwingScilabWindow win = (SwingScilabWindow) SwingUtilities.getAncestorOfClass(SwingScilabWindow.class, this);
        if (win != null && win.getDockingPort() != null) {
            Set<SwingScilabDockablePanel> set = (Set<SwingScilabDockablePanel>) win.getDockingPort().getDockables();
            for (SwingScilabDockablePanel tab : set) {
                if (tab == editor) {
                    addToolBar(editor.getToolBar());
                    break;
                }
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
    public void changePaneOnSplit(ScilabEditorPane pane, ScilabEditorPane split) {
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
    public void removePane(ScilabEditorPane pane) {
        if (mapNode.containsKey(pane)) {
            DefaultMutableTreeNode node = mapNode.get(pane);
            ((DefaultMutableTreeNode) model.getRoot()).remove(node);
            model.reload();
            mapNode.remove(pane);
            mapFunPath.remove(pane);
            mapAnchorPath.remove(pane);
            panes.remove(pane);
            this.pane = null;
            this.doc = null;
        }
    }

    /**
     * This method is called from within the constructor to
     * initialize the form.
     */
    private void initComponents() {
        labelNumerotation = new JLabel();
        lineNumber = new JTextField();
        numType = new JComboBox();
        jSeparator1 = new JSeparator();
        labelGotoLine = new JLabel();
        scrollPane = new JScrollPane();

        setTitle(SciNotesMessages.CODE_NAVIGATOR);
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

        CommonCallBack callback = new CommonCallBack(null) {
            public void callBack() {
                ClosingOperationsManager.startClosingOperation((SwingScilabDockablePanel) NavigatorWindow.this);
            }

            public void actionPerformed(ActionEvent e) {
                callBack();
            }
        };

        MenuBar menubar = ScilabMenuBar.createMenuBar();
        Menu fileMenu = ScilabMenu.createMenu();
        fileMenu.setText(SciNotesMessages.FILE);
        fileMenu.setMnemonic('F');
        MenuItem menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(callback);
        ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(SciNotes.getActionKeys().get("scinotes-exit"));
        menu.setText(SciNotesMessages.EXIT);
        fileMenu.add(menu);
        menubar.add(fileMenu);

        Menu orderMenu = ScilabMenu.createMenu();
        orderMenu.setText(SciNotesMessages.ORDER);
        orderMenu.setMnemonic('O');
        menu = ScilabMenuItem.createMenuItem();
        menu.setCallback(new CommonCallBack(null) {
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
        menu.setCallback(new CommonCallBack(null) {
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
    public void initTree() {
        mapNode.clear();
        mapFunPath.clear();
        if (functionNavigator == null) {
            functionNavigator = new JTree();
        }
        DefaultMutableTreeNode root = new DefaultMutableTreeNode("root");
        model = new DefaultTreeModel(root);

        for (ScilabEditorPane sep : panes) {
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

        functionNavigator.setModel(model);
        functionNavigator.setRootVisible(false);
    }

    /**
     * Update the tree
     */
    public void updateTree() {
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
    private void updateFunctions() {
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
    private void updateAnchors() {
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
    private void handleOrder(boolean alpha) {
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
     * @param offset offset where the event occurred
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
                int pos = ((ScilabDocument.ScilabLeafElement) node.getUserObject()).getStartOffset();
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
                    int pos = ((ScilabDocument.ScilabLeafElement) node.getUserObject()).getStartOffset();
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
    private void updatePaneDoc(DefaultMutableTreeNode node) {
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
    private ScilabDocument getDocumentInNode(DefaultMutableTreeNode node) {
        return (ScilabDocument) getPaneInNode(node).getDocument();
    }

    /**
     * @param node corresponding to the pane to get
     * @return the pane
     */
    private ScilabEditorPane getPaneInNode(DefaultMutableTreeNode node) {
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
