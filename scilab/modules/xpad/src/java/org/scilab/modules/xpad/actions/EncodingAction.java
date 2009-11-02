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
import javax.swing.undo.UndoManager;

import org.scilab.modules.gui.messagebox.MessageBox;
import org.scilab.modules.gui.messagebox.ScilabMessageBox;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.XpadMessages;

public class EncodingAction extends DefaultCheckAction {
	
	private String encoding;

	public EncodingAction(String encodingName, Xpad editor) {
		super(encodingName, editor);
		encoding = encodingName;
	}

	public JRadioButtonMenuItem createRadioButtonMenuItem(Xpad editor) {
		JRadioButtonMenuItem radio = new JRadioButtonMenuItem(encoding);
		radio.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				doAction();
			}
		});
		return radio;
	}
	
	public static ArrayList<String> getEcodings() {
		SortedMap<String,Charset> charsetList = Charset.availableCharsets();
		Set cles = charsetList.keySet();
		Iterator iterator = cles.iterator();
		ArrayList<String> completEncodingList = new ArrayList<String>();
		ArrayList<String> encodingList = new ArrayList<String>();
		while (iterator.hasNext()) {
			completEncodingList.add(charsetList.get(iterator.next()).toString());
		}

		for (int i = 0; i < completEncodingList.size(); i++) {
			if (completEncodingList.get(i).toLowerCase().startsWith("ibm") ||
					completEncodingList.get(i).toLowerCase().startsWith("x-")) {
				continue;
			} else {
				encodingList.add(completEncodingList.get(i));
			}
		}

		return encodingList;
	}
	
	public void doAction() {
		boolean isSuccess = false;
		
		((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).setEncoding(encoding);
		
		// If file associated then reload
		ScilabStyleDocument styleDocument = (ScilabStyleDocument) getEditor().getTextPane().getStyledDocument();
		EditorKit editorKit = getEditor().getEditorKit();
		String fileName = getEditor().getTextPane().getName();
		
		try {
			if (fileName != null) {
				File file = new File(getEditor().getTextPane().getName());
				if (file.exists()) {
					if (styleDocument.getLength() > 0) {
						styleDocument.remove(0, styleDocument.getLength());
						editorKit.read(new BufferedReader(new InputStreamReader(new FileInputStream(file),encoding)), styleDocument, 0);
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

		getEditor().getTextPane().repaint();
		
		UndoManager undo = ((ScilabStyleDocument) getEditor().getTextPane().getStyledDocument()).getUndoManager();
		undo.discardAllEdits();
		
		if (!isSuccess) {
			MessageBox messageBox = ScilabMessageBox.createMessageBox();
			messageBox.setTitle(XpadMessages.XPAD_ERROR);
			messageBox.setMessage(XpadMessages.COULD_NOT_CONVERT_FILE);
			messageBox.setModal(true);
			messageBox.setIcon("error");
			messageBox.displayAndWait();
		}
	}
}
