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

package org.scilab.modules.xpad.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JRadioButtonMenuItem;

import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class EndOfLineAction  extends DefaultCheckAction {
	
	private static final String eolLinux = "\n";
	private static final String eolMacOs = "\r";
	private static final String eolWindows = "\r\n";

	/**
	 * generated serialVersionUID
	 */
	private static final long serialVersionUID = 7147038540238271944L;

	/* default */
	private String eolMenuLabel = XpadMessages.EOL_AUT0;

	public EndOfLineAction(String eolName, Xpad editor) {
		super(eolName, editor);
		eolMenuLabel = eolName;
    }
	
	public JRadioButtonMenuItem createRadioButtonMenuItem(Xpad editor) {
		JRadioButtonMenuItem radio = new JRadioButtonMenuItem(eolMenuLabel);
		radio.addActionListener(new ActionListener() {
		    public void actionPerformed(ActionEvent arg0) {
			doAction();
		    }
		});
		return radio;
	    }

    public void doAction() {
    	
    	ScilabStyleDocument styleDocument = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument());

    	// default
    	String newOel = System.getProperty("line.separator"); 
    	
    	if (eolMenuLabel.compareToIgnoreCase(XpadMessages.EOL_AUT0) == 0) {
    		newOel = System.getProperty("line.separator"); 
    	} else if (eolMenuLabel.compareToIgnoreCase(XpadMessages.EOL_LINUX) == 0) {
    		newOel = eolLinux;     
    	} else if (eolMenuLabel.compareToIgnoreCase(XpadMessages.EOL_MACOS) == 0) {
    		newOel = eolMacOs; 
    	} else if (eolMenuLabel.compareToIgnoreCase(XpadMessages.EOL_WINDOWS) == 0) {
    		newOel = eolWindows;
    	}
    	
    	if (styleDocument.getEOL().compareTo(newOel) != 0) {
    		styleDocument.setEOL(newOel);
        	styleDocument.setContentModified(true);
        	// changing such a property dicards undo
        	styleDocument.getUndoManager().discardAllEdits();
        	getEditor().updateTabTitle();	
    	}
    	
    }
}
