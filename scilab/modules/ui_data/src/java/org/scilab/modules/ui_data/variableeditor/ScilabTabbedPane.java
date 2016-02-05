/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
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

package org.scilab.modules.ui_data.variableeditor;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Point;
import java.awt.datatransfer.DataFlavor;
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
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.io.IOException;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRootPane;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.events.callback.CommonCallBack;

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
    private static final int BUTTONSIZE = 18;

    private static DataFlavor DATAFLAVOR;
    private static ScilabTabbedPane currentWhenDragged;

    static {
        try {
            DATAFLAVOR = new DataFlavor(DataFlavor.javaJVMLocalObjectMimeType + ";class=" + ScilabTabbedPane.class.getName());
        } catch (ClassNotFoundException e) { }
    }

    private SwingScilabVariableEditor editor;
    private int draggedIndex;

    /**
     * Constructor
     * @param editor the editor
     */
    public ScilabTabbedPane(SwingScilabVariableEditor editor) {
        super();
        this.editor = editor;
        DragSource dragsource = DragSource.getDefaultDragSource();
        dragsource.createDefaultDragGestureRecognizer(this, DnDConstants.ACTION_MOVE, this);
        DropTarget droptarget = new DropTarget(this, this);
        addFocusListener(new FocusAdapter() {
            public void focusGained(FocusEvent e) {
                if (getSelectedIndex() > 0) {
                    ((JScrollPane) getComponentAt(getSelectedIndex())).getViewport().getView().requestFocus();
                }
            }
        });
    }

    /**
     * {@inheritDoc}
     */
    public void insertTab(String title, Icon icon, Component component, String tip, int index) {
        super.insertTab(title, icon, component, tip, index);
        setTabComponentAt(index, new CloseTabButton(title));
    }

    /**
     * {@inheritDoc}
     */
    public Component add(String title, Component c) {
        Component cc = super.add(title, c);
        setTitleAt(getTabCount() - 1, title);
        return cc;
    }

    /**
     * @param index the index of the tab
     * @return the title of the tab
     */
    public String getScilabTitleAt(int index) {
        if (index != -1) {
            CloseTabButton tab = (CloseTabButton) getTabComponentAt(index);
            if (tab != null) {
                return tab.getText();
            }
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
        if (index != -1) {
            CloseTabButton tab = (CloseTabButton) getTabComponentAt(index);
            if (tab != null) {
                tab.setText(title);
            }
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

        return this;
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
                ScilabTabbedPane tabbedPane = (ScilabTabbedPane) dtde.getTransferable().getTransferData(DATAFLAVOR);
                int index = indexAtLocation(dtde.getLocation().x, dtde.getLocation().y);
                if (tabbedPane == this && index == tabbedPane.draggedIndex) {
                    dtde.rejectDrop();
                } else {
                    Component c = tabbedPane.getComponentAt(tabbedPane.draggedIndex);
                    String title = tabbedPane.getScilabTitleAt(tabbedPane.draggedIndex);
                    tabbedPane.remove(c);
                    if (index == -1) {
                        index = getTabCount();
                    }

                    insertTab(title, null, c, null, index);

                    setSelectedIndex(index);
                    dtde.acceptDrop(DnDConstants.ACTION_MOVE);
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
        dsde.getDragSourceContext().setCursor(DragSource.DefaultMoveNoDrop);
    }

    /**
     * Interface DragSourceListener
     * {@inheritDoc}
     */
    public void dragExit(DragSourceEvent dse) {
        dse.getDragSourceContext().setCursor(DragSource.DefaultMoveNoDrop);
    }

    /**
     * Interface DragSourceListener
     * {@inheritDoc}
     */
    public void dragOver(DragSourceDragEvent dsde) {
        Point pt = new Point(dsde.getX(), dsde.getY());
        JRootPane rootpane = (JRootPane) SwingUtilities.getAncestorOfClass(JRootPane.class, currentWhenDragged);

        if (rootpane == null) {
            dsde.getDragSourceContext().setCursor(DragSource.DefaultMoveNoDrop);
        } else {
            SwingUtilities.convertPointFromScreen(pt, rootpane);
            Component c = SwingUtilities.getDeepestComponentAt(rootpane, pt.x, pt.y);

            if (c != null && !(c instanceof JTable) && !(c instanceof ScilabTabbedPane)) {
                c = SwingUtilities.getAncestorOfClass(ScilabTabbedPane.class, c);
            }

            if (c instanceof ScilabTabbedPane) {
                dsde.getDragSourceContext().setCursor(DragSource.DefaultMoveDrop);
            } else {
                dsde.getDragSourceContext().setCursor(DragSource.DefaultMoveNoDrop);
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
            currentWhenDragged = this;

            if (index != -1) {
                draggedIndex = index;
                dge.startDrag(DragSource.DefaultMoveDrop, this, this);
            }
        }
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
                setBorderPainted(false);
                setPreferredSize(new Dimension(BUTTONSIZE, BUTTONSIZE));
                addActionListener(new CommonCallBack("") {
                    public void callBack() {
                        String name = CloseTabButton.this.getText().substring(SwingScilabVariableEditor.PREFIX.length());
                        removeTabAt(indexOfTabComponent(CloseTabButton.this));
                        ScilabVariableEditor.close(name);
                    }
                });
            }
        }
    }
}
