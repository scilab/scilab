/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JOptionPane;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.SimpleCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.SimpleMenu;
import org.scilab.modules.gui.menuitem.SimpleMenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * DefaultCheckAction Class
 * @author Bruno JOFRET
 *
 */
public class DefaultCheckAction extends SwingScilabCheckBoxMenuItem implements CheckBoxMenuItem, ActionListener {
    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = 5618123082228356437L;
    private SciNotes editorBackup;

    /**
     * Constructor
     * @param editor SciNotes
     */
    @SuppressWarnings("serial")
    public DefaultCheckAction(SciNotes editor) {
        super();
        setText(SciNotesMessages.DEFAULT + SciNotesMessages.DOTS);
        setState(true);
        editorBackup = editor;

        setCallback(new CommonCallBack(SciNotesMessages.DEFAULT + SciNotesMessages.DOTS) {
            public void callBack() {
                doAction();
            }

            public void actionPerformed(ActionEvent e) {
                callBack();
            }
        });
    }

    /**
     *
     * @param label label (string)
     * @param editor SciNotes
     */
    @SuppressWarnings("serial")
    protected DefaultCheckAction(String label, SciNotes editor) {
        super();
        setText(label);
        setState(true);
        editorBackup = editor;
        setCallback(new CommonCallBack(SciNotesMessages.DEFAULT + SciNotesMessages.DOTS) {
            public void callBack() {
                doAction();
            }

            public void actionPerformed(ActionEvent e) {
                callBack();
            }
        });
    }

    /**
     * get Editor
     * @return SciNotes
     */
    public SciNotes getEditor() {
        return editorBackup;
    }

    /**
     * doAction
     */
    public void doAction() {
        JOptionPane.showMessageDialog(getEditor(), "Not Implemented Now !!! (state = " + getState() + ")", null, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * actionPerformed
     * @param arg0 ActionEvent
     */
    public void actionPerformed(ActionEvent arg0) {
        doAction();
    }

    /**
     * createCheckBoxMenu
     * @param title String
     * @param icon  String
     * @param defaultCheckAction DefaultCheckAction
     * @param keyStroke KeyStroke
     * @return CheckBoxMenuItem
     */
    protected static CheckBoxMenuItem createCheckBoxMenu(String title, String icon, DefaultCheckAction defaultCheckAction, KeyStroke keyStroke) {
        defaultCheckAction.setText(title);
        defaultCheckAction.setChecked(false);
        if (keyStroke != null) {
            ((SwingScilabCheckBoxMenuItem) defaultCheckAction.getAsSimpleCheckBoxMenuItem()).setAccelerator(keyStroke);
        }
        return defaultCheckAction;
    }

    /**
     * getAsSimpleCheckBoxMenuItem
     * @return SimpleCheckBoxMenuItem
     */
    public SimpleCheckBoxMenuItem getAsSimpleCheckBoxMenuItem() {
        return this;
    }

    /**
     * getAsSimpleMenuItem
     * @return SimpleMenuItem
     */
    public SimpleMenuItem getAsSimpleMenuItem() {
        return null;
    }

    /**
     * getAsSimpleMenu
     * @return SimpleMenu
     */
    public SimpleMenu getAsSimpleMenu() {
        return null;
    }
}
