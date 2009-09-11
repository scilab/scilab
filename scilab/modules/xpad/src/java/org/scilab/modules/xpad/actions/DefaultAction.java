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

import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;
import org.scilab.modules.xpad.Xpad;

public class DefaultAction extends CallBack {
    private Xpad _editor;

    private DefaultAction(Xpad editor) {
	super("Default...");
	_editor = editor;
    }

    protected DefaultAction(String label, Xpad editor) {
	super(label);
	_editor = editor;
    }

    public Xpad getEditor() {
	return _editor;
    }

    protected static PushButton createButton(String title, String icon, ActionListener listener) {
	PushButton button = ScilabPushButton.createPushButton(); 
	((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(listener);
	 button.setToolTipText(title);
	 if (icon == null) {
	    button.setText(title);
	}
	else {
	    ((SwingScilabPushButton) button.getAsSimplePushButton()).setIcon(new ImageIcon(System.getenv("SCI")+"/modules/gui/images/icons/"+icon));
	}
	
	return button;
    }

    protected static MenuItem createMenu(String title, String icon, DefaultAction listener, KeyStroke keyStroke) {
	MenuItem menu = ScilabMenuItem.createMenuItem();
	menu.setCallback(listener);
	menu.setText(title);
	
	if (keyStroke != null) {
	    ((SwingScilabMenuItem) menu.getAsSimpleMenuItem()).setAccelerator(keyStroke);
	}
	
	return menu;
    }

    public void doAction() {
	  JOptionPane.showMessageDialog(getEditor(), "Not Implemented Now !!!", null, JOptionPane.ERROR_MESSAGE);
    }

    public void callBack() {
	doAction();
    }

}
