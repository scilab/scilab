/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * AutoIndentAction Class
 * @author Calixte DENIZET
 *
 */
public final class HorizontalWrapAction extends DefaultCheckAction  {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public HorizontalWrapAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    SciNotes.setHorizontalWrap(HorizontalWrapAction.this.getState());
                    ConfigSciNotesManager.saveHorizontalWrap(HorizontalWrapAction.this.getState());
                }
            });
    }

    /**
     * createCheckBoxMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        final CheckBoxMenuItem horizontalWrap = createCheckBoxMenu(label, null, new HorizontalWrapAction(label, editor), key);
        horizontalWrap.setChecked(ConfigSciNotesManager.getHorizontalWrap());
        ((JCheckBoxMenuItem) horizontalWrap.getAsSimpleCheckBoxMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    horizontalWrap.setChecked(ConfigSciNotesManager.getHorizontalWrap());
                }
            });

        return horizontalWrap;
    }
}
