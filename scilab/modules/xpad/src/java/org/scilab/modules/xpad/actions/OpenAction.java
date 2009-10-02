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

import javax.swing.JFileChooser;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.filechooser.Juigetfile;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.style.ScilabStyleDocument;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class OpenAction extends DefaultAction {

    private OpenAction(Xpad editor) {
	super("Open...", editor);
    }

    public void doAction() {
    	
    	String initialDirectoryPath = getEditor().getTextPane().getName();
    	if (initialDirectoryPath == null ){
    		initialDirectoryPath =  ConfigManager.getLastOpenedDirectory() ;
    	}
    	
		String[] mask = new String[]{ "*.sce",  "*.sc*", "*.cos*", "*.sci",}; 
		
		SwingScilabFileChooser _fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
		 
		 _fileChooser .setAcceptAllFileFilterUsed(true);
		 _fileChooser .addMask(mask , new String[0]);
		 _fileChooser .setInitialDirectory( initialDirectoryPath );
		 
		 //_fileChooser .setInitialDirectory( System.getProperty("user.dir"));		
		 _fileChooser .setUiDialogType(Juigetfile.SAVE_DIALOG);	 	
    
		int retval = _fileChooser.showOpenDialog(getEditor());
		if (retval == JFileChooser.APPROVE_OPTION) {
		    File f = _fileChooser.getSelectedFile();
		    System.out.println(f.getParent());

			synchronized (getEditor()) {
				ConfigManager.saveLastOpenedDirectory(f.getParent());
				getEditor().setTitle(f.getPath() + " - Xpad");
			    ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
			    getEditor().updateRecentOpenedFilesMenu();
				
			}
		    
		    getEditor().readFile(f);
		    



	    
	    
	}
    }
    
    public static MenuItem createMenu(Xpad editor) {
	return createMenu("Open...", null, new OpenAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
    }
    
    public static PushButton createButton(Xpad editor) {
	return createButton("Open...", "document-open.png", new OpenAction(editor));
    }
}
