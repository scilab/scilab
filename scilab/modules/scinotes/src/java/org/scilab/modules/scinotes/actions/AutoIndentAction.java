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

package org.scilab.modules.scinotes.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * AutoIndentAction Class
 * @author Bruno JOFRET
 *
 */
public final class AutoIndentAction extends DefaultCheckAction  {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1937347660350539353L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public AutoIndentAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        SciNotes.setAutoIndent(this.getState());
        ConfigSciNotesManager.saveAutoIndent(this.getState());
    }

    /**
     * createCheckBoxMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        final CheckBoxMenuItem autoIndent = createCheckBoxMenu(label, null, new AutoIndentAction(label, editor), key);
        ((JCheckBoxMenuItem) autoIndent.getAsSimpleCheckBoxMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    autoIndent.setChecked(ConfigSciNotesManager.getAutoIndent());
                }
            });

        return autoIndent;
    }
}
