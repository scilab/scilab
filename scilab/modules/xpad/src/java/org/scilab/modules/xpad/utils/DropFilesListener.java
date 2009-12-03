/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.utils;

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

import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.StyledDocument;

import org.scilab.modules.xpad.Xpad;


/**
 * DropFiles Listener
 * @author Allan CORNET
 *
 */
public class DropFilesListener implements DropTargetListener {

	private DropTarget dropTarget;
	private JTextPane pane;

	/**
	 * Constructor 
	 * @param pane JTextPane
	 */
	public DropFilesListener(JTextPane pane) {
		this.pane = pane;
		dropTarget = new DropTarget(pane, DnDConstants.ACTION_COPY_OR_MOVE, this, true, null);
	}

	/**
	 * dragEnter 
	 * @param arg0 DropTargetDragEvent
	 */
	public void dragEnter(DropTargetDragEvent arg0) {
		// TODO Auto-generated method stub
	}

	/**
	 * dragExit
	 * @param arg0 DropTargetEvent
	 */
	public void dragExit(DropTargetEvent arg0) {
		// TODO Auto-generated method stub
	}

	/**
	 * dragOver
	 * @param arg0 DropTargetDragEven
	 */
	public void dragOver(DropTargetDragEvent arg0) {
		// TODO Auto-generated method stub
	}

	/**
	 * Drop handling
	 * @param arg0 the drop event
	 * @see java.awt.dnd.DropTargetListener#drop(java.awt.dnd.DropTargetDropEvent)
	 */	
	@SuppressWarnings("unchecked")
	public void drop(DropTargetDropEvent arg0) {
		// TODO Auto-generated method stub
		arg0.acceptDrop(DnDConstants.ACTION_COPY_OR_MOVE);
		Transferable transferable = arg0.getTransferable();
		
		try {
			DataFlavor uriListFlavor = new DataFlavor("text/uri-list;class=java.lang.String");
			if (transferable.isDataFlavorSupported(DataFlavor.javaFileListFlavor)) {
				try {
					java.util.List data = (java.util.List) transferable.getTransferData(DataFlavor.javaFileListFlavor);
					for (int i = 0; i < data.size(); i++) {
						Xpad.xpad(data.get(i).toString());
					} 
				} catch (UnsupportedFlavorException e) {
					// TODO Auto-generated catch block
					arg0.dropComplete(false);
					return;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					arg0.dropComplete(false);
					return;
				}
				
			} else if (transferable.isDataFlavorSupported(uriListFlavor)) {
				// http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4899516
				String uriData;
				try {
					uriData = (String) transferable.getTransferData(uriListFlavor);
				} catch (UnsupportedFlavorException e) {
					// TODO Auto-generated catch block
					arg0.dropComplete(false);
					return;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					arg0.dropComplete(false);
					return;
				}
				java.util.List data = textURIListToFileList(uriData);
				for (int i = 0; i < data.size(); i++) {
					Xpad.xpad(data.get(i).toString());
				}
			} else if (transferable.isDataFlavorSupported(DataFlavor.stringFlavor)) {
				try {
					String dropString = (String) transferable.getTransferData(DataFlavor.stringFlavor);
					StyledDocument doc = pane.getStyledDocument();
					doc.insertString(doc.getLength(), dropString, doc.getStyle(dropString));
					
				} catch (UnsupportedFlavorException e) {
					// TODO Auto-generated catch block
					arg0.dropComplete(false);
					return;
				} catch (IOException e) {
					// TODO Auto-generated catch block
					arg0.dropComplete(false);
					return;
				} catch (BadLocationException e) {
					arg0.dropComplete(false);
					return;
				}
			}
		
		} catch (ClassNotFoundException e1) {
			// TODO Auto-generated catch block
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
