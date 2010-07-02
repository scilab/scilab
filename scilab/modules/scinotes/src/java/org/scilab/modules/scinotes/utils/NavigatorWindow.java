/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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
import javax.swing.text.Element;
import javax.swing.event.DocumentListener;
import javax.swing.event.DocumentEvent;

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

/**
 *
 * @author Calixte DENIZET
 */
public class NavigatorWindow extends SwingScilabTab implements Tab, DocumentListener {

    private static final String EMPTY = "";

    private Window parentWindow;
    private ScilabEditorPane pane;
    private ScilabDocument doc;
    private boolean isAbsolute = true;
    private boolean lineNumberActive = true;
    private boolean alphaOrder;

    private JTree functionNavigator;
    private JSeparator jSeparator1;
    private JLabel labelGotoLine;
    private JLabel labelNumerotation;
    private JTextField lineNumber;
    private JComboBox numType;
    private JScrollPane scrollPane;

    /**
     * Creates new Navigator Window
     * @param pane the pane associated with this navigator
     */
    public NavigatorWindow(ScilabEditorPane pane) {
        super(SciNotesMessages.CODE_NAVIGATOR);
        this.pane = pane;
        pane.setNavigator(this);
        doc = (ScilabDocument) pane.getDocument();
        doc.addDocumentListener(this);
        initComponents();
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

    /**
     * Close this Navigator.
     */
    public void closeNavigator() {
        ScilabWindow win = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentWindowId());
        win.removeTab(this);
        setVisible(false);
        close();
        if (pane != null) {
            pane.setNavigator(null);
        }
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
        functionNavigator = new JTree();

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

        doc.fillTree(functionNavigator, alphaOrder);
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
                    alphaOrder = true;
                    doc.fillTree(functionNavigator, alphaOrder);
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
                    alphaOrder = false;
                    doc.fillTree(functionNavigator, alphaOrder);
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
     * Creates a popup menu on right click
     */
    private void createPopupMenuOnJTree() {
        final JPopupMenu popup = new JPopupMenu();

        final JMenuItem alpha = new JMenuItem(SciNotesMessages.ALPHABETIC_ORDER);
        final JMenuItem natural = new JMenuItem(SciNotesMessages.NATURAL_ORDER);

        alpha.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    alphaOrder = true;
                    doc.fillTree(functionNavigator, alphaOrder);
                }
            });
        popup.add(alpha);

        natural.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent actionEvent) {
                    alphaOrder = false;
                    doc.fillTree(functionNavigator, alphaOrder);
                }
            });
        popup.add(natural);
        functionNavigator.setComponentPopupMenu(popup);
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
            doc.fillTree(functionNavigator, alphaOrder);
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
            if (node.isLeaf()) {
                int pos = ((ScilabDocument.ScilabLeafElement) node.getUserObject()).getStart();
                if (pos != -1) {
                    pane.scrollTextToPos(pos);
                    pos = doc.getDefaultRootElement().getElementIndex(pos) + 1;
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

        try {
            line = Integer.decode(lineNumber.getText()).intValue() - 1;
            if (isAbsolute) {
                correct = true;
                line = correctLineNumber(line);
            } else if (functionNavigator.getRowCount() >= 2) {
                if (functionNavigator.isSelectionEmpty()) {
                    functionNavigator.setSelectionRow(1);
                }

                TreePath path = functionNavigator.getSelectionPath();
                DefaultMutableTreeNode node = (DefaultMutableTreeNode) path.getLastPathComponent();
                if (node.isLeaf()) {
                    int pos = ((ScilabDocument.ScilabLeafElement) node.getUserObject()).getStart();
                    if (pos != -1) {
                        line += doc.getDefaultRootElement().getElementIndex(pos) + 1;
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
            int start = doc.getDefaultRootElement().getElement(line).getStartOffset();
            pane.scrollTextToPos(start);
            pane.setCaretPosition(start);
        }
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
