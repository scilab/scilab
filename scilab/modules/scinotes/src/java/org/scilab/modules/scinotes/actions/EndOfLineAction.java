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

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JRadioButtonMenuItem;

import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class  EndOfLineAction
 * @author Allan CORNET
 *
 */
public class EndOfLineAction  extends DefaultCheckAction {
	
	private static final String EOL_LINUX = "\n";
	private static final String EOL_MACOS = "\r";
	private static final String EOL_WINDOWS = "\r\n";
	private static final String LINE_SEPARATOR = "line.separator";

	/**
	 * generated serialVersionUID
	 */
	private static final long serialVersionUID = 7147038540238271944L;

	/* default */
	private String eolMenuLabel = SciNotesMessages.EOL_AUT0;

	/**
	 * Constructor
	 * @param eolName String
	 * @param editor SciNotes
	 */
	public EndOfLineAction(String eolName, SciNotes editor) {
		super(eolName, editor);
		eolMenuLabel = eolName;
    }
	
	/**
	 * createRadioButtonMenuItem
	 * @param editor SciNotes
	 * @return JRadioButtonMenuItem
	 */
	public JRadioButtonMenuItem createRadioButtonMenuItem(SciNotes editor) {
		JRadioButtonMenuItem radio = new JRadioButtonMenuItem(eolMenuLabel);
		radio.addActionListener(new ActionListener() {
		    public void actionPerformed(ActionEvent arg0) {
			doAction();
		    }
		});
		return radio;
	    }

	/**
	 * doAction
	 */
    public void doAction() {
    	ScilabDocument styleDocument = ((ScilabDocument) getEditor().getTextPane().getDocument());

    	// default
    	String newOel = System.getProperty(LINE_SEPARATOR); 
    	
    	if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_AUT0) == 0) {
    		newOel = System.getProperty(LINE_SEPARATOR); 
    	} else if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_LINUX) == 0) {
    		newOel = EOL_LINUX;     
    	} else if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_MACOS) == 0) {
    		newOel = EOL_MACOS; 
    	} else if (eolMenuLabel.compareToIgnoreCase(SciNotesMessages.EOL_WINDOWS) == 0) {
    		newOel = EOL_WINDOWS;
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
