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
package org.scilab.modules.scinotes.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

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
     * @param name the name of the action
     * @param editor SciNotes
     */
    public HighlightCurrentLineAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        SciNotes.enableHighlightedLine(this.getState());
        ConfigSciNotesManager.saveHighlightState(this.getState());
    }

    /**
     * createCheckBoxMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        final CheckBoxMenuItem cb = createCheckBoxMenu(label, null, new HighlightCurrentLineAction(label, editor), key);
        cb.setChecked(ConfigSciNotesManager.getHighlightState());
        ((JCheckBoxMenuItem) cb.getAsSimpleCheckBoxMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    cb.setChecked(ConfigSciNotesManager.getHighlightState());
                }
            });

        return cb;
    }
}
