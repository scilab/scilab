/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Set;
import java.util.SortedMap;

import javax.swing.JRadioButtonMenuItem;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ColorizationManager;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;


/**
 * EncodingAction Class
 * @author Bruno JOFRET
 *
 */
public class EncodingAction extends DefaultCheckAction {

    /**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -5421313717126859924L;
	private String encoding;

    /**
     * Constructor
     * @param encodingName Encoding Name
     * @param editor Xpad
     */
    public EncodingAction(String encodingName, Xpad editor) {
	super(encodingName, editor);
	encoding = encodingName;
    }

    /**
     * createRadioButtonMenuItem
     * @param editor Xpad 
     * @return JRadioButtonMenuItem
     */
    public JRadioButtonMenuItem createRadioButtonMenuItem(Xpad editor) {
    	JRadioButtonMenuItem radio = new JRadioButtonMenuItem(encoding);
    	radio.addActionListener(new ActionListener() {
    		public void actionPerformed(ActionEvent arg0) {
    			doAction();
    		}
    	});
    	
    	return radio;
    }

    /**
     * getEcodings
     * @return ArrayList String 
     */
    public static ArrayList<String> getEcodings() {
    	
	SortedMap<String, Charset> charsetList = Charset.availableCharsets();
	Set cles = charsetList.keySet();
	Iterator iterator = cles.iterator();
	ArrayList<String> completEncodingList = new ArrayList<String>();
	ArrayList<String> encodingList = new ArrayList<String>();
	while (iterator.hasNext()) {
	    completEncodingList.add(charsetList.get(iterator.next()).toString());
	}

	for (int i = 0; i < completEncodingList.size(); i++) {
	    if (completEncodingList.get(i).toLowerCase().startsWith("ibm") 
	    	|| completEncodingList.get(i).toLowerCase().startsWith("x-")) {
		continue;
	    } else {
		encodingList.add(completEncodingList.get(i));
	    }
	}

	return encodingList;
    }

    /**
     * doAction
     */
    public void doAction() {
    	boolean isSuccess = false;

    	ScilabStyleDocument styleDocument = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument());

    	if (styleDocument.isContentModified()) {
    		/* File modified */
    		if (getEditor().getTextPane().getName() != null) {
	    	/* Not untitled */
	    	switch (ScilabModalDialog.show(Xpad.getEditor(), 
	    			XpadMessages.MODIFICATIONS_WILL_BE_LOST, XpadMessages.CONTINUE,
	    			IconType.QUESTION_ICON, ButtonType.YES_NO)) {
	    			case YES_OPTION : //Yes, continue
	    				break;
	    			case NO_OPTION ://No, exit
	    				//Back to previous menu checked
	    				getEditor().getXpadGUI().updateEncodingMenu(styleDocument);
	    				return;
	    			default:
	    				break;
	    	}
	    }		
	}

	// Avoid modifications to be saved
	styleDocument.setUpdater(false);
	boolean indentMode = styleDocument.getAutoIndent();
	styleDocument.setAutoIndent(false); 

	styleDocument.setEncoding(encoding);
	ConfigXpadManager.saveDefaultEncoding(encoding);

	// If file associated then reload
	EditorKit editorKit = getEditor().getEditorKit();
	String fileName = getEditor().getTextPane().getName();

	try {
	    if (fileName != null) {
	    	File file = new File(getEditor().getTextPane().getName());
	    	if (file.exists()) {
	    		if (styleDocument.getLength() > 0) {
	    			styleDocument.getUndoManager().discardAllEdits();
	    			styleDocument.disableUndoManager();
	    			styleDocument.remove(0, styleDocument.getLength());
	    			editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(file), encoding)), styleDocument, 0);
	    			styleDocument.enableUndoManager();
	    		}
	    	}
	    }
	    isSuccess = true;
	} catch (UnsupportedEncodingException e) {
	    isSuccess = false;
	} catch (FileNotFoundException e) {
	    isSuccess = false;
	} catch (IOException e) {
	    isSuccess = false;
	} catch (BadLocationException e) {
	    isSuccess = false;
	}

	/* Allow changes to be saved */
	new ColorizationManager().colorize(styleDocument, 0, styleDocument.getLength());
	styleDocument.setAutoIndent(indentMode);
	styleDocument.setUpdater(true);

	styleDocument.setContentModified(false);
	// changing such a property dicards undo
	styleDocument.getUndoManager().discardAllEdits();
	if (!isSuccess) {
	    ScilabModalDialog.show(Xpad.getEditor(), XpadMessages.COULD_NOT_CONVERT_FILE,
		    XpadMessages.XPAD_ERROR, IconType.ERROR_ICON);
	}
  }
}
