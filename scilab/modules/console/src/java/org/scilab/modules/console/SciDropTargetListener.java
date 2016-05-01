/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.console;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTargetDragEvent;
import java.awt.dnd.DropTargetDropEvent;
import java.awt.dnd.DropTargetEvent;
import java.awt.dnd.DropTargetListener;
import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.util.StringTokenizer;

/**
 * DropTragetListener specific for Scilab console
 * @author Vincent COUVERT
 */
public class SciDropTargetListener implements DropTargetListener {

    private SciConsole associatedConsole;

    /**
     * Constructor
     * @param sciConsole console object associated to this listener
     */
    public SciDropTargetListener(SciConsole sciConsole) {
        super();
        associatedConsole = sciConsole;
    }

    /**
     * Drop handling
     * @param dtde the drop event
     * @see java.awt.dnd.DropTargetListener#drop(java.awt.dnd.DropTargetDropEvent)
     */
    public void drop(DropTargetDropEvent dtde) {

        dtde.acceptDrop(DnDConstants.ACTION_COPY_OR_MOVE);

        Transferable transferable = dtde.getTransferable();

        // New dataflavor created for tests about the type of the object dropped
        DataFlavor uriListFlavor = null;
        try {
            uriListFlavor = new DataFlavor("text/uri-list;class=java.lang.String");
        } catch (ClassNotFoundException e1) {
            e1.printStackTrace();
            dtde.rejectDrop();
            dtde.dropComplete(false);
        }

        try {

            if (transferable.isDataFlavorSupported(DataFlavor.javaFileListFlavor)) {
                List<File> data = (List<File>) transferable.getTransferData(DataFlavor.javaFileListFlavor);
                // Send file names to Scilab

                String[] fileNames = new String[data.size()];
                for (int i = 0; i < data.size(); i++) {
                    File tmpfile = data.get(i);
                    fileNames[i] = tmpfile.toString();
                }
                DropFiles.dropFiles(fileNames);

                // Crappy method to make Scilab parser execute the commands stored by dropFiles
                ((SciOutputView) associatedConsole.getConfiguration().getOutputView())
                .getConsole().sendCommandsToScilab("", false, false);
            } else if (transferable.isDataFlavorSupported(uriListFlavor)) {

                // --- FILE(S) DROP ---

                String data = (String) transferable.getTransferData(uriListFlavor);

                // Get file names as a list
                List<String> listOfFiles = new ArrayList<String>(1);
                for (StringTokenizer st = new StringTokenizer(data, "\r\n"); st.hasMoreTokens();) {
                    String s = st.nextToken();
                    // Have to test length because of last token which length is 1 and has to be ignored
                    if (s.length() > 1) {
                        listOfFiles.add(s);
                    }
                }

                // Send file names to Scilab
                String[] fileNames = new String[listOfFiles.size()];
                for (int i = 0; i < listOfFiles.size(); i++) {
                    fileNames[i] = listOfFiles.get(i);
                }
                DropFiles.dropFiles(fileNames);

                // Crappy method to make Scilab parser execute the commands stored by dropFiles
                ((SciInputCommandView) associatedConsole.getConfiguration().getInputCommandView()).setCmdBuffer("", false);

            } else if (transferable.isDataFlavorSupported(DataFlavor.stringFlavor)) {

                // --- TEXT DROP ---

                String dropContents = (String) transferable.getTransferData(DataFlavor.stringFlavor);

                // Send text to the console and remove prompts in text if there are
                //associatedConsole.sendCommandsToScilab(dropContents, true);
                ((SciInputCommandView) associatedConsole.getConfiguration().getInputCommandView()).append(dropContents);
            }

            // If we made it this far, everything worked.
            dtde.dropComplete(true);

        } catch (UnsupportedFlavorException e) {
            e.printStackTrace();
            dtde.rejectDrop();
            dtde.dropComplete(false);
        } catch (IOException e) {
            e.printStackTrace();
            dtde.rejectDrop();
            dtde.dropComplete(false);
        }
    }

    /**
     * Drag beginning handling
     * @param dtde the drop event
     * @see java.awt.dnd.DropTargetListener#dragEnter(java.awt.dnd.DropTargetDragEvent)
     */
    public void dragEnter(DropTargetDragEvent dtde) {
        // Nothing to do in Scilab Console
    }

    /**
     * Drag end handling
     * @param dtde the drop event
     * @see java.awt.dnd.DropTargetListener#dragExit(java.awt.dnd.DropTargetEvent)
     */
    public void dragExit(DropTargetEvent dtde) {
        // Nothing to do in Scilab Console
    }

    /**
     * Drag over this object handling
     * @param dtde the drop event
     * @see java.awt.dnd.DropTargetListener#dragOver(java.awt.dnd.DropTargetDragEvent)
     */
    public void dragOver(DropTargetDragEvent dtde) {
        // Nothing to do in Scilab Console
    }

    /**
     * Drag action change handling
     * @param dtde the drop event
     * @see java.awt.dnd.DropTargetListener#dropActionChanged(java.awt.dnd.DropTargetDragEvent)
     */
    public void dropActionChanged(DropTargetDragEvent dtde) {
        // Nothing to do in Scilab Console
    }
};
