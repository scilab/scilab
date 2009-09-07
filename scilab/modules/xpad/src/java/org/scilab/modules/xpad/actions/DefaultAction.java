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

import javax.swing.JOptionPane;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.SimpleMenuItem;
import org.scilab.modules.xpad.Xpad;

public class DefaultAction extends SwingScilabMenuItem implements MenuItem {
    private Xpad _editor;

    public DefaultAction(Xpad editor) {
	super();
	setText("Default...");
	_editor = editor;
	setCallback(new CallBack("Default...") {

	    public void callBack() {
		doAction();
	    }

	    public void actionPerformed(ActionEvent e) {
		callBack();
	    } 
	});
    }

    protected DefaultAction(String label, Xpad editor) {
	super();
	setText(label);
	_editor = editor;
	setCallback(new CallBack("Default...") {
	    public void callBack() {
		doAction();
	    }

	    public void actionPerformed(ActionEvent e) {
		callBack();
	    } 
	});
    }

    public Xpad getEditor() {
	return _editor;
    }

    public SimpleMenuItem getAsSimpleMenuItem() {
	return (SwingScilabMenuItem) this;
    }

    public void doAction() {
	  JOptionPane.showMessageDialog(getEditor(), "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
    }

    public SimpleMenu getAsSimpleMenu() {
	return null;
    }

}
