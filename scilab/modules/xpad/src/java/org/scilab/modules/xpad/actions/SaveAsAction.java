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

import java.io.File;
import java.io.FileWriter;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import org.scilab.modules.xpad.Xpad;

public class SaveAsAction extends DefaultAction {

	public SaveAsAction(Xpad editor) {
		super("Save As...", editor);
	}

	public void doAction() {
		JFileChooser _fileChooser = new JFileChooser();
		int retval = _fileChooser.showSaveDialog(getEditor());
		if (retval == JFileChooser.APPROVE_OPTION) {
			File f = _fileChooser.getSelectedFile();
			try {

				String doc = getEditor().getTextPane().getText();

				FileWriter writer = new FileWriter(f);
				writer.write(doc);
				writer.flush();
				writer.close();

			} catch (Exception ioex) {
			    JOptionPane.showMessageDialog(getEditor(), ioex);
			}
		}
	}
}
