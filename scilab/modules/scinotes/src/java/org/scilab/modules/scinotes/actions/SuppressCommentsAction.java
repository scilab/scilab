/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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
 * SuppressCommentsAction Class
 * @author Calixte DENIZET
 *
 */
public final class SuppressCommentsAction extends DefaultCheckAction  {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -1L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SuppressCommentsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    SciNotes.setSuppressComments(SuppressCommentsAction.this.getState());
                    ConfigSciNotesManager.saveSuppressComments(SuppressCommentsAction.this.getState());
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
        final CheckBoxMenuItem sca = createCheckBoxMenu(label, null, new SuppressCommentsAction(label, editor), key);
        sca.setChecked(ConfigSciNotesManager.getSuppressComments());
        ((JCheckBoxMenuItem) sca.getAsSimpleCheckBoxMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
                public void propertyChange(PropertyChangeEvent e) {
                    sca.setChecked(ConfigSciNotesManager.getSuppressComments());
                }
            });

        return sca;
    }
}
