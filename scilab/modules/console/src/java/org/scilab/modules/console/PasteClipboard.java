/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.io.IOException;

import com.artenum.rosetta.core.action.AbstractConsoleAction;

/**
 * Paste the contents of the clipboard in Scilab Console
 * Each line is executed by Scilab before a new one can be pasted
 * This event is configured in configuration.xml file
 * @author Vincent COUVERT
 */
public class PasteClipboard extends AbstractConsoleAction {

    private static final long serialVersionUID = 1L;

    /**
     * Constructor
     */
    public PasteClipboard() {
        super();
    }

    /**
     * Threats the event
     * @param e the action event that occurred
     * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
     */
    public void actionPerformed(ActionEvent e) {
        // Gets the contents of the clipboard
        Toolkit toolkit = Toolkit.getDefaultToolkit();
        Clipboard systemClipboard = toolkit.getSystemClipboard();

        // Verify that clibpboard data is of text type
        boolean dataAvailable;
        try {
            dataAvailable = systemClipboard.isDataFlavorAvailable(DataFlavor.stringFlavor);
        } catch (IllegalStateException exception) {
            return;
        }

        // Exit if text data not available
        if (!dataAvailable) {
            return;
        }

        // Read data
        String clipboardContents = null;
        try {
            clipboardContents = (String) systemClipboard.getData(DataFlavor.stringFlavor);
        } catch (UnsupportedFlavorException e1) {
            // Should never be here
            e1.printStackTrace();
        } catch (IOException e1) {
            // Should never be here
            e1.printStackTrace();
        }

        // Send data to Scilab Console
        ((SciOutputView) configuration.getOutputView()).getConsole().sendCommandsToScilab(clipboardContents, true, true);
    }
}
