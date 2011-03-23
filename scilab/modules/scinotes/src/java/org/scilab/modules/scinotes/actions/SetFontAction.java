/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.Font;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * Class to set the font
 * @author DIGITEO
 * @author Calixte DENIZET
 */
public class SetFontAction extends DefaultAction {

    /**
     * Constructor
     * @param name the name of the action
     * @param editor associated with this action
     */
    public SetFontAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * DoAction
     */
    public void doAction() {
        SwingScilabFontChooser fontChooser = new SwingScilabFontChooser((JFrame) getEditor().getParentWindow().getAsSimpleWindow(), ConfigSciNotesManager.getFont(), true);
        fontChooser.setLocationRelativeTo(getEditor());
        fontChooser.displayAndWait();

        Font newFont = fontChooser.getSelectedFont();

        if (newFont != null) {

            List<String> listStylesName = ConfigSciNotesManager.getAllStyleName();

            int numberOfTab = getEditor().getTabPane().getTabCount();
            for (int i = 0; i < numberOfTab; i++) {
                ScilabEditorPane textPane = getEditor().getTextPane(i);
                textPane.resetFont(newFont);
                if (textPane.getOtherPaneInSplit() != null) {
                    textPane.getOtherPaneInSplit().resetFont(newFont);
                }
            }
            getEditor().getTextPane().setFocusable(true);
            ConfigSciNotesManager.saveFont(newFont);
        }
    }

    /**
     * Create the MenuItem
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SetFontAction(label, editor), key);
    }
}
