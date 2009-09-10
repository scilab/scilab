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

import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

public class OpenAction extends DefaultAction {

    public OpenAction(Xpad editor) {
	super("Open...", editor);
	//setMnemonic('O');
	setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, ActionEvent.CTRL_MASK));
    }

    public void doAction() {
	JFileChooser _fileChooser = new JFileChooser();
	int retval = _fileChooser.showOpenDialog(getEditor());
	if (retval == JFileChooser.APPROVE_OPTION) {
	    File f = _fileChooser.getSelectedFile();
	    getEditor().readFile(f);

	    ConfigXpadManager.saveToRecentOpenedFiles(f.getPath());
	    
	}
    }

}
