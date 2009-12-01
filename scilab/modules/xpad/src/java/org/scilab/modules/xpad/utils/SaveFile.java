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
 *
 */
package org.scilab.modules.xpad.utils;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;

import javax.swing.JTextPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.EditorKit;

import org.scilab.modules.xpad.style.ScilabStyleDocument;

/**
 * Save File utility class
 * @author Allan CORNET
 *
 */
public class SaveFile {

	/**
	 * save text in JTextPane
	 * @param textPane JTextPane
	 * @param fOut File
	 * @param editorKit EditorKit
	 * @return true if saved
	 */
	
	public static boolean doSave(JTextPane textPane, File fOut, EditorKit editorKit) {
		
		ScilabStyleDocument styledDocument = (ScilabStyleDocument) textPane.getStyledDocument();

		BufferedWriter out = null;
		try {
			out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(fOut), styledDocument.getEncoding()));
			try {
				editorKit.write(out, styledDocument, 0, styledDocument.getLength());
				out.flush();
				out.close();
			} catch (IOException e) {
				return false;
			} catch (BadLocationException e) {
				return false;
			}
		} catch (UnsupportedEncodingException e) {
			return false;
		} catch (FileNotFoundException e) {
			return false;
		}
		return true;
	}
}
