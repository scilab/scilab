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
import java.awt.event.KeyEvent;
import java.io.File;
import java.io.FileWriter;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.StyleConstants;

import org.scilab.modules.xpad.Xpad;

public class SaveAction extends DefaultAction {

	public SaveAction(Xpad editor) {
		super("Save", editor);
		//setMnemonic('S');
		setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
	}

	public void doAction() {
		JFileChooser _fileChooser = new JFileChooser();
		int retval = _fileChooser.showSaveDialog(getEditor());
		if (retval == JFileChooser.APPROVE_OPTION) {
			File f = _fileChooser.getSelectedFile();
			try {

				String doc = getIndentedText(getEditor().getTextPane());

				FileWriter writer = new FileWriter(f);
				writer.write(doc);
				writer.flush();
				writer.close();

			} catch (Exception ioex) {
			    JOptionPane.showMessageDialog(getEditor(), ioex);
			}
		}
	}

	public String getIndentedText(JTextPane ed) {
		
		int startOffset;
		int endOffset;
		double indent;
		String textLine = "";
		
		Document doc = ed.getStyledDocument();
		String text = "";
		String space = "";

		for (int i = 0; i < doc.getLength(); ) {

			startOffset = ed.getStyledDocument().getParagraphElement(i).getStartOffset();
			endOffset = ed.getStyledDocument().getParagraphElement(i).getEndOffset();
			String ind = ed.getStyledDocument().getParagraphElement(i).getAttributes().getAttribute(StyleConstants.LeftIndent).toString();
			indent = Double.parseDouble(ind);

			try {
				textLine = ed.getStyledDocument().getText(startOffset, endOffset - startOffset);
			} catch (BadLocationException e) {
				e.printStackTrace();
			}

			for (int j = 0; j < indent; j=j+10) {
				if (indent == 0) {
					space = "";
				} else {
					space += " ";
				}
			}

			text += space + textLine;

			space = "";
			i = endOffset;
		}	
		return text;		
	}

}