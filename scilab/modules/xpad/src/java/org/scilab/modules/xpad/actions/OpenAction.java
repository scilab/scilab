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
import java.awt.Toolkit;
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
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * File opening management
 * @author Bruno JOFRET
 */
public final class OpenAction extends DefaultAction {

	private static final long serialVersionUID = -8765712033802048782L;

	/**
	 * Constructor
	 * @param editor associated Xpad instance
	 */
	private OpenAction(Xpad editor) {
		super(XpadMessages.OPEN, editor);
	}

	/**
	 * Open file action
	 * @see org.scilab.modules.xpad.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		
		String initialDirectoryPath = getEditor().getTextPane().getName();
		if (initialDirectoryPath == null ){
			initialDirectoryPath =  ConfigManager.getLastOpenedDirectory() ;
		}

		String[] mask = new String[]{"*.cos*", "*.sci", "*.sce", "*.sc*"}; 

		SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
		fileChooser.setInitialDirectory(initialDirectoryPath);
		fileChooser .setAcceptAllFileFilterUsed(true);
		fileChooser .addMask(mask, null);		
		fileChooser .setUiDialogType(Juigetfile.SAVE_DIALOG);	 	

		int retval = fileChooser.showOpenDialog(getEditor());
		if (retval == JFileChooser.APPROVE_OPTION) {
			File f = fileChooser.getSelectedFile();
			ConfigManager.saveLastOpenedDirectory(f.getPath());
			ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());

			getEditor().setTitle(f.getPath() + " - " + XpadMessages.SCILAB_EDITOR);
			getEditor().updateRecentOpenedFilesMenu();
			getEditor().readFile(f);


		}
	}

	/**
	 * Create a menu to add to Xpad menu bar
	 * @param editor associated Xpad instance
	 * @return the menu
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.OPEN, null, new OpenAction(editor), KeyStroke.getKeyStroke(KeyEvent.VK_O, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Create a button to add to Xpad tool bar
	 * @param editor associated Xpad instance
	 * @return the button
	 */
	public static PushButton createButton(Xpad editor) {
		return createButton(XpadMessages.OPEN, "document-open.png", new OpenAction(editor));
	}
}
