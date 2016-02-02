/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.scinotes.actions;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.HelpOnTypingManager;

/**
 * ActivateHelpOnTypingForKeywordsAction Class
 * @author Calixte DENIZET
 *
 */
public final class ActivateHelpOnTypingForKeywordsAction extends DefaultCheckAction {

    private static final long serialVersionUID = 3793710651553964430L;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public ActivateHelpOnTypingForKeywordsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        HelpOnTypingManager.getInstance().enableKeywords(getState());
        SciNotes.activateHelpOnTyping();
    }

    /**
     * createCheckBoxMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return CheckBoxMenuItem
     */
    public static CheckBoxMenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        final CheckBoxMenuItem cb = createCheckBoxMenu(label, null, new ActivateHelpOnTypingForKeywordsAction(label, editor), key);
        cb.setChecked(HelpOnTypingManager.getInstance().isKeywordsActive());
        ((JCheckBoxMenuItem) cb.getAsSimpleCheckBoxMenuItem()).addPropertyChangeListener(new PropertyChangeListener() {
            public void propertyChange(PropertyChangeEvent e) {
                cb.setChecked(HelpOnTypingManager.getInstance().isKeywordsActive());
            }
        });

        return cb;
    }
}
