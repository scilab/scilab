/*
 * Scilab (https://www.scilab.org/) - This file is part of Scilab
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

package org.scilab.modules.scinotes.utils;

import java.awt.Component;
import java.awt.Container;
import java.awt.DefaultFocusTraversalPolicy;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.StringSelection;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.DragSourceDragEvent;
import java.awt.dnd.DragSourceDropEvent;
import java.awt.dnd.DragSourceEvent;
import java.awt.dnd.DragSourceListener;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.IOException;
import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JRootPane;
import javax.swing.JTabbedPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.utils.ClosingOperationsManager;

import org.scilab.modules.scinotes.EditorComponent;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.actions.CloseAction;
import org.scilab.modules.scinotes.actions.CloseAllButThisAction;
import org.scilab.modules.scinotes.actions.RestoreOpenedFilesAction;
import org.scilab.modules.scinotes.actions.SaveAction;

/**
 * Class for a tabbedpane with close-button
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabTabbedPane extends JTabbedPane implements DragGestureListener,
    DragSourceListener,
    DropTargetListener,
    Transferable {

    private static final ImageIcon CLOSEICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/close-tab.png");
    private static final ImageIcon CLOSEONICON = new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/close-tab-on.png");
    private static final int BUTTONSIZE = 18;

    private static DataFlavor DATAFLAVOR;
    private static ScilabTabbedPane currentWhenDragged;

    /**
     * What a tab drag carries: the source tabbed pane, never the pane itself.
     * Windows offers the drag data to every native drop target on the way,
     * and Chromium (the JCEF browsers) asks for it: Java then serializes the
     * transferred object. Serializing a Swing component uninstalls the UI of
     * its whole tree, and a non-serializable listener made it fail before the
     * UIs were installed back: the source window was left unable to paint.
     * The reference is transient, so only this empty holder is serialized; a
     * drop in the same JVM gets the object itself.
     */
    static final class TabTransfer implements Serializable {
        private static final long serialVersionUID = 1L;
        final transient ScilabTabbedPane source;

        TabTransfer(ScilabTabbedPane source) {
            this.source = source;
        }
    }

    static {
        try {
            DATAFLAVOR = new DataFlavor(DataFlavor.javaJVMLocalObjectMimeType + ";class=" + TabTransfer.class.getName());
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    private SciNotes editor;
    private int draggedIndex;

    /**
     * Constructor
     * @param editor the editor
     */
    public ScilabTabbedPane(SciNotes editor) {
        super();
        this.editor = editor;
        setFocusTraversalPolicy(new java.awt.DefaultFocusTraversalPolicy() {
            public Component getComponentAfter(Container aContainer, Component aComponent) {
                if (aComponent instanceof ScilabEditorPane) {
                    return aComponent;
                }
                return super.getComponentAfter(aContainer, aComponent);
            }
        });

        setFocusCycleRoot(true);

        setComponentPopupMenu(createPopupMenu());
        DragSource dragsource = DragSource.getDefaultDragSource();
        dragsource.createDefaultDragGestureRecognizer(this, DnDConstants.ACTION_MOVE, this);
        DropTarget droptarget = new DropTarget(this, this);
        addMouseListener(new MouseAdapter() {
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 1 && SwingUtilities.isMiddleMouseButton(e)) {
                    int index = indexAtLocation(e.getX(), e.getY());
                    ((CloseTabButton) getTabComponentAt(index)).closeTab();
                    e.consume();
                }
            }
        });
    }

    /**
     * {@inheritDoc}
     */
    public void insertTab(String title, Icon icon, Component component, String tip, int index) {
        // Swing renders an empty tooltip as a small blank yellow rectangle.
        super.insertTab(title, icon, component, tip == null || tip.isEmpty() ? null : tip, index);
        setTabComponentAt(index, new CloseTabButton(title));
    }

    /**
     * @param index the index of the tab
     * @return the title of the tab
     */
    public String getScilabTitleAt(int index) {
        CloseTabButton tab = (CloseTabButton) getTabComponentAt(index);
        if (tab != null) {
            return tab.getText();
        }

        return "";
    }

    /**
     * {@inheritDoc}
     */
    public String getTitleAt(int index) {
        return "";
    }

    /**
     * {@inheritDoc}
     */
    public void setTitleAt(int index, String title) {
        CloseTabButton tab = (CloseTabButton) getTabComponentAt(index);
        if (tab != null) {
            tab.setText(title);
        }
    }

    /**
     * Implements getTransferData in Transferable
     * @param flavor to get
     * @return the object to transfer
     * @throws UnsupportedFlavorException for an unknown flavor
     */
    public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException {
        if (!flavor.equals(DATAFLAVOR)) {
            throw new UnsupportedFlavorException(flavor);
        }

        return new TabTransfer(this);
    }

    /**
     * Implements getTransferDataFlavors in Transferable
     * @return the supported flavors
     */
    public DataFlavor[] getTransferDataFlavors() {
        return new DataFlavor[] {DATAFLAVOR};
    }

    /**
     * Implements isDataFlavorSupported in Transferable
     * @param flavor to support
     * @return true if the flavor is supported
     */
    public boolean isDataFlavorSupported(DataFlavor flavor) {
        return flavor.equals(DATAFLAVOR);
    }

    /**
     * Interface DropTargetListener
     * {@inheritDoc}
     */
    public void dragEnter(DropTargetDragEvent dtde) {
        if (dtde.isDataFlavorSupported(DATAFLAVOR)) {
            dtde.acceptDrag(DnDConstants.ACTION_MOVE);
            Component c = dtde.getDropTargetContext().getComponent();
            if (c instanceof ScilabTabbedPane) {
                currentWhenDragged = (ScilabTabbedPane) c;
            }
        } else {
            dtde.rejectDrag();
        }
    }

    /**
     * Interface DropTargetListener
     * {@inheritDoc}
     */
    public void dragOver(DropTargetDragEvent dtde) {
        if (dtde.isDataFlavorSupported(DATAFLAVOR)) {
            dtde.acceptDrag(DnDConstants.ACTION_MOVE);
        } else {
            dtde.rejectDrag();
        }
    }

    /**
     * Interface DropTargetListener
     * {@inheritDoc}
     */
    public void drop(DropTargetDropEvent dtde) {
        if (!dtde.isDataFlavorSupported(DATAFLAVOR) || dtde.getSourceActions() != DnDConstants.ACTION_MOVE) {
            dtde.rejectDrop();
        } else {
            try {
                ScilabTabbedPane tabbedPane = ((TabTransfer) dtde.getTransferable().getTransferData(DATAFLAVOR)).source;
                int index = indexAtLocation(dtde.getLocation().x, dtde.getLocation().y);
                if (tabbedPane == null) {
                    dtde.rejectDrop();
                } else if (tabbedPane == this && (index == tabbedPane.draggedIndex || getTabCount() == 1)) {
                    dtde.rejectDrop();
                } else {
                    ScilabEditorPane sourcePane = tabbedPane.editor.getTextPane(tabbedPane.draggedIndex);
                    Component c = tabbedPane.getComponentAt(tabbedPane.draggedIndex);
                    String title = tabbedPane.getScilabTitleAt(tabbedPane.draggedIndex);

                    /*
                     * A Monaco pane owns a model in the SharedBrowser of the
                     * SciNotes window that constructed it. Moving its Swing
                     * component and only changing setEditor() leaves that
                     * immutable ownership pointing at the source window.
                     * Create a destination-owned pane instead, then close the
                     * source after the native DnD callback has completed.
                     */
                    if (tabbedPane != this && sourcePane instanceof MonacoEditorPane) {
                        if (index == -1) {
                            index = getTabCount();
                        }
                        ScilabEditorPane copiedPane = SciNotes.copyTabForTransfer(editor, sourcePane, title, index);
                        if (copiedPane == null) {
                            dtde.rejectDrop();
                            return;
                        }

                        SciNotes sourceEditor = tabbedPane.editor;
                        int copiedIndex = indexOfComponent(copiedPane.getEditorComponent());
                        boolean sourceHidden = SciNotes.prepareTabTransferSource(sourceEditor, sourcePane);
                        dtde.acceptDrop(DnDConstants.ACTION_MOVE);
                        dtde.dropComplete(true);

                        SwingUtilities.invokeLater(new Runnable() {
                            public void run() {
                                if (!SciNotes.activateTransferredTab(editor, copiedPane)) {
                                    int failedIndex = editor.getTextPaneIndex(copiedPane);
                                    if (failedIndex >= 0) {
                                        editor.closeTabAtWithoutConfirmation(failedIndex);
                                    }
                                    SciNotes.restoreTabTransferSource(sourceEditor, sourcePane, sourceHidden);
                                    return;
                                }
                                if (copiedPane.getName() != null) {
                                    ConfigSciNotesManager.saveToOpenFiles(copiedPane.getName(), editor,
                                            copiedPane, copiedIndex);
                                }
                                editor.updateTabTitle();

                                SwingUtilities.invokeLater(new Runnable() {
                                    public void run() {
                                        /*
                                         * Closing the source tab can auto-select a
                                         * sibling tab still open in the source
                                         * window. Without this guard that sibling's
                                         * MonacoEditorPane would grab native CEF
                                         * focus for the source browser and
                                         * immediately hand it back, flashing both
                                         * windows' titlebars right after the tab
                                         * was just moved.
                                         */
                                        SciNotes.setTabTransferInProgress(true);
                                        try {
                                            int sourceIndex = sourceEditor.getTextPaneIndex(sourcePane);
                                            if (sourceIndex >= 0) {
                                                sourceEditor.closeTabAtWithoutConfirmation(sourceIndex);
                                            }
                                            if (sourceEditor.getTabPane().getTabCount() == 0) {
                                                SciNotes.closeEditor(sourceEditor);
                                            }
                                        } finally {
                                            SciNotes.setTabTransferInProgress(false);
                                        }
                                        if (editor.getTextPaneIndex(copiedPane) >= 0) {
                                            copiedPane.requestFocus();
                                        }
                                    }
                                });
                            }
                        });
                        return;
                    }

                    ConfigSciNotesManager.removeFromOpenFiles(tabbedPane.editor, tabbedPane.editor.getTextPane(tabbedPane.draggedIndex));
                    if (tabbedPane.editor.getNavigator() != null) {
                        tabbedPane.editor.getNavigator().removePane(tabbedPane.editor.getTextPane(tabbedPane.draggedIndex));
                    }
                    tabbedPane.remove(c);
                    if (tabbedPane.getTabCount() == 0) {
                        SciNotes.closeEditor(tabbedPane.editor);
                    }
                    if (index == -1) {
                        index = getTabCount();
                    }

                    insertTab(title, null, c, null, index);
                    ScilabEditorPane sep = editor.getTextPane(indexOfComponent(c));

                    if (tabbedPane != this) {
                        sep.setEditor(editor);
                        editor.initInputMap(sep);
                    }

                    if (sep.getName() != null) {
                        ConfigSciNotesManager.saveToOpenFiles(sep.getName(), editor, sep, index);
                    } else if (tabbedPane != this) {
                        int n = editor.getNumberForEmptyTab();
                        sep.setShortName(SciNotesMessages.UNTITLED + n);
                        sep.setTitle(SciNotesMessages.UNTITLED + n);
                        setTitleAt(index, SciNotesMessages.UNTITLED + n);
                        editor.setTitle(sep.getTitle());
                    }

                    setSelectedIndex(index);
                    editor.updateTabTitle();

                    dtde.acceptDrop(DnDConstants.ACTION_MOVE);
                    dtde.dropComplete(true);
                }
            } catch (IOException e) {
                dtde.rejectDrop();
            } catch (UnsupportedFlavorException e) {
                dtde.rejectDrop();
            }
        }
    }

    /**
     * Interface DropTargetListener
     * {@inheritDoc}
     */
    public void dropActionChanged(DropTargetDragEvent dtde) { }

    /**
     * Interface DropTargetListener
     * {@inheritDoc}
     */
    public void dragExit(DropTargetEvent dte) { }

    /**
     * Interface DragSourceListener
    * {@inheritDoc}
    */
    public void dragDropEnd(DragSourceDropEvent dsde) {
        /*
         * A target SciNotes already completed the transfer transaction. Do
         * not second-guess that result from screen geometry: while Monaco's
         * heavyweight JCEF component is being reparented, the point lookup
         * can transiently return null and used to start a second "drop outside"
         * clone/close operation for the same source pane.
         */
        if (dsde.getDropSuccess()) {
            draggedIndex = -1;
            currentWhenDragged = null;
            return;
        }

        Component c = null;
        for (SciNotes sn : SciNotes.getSciNotesList()) {
            try {
                Point point = new Point(dsde.getLocation());
                SwingUtilities.convertPointFromScreen(point, sn);
                c = SwingUtilities.getDeepestComponentAt(sn, (int) point.getX(), (int) point.getY());
            } catch (NullPointerException e) {
                e.printStackTrace();
            }
            if (c != null) {
                break;
            }
        }

        if (c == null) {
            if (draggedIndex >= 0 && draggedIndex < getTabCount()) {
                setSelectedIndex(draggedIndex);
            }
            SciNotes.cloneAndCloseCurrentTab(editor, true, dsde.getX(), dsde.getY());
        }
        draggedIndex = -1;
        currentWhenDragged = null;
    }

    /**
     * Interface DragSourceListener
     * {@inheritDoc}
     */
    public void dragExit(DragSourceEvent dse) { }

    /**
     * Interface DragSourceListener
     * {@inheritDoc}
     */
    public void dragOver(DragSourceDragEvent dsde) {
        Point pt = new Point(dsde.getX(), dsde.getY());
        JRootPane rootpane = (JRootPane) SwingUtilities.getAncestorOfClass(JRootPane.class, currentWhenDragged);

        if (rootpane != null) {
            SwingUtilities.convertPointFromScreen(pt, rootpane);
            Component c = SwingUtilities.getDeepestComponentAt(rootpane, pt.x, pt.y);

            if (c != null && !(c instanceof ScilabEditorPane) && !(c instanceof ScilabTabbedPane)) {
                c = SwingUtilities.getAncestorOfClass(ScilabTabbedPane.class, c);
            }
        }
    }

    /**
     * Interface DragSourceListener
     * {@inheritDoc}
     */
    public void dragEnter(DragSourceDragEvent dsde) { }

    /**
     * Interface DragSourceListener
     * {@inheritDoc}
     */
    public void dropActionChanged(DragSourceDragEvent dsde) { }

    /**
     * Interface DragGestureListener
     * {@inheritDoc}
     */
    public void dragGestureRecognized(DragGestureEvent dge) {
        InputEvent inputEvent = dge.getTriggerEvent();
        if (inputEvent instanceof MouseEvent) {
            MouseEvent mouseEvent = (MouseEvent) inputEvent;
            int index = indexAtLocation(mouseEvent.getX(), mouseEvent.getY());
            if (index == -1 || (getComponentAt(index) instanceof EditorComponent)) {
                currentWhenDragged = this;

                if (index != -1) {
                    draggedIndex = index;
                    dge.startDrag(DragSource.DefaultMoveDrop, this, this);
                }
            }
        }
    }

    /**
     * Create a popupmenu for the tabs
     * @return the created popupmenu
     */
    private JPopupMenu createPopupMenu() {
        JPopupMenu popup = new JPopupMenu() {
            public void show(Component invoker, int x, int y) {
                int index = ScilabTabbedPane.this.indexAtLocation(x, y);
                ScilabTabbedPane.this.setSelectedIndex(index);
                super.show(invoker, x, y);
            }
        };

        Map<String, KeyStroke> map = SciNotes.getActionKeys();

        SwingScilabMenuItem menuItem;
        menuItem = (SwingScilabMenuItem) SaveAction.createMenu(SciNotesMessages.SAVE, editor, map.get("scinotes-save")).getAsSimpleMenuItem();
        popup.add(menuItem);

        menuItem = (SwingScilabMenuItem) CloseAction.createMenu(SciNotesMessages.CLOSE, editor, map.get("scinotes-close")).getAsSimpleMenuItem();
        popup.add(menuItem);

        menuItem = (SwingScilabMenuItem) CloseAllButThisAction.createMenu(SciNotesMessages.CLOSEALLBUTTHIS, editor, map.get("scinotes-close-all-but")).getAsSimpleMenuItem();
        popup.add(menuItem);

        popup.addSeparator();

        final JMenuItem menuitem = new JMenuItem(SciNotesMessages.COPYFULLFILEPATH);
        menuitem.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent actionEvent) {
                if (editor.getTextPane() != null) {
                    StringSelection sel = new StringSelection(editor.getTextPane().getName());
                    Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
                }
            }
        });
        menuitem.addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                if (editor.getTextPane() != null) {
                    String name = editor.getTextPane().getName();
                    menuitem.setEnabled(name != null && !name.isEmpty());
                }
            }
        });
        popup.add(menuitem);

        return popup;
    }

    /**
     * Inner class to have a label and a button
     */
    class CloseTabButton extends JPanel {

        private JLabel label;

        /**
         * Constructor
         * @param title the title of the tab
         */
        public CloseTabButton(String title) {
            super(new FlowLayout(FlowLayout.LEFT, 0, 0));
            setOpaque(false);
            label = new JLabel(title);
            add(label);
            add(new JLabel("   "));
            add(new CloseButton());
        }

        /**
         * @param text the text for the tab
         */
        public void setText(String text) {
            label.setText(text);
        }

        /**
         * @return the text of the tab
         */
        public String getText() {
            return label.getText();
        }

        public void closeTab() {
            ScilabEditorPane sep = editor.getTextPane(editor.getTabPane().indexOfTabComponent(this));
            if (sep != null) {
                String name = sep.getName();
                editor.closeTabAt(editor.getTabPane().indexOfTabComponent(this));
                if (getTabCount() == 0) {
                    if (name != null) {
                        editor.addEmptyTab();
                    } else {
                        SciNotes.closeEditor(editor);
                    }
                }
            } else if (editor.getTabPane().getTabComponentAt(editor.getTabPane().indexOfTabComponent(this)) != null) {
                editor.getTabPane().remove(editor.getTabPane().indexOfTabComponent(this));
                if (editor.getTabPane().getTabCount() == 0) {
                    editor.addEmptyTab();
                }
                RestoreOpenedFilesAction.restoreEnabledComponents(editor);
            }
        }

        /**
         * Inner class for the close-button
         */
        class CloseButton extends JButton {

            /**
             * Constructor
             */
            CloseButton() {
                super(CLOSEICON);
                setFocusable(false);
                setContentAreaFilled(true);
                setOpaque(false);
                setRolloverEnabled(true);
                setRolloverIcon(CLOSEONICON);
                setBorderPainted(false);
                setPreferredSize(new Dimension(BUTTONSIZE, BUTTONSIZE));
                addActionListener(new ActionListener() {
                    public void actionPerformed(ActionEvent e) {
                        closeTab();
                    }
                });
            }
        }
    }
}
