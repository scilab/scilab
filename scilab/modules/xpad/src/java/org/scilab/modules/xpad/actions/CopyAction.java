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

import java.awt.Toolkit;
import java.awt.event.KeyEvent;

import javax.swing.KeyStroke;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * CopyAction Class
 * @author Bruno JOFRET
 *
 */
public final class CopyAction extends DefaultAction {

    /**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -8858655782952121924L;

	/**
	 * Constructor
	 * @param editor Xpad
	 */
	private CopyAction(Xpad editor) {
	super(XpadMessages.COPY, editor);
    }
    
	/**
	 * doAction
	 */
    public void doAction() {
	getEditor().getTextPane().getActionMap().get(DefaultEditorKit.copyAction).actionPerformed(null);
    }
    
    /**
     * createMenu
     * @param editor Xpad
     * @return createMenu
     */
    public static MenuItem createMenu(Xpad editor) {
	return createMenu(XpadMessages.COPY, null, new CopyAction(editor), 
			KeyStroke.getKeyStroke(KeyEvent.VK_C, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
    }
    
    /**
     * createButton
     * @param editor Xpad
     * @return PushButton
     */
    public static PushButton createButton(Xpad editor) {
	return createButton(XpadMessages.COPY, "edit-copy.png", new CopyAction(editor));
    }
}
