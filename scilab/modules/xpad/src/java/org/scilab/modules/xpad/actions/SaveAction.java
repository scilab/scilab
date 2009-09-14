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
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class SaveAction extends DefaultAction {

	private SaveAction(Xpad editor) {
		super("Save", editor);
	
	}

	 public static MenuItem createMenu(Xpad editor) {
		return createMenu("Save", null, new SaveAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_S, ActionEvent.CTRL_MASK));
	 }
	 
	 public static PushButton createButton(Xpad editor) {
	     return createButton("Save", "media-floppy.png", new SaveAction(editor));
	 }

@Override
	public void doAction() {
		// TODO Auto-generated method stub
		//super.doAction();
		if ( getEditor().getTextPane().getName() != null ){
			try {
				File newSavedFiled = new File(getEditor().getTextPane().getName());
				
				String doc = getEditor().getTextPane().getText();

				FileWriter writer = new FileWriter(newSavedFiled);
				writer.write(doc);
				writer.flush();
				writer.close();
				

			} catch (Exception ioex) {
			    JOptionPane.showMessageDialog(getEditor(), ioex);
			}
		}else{
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
					
					
					ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
					getEditor().getTextPane().setName(f.getPath());
					getEditor().getTabPane().setTitleAt( getEditor().getTabPane().getSelectedIndex() , f.getName());
					getEditor().updateRecentOpenedFilesMenu();
				} catch (Exception ioex) {
				    JOptionPane.showMessageDialog(getEditor(), ioex);
				}
			}
			
		}
		
		
	}
}
