/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xpad.actions;

import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;
import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * HighlightCurrentLineAction Class
 * @author Sylvestre KOUMAR
 *
 */
public final class HighlightCurrentLineAction extends DefaultCheckAction {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1489762718469013039L;  
    
    /**
     * Constructor
     * @param editor Xpad
     */
    private HighlightCurrentLineAction(Xpad editor) {
	super(XpadMessages.HIGHLIGHT_CURRENT_LINE, editor);
	setState(false);
    }
    
    /**
     * doAction
     */
    public void doAction() {
	getEditor().enableHighlightedLine(this.getState());
    }
    
    /**
     * createCheckBoxMenu
     * @param editor Xpad
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor, KeyStroke key) {
	CheckBoxMenuItem cb = createCheckBoxMenu(XpadMessages.HIGHLIGHT_CURRENT_LINE, null, new HighlightCurrentLineAction(editor), key);
	cb.setChecked(ConfigXpadManager.getHighlightState());
	return cb;
    }

    /**
     * Put input map
     * @param textPane JTextpane
     * @param editor Editor
     * @param key KeyStroke
     */
    public static void putInInputMap(JComponent textPane, Xpad editor, KeyStroke key) {
	textPane.getInputMap().put(key, new HighlightCurrentLineAction(editor));
    }
}
