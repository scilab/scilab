/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - Calixte DENIZET
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

import java.awt.Point;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.io.IOException;

import javax.swing.text.BadLocationException;
import javax.swing.text.Element;

import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * DropFiles Listener
 * @author Allan CORNET
 * @author Calixte DENIZET
 *
 */
public class DropFilesListener implements DropTargetListener {

    private DropTarget dropTarget;
    private ScilabEditorPane pane;
    private int p0;
    private int p1;
    private Point previousPoint = new Point();
    private Point actualPoint = new Point();
    private int actualPos;
    private boolean enter;

    /**
     * Constructor
     * @param pane JEditorPane
     */
    public DropFilesListener(ScilabEditorPane pane) {
        this.pane = pane;
        dropTarget = new DropTarget(pane, DnDConstants.ACTION_COPY_OR_MOVE, this, true, null);
        dropTarget.setActive(true);
    }

    /**
     * dragEnter
     * @param arg0 DropTargetDragEvent
     */
    public void dragEnter(DropTargetDragEvent arg0) {
        if (arg0.isDataFlavorSupported(DataFlavor.stringFlavor)) {
            int sp0 = pane.getSelectionStart();
            int sp1 = pane.getSelectionEnd();
            if (sp1 != sp0) {
                p1 = sp1;
                p0 = sp0;
            }
            enter = true;
        }
    }

    /**
     * dragExit
     * @param arg0 DropTargetEvent
     */
    public void dragExit(DropTargetEvent arg0) {
        if (enter) {
            int y = actualPoint.y - previousPoint.y;
            ScilabDocument doc = (ScilabDocument) pane.getDocument();
            Element root = doc.getDefaultRootElement();
            int line0 = root.getElementIndex(actualPos);
            int line1 = Math.max(0, line0 + Math.min(root.getElementCount() - 1 - line0, (int) Math.signum(y) * y * y));
            int diff = actualPos - root.getElement(line0).getStartOffset();
            Element line = root.getElement(line1);
            pane.setCaretPosition(Math.min(line.getStartOffset() + diff, line.getEndOffset() - 1));
        }
    }

    /**
     * dragOver
     * @param arg0 DropTargetDragEven
     */
    public void dragOver(DropTargetDragEvent arg0) {
        if (arg0.isDataFlavorSupported(DataFlavor.stringFlavor)) {
            previousPoint = actualPoint;
            actualPoint = arg0.getLocation();
            actualPos = pane.viewToModel(actualPoint);
            pane.setCaretPosition(actualPos);
        }
    }

    /**
     * Drop handling
     * @param arg0 the drop event
     * @see java.awt.dnd.DropTargetListener#drop(java.awt.dnd.DropTargetDropEvent)
     */
    @SuppressWarnings("unchecked")
    public void drop(DropTargetDropEvent arg0) {
        arg0.acceptDrop(DnDConstants.ACTION_COPY_OR_MOVE);
        Transferable transferable = arg0.getTransferable();

        try {
            DataFlavor uriListFlavor = new DataFlavor("text/uri-list;class=java.lang.String");
            if (transferable.isDataFlavorSupported(DataFlavor.javaFileListFlavor)) {
                try {
                    java.util.List data = (java.util.List) transferable.getTransferData(DataFlavor.javaFileListFlavor);
                    for (int i = 0; i < data.size(); i++) {
                        SciNotes.getEditor().openFile(data.get(i).toString(), 0, null);
                    }
                    arg0.dropComplete(true);
                } catch (UnsupportedFlavorException e) {
                    arg0.dropComplete(false);
                    return;
                } catch (IOException e) {
                    arg0.dropComplete(false);
                    return;
                }

            } else if (transferable.isDataFlavorSupported(uriListFlavor)) {
                // http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4899516
                String uriData;
                try {
                    uriData = (String) transferable.getTransferData(uriListFlavor);
                } catch (UnsupportedFlavorException e) {
                    arg0.dropComplete(false);
                    return;
                } catch (IOException e) {
                    arg0.dropComplete(false);
                    return;
                }
                java.util.List data = textURIListToFileList(uriData);
                for (int i = 0; i < data.size(); i++) {
                    SciNotes.getEditor().openFile(data.get(i).toString(), 0, null);
                }
                arg0.dropComplete(true);
            } else if (transferable.isDataFlavorSupported(DataFlavor.stringFlavor)) {
                try {
                    String dropString = (String) transferable.getTransferData(DataFlavor.stringFlavor);
                    ScilabDocument doc = (ScilabDocument) pane.getDocument();
                    int pos = pane.viewToModel(arg0.getLocation());
                    doc.mergeEditsBegin();
                    if (arg0.getDropAction() == DnDConstants.ACTION_MOVE) {
                        doc.remove(p0, p1 - p0);
                        if (pos > p1) {
                            pos = pos - (p1 - p0);
                        } else if (pos > p0) {
                            pos = p0;
                        }
                    }
                    doc.insertString(pos, dropString, null);
                    doc.mergeEditsEnd();
                    arg0.dropComplete(true);
                } catch (UnsupportedFlavorException e) {
                    arg0.dropComplete(false);
                    return;
                } catch (IOException e) {
                    arg0.dropComplete(false);
                    return;
                } catch (BadLocationException e) {
                    arg0.dropComplete(false);
                    return;
                }
            }
        } catch (ClassNotFoundException e1) {
            arg0.dropComplete(false);
        }
    }

    /**
     * dropActionChanged
     * @param arg0 DropTargetDragEvent
     */
    public void dropActionChanged(DropTargetDragEvent arg0) {
        // TODO Auto-generated method stub
    }

    /**
     * textURIListToFileList
     * @param data String URI
     * @return java.util.List
     */
    private static java.util.List textURIListToFileList(String data) {
        java.util.List list = new java.util.ArrayList(1);
        for (java.util.StringTokenizer st = new java.util.StringTokenizer(data, "\r\n");
                st.hasMoreTokens();) {
            String s = st.nextToken();
            if (s.startsWith("#")) {
                // the line is a comment (as per the RFC 2483)
                continue;
            }
            try {
                java.net.URI uri = new java.net.URI(s);
                java.io.File file = new java.io.File(uri);
                list.add(file);
            } catch (java.net.URISyntaxException e) {
                // malformed URI
            } catch (IllegalArgumentException e) {
                // the URI is not a valid 'file:' URI
            }
        }
        return list;
    }
}
