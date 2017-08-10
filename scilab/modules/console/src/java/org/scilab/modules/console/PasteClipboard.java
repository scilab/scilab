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
